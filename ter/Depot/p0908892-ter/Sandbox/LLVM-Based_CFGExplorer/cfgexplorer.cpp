//===--- cfgexplorer.cpp ----------------------------------------*- C++ -*-===//
//                                    AUTHOR
// Name: Christophe Bacara
// Mail: christophe dot bacara at etudiant dot univ hyphen lille1 dot fr
// 
// Resp.: Laure Gonnord - INRIA/IRCICA - DaRT/Emeraude
//
//===----------------------------------------------------------------------===//
//
// This file contains the necessary stuff to visit an AST and build a CFG for
// each function found, then look for any use of AddrOf/Deref operators, and
// pointers/array decl/read/write.
// 
// ATM, recognizes:
// - Ptr and Array declarations
// - Expressions dereferencing
// - Var's address request
// - Ptr and Array ref uses
//
// DO NOT RECOGNIZE: Array Subscript
//
//===----------------------------------------------------------------------===//
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <vector>
#include <stack>
#include <unistd.h>

#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"   

#include "llvm/ADT/IntrusiveRefCntPtr.h"
#include "llvm/ADT/StringRef.h"

#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Frontend/CompilerInstance.h"

#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/FileManager.h"  
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/Diagnostic.h"

#include "clang/Lex/Preprocessor.h"
#include "clang/Lex/Lexer.h"

#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/Expr.h"
#include "clang/AST/OperationKinds.h"

#include "clang/Parse/ParseAST.h"

#include "clang/Rewrite/Frontend/Rewriters.h"
#include "clang/Rewrite/Core/Rewriter.h"

#include "clang/Analysis/CFG.h"
#include "clang/Analysis/Visitors/CFGRecStmtDeclVisitor.h"

#define PROGRAM_NAME "cfgexplorer"

using namespace clang;
using namespace llvm;

//===----------------------------------------------------------------------===//
// CLASS: CFGPointerSeeker
// DESC: Visit a CFG and dump usages of pointer, addrof and deref operator
//===----------------------------------------------------------------------===//
class CFGPointerSeeker : public CFGRecStmtVisitor<CFGPointerSeeker> {
	private:
	SourceManager &m_SourceManager; // SourceManager used by the CompilerInstance
	ASTContext &m_ASTContext;       // AST Context when CFG builded.
	CFG &m_Cfg;                     // CFG to visit

	unsigned m_indentLevel;         // Indentation level for output

	public:
	// Constructor
	CFGPointerSeeker(SourceManager &smgr, ASTContext &astcontext, CFG &cfg) : m_SourceManager(smgr), m_ASTContext(astcontext), m_Cfg(cfg),m_indentLevel(1) {}

	// Kind of statement where VarDecl comes from
	enum VarDeclFromStmtKind { 
		DeclStmtKind,
		DeclRefExprKind,
		ArraySubscriptExprKind
	};

	// Template necessary stuff
	CFG &getCFG() { return m_Cfg; }
	void operator()(Stmt *S) { BlockStmt_Visit(S); }

	// Top-Level statements visiting methods
	void BlockStmt_VisitStmt(Stmt *S);

	// Visiting methods
	void VisitBinaryOperator(BinaryOperator *BOP);
	void VisitUnaryOperator(UnaryOperator *UOP);
	void VisitArraySubscriptExpr(ArraySubscriptExpr *ASE);
	void VisitDeclRefExpr(DeclRefExpr *DRE);
	void VisitCallExpr(CallExpr *CE);
	void VisitDeclStmt(DeclStmt *DS);
	void Visit(Stmt *S);

	// Processing methods
	void ProcessVarDeclRef(VarDecl *VD,
	VarDeclFromStmtKind kind,
	SourceLocation location);
	private:
	void printIndent();
};

void CFGPointerSeeker::BlockStmt_VisitStmt(Stmt *S) {
	CFGPointerSeeker::VisitStmt(S);
}

