#!/usr/local/bin/MathematicaScript -script

<<"inputs.m";

ClearAll[argv,argc,cwd,dir,datdir,soldir,tmp,filebase,filename,Nd,phis]
argv=$ScriptCommandLine[[2;;-1]];
argc=Length[argv];
cwd=Directory[];
If[argc==0,Abort[];,0.0];
If[argc>=1,
  tmp=FileNameJoin[{cwd,argv[[1]]}];
  dir=DirectoryName[tmp];
  filebase=FileNameJoin[{dir,FileBaseName[tmp]}];
  filename=filebase<>".msh";
  datdir=FileNameJoin[{filebase<>sprtr<>"dat"}];
  soldir=FileNameJoin[{filebase<>sprtr<>"sol"}];
  If[FileExistsQ[filename]&&!DirectoryQ[filename],
	Print["dat files (if any) in: ",datdir];
	Print["solutions (if any) in: ",soldir];
    CreateDirectory[soldir]//Quiet;
    ,
    Print["invalid filename"];Abort[];
	];,
  0.0;
  ];
If[argc>=2,Nd=Floor@ToExpression@argv[[2]];,Print["default Nd=1"];Nd=1;];
If[argc>=3,phis=ToExpression@argv[[3]];,Print["default phis=0.0"];phis=0.0;];
 
ClearAll[Zon1,Zon2,Zoff,vTblC,uTblC,TZon1,TZon2,TZoff,TvTblC,TuTblC]
TZon1=(Zon1=2\[Pi] area+2\[Pi]^(1/2) mutTbl area^(3/2);)//AbsoluteTiming//#[[1]]&;
TZon2=(Zon2=-2 \[Pi]^(1/2) musTbl area^(3/2);)//AbsoluteTiming//#[[1]]&;
TZoff=(Zoff=Outer[If[#2>#1,area[[#1]]area[[#2]]/r2rR[[#1,#2]],0.0]&,Range[Ns],Range[Ns]]//ToPackedArray//#+#\[Transpose]&;)//AbsoluteTiming//#[[1]]&;

TvTblC=(vTblC=Compile[{{r2rPhi,_Real,2},{Ns,_Integer},{Nd,_Integer}},
Table[Exp[I r2rPhi[[n,np]]Range[-Nd,Nd]],{n,Ns},{np,Ns}],
Parallelization->True,
CompilationTarget:>"C"
][r2rPhi,Ns,Nd];)//AbsoluteTiming//#[[1]]&;
TuTblC=(uTblC=Compile[{{musTbl,_Real,1},{mutTbl,_Real,1},{gTbl,_Real,1},{vTblC,_Complex,3},{r2rPhi,_Real,2},{Ns,_Integer}},
Table[mutTbl[[np]]vTblC[[n,np]]-musTbl[[np]]gTbl vTblC[[n,np]],{n,Ns},{np,Ns}],
Parallelization->True,
CompilationTarget:>"C"
][musTbl,mutTbl,gTbl,vTblC,r2rPhi,Ns];
)//AbsoluteTiming//#[[1]]&;

Print["{TZon1,TZon2,TZoff,TvTblC,TuTblC}=",{TZon1,TZon2,TZoff,TvTblC,TuTblC}//{Total[#],#}&];
Print["{Zon1,Zon2,Zoff,vTblC,uTblC}=",PackedArrayQ/@{Zon1,Zon2,Zoff,vTblC,uTblC}];

ClearAll[ZDotXC,ZDotXCcounter]
ZDotXCcounter=0;
ZDotXC=Compile[{{X,_Complex,1},{Ns,_Integer},{Nd,_Integer},{gTbl,_Real,1},{Zon1,_Real,1},{Zon2,_Real,1},{Zoff,_Real,2},{vTblC,_Complex,3},{uTblC,_Complex,3}},
Module[{Xp,Yp,temp,n,np},
Xp=Partition[X,2Nd+1];
Flatten@Table[
temp=ConstantArray[0.0+0.0I,2Nd+1];
For[np=1,np<=Ns,np++,
If[np==n,
temp+=Zon1[[n]]Xp[[n]]+Zon2[[n]]gTbl Xp[[n]];,
temp+=Zoff[[n,np]](Xp[[np]].uTblC[[n,np]])vTblC[[n,np]]\[Conjugate];
];
];
temp
,{n,Ns}]
],
Parallelization->True,
CompilationTarget:>"C"
][#,Ns,Nd,gTbl,Zon1,Zon2,Zoff,vTblC,uTblC]&;
Print["ZDotXC[V] time=",ZDotXC[V]//AbsoluteTiming//#[[1]]&];
ClearAll[sol2,Tsol2,fsol2Re,fsol2Im]
Tsol2=(sol2=LinearSolve[ZDotXC,V,Method->{"Krylov",Tolerance->Norm[V]*10^-7}];)//AbsoluteTiming//#[[1]]&;
Print["Tsol2=",Tsol2];
Print["Norm[ZDotXC[sol2]-V]/Norm[V]=",Norm[ZDotXC[sol2]-V]/Norm[V]];
sol2p=Partition[sol2,Nm];
Print["sym sol2=",Table[Norm[sol2p[[n]]-sol2p[[n+Ns/2]]\[Conjugate]],{n,1,Ns/2-1,1}]//Norm];

fsol2Re=FileNameJoin[{soldir,"sol"<>ToString[Nd]<>"_"<>ToString[phis]<>"re.dat"}];
fsol2Im=FileNameJoin[{soldir,"sol"<>ToString[Nd]<>"_"<>ToString[phis]<>"im.dat"}];
Export[fsol2Re,N@Re@sol2];
Export[fsol2Im,N@Im@sol2];
