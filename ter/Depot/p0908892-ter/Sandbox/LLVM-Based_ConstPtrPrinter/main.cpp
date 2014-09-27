//===--- main.cpp -----------------------------------------------*- C++ -*-===//
//                                    AUTHOR
// Name: Christophe Bacara
// Mail: christophe dot bacara at etudiant dot univ hyphen lille1 dot fr
// 
// Resp.: Laure Gonnord - INRIA/IRCICA - DaRT/Emeraude
//
//===----------------------------------------------------------------------===//
//
// TODO: Complete here
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
#include "llvm/ADT/APSInt.h"

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
#include "clang/AST/PrettyPrinter.h"

#include "clang/Parse/ParseAST.h"

#include "clang/Rewrite/Frontend/Rewriters.h"
#include "clang/Rewrite/Core/Rewriter.h"

#include "clang/Analysis/CFG.h"
#include "clang/Analysis/Visitors/CFGRecStmtDeclVisitor.h"

using namespace clang;
using namespace llvm;
using namespace std;

//===----------------------------------------------------------------------===//
//                                   GLOBAL
//===----------------------------------------------------------------------===//
#define PROGRAM_NAME "cppr"

// Definition des options acceptables
#define OPTION_STRING "r"
// Variables globales de gestion d'options
static bool opt_rewrite = false;

//===----------------------------------------------------------------------===//
//                             CLASSES DEFINITIONS
//===----------------------------------------------------------------------===//

#include "Utils.cpp"
#include "PointerInfo.cpp"
#include "AliasingManager.cpp"
#include "ConstPtrSearchCFGVisitor.cpp"
#include "ASTManagement.cpp"

//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//                             MAIN FUNCTION
//===----------------------------------------------------------------------===//

int main(int argc, char *argv[])
{
  struct stat filestat;
  int opt;
  int i;
								
  // Gestion des options de la ligne de commandes
  while ((opt = getopt(argc, argv, OPTION_STRING)) != -1) {
    switch (opt) {
    case 'r':
      opt_rewrite = true;
      break;
    default:
      exit(EXIT_FAILURE);
    }
  }

  // Verification de l'existence de l'argument FILE
  if (optind != (argc - 1)) {
    errs() << "Usage: "
	   << PROGRAM_NAME
	   << " [OPTION] FILE\n";
    return 1;
  }

  // Verification de l'existence du fichier specifie
  std::string fname(argv[optind]);
  if (stat(fname.c_str(), &filestat) == -1) {
    perror(fname.c_str());
    exit(EXIT_FAILURE);
  }

  CompilerInstance compiler;

  DiagnosticOptions diagnosticOptions;
  TextDiagnosticPrinter *pTextDiagnosticPrinter =
    new TextDiagnosticPrinter(outs(),
			      &diagnosticOptions,
			      true);
  compiler.createDiagnostics(pTextDiagnosticPrinter);

  // bacara: <!> Pas entierement compris ce bout de code <!> 
  CompilerInvocation *Invocation = new CompilerInvocation;
  CompilerInvocation::CreateFromArgs(*Invocation, argv + 1, argv + argc,
				     compiler.getDiagnostics());
  compiler.setInvocation(Invocation);

  // Reglage de la cible
  llvm::IntrusiveRefCntPtr<TargetOptions> pto( new TargetOptions());
  pto->Triple = llvm::sys::getDefaultTargetTriple();
  llvm::IntrusiveRefCntPtr<TargetInfo>
    pti(TargetInfo::CreateTargetInfo(compiler.getDiagnostics(),
				     pto.getPtr()));
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
  //~ headerSearchOptions.AddPath("/usr/include/c++/4.6",
			      //~ clang::frontend::Angled,
			      //~ false,
			      //~ false);
  //~ headerSearchOptions.AddPath("/usr/include/c++/4.6/i686-linux-gnu",
			      //~ clang::frontend::Angled,
			      //~ false,
			      //~ false);
  //~ headerSearchOptions.AddPath("/usr/include/c++/4.6/backward",
			      //~ clang::frontend::Angled,
			      //~ false,
			      //~ false);
  //~ headerSearchOptions.AddPath("/usr/local/include",
			      //~ clang::frontend::Angled,
			      //~ false,
			      //~ false);
  //~ headerSearchOptions.AddPath("/usr/local/lib/clang/3.3/include",
			      //~ clang::frontend::Angled,
			      //~ false,
			      //~ false);
  //~ headerSearchOptions.AddPath("/usr/include/i386-linux-gnu",
			      //~ clang::frontend::Angled,
			      //~ false,
			      //~ false);
  //~ headerSearchOptions.AddPath("/usr/include",
			      //~ clang::frontend::Angled,
			      //~ false,
			      //~ false);
  // </Warning!!> -- End of Platform Specific Code

  /// / bacara: <!> Pas entierement compris non plus <!>
  // Activation de la reecriture du C++
  // LangOptions langOpts;
  // langOpts.GNUMode = 1;
  // langOpts.CXXExceptions = 1;
  // langOpts.RTTI = 1;
  // langOpts.Bool = 1;
  // langOpts.CPlusPlus = 1;
  // Invocation->setLangDefaults(langOpts,
  //                             clang::IK_CXX,
  //                             clang::LangStandard::lang_cxx0x);

  compiler.createPreprocessor();
  compiler.getPreprocessorOpts().UsePredefines = false;
  compiler.createASTContext();

  // Parametrage du Rewriter
  Rewriter Rewrite;
  Rewrite.setSourceMgr(compiler.getSourceManager(), compiler.getLangOpts());

  // Reglage du fichier d'entree
  const FileEntry *pFile = compiler.getFileManager().getFile(fname);
  compiler.getSourceManager().createMainFileID(pFile);
  compiler.getDiagnosticClient().BeginSourceFile(compiler.getLangOpts(),
						 &compiler.getPreprocessor());

  // Instanciation de l'ASTVisitor
  CFGBuilderRecursiveASTVisitor astVisitor(compiler.getSourceManager());

  // Instanciation de l'ASTConsumer
  CFGBuilderASTConsumer astConsumer(astVisitor);

  // Parsing de l'AST
  ParseAST(compiler.getPreprocessor(),
	   &astConsumer,
	   compiler.getASTContext());

  compiler.getDiagnosticClient().EndSourceFile();
  return 0;
}





