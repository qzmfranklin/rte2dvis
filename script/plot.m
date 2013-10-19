#!/usr/local/bin/MathematicaScript -script

<<"MSH.m";

ClearAll[argv,argc,cwd,dir,datdir,soldir,tmp,filebase,filename,Nd,phis]
argv=$ScriptCommandLine[[2;;-1]];
argc=Length[argv];

cwd=Directory[];
If[argc<3,Print["need 3 arguments, abort"];Abort[];,0.0];
tmp=FileNameJoin[{cwd,argv[[1]]}];
dir=DirectoryName[tmp];
filebase=FileNameJoin[{dir,FileBaseName[tmp]}];
soldir=FileNameJoin[{filebase<>sprtr<>"sol"}];
Nd=Floor@ToExpression@argv[[2]];
phis=ToExpression@argv[[3]]//N;
If[FileExistsQ[soldir]&&DirectoryQ[soldir],0.0;,Print["solution directory not found, abort"];Abort[];];
If[!IntegerQ[Nd],Print["2nd arg should be positive integer, abort"];Abort[];,0.0;];
If[!RealQ[phis],Print["3rd arg should be real, abort"];Abort[];,0.0;];
If[argc>3,Print["only the first 3 arguments are used"];,0.0;];

(*import solution with given Nd and phis*)
ClearAll[solRe,solIm,sol]
solRe=Import[FileNameJoin[{soldir,"sol"<>ToString[Nd]<>"_"<>ToString[phis]<>"re.dat"}]]//Flatten//ToPackedArray;
solIm=Import[FileNameJoin[{soldir,"sol"<>ToString[Nd]<>"_"<>ToString[phis]<>"im.dat"}]]//Flatten//ToPackedArray;
sol=solRe+I*solIm;
Print["{solRe,solIm,sol}=",PackedArrayQ/@{solRe,solIm,sol}];

ClearAll[InTriangle,PickTriangle]
InTriangle[v_,ptn_]:=Module[{det,v0,v1,v2,a,b}, det=#1[[1]]#2[[2]]-#1[[2]]#2[[1]]&;
v0=ptn[[1]]; v1=ptn[[2]]-v0; v2=ptn[[3]]-v0; a=(det[v,v2]-det[v0,v2])/det[v1,v2];
b=-(det[v,v1]-det[v0,v1])/det[v1,v2]; a>0.0&&b>0.0&&a+b<1.0 ]
PickTriangle[v_]:=Module[{n},For[n=1,n<=Ns,n++,If[InTriangle[v,pt[[n]]],Break[]]];n]

ClearAll[nXTbl,nTbl]
(*nXTbl=Table[{0.5,y},{y,0.01,0.99,0.9Sqrt[1.0/N[Ns]]}]//ToPackedArray;*)
nXTbl=Table[{x,0.5},{x,0.01,0.99,0.9*Sqrt[1.0/N[Ns]]}]//ToPackedArray;
nTbl=PickTriangle/@nXTbl;
(*cntr[[nTbl]][[All,1]]//ListPlot;*)

ClearAll[muaa]
Show[
ListPlot[Transpose@{nXTbl[[All,1]],solp[[nTbl,Nd+1]]//Re},PlotRange->{0,1/(2\[Pi])+0.05},PlotLabel->"Nd=10,Ns=948,phis=0.0,y=0.5",AxesLabel->{"x","fluence"},PlotStyle->{PointSize[Medium]},ImageSize->Large],
(*ListPlot[Transpose@{nXTbl[[All,2]],solp[[nTbl,Nd+1]]//Re},PlotRange->{0,1/(2\[Pi])},PlotLabel->"Nd=10,Ns=614,phis=\[Pi]/2,x=0.5",AxesLabel->{"y","fluence"},PlotStyle->{PointSize[Medium]},ImageSize->Large],*)
Plot[muaa=mua[{0.0,0.0}];Exp[-muaa(x)]/(2\[Pi]),{x,-1,1},PlotStyle->{Red,Dashed,Thick}]
]
(*ClearAll[data]*)
(*data={cntr,solp[[All,Nd+1]]}//Chop//Transpose;*)
(*ma=Interpolation[data]//Quiet;*)
(*DensityPlot[ma[x,y],{x,0,1},{y,0,1},AspectRatio->Automatic,ColorFunction->"SunsetColors",PlotLegends->Automatic,ImageSize->Large,AxesLabel->{"x","y"}]*)
