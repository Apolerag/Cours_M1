CXX := clang++
RTTIFLAG := -fno-rtti
LLVMCOMPONENTS := cppbackend
LLVMCONFIG := llvm-config-3.3
CLANG_PATH := tools/clang/include
CLANG_H := 	-I/usr/lib/llvm-3.3/include/
#	-I$(shell $(LLVMCONFIG) --src-root)/$(CLANG_PATH) \
#	-I$(shell $(LLVMCONFIG) --obj-root)/$(CLANG_PATH) \
	
# Flags de compilation
CXXEXT := -Wc++11-extensions
CXXFLAGS := $(shell $(LLVMCONFIG) --cxxflags) $(RTTIFLAG)
CXXFLAGS += -I/usr/include/i386-linux-gnu/c++/4.8/
LLVMLDFLAGS := $(shell $(LLVMCONFIG) --ldflags --libs $(LLVMCOMPONENTS))


SRC = cfgdumper.cpp 
OBJ = $(SRC:.cpp=.o)
BIN = $(OBJ:.o=)

CLANG_LIBS = \
	-lclangTooling \
	-lclangFrontendTool \
	-lclangFrontend \
	-lclangDriver \
	-lclangSerialization \
	-lclangCodeGen \
	-lclangParse \
	-lclangSema \
	-lclangStaticAnalyzerFrontend \
	-lclangStaticAnalyzerCheckers \
	-lclangStaticAnalyzerCore \
	-lclangAnalysis \
	-lclangARCMigrate \
	-lclangRewriteFrontend \
	-lclangRewriteCore \
	-lclangEdit \
	-lclangAST \
	-lclangLex \
	-lclangBasic \
	$(shell $(LLVMCONFIG) --libs)

all: $(OBJ) $(BIN)

%: %.o
	$(CXX) -I/usr/include/i386-linux-gnu/c++/4.8/		 -o $@ $< $(CLANG_H) $(LLVMLDFLAGS) $(CLANG_LIBS) 

clean:
	-rm -fv $(BIN) $(OBJ) *~
