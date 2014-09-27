//===--- ConstPtrSearchCFGVisitor.cpp ---------------------------*- C++ -*-===//
//                                    AUTHOR
// Name: Christophe Bacara
// Mail: christophe dot bacara at etudiant dot univ hyphen lille1 dot fr
// 
// Resp.: Laure Gonnord - INRIA/IRCICA - DaRT/Emeraude
//
//===----------------------------------------------------------------------===//
//
// This file contains implementation for ConstPtrSearchCFGVisitor class.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// CLASS: ConstPtrSearchCFGVisitor
// DESC:  Customized visitor template for CFG. Look up for constant pointers.
//===----------------------------------------------------------------------===//
using namespace util;

class ConstPtrSearchCFGVisitor
  : public CFGRecStmtVisitor<ConstPtrSearchCFGVisitor> {
public:
  // Visitor Mode
  typedef enum VisitorMode_e {
    VM_Initialized = 0,
    VM_Gathering,
    VM_Analyzing,
    VM_Done
  } VisitorMode;

private:
  SourceManager &m_SourceManager; // SourceManager used by the CompilerInstance
  ASTContext &m_ASTContext;       // AST Context when CFG builded.
  CFG &m_Cfg;                     // CFG to visit

  AliasingManager m_aliasingManager;
  VisitorMode m_mode;

  map<VarDecl*, PointerInfo*> m_pinfosMap;

public:
  // Constructor
  ConstPtrSearchCFGVisitor(SourceManager &smgr, FunctionDecl* f, CFG &cfg)
    : m_SourceManager(smgr), m_ASTContext(f->getASTContext()), m_Cfg(cfg) {
    // Gathering all pointers into params array
    typedef FunctionDecl::param_iterator it;

    for (it b = f->param_begin(), e = f->param_end(); b != e; ++b) {
      if (isa<PointerType>((*b)->getTypeSourceInfo()->getType().getTypePtr())) {
	m_pinfosMap[*b] = new PointerInfo(*b, true);
      }
    }

    m_mode = VM_Initialized;
  }

  // Template necessary stuff
  CFG &getCFG() { return m_Cfg; }
  void operator()(Stmt *S);

  VisitorMode_e setNextMode();

  // Matrix dump
  void summarize();

  // Visiting methods
  void VisitArraySubscriptExpr(ArraySubscriptExpr *ASE);
  void VisitBinaryOperator(BinaryOperator *BOP);
  void VisitUnaryOperator(UnaryOperator *UOP);
  void VisitDeclStmt(DeclStmt *DS);
};

void ConstPtrSearchCFGVisitor::operator()(Stmt *S)
{
  switch (m_mode) {
  case VM_Gathering:
    // Fall through
  case VM_Analyzing:
    // Fall through

    BlockStmt_Visit(S);
    break;

  default:
    llvm_unreachable("Incorrect visitor mode. Please use setNextMode().");
  }
}

void ConstPtrSearchCFGVisitor::summarize()
{  
  typedef map<VarDecl*, PointerInfo*>::iterator it;
  
  // outs() << "----------------------------------------\n";
  // for (it b = m_pinfosMap.begin(), e = m_pinfosMap.end(); b != e; ++b)
  //   if (!b->second->isAnonymousPointer())
  //     outs() << *(b->second)
  // 	     << "----------------------------------------\n";

  outs() << "----------------------------------------\n";
  for (it b = m_pinfosMap.begin(), e = m_pinfosMap.end(); b != e; ++b)
    if (!b->second->isAnonymousPointer()) {
      b->second->print(outs(), m_SourceManager);
      outs() << "----------------------------------------\n";
    }
}

ConstPtrSearchCFGVisitor::VisitorMode_e ConstPtrSearchCFGVisitor::setNextMode()
{
  if (m_mode < VM_Done)
    ++m_mode;

  if (m_mode == VM_Analyzing)
    m_aliasingManager.setPointerInfoMap(m_pinfosMap);

  return m_mode;
}

void ConstPtrSearchCFGVisitor::VisitArraySubscriptExpr(ArraySubscriptExpr *ASE)
{
  DeclRefExpr *DRE;
  Expr *LHS, *RHS;
  VarDecl *VD;

  switch (m_mode) {
  case VM_Gathering:
    // Do nothing
    break;

  case VM_Analyzing:
    LHS = ASE->getLHS()->IgnoreParenCasts();
    RHS = ASE->getRHS()->IgnoreParenCasts();
    
    if ((DRE = dyn_cast<DeclRefExpr>(LHS))
	&& (VD = getVarDeclFromDeclRefExpr<PointerType>(DRE))) {
      m_pinfosMap[VD]->setDereferenced();
      m_pinfosMap[VD]->memorizeStatement(ASE, PointerInfo::SK_Deref);
    }
    break;

  default:
    llvm_unreachable("Incorrect visitor mode. Please use setNextMode().");
  }
}