void  CFGPointerSeeker::VisitBinaryOperator(BinaryOperator *BOP) {
	// If not assign, recursive visit of children
	if (!BOP->isAssignmentOp()) {
		VisitChildren(BOP);
	} else {
		printIndent();
		outs() << "Assignment expression found: ";
		BOP->dumpPretty(m_ASTContext);
		outs() << "\n";

		printIndent();
		outs() << "`Visiting LHS...\n";

		// LHS Visit
		++m_indentLevel;
		Visit(BOP->getLHS());
		--m_indentLevel;

		printIndent();
		outs() << " LHS visit ended. Now visiting RHS...\n";

		// RHS Visit
		++m_indentLevel;
		Visit(BOP->getRHS());
		--m_indentLevel;

		printIndent();
		outs() << " RHS visit ended.\n";
	}
}

void CFGPointerSeeker::VisitUnaryOperator(UnaryOperator *UOP) {
std::string str;

switch (UOP->getOpcode()) {
	// Deref operator (*)
	case UO_Deref:
		str = "Dereferencing expression";
		break;
	// AddrOf operator (&)
	case UO_AddrOf:
		str = "Addressing from variable";
		break;
	// Others
	default:
		return VisitChildren(UOP);
	}

	// Output
	printIndent();
	outs() << "|-- " << str << " at line " << m_SourceManager.getPresumedLoc(UOP->getOperatorLoc()).getLine() << ": ";
	UOP->getSubExpr()->dumpPretty(m_ASTContext);
	outs() << "\n";

	// Recursive visit
	VisitChildren(UOP);
}

void CFGPointerSeeker::VisitArraySubscriptExpr(ArraySubscriptExpr *ASE) {
	VarDecl *VD;

	printIndent();
	outs() << "Array subscript expression found: ";
	ASE->dumpPretty(m_ASTContext);
	outs() << "\n";

	printIndent();
	outs() << "`Visiting LHS...\n";

	// LHS Visit
	++m_indentLevel;
	Visit(ASE->getLHS());
	--m_indentLevel;

	printIndent();
	outs() << " LHS visit ended. Now visiting RHS...\n";

	// RHS Visit
	++m_indentLevel;
	Visit(ASE->getRHS());
	--m_indentLevel;

	printIndent();
	outs() << " RHS visit ended.\n";
}

void CFGPointerSeeker::VisitDeclRefExpr(DeclRefExpr *DRE) {
	VarDecl *VD;

	// S'il s'agit d'une reference a une variable, processing !
	if ((VD = dyn_cast<VarDecl>(DRE->getDecl())))
		ProcessVarDeclRef(VD, DeclRefExprKind, DRE->getLocation());
}

void CFGPointerSeeker::VisitCallExpr(CallExpr *CE) {
	if (CE->getNumArgs()) {
		printIndent();
		outs() << "Call expression with args found: ";
		CE->dumpPretty(m_ASTContext);
		outs() << "\n";

		printIndent();
		outs() << "`Visiting args...\n";
		++m_indentLevel;

		VisitChildren(CE);

		--m_indentLevel;
		printIndent();
		outs() << "`Args visit ended.\n";
	}
}

void CFGPointerSeeker::VisitDeclStmt(DeclStmt *DS) {
	VarDecl *VD;

	// Process something if this is a var ref
	if ((VD = dyn_cast<VarDecl>(DS->getSingleDecl())))
		ProcessVarDeclRef(VD, DeclStmtKind, VD->getInnerLocStart());
}

