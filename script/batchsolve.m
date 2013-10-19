#!/usr/caen/mathematica-9.0/bin/MathematicaScript -script
(*#!/usr/local/bin/MathematicaScript -script*)

<<"MSH.m";

ClearAll[argv,argc,cwd,dir,datdir,tmp,filebase,filename,Nd,phis]
argv=$ScriptCommandLine[[2;;-1]];
argc=Length[argv];
cwd=Directory[];
If[argc<3,Print["takes three arguments: [filename] [Nd] [phis]"];Abort[];,0.0];
tmp=FileNameJoin[{cwd,argv[[1]]}];
dir=DirectoryName[tmp];
filebase=FileNameJoin[{dir,FileBaseName[tmp]}];
filename=filebase<>".msh";
If[FileExistsQ[filename]&&!DirectoryQ[filename], 0.0;, Print["invalid filename: ",filename];Abort[]; ];
Nd=Floor@ToExpression@argv[[2]];
phis=ToExpression@argv[[3]];
Print["filename=",filename];

ClearAll[mus,mua,mut,g,f,f1,f2,func,func1]
(*filename=FileNameJoin[{Directory[],"../msh","square90.msh"}];*)
(*Print["filename=",filename];*)
(*phis=0;*)
(*Nd=10;*)
DumpMSHFileTo2DATFiles[filename];
mus[{x_,y_}]:=0.5Log[2.0]
mua[{x_,y_}]:=Log[2.0]
mut[{x_,y_}]:=mua[{x,y}]+mus[{x,y}]
g=0.6;
f1[m_]:=g^Abs[m]
f2[m_]:=If[m==0,1.0,0.0]
f=f1;
func1=Compile[{{phi,_Real}},(1.0-g^2)/(2\[Pi](1+g^2-2g Cos[phi])),CompilationTarget:>"C"];
func=func1;

ClearAll[iG,iL,Rank,ToCmplxPckdArry]
iG[{n_,m_},{Ns_,Nd_}]:=(n-1)*(2Nd+1)+m+Nd+1
iL[ng_,{Ns_,Nd_}]:={IntegerPart[(ng-1)/(2Nd+1)+1],Mod[ng-Nd-1,(2Nd+1),-Nd]}
Rank[x_]:=Length/@{x,x\[Transpose]}//Quiet
ToCmplxPckdArry=ToPackedArray[N[Re[#]]]+I ToPackedArray[N[Im[#]]]&;

ClearAll[TriPtPreCalc,CntrPreCalc,r2rXYPreCalc,r2rRPreCalc,r2rPhiPreCalc,r2rFPreCalc,AreaPreCalc]
TriPtPreCalc[p_,t_]:=p[[#]]&/@t//ToPackedArray
CntrPreCalc[pt_]:=(Total/@pt)/3.0//ToPackedArray
r2rXYPreCalc[cntr_]:=Outer[If[#1<#2,Subtract@@cntr[[{#1,#2}]],{0.0,0.0}]&,Range[Length[cntr]],Range[Length[cntr]]]//ToPackedArray//#-#\[Transpose]&
r2rRPreCalc[cntr_]:=Outer[If[#1<#2,Norm[Subtract@@cntr[[{#1,#2}]]],0.0]&,Range[Length[cntr]],Range[Length[cntr]]]//ToPackedArray//#+#\[Transpose]&
r2rPhiPreCalc[cntr_]:=Outer[If[#1!=#2,(Subtract@@cntr[[{#1,#2}]]//ArcTan[#[[1]],#[[2]]]&),0.0]&,Range[Length[cntr]],Range[Length[cntr]]]//ToPackedArray
r2rFPreCalc[r2rPhi_,phis_]:=Map[func[#-phis]&,r2rPhi,{2}];
AreaPreCalc[pt_]:=pt//Map[{#[[1]]-#[[2]],#[[1]]-#[[3]]}&,#]&//Map[#[[1,1]]#[[2,2]]-#[[1,2]]#[[2,1]]&,#]&//ToPackedArray//Abs//Divide[#,2.0]&;

ClearAll[Np,p,t,Nn,Nm,Ns,Ng,gTbl,iGTbl,iLTbl,pt,cntr,r2rXY,r2rR,r2rPhi,r2rF,area,Tpt,Tcntr,Tr2rXY,Tr2rR,Tr2rPhi,Tr2rF,Tarea,musTbl,muaTbl,mutTbl]
{p,t}=ReadMSHFileFrom2DATFiles[filename];

(*[>symmetrize the geometry w.r.p.t. y=0<]*)
(*Np=Length[p];*)
(*t=t//Join[#,Map[#+ConstantArray[Np,3]&,t]]&;*)
(*p=Join[p,{#[[1]],-#[[2]]}&/@p];*)
(*Np=Length[p];*)
(*[>end of symmetrization<]*)

(*[>symmetrize the geometry w.r.p.t. x=0<]*)
(*Np=Length[p];*)
(*t=t//Join[#,Map[#+ConstantArray[Np,3]&,t]]&;*)
(*p=Join[p,{-#[[1]],#[[2]]}&/@p];*)
(*Np=Length[p];*)
(*[>end of symmetrization<]*)

Np=Length[p];Ns=Length[t];Nn=Ns;Nm=2Nd+1;Ng=Nn*Nm;
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
Tr2rF=(r2rF=Map[func[#-phis]&,r2rPhi,{2}];)//AbsoluteTiming//#[[1]]&;
Tarea=(area=AreaPreCalc[pt];)//AbsoluteTiming//#[[1]]&;
(*Print["{Tpt,Tcntr,Tr2rXY,Tr2rR,Tr2rPhi,Tr2rF,Tarea}=",{Tpt,Tcntr,Tr2rXY,Tr2rR,Tr2rPhi,Tr2rF,Tarea}//{Total[#],#}&];*)
Print["{pt,cntr,r2rXY,r2rR,r2rPhi,r2rF,area}=",PackedArrayQ/@{pt,cntr,r2rXY,r2rR,r2rPhi,r2rF,area}];
musTbl=mus/@cntr//ToPackedArray;
muaTbl=mua/@cntr//ToPackedArray;
mutTbl=musTbl+muaTbl;
Print["{musTbl,muaTbl,mutTbl}=",{musTbl,muaTbl,mutTbl}//Map[{PackedArrayQ[#],Rank[#]}&,#]&];


ClearAll[V,VRe,VIm,VElemtRe,VElemtIm,TVRe,TVIm]
VElemtRe=area[[#[[1]]]]Cos[#[[2]]phis]&;
VElemtIm=area[[#[[1]]]]Sin[#[[2]]phis]&;
VElemt=area[[#[[1]]]]Exp[-I#[[2]]phis]&;
TVRe=(VRe=1.0*VElemtRe[iLTbl[[#]]]&/@Range[Ng]//ToPackedArray;)//AbsoluteTiming//#[[1]]&;
TVIm=(VIm=1.0*VElemtIm[iLTbl[[#]]]&/@Range[Ng]//ToPackedArray;)//AbsoluteTiming//#[[1]]&;
V=1.0*VRe-1.0I*VIm;
Print["{TVRe,TVIm}=",{TVRe,TVIm}//{Total[#],#}&];
Print["{V,VRe,VIm}=",PackedArrayQ/@{V,VRe,VIm}];
Print["Norm[V]=",Norm[V]];
ClearAll[Zon1,Zon2,Zoff,vTblC,uTblC,TZon1,TZon2,TZoff,TvTblC,TuTblC]
TZon1=(Zon1=2\[Pi] area+2\[Pi]^(1/2) mutTbl area^(3/2);)//AbsoluteTiming//#[[1]]&;
TZon2=(Zon2=-2 \[Pi]^(1/2) musTbl area^(3/2);)//AbsoluteTiming//#[[1]]&;
TZoff=(Zoff=Outer[If[#2>#1,area[[#1]]area[[#2]]/r2rR[[#1,#2]],0.0]&,Range[Ns],Range[Ns]]//ToPackedArray//#+#\[Transpose]&;)//AbsoluteTiming//#[[1]]&;

TvTblC=(vTblC=Compile[{{r2rPhi,_Real,2},{Ns,_Integer},{Nd,_Integer}},
  Table[Exp[-I r2rPhi[[n,np]]Range[-Nd,Nd]],{n,Ns},{np,Ns}],
  Parallelization->True
  ,CompilationTarget:>"C"
][r2rPhi,Ns,Nd];)//AbsoluteTiming//#[[1]]&;
TuTblC=(uTblC=Compile[{{musTbl,_Real,1},{mutTbl,_Real,1},{gTbl,_Real,1},{vTblC,_Complex,3},{r2rPhi,_Real,2},{Ns,_Integer}},
  Table[mutTbl[[np]]vTblC[[n,np]]\[Conjugate]-musTbl[[np]]gTbl vTblC[[n,np]]\[Conjugate],{n,Ns},{np,Ns}],
  Parallelization->True
  ,CompilationTarget:>"C"
][musTbl,mutTbl,gTbl,vTblC,r2rPhi,Ns];
)//AbsoluteTiming//#[[1]]&;

(*Print["{TZon1,TZon2,TZoff,TvTblC,TuTblC}=",{TZon1,TZon2,TZoff,TvTblC,TuTblC}//{Total[#],#}&];*)
Print["{Zon1,Zon2,Zoff,vTblC,uTblC}=",PackedArrayQ/@{Zon1,Zon2,Zoff,vTblC,uTblC}];

ClearAll[ZDotXC0,ZDotXC,ZDotXCcounter]
ZDotXCcounter=0;
ZDotXC0=Compile[{{X,_Complex,1},{Ns,_Integer},{Nd,_Integer},{gTbl,_Real,1},{Zon1,_Real,1},{Zon2,_Real,1},{Zoff,_Real,2},{vTblC,_Complex,3},{uTblC,_Complex,3}},
  Module[{Xp,Yp,temp,n,np},
    Xp=Partition[X,2Nd+1];
    Flatten@Table[
      temp=ConstantArray[0.0+0.0I,2Nd+1];
      For[np=1,np<=Ns,np++,
        If[np==n,
          temp+=Zon1[[n]]Xp[[n]]+Zon2[[n]]gTbl Xp[[n]];,
          temp+=Zoff[[n,np]](Xp[[np]].uTblC[[n,np]])vTblC[[n,np]];
        ];
      ];
      temp
      ,{n,Ns}]
  ],
  Parallelization->True
  ,CompilationTarget:>"C"
][#,Ns,Nd,gTbl,Zon1,Zon2,Zoff,vTblC,uTblC]&;
ZDotXC[V_]:=Module[{},ZDotXCcounter++;ZDotXC0[V]]
Print["ZDotXC[V] time=",ZDotXC[V]//AbsoluteTiming//#[[1]]&];

ClearAll[InTriangle,PickTriangle]
InTriangle[v_,ptn_]:=Module[{det,v0,v1,v2,a,b},
  det=#1[[1]]#2[[2]]-#1[[2]]#2[[1]]&;
  v0=ptn[[1]];
  v1=ptn[[2]]-v0;
  v2=ptn[[3]]-v0;
  a=(det[v,v2]-det[v0,v2])/det[v1,v2];
  b=-(det[v,v1]-det[v0,v1])/det[v1,v2];
  a>0.0&&b>0.0&&a+b<1.0
]
PickTriangle[v_]:=Module[{n},For[n=1,n<=Ns,n++,If[InTriangle[v,pt[[n]]],Break[]]];n]
ClearAll[HalfLineTriangleSegmentC,P0,PT,alpha]
HalfLineTriangleSegmentC=Compile[{{alpha,_Real},{p,_Real,1},{pt,_Real,2}},
  Module[{n,d,ds,pts,k,t},
    n={Cos[alpha],Sin[alpha]};d=Det[{#-p,n}]&/@pt;ds=Sort[d];
    If[ds[[1]]ds[[3]]<=0.0&&!(Abs[ds[[1]]]<1.0*10^-7&&Abs[ds[[3]]]<1.0*10^-7),0.0;,Return[0.0];];
    k={0,0,0};
    k[[1]]=Position[d,ds[[1]]][[1,1]];k[[3]]=Position[d,ds[[3]]][[1,1]];
    k[[2]]=Delete[Range[3],{{k[[1]]},{k[[3]]}}][[1]];pts=pt[[k[[#]]]]&/@Range[3];
    t=If[ds[[1]]*ds[[2]]>=0,
    {-Det[{pts[[3]]-pts[[1]],p-pts[[1]]}]/Det[{pts[[3]]-pts[[1]],n}],-Det[{pts[[3]]-pts[[2]],p-pts[[2]]}]/Det[{pts[[3]]-pts[[2]],n}]},
    {-Det[{pts[[2]]-pts[[1]],p-pts[[1]]}]/Det[{pts[[2]]-pts[[1]],n}],-Det[{pts[[3]]-pts[[1]],p-pts[[1]]}]/Det[{pts[[3]]-pts[[1]],n}]}
    ];
    t=Sort[t];
    If[t[[1]]<0.0,t[[1]]=0.0;,0.0;];
    If[t[[2]]<0.0,t[[2]]=0.0;,0.0;];
    t[[2]]-t[[1]]
  ]
  ,CompilationTarget:>"C"
];
ClearAll[damp,Tdamp1,damp1,Tdamp2,damp2,dampLength]
Tdamp1=(damp1=Exp[-mutTbl[[1]]cntr[[All,1]]]//ToPackedArray;)//AbsoluteTiming//#[[1]]&;
Print["Tdamp1=",Tdamp1];
Print["damp1=",damp//PackedArrayQ];

dampLength[n_]:=Module[{np,tau=0.0},For[np=1,np<=Ns,np++,tau+=mutTbl[[np]]*HalfLineTriangleSegmentC[phis+\[Pi],cntr[[n]],pt[[np]]];];tau]
Tdamp2=(damp2=Exp[ParallelTable[-dampLength[n],{n,Ns}]];)//AbsoluteTiming//#[[1]]&;
Print["Tdamp2=",Tdamp2]
Print["damp2=",damp2//PackedArrayQ];

Print["Norm[damp2-damp1]/Norm[damp1]=",Norm[damp2-damp1]/Norm[damp1]];
damp=damp2;
ClearAll[Von,Voff,Vt,TVt]
TVt=(
  Von=musTbl Sqrt[area/\[Pi]]damp;
  Voff=((Zoff r2rF)\[Transpose](damp musTbl))\[Transpose];
  Vt=Von (Partition[V,Nm]\[Transpose]g)\[Transpose];
  For[n=1,n<=Ns,n++,
    For[np=1,np<=Ns,np++,
      Vt[[n]]+=Voff[[n,np]]vTblC[[n,np]];
    ];
  ];
  Vt=Flatten[Vt];
  )//AbsoluteTiming//#[[1]]&;
Print["Vt=",PackedArrayQ[Vt]];
Print["TVt=",TVt];
ClearAll[sol2t,Tsol2t,sol2tp]
ZDotXCcounter=0;
Tsol2t=(sol2t=LinearSolve[ZDotXC[#]&,Vt,Method->{"Krylov",Tolerance->Norm[Vt]*10^-7}];)//AbsoluteTiming//#[[1]]&;
Print["ZDotXCcounter=",ZDotXCcounter];
Print["Tsol2t=",Tsol2t];
Print["Norm[ZDotXC[sol2t]-Vt]/Norm[Vt]=",Norm[ZDotXC[sol2t]-Vt]/Norm[Vt]];
sol2tp=Partition[sol2t,Nm];
Print["sym sol2t=",Table[Norm[sol2tp[[n]]-sol2tp[[n+Ns/2]]\[Conjugate]],{n,1,Ns/2-1,1}]//Norm];
Print["Nd residual=",(sol2tp[[All,1]]//Map[Norm,#]&//Norm)/(sol2tp[[All,Nd+1]]//Map[Norm,#]&//Norm)];
ClearAll[SaveSolution]
SaveSolution[filename_,Nd_,phis_,sol_]:=Module[{soldir},
  soldir=DirectoryName[filename]<>FileBaseName[filename]<>sprtr<>"sol"<>"Nd"<>ToString[Nd]<>"phis"<>ToString[phis];
  CreateDirectory[soldir]//Quiet;
  {Export[FileNameJoin[{soldir,"niter+time"}],{ZDotXCcounter,Tsol2t},"Table"]
  ,Export[FileNameJoin[{soldir,"re.dat"}],N@Re[sol]]
  ,Export[FileNameJoin[{soldir,"im.dat"}],N@Im[sol]]}//Parallelize//Quiet;
]
Print["save solution time=",SaveSolution[filename,Nd,phis,sol2t]//AbsoluteTiming//#[[1]]&];
ClearAll[ReadSolution,sol2tr]
ReadSolution[filename_,Nd_,phis_]:=Module[{soldir},
  soldir=DirectoryName[filename]<>FileBaseName[filename]<>sprtr<>"sol"<>"Nd"<>ToString[Nd]<>"phis"<>ToString[phis];
  {Import[FileNameJoin[{soldir,"niter+time"}],"Table"]//Flatten
  ,Import[FileNameJoin[{soldir,"re.dat"}]]+I*Import[FileNameJoin[{soldir,"im.dat"}]]//Flatten}
]
ClearAll[niter,Tsol]
Print["read solution time=",({{niter,Tsol},sol2tr}=ReadSolution[filename,Nd,phis];)//AbsoluteTiming//#[[1]]&];
Print["{nitr,Tsol}=",{niter,Tsol}];
Print["sol2tr=",sol2tr//PackedArrayQ]
