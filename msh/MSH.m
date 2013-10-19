#!/usr/caen/mathematica-9.0/bin/MathematicaScript -script 
(*#!/usr/local/bin/MathematicaScript -script*) 

ClearAll[sprtr](*separator in filenames*)
(*If[$OperatingSystem=="MacOSX",sprtr=".",0.0;];*)
(*If[$OperatingSystem=="Linux",sprtr=".",0.0;];*)
(*If[$OperatingSystem=="Windows",sprtr="_",0.0;];*)
sprtr="_";


Needs["Developer`"]
ReadMSHFile[filename_]:=Module[{i,SkipStreamNumbers,ReadStreamNumbers,meshStr,nNodes,nodesTable,nElements,nPoints,nLines,nTriangles,trianglesTable,temp},
	SkipStreamNumbers=Skip[#1,Table[Number,{i,Range[#2]}]]&;
	ReadStreamNumbers=Read[#1,Table[Number,{i,Range[#2]}]]&;
	meshStr=OpenRead[filename];
	Find[meshStr,"$Nodes"];
	nNodes=Read[meshStr,Number];
	nodesTable=ConstantArray[0.0,{nNodes,2}];
	For[i=1,i<=nNodes,i++,
		Skip[meshStr,Number];
		nodesTable[[i]]=N@ReadStreamNumbers[meshStr,2];
		Skip[meshStr,Number];
	];
	Find[meshStr,"$Elements"];
	nElements=Read[meshStr,Number];
	nPoints=0;nLines=0;nTriangles=0;
	trianglesTable=ConstantArray[0.0,{nElements,3}];
	For[i=1,i<=nElements,i++,
		Skip[meshStr,Number];
		temp=Read[meshStr,Number];
		If[temp==15,nPoints++;SkipStreamNumbers[meshStr,4];];
		If[temp==1,nLines++;SkipStreamNumbers[meshStr,5];];
		If[temp==2,nTriangles++;SkipStreamNumbers[meshStr,3];trianglesTable[[nTriangles]]=N@ReadStreamNumbers[meshStr,3];];
	];
	trianglesTable=trianglesTable[[1;;nTriangles]];
	Close[meshStr];
	ToPackedArray/@{nodesTable,Floor@trianglesTable}
]
DumpMSHFileTo2DATFiles[mshfilename_,forceflag_:False]:=Module[{dir,nodesTable,trianglesTable},
	{nodesTable,trianglesTable}=ReadMSHFile[mshfilename];
	dir=FileNameJoin[{DirectoryName[mshfilename],FileBaseName[mshfilename]<>sprtr<>"dat"}];
	CreateDirectory[dir]//Quiet;
	Print["dumping..."];
	(*Print[FileNameJoin[{dir,"nodes.dat"}]];*)
	(*Print[FileNameJoin[{dir,"triangles.dat"}]];*)
	Export[FileNameJoin[{dir,"nodes.dat"}],nodesTable];
	Export[FileNameJoin[{dir,"triangles.dat"}],trianglesTable];
]
ReadMSHFileFrom2DATFiles[mshfilename_]:=Module[{dir},
	dir=FileNameJoin[{DirectoryName[mshfilename],FileBaseName[mshfilename]<>sprtr<>"dat"}];
	Print["reading..."];
	(*Print[FileNameJoin[{dir,"nodes.dat"}]];*)
	(*Print[FileNameJoin[{dir,"triangles.dat"}]];*)
	ToPackedArray/@{N@Import[FileNameJoin[{dir,"nodes.dat"}]],Floor@Import[FileNameJoin[{dir,"triangles.dat"}]]}
]
