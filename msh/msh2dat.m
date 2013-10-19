#!/usr/local/bin/MathematicaScript -script

<<"MSH.m";

ClearAll[BatchProcess,argv,argc,cwd,tmp,dir,cmdstr]
(*dump all .msh files in dir, not recursively*)
BatchProcess[dir_]:=Module[{list,i},
list=FileNames["*.msh",{dir}];
For[i=1,i<=Length[list],i++,DumpMSHFileTo2DATFiles[list[[i]]];];
]
argv=$ScriptCommandLine[[2;;-1]];
argc=Length[argv];
cwd=Directory[];
If[argc==0,dir=cwd;,0.0;];
If[argc==1,
If[argv[[1]]=="clean",cmdstr="rm -rf *"<>sprtr<>"dat "<>"*"<>sprtr<>"sol";Print[cmdstr];Run[cmdstr];Abort[];,0.0];
tmp=FileNameJoin[{cwd,argv[[1]]}];
If[FileExistsQ[tmp]==False,Print["file or directory not found, abort"];Abort[];,0.0;];
If[DirectoryQ[tmp]==False,
DumpMSHFileTo2DATFiles[tmp];Abort[];,
dir=FileNameJoin[{cwd,argv[[1]]}];
];,
0.0;
];
BatchProcess[dir];
