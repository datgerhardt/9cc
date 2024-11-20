# Set up on Mac M1

`env /usr/bin/arch -x86_64 /bin/zsh --login`

### don't use the Static linking flag `-static`
Static linking does not work on mac os x with the llvm/clang gcc.
If you use a gcc from homebrew/macports, and use that to build a statically linked toolchain on mac os x, then that will work.
If you are not building a statically linked toolchain, make sure you're not enabling static libstdc++ in the compiler settings.