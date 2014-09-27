svn co http://llvm.org/svn/llvm-project/llvm/trunk llvm

#Checkout Clang:

cd llvm/tools
svn co http://llvm.org/svn/llvm-project/cfe/trunk clang
cd ../..

#Checkout extra Clang Tools: (optional)

cd llvm/tools/clang/tools
svn co http://llvm.org/svn/llvm-project/clang-tools-extra/trunk extra
cd ../../../..

#Checkout Compiler-RT:

cd llvm/projects
svn co http://llvm.org/svn/llvm-project/compiler-rt/trunk compiler-rt
cd ../..

#Build LLVM and Clang:

mkdir build #(for building without polluting the source dir)
cd build
../llvm/configure
make
