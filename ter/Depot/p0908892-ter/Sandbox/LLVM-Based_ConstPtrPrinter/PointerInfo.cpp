//===--- PointerInfo.cpp ---------------------------------------*- C++ -*-===//
//                                    AUTHOR
// Name: Christophe Bacara
// Mail: christophe dot bacara at etudiant dot univ hyphen lille1 dot fr
// 
// Resp.: Laure Gonnord - INRIA/IRCICA - DaRT/Emeraude
//
//===----------------------------------------------------------------------===//
//
// This files contains declaration of PointerInfo class, and all its definitions
// 
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// CLASS: PointerInfo
// DESC: Store informations about a pointer.
//===----------------------------------------------------------------------===//
class PointerInfo {
public:
  typedef enum PointerKind_e { 
    PK_ParmVar,
    PK_Declared,
    PK_Anonymous 
  } PointerKind;

  typedef enum StmtKind_e {
    SK_Assign,
    SK_IncDec,
    SK_Deref
  } StmtKind;

private:
  VarDecl *m_pointer;
  VarDecl *m_target;

  PointerKind m_kind;

  bool m_isConst;
  bool m_isConstAfterFirstAssign;
  bool m_isIncrementedDecremented;
  bool m_isDereferenced;
  bool m_isPointingStaticArray;
  bool m_isPointingAllocatedMemory;

  vector<Stmt*> m_memAssign;
  vector<Stmt*> m_memIncDec;
  vector<Stmt*> m_memDeref;

public:
  PointerInfo(VarDecl* pointer, bool parmVar = false);

  VarDecl *getPointer();
  VarDecl *getTarget();
  PointerKind getKind();

  void setTarget(VarDecl *target);

  void setNonConst();
  void setIncrementedDecremented();
  void setDereferenced();
  
  bool isParmVar();
  bool isConst();
  bool isIncrementedDecremented();
  bool isDereferenced();
  bool isPointingStaticArray();
  bool isPointingAllocatedMemory();

  bool isAnonymousPointer();

  void memorizeStatement(Stmt *S, StmtKind kind);

  void print(raw_ostream &stream, SourceManager &SM);
};

PointerInfo::PointerInfo(VarDecl *pointer, bool parmVar)
{
  m_pointer = pointer;
  
  if (parmVar)
    m_kind = PK_ParmVar;
  else if (isa<PointerType>(m_pointer->getTypeSourceInfo()->getType().getTypePtr()))
    m_kind = PK_Declared;
  else
    m_kind = PK_Anonymous;

  switch (m_kind) {
  case PK_ParmVar:
    m_target = NULL;
    break;
  case PK_Declared:
    m_target = NULL;
    break;
  case PK_Anonymous:
    m_target = m_pointer;
    break;
  }

  m_isConst = true;

  m_isIncrementedDecremented = false;
  m_isDereferenced = false;
}

VarDecl* PointerInfo::getPointer()
{
  return m_pointer;
}

VarDecl* PointerInfo::getTarget()
{
  return m_target;
}

PointerInfo::PointerKind PointerInfo::getKind()
{
  return m_kind;
}

void PointerInfo::setTarget(VarDecl *target)
{
  m_target = target;
}

void PointerInfo::setNonConst()
{
  m_isConst = false;
}

void PointerInfo::setIncrementedDecremented()
{
  m_isConst = false;
  m_isIncrementedDecremented = true;
}

void PointerInfo::setDereferenced()
{
  m_isDereferenced = true;
}

bool PointerInfo::isParmVar()
{
  return (m_kind == PK_ParmVar);
}

bool PointerInfo::isConst()
{
  return m_isConst;
}

bool PointerInfo::isIncrementedDecremented()
{
  return m_isIncrementedDecremented;
}

bool PointerInfo::isDereferenced()
{
  return m_isDereferenced;
}

bool PointerInfo::isAnonymousPointer()
{
  return (m_kind == PK_Anonymous);
}

void PointerInfo::memorizeStatement(Stmt *S, StmtKind kind)
{
  switch (kind) {
  case SK_Assign:
    m_memAssign.push_back(S);
    break;
  case SK_IncDec:
    m_memIncDec.push_back(S);
    break;
  case SK_Deref:
    m_memDeref.push_back(S);
    break;
  }
}

void PointerInfo::print(raw_ostream &stream, SourceManager &SM)
{
  typedef vector<Stmt*>::const_iterator it;
  LangOptions LangOpts;
  PrintingPolicy Policy(LangOpts);
  PresumedLoc ploc;

  // Output pointer's name
  stream << "# PointerInfo: ";
  if (m_kind == PK_Anonymous)
    stream << "&";
  stream << m_pointer->getNameAsString();

  // Output pointer's type
  stream << "\t[";
  switch (m_kind) {
  case PK_ParmVar:
    stream << "ParmVar";
    break;
  case PK_Declared:
    stream << "Declared";
    break;
  case PK_Anonymous:
    stream << "Anonymous";
    break;
  }
  stream << "]\n";   
  
  // Output pointer's target's name (or NULL)
  stream << "|- last target: ";
  if (m_target) {
    if (!isa<PointerType>(m_target->getTypeSourceInfo()->getType().getTypePtr()))
      stream << "&";
    stream << m_target->getNameAsString();
  }
  else {
    if (m_kind == PK_ParmVar)
      stream << "Unknown";
    else
      stream << "NULL";
  }
  stream << "\n";
  
  if (m_kind != PK_Anonymous) {
    // Output constantness informations
    stream << "|- is const: "
	   << (m_isConst ? "yes" : "no")
	   << "\n";

    for (it b = m_memAssign.begin(), e = m_memAssign.end(); b != e; ++b) {
      ploc = SM.getPresumedLoc((*b)->getLocStart());
      
      stream << "\t- ";
      (*b)->printPretty(stream, 0, Policy);

      stream << " <"
	     << ploc.getFilename()
	     << ":"
	     << ploc.getLine()
	     << ":"
	     << ploc.getColumn()
	     << ">\n";
    }
    
    // Output inc/dec informations
    stream << "|- is incremented or decremented: "
	   << (m_isIncrementedDecremented ? "yes" : "no")
	   << "\n";
    
    for (it b = m_memIncDec.begin(), e = m_memIncDec.end(); b != e; ++b) {
      ploc = SM.getPresumedLoc((*b)->getLocStart());
      
      stream << "\t- ";
      (*b)->printPretty(stream, 0, Policy);

      stream << " <"
	     << ploc.getFilename()
	     << ":"
	     << ploc.getLine()
	     << ":"
	     << ploc.getColumn()
	     << ">\n";
    }
    
    // Output dereference informations
    stream << "`- is dereferenced: "
	   << (m_isDereferenced ? "yes" : "no")
	   << "\n";

    for (it b = m_memDeref.begin(), e = m_memDeref.end(); b != e; ++b) {
      ploc = SM.getPresumedLoc((*b)->getLocStart());
      
      stream << "\t- ";
      (*b)->printPretty(stream, 0, Policy);

      stream << " <"
	     << ploc.getFilename()
	     << ":"
	     << ploc.getLine()
	     << ":"
	     << ploc.getColumn()
	     << ">\n";
    }
  }
}
