//===--- ASTManagement.cpp --------------------------------------*- C++ -*-===//
//                                    AUTHOR
// Name: Christophe Bacara
// Mail: christophe dot bacara at etudiant dot univ hyphen lille1 dot fr
// 
// Resp.: Laure Gonnord - INRIA/IRCICA - DaRT/Emeraude
//
//===----------------------------------------------------------------------===//
//
// This file contains all the necessary stuff for consumering an AST, then visit
// it. While visiting, the CFGBuilderRecursiveASTVisitor class tries to build
// the Control-Flow Graph for each function visited, if it has a body. If a CFG
// is built this way, it starts visiting CFG with ConstantPtrSearchCFGVisitor.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// CLASS: CFGBuilderRecursiveASTVisitor
// DESC:  Customized visitor template for AST. Visit any function declaration,
//        and build+visit the CFG if that function has a body.
//===----------------------------------------------------------------------===//
class CFGBuilderRecursiveASTVisitor
  : public RecursiveASTVisitor<CFGBuilderRecursiveASTVisitor> {
private:
  SourceManager &m_SourceManager;

public:
  // Constructor
  CFGBuilderRecursiveASTVisitor(SourceManager &manager)
    : m_SourceManager(manager) { }

  // Visiting methods
  bool VisitFunctionDecl(FunctionDecl *f);

private:
  template <typename CALLBACK>
  void RVisitCFGBlocks(CFG &cfg, CALLBACK &visitor);
};

bool CFGBuilderRecursiveASTVisitor::VisitFunctionDecl(FunctionDecl *f) 
{
  if (f->hasBody()) {
    // Output
    outs() << "########################################\n";
    outs() << "Function with body found: "
	   << f->getNameInfo().getAsString()
	   << "\n";

    f->getBody()->dumpPretty(f->getASTContext());

    // Construction du CFG
    CFG *cfg = CFG::buildCFG(f,
			     f->getBody(),
			     &f->getASTContext(),
			     CFG::BuildOptions());

    /* DEBUG SNIPPET: Hand-made CFGBlockStmt dumping
    for (CFG::iterator b = cfg->begin(), e = cfg->end(); b !=e; b++) {
      for (CFGBlock::iterator bb = (*b)->begin(), ee = (*b)->end(); bb != ee; bb++) {
	(*bb).castAs<CFGStmt>().getStmt()->dumpColor();
      }
    }
    */

    // Instanciation du visitor
    ConstPtrSearchCFGVisitor visitor(m_SourceManager, f, *cfg);
   
    // // Premiere visite : reperage des declarations et &var
    // visitor.beginPointerGathering();
    // cfg->VisitBlockStmts(visitor);
    // // Seconde visite : construction des matrices d'alias
    // visitor.beginConstPtrAnalysis();
    // RVisitCFGBlocks(*cfg, visitor);

    while (visitor.setNextMode() != ConstPtrSearchCFGVisitor::VM_Done) {
      RVisitCFGBlocks(*cfg, visitor);
    }

    // Resume des informations obtenus
    visitor.summarize();

    // Liberation des ressources
    delete cfg;
  }

  return true;
}

template <typename CALLBACK>
void CFGBuilderRecursiveASTVisitor::RVisitCFGBlocks(CFG &cfg, CALLBACK &visitor)
{
  typedef CFG::const_reverse_iterator cfg_it;
  typedef CFGBlock::const_iterator cfgblock_it;

  for (cfg_it b = cfg.rbegin(), e = cfg.rend(); b != e; ++b) {
    for (cfgblock_it bb = (*b)->begin(), ee = (*b)->end(); bb != ee; ++bb)
      if (Optional<CFGStmt> stmt = bb->getAs<CFGStmt>())
	visitor(const_cast<Stmt*>(stmt->getStmt()));
  }
}

//===----------------------------------------------------------------------===//
// CLASS: CFGBuilderASTConsumer
// DESC:  Customized template for AST consumering.
//===----------------------------------------------------------------------===//
class CFGBuilderASTConsumer
  : public ASTConsumer {
private:
  CFGBuilderRecursiveASTVisitor &m_visitor; // AST Visitor to use

public:
  // Constructor
  CFGBuilderASTConsumer(CFGBuilderRecursiveASTVisitor &visitor)
    : m_visitor(visitor) { }
  
  virtual bool HandleTopLevelDecl(DeclGroupRef d);
};

bool CFGBuilderASTConsumer::HandleTopLevelDecl(DeclGroupRef d)
{
  typedef DeclGroupRef::iterator it;

  for (it b = d.begin(), e = d.end(); b != e; b++)
    m_visitor.TraverseDecl(*b);

  return true;
}
