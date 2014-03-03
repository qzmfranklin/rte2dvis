#!/opt/Wolfram/Mathematica/9.0/Executables/MathKernel -script

ClearAll[lib]
lib="libmll";

ClearAll[LegendreRule,DunavantRule,ArcSinhRule]
LegendreRule=LibraryFunctionLoad[lib,"LegendreRule_MLL",{Integer,Real,Real},{Real,2,"Shared"}];
DunavantRule=LibraryFunctionLoad[lib,"DunavantRule_MLL",{Integer,{Real,2,"Constant"}},{Real,2,"Shared"}];
ArcSinhRule=LibraryFunctionLoad[lib,"ArcSinhRule_MLL",{{Real,2,"Constant"},{Real,1,"Constant"},{Real,2,"Constant"},{Real,2,"Constant"}},{Real,2,"Shared"}];

ClearAll[VHomoFull,BHomo,BHomoFull,BHomoS,BHomoN,HomoMul]
VHomoFull=LibraryFunctionLoad[lib,"VHomoFull_MLL",{{Real,3,"Constant"},Real,Real,Real,Integer,Integer,Integer,Integer,Integer},{Complex,1,"Shared"}];
BHomo=LibraryFunctionLoad[lib,"BHomo_MLL",{{Real,3,"Constant"},Integer,Integer,Integer,Integer,Integer},{Real,3,"Shared"}];
BHomoFull=LibraryFunctionLoad[lib,"BHomoFull_MLL",{{Real,3,"Constant"},Integer,Integer,Integer,Integer,Integer,Integer},{Real,3,"Shared"}];
BHomoS=LibraryFunctionLoad[lib,"BHomoS_MLL",{Integer,Integer,{Real,1,"Constant"},{Real,2,"Constant"}},{Real,1,"Shared"}];
BHomoN=LibraryFunctionLoad[lib,"BHomoN_MLL",{Integer,Integer,{Real,1,"Constant"},{Real,2,"Constant"}},{Real,1,"Shared"}];
HomoMul=LibraryFunctionLoad[lib,"HomoMul_MLL",{{Complex,1,"Constant"},{Real,1,"Constant"},{Real,3,"Constant"},{Real,1,"Constant"},Real,Real},{Complex,1,"Shared"}];

(*ClearAll[test,raw]*)
(*test=LibraryFunctionLoad[lib,"test",{{Complex,1,"Constant"}},{Complex,2,"Shared"}];*)
(*raw=RandomComplex[1+I,500000];*)
(*test[raw]//Quiet//AbsoluteTiming//#[[1]]&//Print;*)
(*ClearAll[raw]*)


{{"rule","order","positive"}}~Join~Table[{rule,DunavantRule[rule,{{0,0},{0,1},{1,0}}]//Dimensions//#[[2]]&,DunavantRule[rule,{{0,0},{0,1},{1,0}}]//Quiet//Map[Positive,#]&//Flatten//DeleteDuplicates//#[[1]]&},{rule,20}]//Quiet//TableForm
ClearAll[pp,p0,nu,nv,qu,qv]
LegendreRule[10,0,1]//Quiet;
DunavantRule[10,{{1,0},{3,2},{0,2}}]//Quiet;
pp={{1,0},{0,2},{3,2}};
p0={3,0};
{nu,nv}={10,2};
(qu=LegendreRule[nu,0,1])//Print;
(qv=LegendreRule[nv,0,1])//Print;
(*(ArcSinhRule[pp,p0,qu,qv])//Print;*)
ClearAll[pp,p0,nu,nv,qu,qv]

(*LibraryFunctionUnload[test];*)
(*LibraryUnload[lib] *) 
