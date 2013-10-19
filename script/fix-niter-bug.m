#!/usr/local/bin/MathematicaScript -script 
(*#!/usr/caen/mathematica-9.0/bin/MathematicaScript -script *)

ClearAll[argv,argc,Ns,Nd,phis,geo]
argv=$ScriptCommandLine[[2;;-1]];
argc=Length[argv];
cwd=Directory[];
If[argc<3,Print["takes three (four)  arguments: [Ns] [Nd] [phis] ([geo])"];Abort[];,0.0];
Ns=Round@ToExpression@argv[[1]];
Nd=Round@ToExpression@argv[[2]];
phis=ToExpression@argv[[3]];
If[argc>=4,geo=argc[[4]],geo="square";];

ClearAll[SolDirName,niter,Tsol]
SolDirName[Ns_,Nd_,phis_,geo_:"square"]:=FileNameJoin[{Directory[],geo<>ToString[Ns]<>"_solNd"<>ToString[Nd]<>"phis"<>ToString[phis]}]
filename=FileNameJoin[{SolDirName[Ns,Nd,phis,geo],"niter+time"}];

If[FileExistsQ[filename],Print[filename];;,Print[filename,"not found, abort..."];Abort[];];
{niter,Tsol}=Import[filename,"Table"]//Flatten;
Export[filename,{niter-1,Tsol},"Table"]

(*Print["niter(before)=",niter];*)
(*Print["niter(after)=",niter-1];*)
