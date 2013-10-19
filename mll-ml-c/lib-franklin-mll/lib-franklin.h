
//EXTERN_C DLLEXPORT mint WolframLibrary_getVersion();
//EXTERN_C DLLEXPORT int WolframLibrary_initialize( WolframLibraryData libData);
//EXTERN_C DLLEXPORT void WolframLibrary_uninitialize( WolframLibraryData libData);
EXTERN_C DLLEXPORT int LegendreRule_MLL( WolframLibraryData libData, mint Argc, MArgument *Args, MArgument Res);
EXTERN_C DLLEXPORT int DunavantRule_MLL( WolframLibraryData libData, mint Argc, MArgument *Args, MArgument Res);
