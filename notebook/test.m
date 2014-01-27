#!/opt/Wolfram/Mathematica/9.0/Executables/MathKernel -script


ClearAll[argv,argc]
argv=$CommandLine[[4;;-1]];
argc=Length[argv];


Print[argv[[1]]];
Print[argv[[2]]];
Print[argv[[3]]];

