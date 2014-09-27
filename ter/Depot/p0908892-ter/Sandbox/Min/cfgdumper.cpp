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
	
	compiler.getDiagnosticClient().EndSourceFile();
	return 0;
}



