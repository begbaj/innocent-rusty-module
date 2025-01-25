source env

make -C $KDIR M=$PWD rust-analyzer
make -C $KDIR LLVM=$LLVMDIR rustdoc
