//===--- tokendumper.cpp ----------------------------------------*- C++ -*-===//
//                                    AUTHOR
// Name: Christophe Bacara
// Mail: christophe dot bacara at etudiant dot univ hyphen lille1 dot fr
// 
// Resp.: Laure Gonnord - INRIA/IRCICA - DaRT/Emeraude
//
//===----------------------------------------------------------------------===//
//
// Front-end example : parse the AST, dumping every token along the process.
//
//===----------------------------------------------------------------------===//
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

#include "llvm/Support/Host.h"
#include "llvm/ADT/IntrusiveRefCntPtr.h"
#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Lex/Token.h"

#define PROGRAM_NAME "tokendumper"

using namespace llvm;
using namespace clang;

int main(int argc, char *argv[])
{
  struct stat filestat;

  // Single argument check
  if (argc < 2) {
    llvm::errs() << "Usage: "PROGRAM_NAME" <filename>\n";
    return 1;
  }

  // File existence check
  std::string fname(argv[argc - 1]);
  if (stat(fname.c_str(), &filestat) == - 1) {
    perror(fname.c_str());
    exit(EXIT_FAILURE);
  }
  
  CompilerInstance ci;
  DiagnosticOptions diagnosticOptions;
  TextDiagnosticPrinter *pTextDiagnosticPrinter =  new TextDiagnosticPrinter(llvm::outs(), &diagnosticOptions, true);
  ci.createDiagnostics(pTextDiagnosticPrinter);
  
  // Reglage des informations sur la cible (need more explanation !)
  clang::IntrusiveRefCntPtr<TargetOptions> pto(new TargetOptions());
  pto->Triple = llvm::sys::getDefaultTargetTriple();
  TargetInfo *pti = TargetInfo::CreateTargetInfo(ci.getDiagnostics(), pto.getPtr());
  ci.setTarget(pti);
  
  // Creation des elements necessaires
  ci.createFileManager();
  ci.createSourceManager(ci.getFileManager());
  ci.createPreprocessor();
  
  // Recuperation du fichier specifie
  const clang::FileEntry *pFile = ci.getFileManager().getFile(argv[1]);
  // Reglage du compilateur sur ce fichier
  ci.getSourceManager().createMainFileID(pFile);
  ci.getPreprocessor().EnterMainSourceFile();
  ci.getDiagnosticClient().BeginSourceFile(ci.getLangOpts(),
					   &ci.getPreprocessor());
  
  clang::Preprocessor &pp = ci.getPreprocessor();
  clang::Token tok;

  // Parse/dump loop
  do {
    pp.Lex(tok);

    if (ci.getDiagnostics().hasErrorOccurred())
      break;

    pp.DumpToken(tok);
    errs() << "\n";
  } while (tok.isNot(clang::tok::eof));
  
 ci.getDiagnosticClient().EndSourceFile();
  
  return 0;
}