void CFGPointerSeeker::ProcessVarDeclRef(VarDecl *VD, VarDeclFromStmtKind kind, SourceLocation location) {
	const PointerType *PTY;
	const ArrayType *ATY;
	const Type *TY;
	Expr *INIT;

	TY = VD->getTypeSourceInfo()->getType().getTypePtr();

	// If this is a pointer..
	if ((PTY = dyn_cast<PointerType>(TY))) {
		printIndent();

		// Switch statement kind
		switch (kind) {
			// DeclStmt
			case DeclStmtKind:
				outs() << "|-- PTR DECLARATION FOUND - Name: " << VD->getNameAsString() << " - Pointed type: " << PTY->getPointeeType().getAsString() << " - Line: " << m_SourceManager.getPresumedLoc(location).getLine() << "\n";

				// Check for init expr
				if ((INIT = VD->getInit())) {
					printIndent();
					outs() << "`-Initialized with: ";
					INIT->dumpPretty(m_ASTContext);
					outs() << "\n";

					++m_indentLevel;
					printIndent();
					outs() << "`Visiting initializing expression...\n";
					++m_indentLevel;
					Visit(INIT);
					--m_indentLevel;
					printIndent();
					outs() << " Visit ended.\n";
					--m_indentLevel;
				}
				break;
			// Ref Expr
			case DeclRefExprKind:
				outs() << "|-- PTR REFERENCE FOUND: " << VD->getNameAsString() << " - Line: " << m_SourceManager.getPresumedLoc(location).getLine() << " - Declared at line " << m_SourceManager.getPresumedLoc(VD->getInnerLocStart()).getLine() << "\n";
				break;
			// Others
			default:
				break;
		}
	} else if ((ATY = dyn_cast<ArrayType>(TY))) {
		// else if this is an array...
		
		printIndent();

		// Switch statement kind
		switch (kind) {
			// Decl Stmt
			case DeclStmtKind:
				outs() << "|-- ARRAY DECLARATION FOUND - Name: " << VD->getNameAsString() << " - Data type: "  << ATY->getElementType().getAsString() << " - Line: " << m_SourceManager.getPresumedLoc(location).getLine() << "\n";

				// Check for init expr
				if ((INIT = VD->getInit())) {
					printIndent();
					outs() << "`-Initialized with: ";
					INIT->dumpPretty(m_ASTContext);
					outs() << "\n";

					++m_indentLevel;
					printIndent();
					outs() << "`Visiting initializing expression...\n";
					++m_indentLevel;
					Visit(INIT);
					--m_indentLevel;
					printIndent();
					outs() << " Visit ended.\n";
					--m_indentLevel;
				}      
				break;
			// Ref Expr
			case DeclRefExprKind:
				outs() << "|-- ARRAY REFERENCE FOUND: " << VD->getNameAsString() << " - Line: " << m_SourceManager.getPresumedLoc(location).getLine() << " - Declared at line " << m_SourceManager.getPresumedLoc(VD->getInnerLocStart()).getLine() << "\n";
				break;
			// Others
			default:
				return;
		}
	}
}

void CFGPointerSeeker::Visit(Stmt *S) {
	CFGRecStmtVisitor::Visit(S);
}

void CFGPointerSeeker::printIndent() {
	for (unsigned i = 0; i < m_indentLevel; i++)
	outs() << "  ";
}

//===----------------------------------------------------------------------===//
// CLASS: CustomRecursiveASTVisitor
// DESC:  Customized visitor template for AST. Visit any function declaration,
//        and build/visit the CFG if that function has a body.
//===----------------------------------------------------------------------===//
class CustomRecursiveASTVisitor : public RecursiveASTVisitor<CustomRecursiveASTVisitor> {
	private:
	SourceManager &m_SourceManager;

	public:
	// Constructor
	CustomRecursiveASTVisitor(SourceManager &sManager)  : m_SourceManager(sManager) { }

	// Visiting methods
	bool VisitFunctionDecl(FunctionDecl *F);
};

bool CustomRecursiveASTVisitor::VisitFunctionDecl(FunctionDecl *f) {
	if (f->hasBody()) {
		// Output
		outs() << "########## Function with body found: " << f->getNameInfo().getName().getAsString() << " ##########\n";

		// Build and dump CFG
		outs() << "Building Control-Flow Graph... ";
		CFG *cfg = CFG::buildCFG(f,
		f->getBody(),
		&f->getASTContext(),
		CFG::BuildOptions());
		outs() << "OK\n";

		cfg->dump(LangOptions(), true);

		// Instanciating the visitor
		outs() << "Creating CFG visitor... ";
		CFGPointerSeeker ps(m_SourceManager, *(&f->getASTContext()), *cfg);
		outs() << "OK\n";

		// CFG Visit
		outs() << "Visiting CFG...\n";
		cfg->VisitBlockStmts(ps);
		outs() << "CFG Visit ended.\n\n";

		// Free memory
		delete cfg;
	}

	return true;
}

//===----------------------------------------------------------------------===//
// CLASS: CustomASTConsumer
// DESC: Read the AST and call a CustomRecursiveASTVisitor on it.
//===----------------------------------------------------------------------===//
class CustomASTConsumer : public ASTConsumer {
	private:
	CustomRecursiveASTVisitor astvisitor;

	public:
	CustomASTConsumer(SourceManager &sManager) : astvisitor(sManager) {}

