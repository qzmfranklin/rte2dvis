#ifndef _LIB_FRANKLIN_H_
#define _LIB_FRANKLIN_H_
/******************************************************************************/
#include "WolframLibrary.h" 
#ifdef __cplusplus
extern "C" {
#endif
DLLEXPORT int test_complex( WolframLibraryData libData, mint Argc, MArgument *Args, MArgument Res); 
/**************************************/
DLLEXPORT int BHomoS_MLL( WolframLibraryData libData, mint Argc, MArgument *Args, MArgument Res);
DLLEXPORT int BHomoN_MLL( WolframLibraryData libData, mint Argc, MArgument *Args, MArgument Res);
DLLEXPORT int HomoMulBX_MLL( WolframLibraryData libData, mint Argc, MArgument *Args, MArgument Res);
/**************************************/
DLLEXPORT int ArcSinhRule_MLL( WolframLibraryData libData, mint Argc, MArgument *Args, MArgument Res);
DLLEXPORT int LegendreRule_MLL( WolframLibraryData libData, mint Argc, MArgument *Args, MArgument Res);
DLLEXPORT int DunavantRule_MLL( WolframLibraryData libData, mint Argc, MArgument *Args, MArgument Res); 
/**************************************/
DLLEXPORT mint WolframLibrary_getVersion();
DLLEXPORT int WolframLibrary_initialize( WolframLibraryData libData);
DLLEXPORT void WolframLibrary_uninitialize( WolframLibraryData libData);
/**************************************/
#ifdef __cplusplus
}
#endif
/******************************************************************************/
#endif