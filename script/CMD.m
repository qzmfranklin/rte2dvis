#!/usr/caen/mathematica-9.0/bin/MathematicaScript -script 
(*#!/usr/local/bin/MathematicaScript -script*) 

ClearAll[argv,argc,cwd,dir,datdir,tmp,filebase,filename,Nd,phis]
argv=$ScriptCommandLine[[2;;-1]];
argc=Length[argv];
cwd=Directory[];
If[argc<3,Print["takes three arguments: [filename] [Nd] [phis]"];Abort[];,0.0];
If[argc>=1,
  tmp=FileNameJoin[{cwd,argv[[1]]}];
  dir=DirectoryName[tmp];
  filebase=FileNameJoin[{dir,FileBaseName[tmp]}];
  filename=filebase<>".msh";
  datdir=FileNameJoin[{filebase<>sprtr<>"dat"}];
  If[FileExistsQ[filename]&&!DirectoryQ[filename],
	Print["dat files (if any) in: ",datdir];
    ,
    Print["invalid filename"];Abort[];
	];,
  0.0;
  ];
If[argc>=2,Nd=Floor@ToExpression@argv[[2]];,Print["default Nd=1"];Nd=1;];
If[argc>=3,phis=ToExpression@argv[[3]];,Print["default phis=0.0"];phis=0.0;];
