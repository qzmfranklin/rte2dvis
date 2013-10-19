#!/usr/local/bin/MathematicaScript -script

<<"MSH.m";

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
    (*Print["dat files (if any) in: ",datdir];*)
    Print["solutions (if any) in: ",soldir];
    CreateDirectory[soldir]//Quiet;
    ,
    Print["invalid filename"];Abort[];
	];,
  0.0;
  ];
If[argc>=2,Nd=Floor@ToExpression@argv[[2]];,Print["default Nd=1"];Nd=1;];
If[argc>=3,phis=ToExpression@argv[[3]];,Print["default phis=0.0"];phis=0.0;];

ClearAll[mus,mua,mut,g,f,f1,f2]
DumpMSHFileTo2DATFiles[filename];
mus[{x_,y_}]:=0.3
mua[{x_,y_}]:=0.4
mut[{x_,y_}]:=mua[{x,y}]+mus[{x,y}]
g=0.6;
f1[m_]:=g^Abs[m]
f2[m_]:=If[m==0,1.0,0.0]
f=f1;

ClearAll[iG,iL,Rank,ToCmplxPckdArry]
iG[{n_,m_},{Ns_,Nd_}]:=(n-1)*(2Nd+1)+m+Nd+1
iL[ng_,{Ns_,Nd_}]:={IntegerPart[(ng-1)/(2Nd+1)+1],Mod[ng-Nd-1,(2Nd+1),-Nd]}
Rank[x_]:=Length/@{x,x\[Transpose]}//Quiet
ToCmplxPckdArry=ToPackedArray[N[Re[#]]]+I ToPackedArray[N[Im[#]]]&;

ClearAll[TriPtPreCalc,CntrPreCalc,r2rXYPreCalc,r2rRPreCalc,r2rPhiPreCalc,AreaPreCalc]
TriPtPreCalc[p_,t_]:=p[[#]]&/@t//ToPackedArray
CntrPreCalc[pt_]:=(Total/@pt)/3.0//ToPackedArray
r2rXYPreCalc[cntr_]:=Outer[If[#1<#2,Subtract@@cntr[[{#1,#2}]],{0.0,0.0}]&,Range[Length[cntr]],Range[Length[cntr]]]//ToPackedArray//#-#\[Transpose]&
r2rRPreCalc[cntr_]:=Outer[If[#1<#2,Norm[Subtract@@cntr[[{#1,#2}]]],0.0]&,Range[Length[cntr]],Range[Length[cntr]]]//ToPackedArray//#+#\[Transpose]&
r2rPhiPreCalc[cntr_]:=Outer[If[#1!=#2,(Subtract@@cntr[[{#1,#2}]]//ArcTan[#[[1]],#[[2]]]&),0.0]&,Range[Length[cntr]],Range[Length[cntr]]]//ToPackedArray
AreaPreCalc[pt_]:=pt//Map[{#[[1]]-#[[2]],#[[1]]-#[[3]]}&,#]&//Map[#[[1,1]]#[[2,2]]-#[[1,2]]#[[2,1]]&,#]&//ToPackedArray//Abs//Divide[#,2.0]&;

ClearAll[p,t,Np,Nn,Nm,Ns,Ng,gTbl,iGTbl,iLTbl,pt,cntr,r2rXY,r2rR,r2rPhi,area,Tpt,Tcntr,Tr2rXY,Tr2rR,Tr2rPhi,Tarea,musTbl,muaTbl,mutTbl]
{p,t}=ReadMSHFileFrom2DATFiles[filename];

(*[>symmetrize the geometry w.r.p.t. y=0<]*)
(*np=length[p];*)
(*t=t//join[#,map[#+constantarray[np,3]&,t]]&;*)
(*p=join[p,{#[[1]],-#[[2]]}&/@p];*)
(*Np=Length[p];*)
(*[>end of symmetrization<]*)

(*[>symmetrize the geometry w.r.p.t. x=0<]*)
(*Np=Length[p];*)
(*t=t//Join[#,Map[#+ConstantArray[Np,3]&,t]]&;*)
(*p=Join[p,{-#[[1]],#[[2]]}&/@p];*)
(*Np=Length[p];*)
(*[>end of symmetrization<]*)

Ns=Length[t];Nn=Ns;Nm=2Nd+1;Ng=Nn*Nm;
Print["{p,t}=",PackedArrayQ/@{p,t}];
Print["{Ns,Nd,Nn,Nm,Ng}=",{Ns,Nd,Nn,Nm,Ng}];
gTbl=f/@Range[-Nd,Nd]//ToPackedArray;
iGTbl=Table[iG[{n,m},{Ns,Nd}],{n,Ns},{m,-Nd,Nd}]//ToPackedArray;
iLTbl=iL[#,{Ns,Nd}]&/@Range[Ng]//ToPackedArray;
Print["{gTbl,iGTbl,iLTbl}=",PackedArrayQ/@{gTbl,iGTbl,iLTbl}];
Tpt=(pt=TriPtPreCalc[p,t];)//AbsoluteTiming//#[[1]]&;
Tcntr=(cntr=CntrPreCalc[pt];)//AbsoluteTiming//#[[1]]&;
Tr2rXY=(r2rXY=r2rXYPreCalc[cntr];)//AbsoluteTiming//#[[1]]&;
Tr2rR=(r2rR=r2rRPreCalc[cntr];)//AbsoluteTiming//#[[1]]&;
Tr2rPhi=(r2rPhi=r2rPhiPreCalc[cntr];)//AbsoluteTiming//#[[1]]&;
Tarea=(area=AreaPreCalc[pt];)//AbsoluteTiming//#[[1]]&;
(*Print["{Tpt,Tcntr,Tr2rXY,Tr2rR,Tr2rPhi,Tarea}=",{Tpt,Tcntr,Tr2rXY,Tr2rR,Tr2rPhi,Tarea}//{Total[#],#}&];*)
Print["{pt,cntr,r2rXY,r2rR,r2rPhi,area}=",PackedArrayQ/@{pt,cntr,r2rXY,r2rR,r2rPhi,area}];
musTbl=mus/@cntr//ToPackedArray;
muaTbl=mua/@cntr//ToPackedArray;
mutTbl=musTbl+muaTbl;
Print["{musTbl,muaTbl,mutTbl}=",{musTbl,muaTbl,mutTbl}//Map[{PackedArrayQ[#],Rank[#]}&,#]&];

ClearAll[V,VRe,VIm,VElemtRe,VElemtIm,TVRe,TVIm]
VElemtRe=area[[#[[1]]]]Cos[#[[2]]phis]&;
VElemtIm=area[[#[[1]]]]Sin[#[[2]]phis]&;
VElemt=area[[#[[1]]]]Exp[-I#[[2]]phis]&;
TVRe=(VRe=1.0*VElemtRe[iLTbl[[#]]]&/@Range[Ng]//ToPackedArray)//AbsoluteTiming//#[[1]]&;
TVIm=(VIm=1.0*VElemtIm[iLTbl[[#]]]&/@Range[Ng]//ToPackedArray;)//AbsoluteTiming//#[[1]]&;
V=1.0*VRe-1.0I*VIm;
(*Print["{TVRe,TVIm}=",{TVRe,TVIm}//{Total[#],#}&];*)
Print["{V,VRe,VIm}=",PackedArrayQ/@{V,VRe,VIm}];

(*The following codes are for debug use only*)
(*Print["pt=",pt];*)
(*Print["musTbl=",musTbl];*)
(*Print["muaTbl=",muaTbl];*)
(*Print["mutTbl=",mutTbl];*)
(*Print["cntr=",cntr];*)
(*Print["area=",area];*)
(*Print["r2rXY=",r2rXY];*)
(*Print["r2rR=",r2rR];*)
(*Print["r2rPhi=",r2rPhi];*)
(*Print["V=",V//Chop//N];*)
(*Print["inputs.m passed symmetry test"];*)
