#!/opt/Wolfram/Mathematica/9.0/Executables/MathKernel -script

Needs["CCompilerDriver`"]
ClearAll[cflags,dir,incdir,libdir,xlib,src,lib]
cflags="-O3 -Wall -m64";
(*dir=FileNameJoin[{NotebookDirectory[],"../src/mll"}];*)
dir=".";
incdir="/opt/intel/mkl/include";
libdir="/opt/intel/mkl/lib/intel64";
xlib="";
(*xlib={"fftw3","mkl_core","mkl_intel_lp64","mkl_sequential","pthread","m"};*)
(*xlib="-lmkl_intel_lp64 -lmkl_core -lmkl_sequential -lpthread -lm";*)
(*xlib={"-lmkl_intel_lp64","-lmkl_core","-lmkl_sequential;","-lpthread","-lm"};*)
src={"wandzura-rule.cpp","dunavant-rule.cpp","legendre-rule.cpp","arcsinh-rule.cpp","mll.cpp"}//Map[FileNameJoin[{dir,#}]&,#]&;
lib=CreateLibrary[src,"libmll","Libraries"->xlib,"IncludeDirectories"->incdir,"LibraryDirectories"->libdir,"CompileOptions"->cflags,"TargetDirectory"->Directory[]];
