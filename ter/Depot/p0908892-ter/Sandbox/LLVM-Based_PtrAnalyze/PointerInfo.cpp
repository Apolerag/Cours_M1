//===--- PointerInfos.cpp ---------------------------------------*- C++ -*-===//
//                                    AUTHOR
// Name: Christophe Bacara
// Mail: christophe dot bacara at etudiant dot univ hyphen lille1 dot fr
// 
// Resp.: Laure Gonnord - INRIA/IRCICA - DaRT/Emeraude
//
//===----------------------------------------------------------------------===//
//
// 
// 
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// CLASS: PointerInfos
// DESC: 
//===----------------------------------------------------------------------===//
class PointerInfos {
	public:
	typedef enum StmtKind_e {
		SK_Assign,
		SK_IncDec,
		SK_Deref
	} StmtKind;

	private:
	VarDecl *m_target;

	PointerInfos() { m_target = NULL; }

	public:
	VarDecl *getTarget();

	virtual void print(raw_ostream &os, SourceManager &SM);
};

VarDecl* PointerInfos::getTarget() {
	return m_target();
}

//===----------------------------------------------------------------------===//
// CLASS: DeclPointerInfos
// DESC: 
//===----------------------------------------------------------------------===//
class DeclPointerInfos : public PointerInfos {
	VarDecl *m_pointer;

	bool m_isConst;
	bool m_isIncrementedDecremented;
	bool m_isDereferenced;
	bool m_isPointingStaticArray;
	bool m_isPointingAllocatedMemory;

	vector<Stmt*> m_memAssign;
	vector<Stmt*> m_memIncDec;
	vector<Stmt*> m_memDeref;

	public:
	DeclPointerInfos(VarDecl* pointer, bool parmVar = false);

	VarDecl *getPointer();
	VarDecl *getTarget();

	void setTarget(VarDecl *target);

	void setNonConst();
	void setIncrementedDecremented();
	void setDereferenced();

	bool isConst();
	bool isIncrementedDecremented();
	bool isDereferenced();
	bool isPointingStaticArray();
	bool isPointingAllocatedMemory();

	bool isAnonymousPointer();

	void memorizeStatement(Stmt *S, StmtKind kind);

	void print(raw_ostream &os, SourceManager &SM);
};

VarDecl* PointerInfos::getPointer() {
	return m_pointer;
}

VarDecl* PointerInfos::getTarget() {
	return m_target;
}

void PointerInfos::setTarget(VarDecl *target) {
	m_target = target;
}

void PointerInfos::setNonConst() {
	m_isConst = false;
}

void PointerInfos::setIncrementedDecremented() {
	m_isConst = false;
	m_isIncrementedDecremented = true;
}

void PointerInfos::setDereferenced() {
	m_isDereferenced = true;
}

bool PointerInfos::isConst() {
	return m_isConst;
}

bool PointerInfos::isIncrementedDecremented() {
	eturn m_isIncrementedDecremented;
}

bool PointerInfos::isDereferenced() {
	return m_isDereferenced;
}

void PointerInfos::memorizeStatement(Stmt *S, StmtKind kind) {
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

void PointerInfos::print(raw_ostream &os, SourceManager &SM) {
	typedef vector<Stmt*>::const_iterator it;
	LangOptions LangOpts;
	PrintingPolicy Policy(LangOpts);
	PresumedLoc ploc;

	// Output pointer's name
	stream << "# PointerInfos: ";
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
		stream << "|- is const: " << (m_isConst ? "yes" : "no") << "\n";

		for (it b = m_memAssign.begin(), e = m_memAssign.end(); b != e; ++b) {
			ploc = SM.getPresumedLoc((*b)->getLocStart());

			stream << "\t- ";
			(*b)->printPretty(stream, 0, Policy);

			stream << " <" << ploc.getFilename() << ":" << ploc.getLine() << ":" << ploc.getColumn() << ">\n";
		}

		// Output inc/dec informations
		stream << "|- is incremented or decremented: "<< (m_isIncrementedDecremented ? "yes" : "no") << "\n";

		for (it b = m_memIncDec.begin(), e = m_memIncDec.end(); b != e; ++b) {
			ploc = SM.getPresumedLoc((*b)->getLocStart());

			stream << "\t- ";
			(*b)->printPretty(stream, 0, Policy);

			stream << " <"<< ploc.getFilename()<< ":"<< ploc.getLine()<< ":"<< ploc.getColumn()<< ">\n";
		}

		// Output dereference informations
		stream << "`- is dereferenced: "<< (m_isDereferenced ? "yes" : "no")<< "\n";

		for (it b = m_memDeref.begin(), e = m_memDeref.end(); b != e; ++b) {
			ploc = SM.getPresumedLoc((*b)->getLocStart());

			stream << "\t- ";
			(*b)->printPretty(stream, 0, Policy);

			stream << " <"<< ploc.getFilename()<< ":"<< ploc.getLine()	<< ":"<< ploc.getColumn()<< ">\n";
		}
	}
}

//===----------------------------------------------------------------------===//
// CLASS: AnonymousPointerInfos
// DESC: 
//===----------------------------------------------------------------------===//
class AnonymousPointerInfos : public PointerInfos {
	public:
	AnonymousPointerInfoss(VarDecl *target);
};

AnonymousPointerInfoss::AnonymousPointerInfoss(VarDecl *target) {
	if (isa<PointerType>(getTypePtrFromVarDecl(target)))
		llvm_unreachable("AnonymousPointerInfoss(): Tried to instanciate\	anonymous pointer from pointer typed VarDecl.");

	m_target = target;
}

//===----------------------------------------------------------------------===//
// CLASS: ParmDeclPointerInfos
// DESC: 
//===----------------------------------------------------------------------===//
class ParmDeclPointerInfos : public DeclPointerInfos {
	public:
}

