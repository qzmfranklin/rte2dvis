#!/usr/local/bin/MathematicaScript -script

<<"inputs.m";

ClearAll[Z1,ZElemt1,TZ1,TZ1p]
ZElemt1=If[#1[[1]]==#2[[1]],If[#1[[2]]==#2[[2]],2\[Pi] area[[#1[[1]]]](1+(-musTbl[[#1[[1]]]]gTbl[[#1[[2]]+Nd+1]]+mutTbl[[#1[[1]]]])Sqrt[area[[#1[[1]]]]/\[Pi]]),0.0],area[[#1[[1]]]]area[[#2[[1]]]](-musTbl[[#2[[1]]]]gTbl[[#2[[2]]+Nd+1]]+mutTbl[[#2[[1]]]])Exp[-I(#1[[2]]-#2[[2]])r2rPhi[[#1[[1]],#2[[1]]]]]/r2rR[[#1[[1]],#2[[1]]]]]&;
TZ1=(Z1=Outer[ZElemt1[iLTbl[[#1]],iLTbl[[#2]]]&,Range[Ng],Range[Ng]]//ToPackedArray;)//AbsoluteTiming//#[[1]]&;
TZ1p=(Z1=ToCmplxPckdArry[Z1];)//AbsoluteTiming//#[[1]]&;
(*Print["{TZ1,TZ1p}=",{TZ1,TZ1p}//{Total[#],#}&];*)
Print["Z1=",PackedArrayQ[Z1]];

ClearAll[sol1,Tsol1,fsol1Re,fsol1Im]
Tsol1 = (sol1 = LinearSolve[Z1,V] ) // AbsoluteTiming // #[[1]] &;
Print["Tsol1=",Tsol1];
Print["sol1=", PackedArrayQ@sol1];
Print["Norm[Z1.sol1-V]/Norm[V]=", Norm[Z1.sol1-V]/Norm[V]];
(*Print["sol1=", sol1//Chop];*)
fsol1Re=FileNameJoin[{soldir,"sol_"<>ToString[Nd]<>"_"<>ToString[phis]<>"re.dat"}];
fsol1Im=FileNameJoin[{soldir,"sol_"<>ToString[Nd]<>"_"<>ToString[phis]<>"im.dat"}];
Export[fsol1Re,N@Re@sol1];
Export[fsol1Im,N@Im@sol1];