	virtual bool HandleTopLevelDecl(DeclGroupRef d);
};

bool CustomASTConsumer::HandleTopLevelDecl(DeclGroupRef d) {
	typedef DeclGroupRef::iterator it;

	for (it b = d.begin(), e = d.end(); b != e; b++)
		astvisitor.TraverseDecl(*b);

	return true;
}
//===----------------------------------------------------------------------===//
//                             MAIN FUNCTION
//===----------------------------------------------------------------------===//
int main(int argc, char *argv[]) {
	struct stat filestat;

	// Single argument check
	if (argc != 2) {
		errs() << "Usage: "PROGRAM_NAME" <filename>\n";
		return 1;
	}

	// File existence check
	std::string fname(argv[argc - 1]);
	if (stat(fname.c_str(), &filestat) == -1) {
		perror(fname.c_str());
		exit(EXIT_FAILURE);
	}

	CompilerInstance compiler;
	DiagnosticOptions diagnosticOptions;
	TextDiagnosticPrinter *pTextDiagnosticPrinter = new TextDiagnosticPrinter(outs(), &diagnosticOptions, true);
	compiler.createDiagnostics(pTextDiagnosticPrinter);

	CompilerInvocation *Invocation = new CompilerInvocation;
	CompilerInvocation::CreateFromArgs(*Invocation, argv + 1, argv + argc,
	     compiler.getDiagnostics());
	compiler.setInvocation(Invocation);

	// Reglage de la cible
	llvm::IntrusiveRefCntPtr<TargetOptions> pto( new TargetOptions());
	pto->Triple = llvm::sys::getDefaultTargetTriple();
	llvm::IntrusiveRefCntPtr<TargetInfo>
	pti(TargetInfo::CreateTargetInfo(compiler.getDiagnostics(), pto.getPtr()));
	compiler.setTarget(pti.getPtr());

	compiler.createFileManager();
	compiler.createSourceManager(compiler.getFileManager());

	// <!>
	HeaderSearchOptions &headerSearchOptions = compiler.getHeaderSearchOpts();
	// <Warning!!> -- Platform Specific Code lives here
	// This depends on A) that you're running linux and
	// B) that you have the same GCC LIBs installed that
	// I do.
	// Search through Clang itself for something like this,
	// go on, you won't find it. The reason why is Clang
	// has its own versions of std* which are installed under
	// /usr/local/lib/clang/<version>/include/
	// See somewhere around Driver.cpp:77 to see Clang adding
	// its version of the headers to its include path.
	// To see what include paths need to be here, try
	// clang -v -c test.c
	// or clang++ for C++ paths as used below:
	headerSearchOptions.AddPath("/usr/include/c++/4.6", clang::frontend::Angled,false, false);
	headerSearchOptions.AddPath("/usr/include/c++/4.6/i686-linux-gnu",clang::frontend::Angled, false, false);
	headerSearchOptions.AddPath("/usr/include/c++/4.6/backward", clang::frontend::Angled,false,  false);
	headerSearchOptions.AddPath("/usr/local/include", clang::frontend::Angled, false, false);
	headerSearchOptions.AddPath("/usr/local/lib/clang/3.3/include", clang::frontend::Angled, false,false);
	headerSearchOptions.AddPath("/usr/include/i386-linux-gnu",clang::frontend::Angled, false, false);
	headerSearchOptions.AddPath("/usr/include", clang::frontend::Angled,false,false);
	// </Warning!!> -- End of Platform Specific Code

	compiler.createPreprocessor();
	compiler.getPreprocessorOpts().UsePredefines = false;
	compiler.createASTContext();

	// Reglage du fichier d'entree
	const FileEntry *pFile = compiler.getFileManager().getFile(fname);
	compiler.getSourceManager().createMainFileID(pFile);
	compiler.getDiagnosticClient().BeginSourceFile(compiler.getLangOpts(), &compiler.getPreprocessor());

	// Instanciation de l'ASTConsumer
	CustomASTConsumer astConsumer(compiler.getSourceManager());

	// Parsing de l'AST + Visit
	ParseAST(compiler.getPreprocessor(), &astConsumer, compiler.getASTContext());

	compiler.getDiagnosticClient().EndSourceFile();
	return 0;
}