void ConstPtrSearchCFGVisitor::VisitBinaryOperator(BinaryOperator *BOP)
{
  UnaryOperator *UOP;
  DeclRefExpr *DRE;
  VarDecl *VD_LHS, *VD_RHS;
  Expr *LHS, *RHS;

  if (BOP->isAssignmentOp()) {
    // Getting LHS and RHS stripped of parenthesis and cast.
    LHS = BOP->getLHS()->IgnoreParenCasts();
    RHS = BOP->getRHS()->IgnoreParenCasts();

    switch (m_mode) {
    case VM_Gathering:
      // Do nothing
      break;

      // Analyzing mode
    case VM_Analyzing:
      // If LHS is a pointer ...
      if ((DRE = dyn_cast<DeclRefExpr>(LHS))
	  && (VD_LHS = getVarDeclFromDeclRefExpr<PointerType>(DRE))) {

	// Switch rhs statement class
	switch (RHS->getStmtClass()) {
	  // Case: DeclRefExpr
	case Stmt::DeclRefExprClass:
	  if ((VD_RHS = 
	       getVarDeclFromDeclRefExpr<PointerType>(cast<DeclRefExpr>(RHS)))) {
	    m_aliasingManager.alias(VD_LHS, VD_RHS);
	    m_pinfosMap[VD_LHS]->memorizeStatement(BOP, PointerInfo::SK_Assign);
	  }
	  break;

	  // Case: UnaryOperator
	case Stmt::UnaryOperatorClass:
	  if ((VD_RHS = getVarDeclFromAddrOfOperator(cast<UnaryOperator>(RHS)))) {
	    m_aliasingManager.alias(VD_LHS, VD_RHS);
	    m_pinfosMap[VD_LHS]->memorizeStatement(BOP, PointerInfo::SK_Assign);
	  }
	  break;

	  // Default: setNonConst()
	default:
	  m_aliasingManager.unset(VD_LHS);
	  m_pinfosMap[VD_LHS]->setNonConst();
	  break;
	}
      }
      break;

    default:
      llvm_unreachable("Incorrect visitor mode. Please use setNextmode().");
    }
  }

  VisitChildren(BOP);
}

void ConstPtrSearchCFGVisitor::VisitUnaryOperator(UnaryOperator *UOP)
{
  DeclRefExpr *DRE;
  VarDecl *VD;

  switch (m_mode) {
  case VM_Gathering:
    if ((VD = getVarDeclFromAddrOfOperator(UOP))) {
      m_pinfosMap[VD] = new PointerInfo(VD);
    }
    break;
    
  case VM_Analyzing:
    if ((VD = getVarDeclFromIncDecOperator<PointerType>(UOP))) {
      m_aliasingManager.unset(VD);
      m_pinfosMap[VD]->setIncrementedDecremented();
      m_pinfosMap[VD]->memorizeStatement(UOP, PointerInfo::SK_IncDec);
    }
    else if ((VD = getVarDeclFromDerefOperator<PointerType>(UOP))) {
      m_pinfosMap[VD]->setDereferenced();
      m_pinfosMap[VD]->memorizeStatement(UOP, PointerInfo::SK_Deref);
    }
    break;

  default:
    llvm_unreachable("Incorrect visitor mode. Please use setNextMode().");
  }

  VisitChildren(UOP);
}

void ConstPtrSearchCFGVisitor::VisitDeclStmt(DeclStmt *DS)
{
  // A l'interieur d'un CFG, les declarations sont toutes "inlinees".
  // Ainsi, pour chaque declaration existante, cette methode sera appellee.
  VarDecl *VD;
  VarDecl *VD_INIT;
  DeclRefExpr *DRE;
  Expr *E;
  UnaryOperator *UOP;
  const Type *TY;
  const PointerType *PTY;

  // S'il s'agit de la declaration d'un pointeur.
  if ((VD = dyn_cast<VarDecl>(DS->getSingleDecl()))
      && isa<PointerType>(getTypePtrFromVarDecl(VD))) {

    // Selon le type de visite
    switch (m_mode) {

      // Gathering mode
    case VM_Gathering:
      m_pinfosMap[VD] = new PointerInfo(VD);

      if ((E = VD->getInit())
	  && (UOP = dyn_cast<UnaryOperator>(E))
	  && (VD_INIT = getVarDeclFromAddrOfOperator(UOP)))
	m_pinfosMap[VD_INIT] = new PointerInfo(VD_INIT);
      break;
	
      // Analyzing mode
    case VM_Analyzing:
      if ((E = VD->getInit())
	  && (DRE = dyn_cast<DeclRefExpr>(E->IgnoreParenCasts()))
	  && (VD_INIT = getVarDeclFromDeclRefExpr<PointerType>(DRE))) {
	m_aliasingManager.alias(VD, VD_INIT, true);
      }
      else if ((UOP = dyn_cast<UnaryOperator>(VD->getInit()))
	       && (VD_INIT = getVarDeclFromAddrOfOperator(UOP))
	       && !isa<PointerType>(getTypePtrFromVarDecl(VD_INIT))) {
	m_aliasingManager.alias(VD, VD_INIT, true);
      }
      break;

    default:
      llvm_unreachable("Incorrect visitor mode. Please use setNextMode().");
    }
  }

  VisitChildren(DS);
}
