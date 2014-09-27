	//===--- cfgdumper.cpp ------------------------------------------*- C++ -*-===//
//                                    AUTHOR
// Name: Christophe Bacara
// Mail: christophe dot bacara at etudiant dot univ hyphen lille1 dot fr
// 
// Resp.: Laure Gonnord - INRIA/IRCICA - DaRT/Emeraude
//
//===----------------------------------------------------------------------===//
//
// Front-end example : parse the AST, visit it, and for each FunctionDecl with
// a body, build and dump a Control-Flow Graph.
//
//===----------------------------------------------------------------------===//
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"   
#include "llvm/ADT/IntrusiveRefCntPtr.h"
#include "llvm/ADT/StringRef.h"

#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/FileManager.h"  
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Lex/Lexer.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/Parse/ParseAST.h"
#include "clang/Rewrite/Frontend/Rewriters.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Analysis/CFG.h"

#define PROGRAM_NAME "cfgdumper"

using namespace clang;
using namespace llvm;

//===----------------------------------------------------------------------===//
// CLASS: CustomRecursiveASTVisitor
// DESC: Customized AST Visitor (dump a CFG for each FunctionDecl with body
//       found)
//===----------------------------------------------------------------------===//
class CustomRecursiveASTVisitor : public RecursiveASTVisitor<CustomRecursiveASTVisitor> {
	public:
	CustomRecursiveASTVisitor() { }

	bool VisitFunctionDecl(FunctionDecl *f);
};

bool CustomRecursiveASTVisitor::VisitFunctionDecl(FunctionDecl *f) 
{
	if (f->hasBody()) {
	// CFG Building
	CFG* cfg = CFG::buildCFG(f,
			     f->getBody(),
			     &f->getASTContext(),
			     CFG::BuildOptions());
	// Output
	errs() << "# Function with body found: " << f->getNameInfo().getName().getAsString() << "\n";
	cfg->dump(LangOptions(), true);
	}

	return true;
}

//===----------------------------------------------------------------------===//
// CLASS: CustomASTConsumer
// DESC: Read the AST and call a CustomRecursiveASTVisitor on it.
//===----------------------------------------------------------------------===//
class CustomASTConsumer : public ASTConsumer {
	public:
	CustomASTConsumer()  { }

	virtual bool HandleTopLevelDecl(DeclGroupRef d);

	CustomRecursiveASTVisitor rv; 
};

bool CustomASTConsumer::HandleTopLevelDecl(DeclGroupRef d)
{
	typedef DeclGroupRef::iterator it;

	// For each top-level declaration, we traverse it.
	for (it b = d.begin(), e = d.end(); b != e; b++)
		rv.TraverseDecl(*b);

	return true;
}

//===----------------------------------------------------------------------===//
//                                 MAIN FUNCTION
//===----------------------------------------------------------------------===//
int main(int argc, char *argv[])
{
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
	TextDiagnosticPrinter *pTextDiagnosticPrinter = new TextDiagnosticPrinter(outs(),&diagnosticOptions,true);
	compiler.createDiagnostics(pTextDiagnosticPrinter);

	// bacara: <!> Did not understand all of this block <!> 
	CompilerInvocation *Invocation = new CompilerInvocation;
	CompilerInvocation::CreateFromArgs(*Invocation, argv + 1, argv + argc, compiler.getDiagnostics());
	compiler.setInvocation(Invocation);

	// Target setting
	llvm::IntrusiveRefCntPtr<TargetOptions> pto( new TargetOptions());
	pto->Triple = llvm::sys::getDefaultTargetTriple();
	llvm::IntrusiveRefCntPtr<TargetInfo> pti(TargetInfo::CreateTargetInfo(compiler.getDiagnostics(),
	pto.getPtr()));
	compiler.setTarget(pti.getPtr());

	compiler.createFileManager();
	compiler.createSourceManager(compiler.getFileManager());

	// <!> bacara: Not from me <!>
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
	
	//~ headerSearchOptions.AddPath("/usr/include/c++/4.6",clang::frontend::Angled, false, false);
	//~ headerSearchOptions.AddPath("/usr/include/c++/4.6/i686-linux-gnu",clang::frontend::Angled,false,false);
	//~ headerSearchOptions.AddPath("/usr/include/c++/4.6/backward",clang::frontend::Angled,false,false);
	//~ headerSearchOptions.AddPath("/usr/local/include",clang::frontend::Angled,false,false);
	//~ headerSearchOptions.AddPath("/usr/local/lib/clang/3.3/include",clang::frontend::Angled,false,false);
	//~ headerSearchOptions.AddPath("/usr/include/i386-linux-gnu",clang::frontend::Angled,false,false);
	//~ headerSearchOptions.AddPath("/usr/include",clang::frontend::Angled,false,false);
	// </Warning!!> -- End of Platform Specific Code

	compiler.createPreprocessor();
	compiler.getPreprocessorOpts().UsePredefines = false;

	compiler.createASTContext();

	const FileEntry *pFile = compiler.getFileManager().getFile(fname);
	compiler.getSourceManager().createMainFileID(pFile);
	compiler.getDiagnosticClient().BeginSourceFile(compiler.getLangOpts(),&compiler.getPreprocessor());

	CustomASTConsumer astConsumer;
	ParseAST(compiler.getPreprocessor(),&astConsumer,compiler.getASTContext());
	
	compiler.getDiagnosticClient().EndSourceFile();
	return 0;
}



