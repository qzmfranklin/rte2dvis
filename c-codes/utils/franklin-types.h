#ifdef _FRANKLIN_TYPES_H_
#define _FRANKLIN_TYPES_H_

// Compatible with C++ compilers
#ifndef EXTERN_C
	#ifdef __cplusplus
		#define EXTERN_C extern "C"
	#else
		#define EXTERN_C extern
	#endif
#endif

// Compatibility with non-clang compilers.
#ifndef __has_feature         
	#define __has_feature(x) 0
#endif
#ifndef __has_extension
	#define __has_extension __has_feature 
#endif 

/******************************************************************************/

#endif /*_FRANKLIN_TYPES_H_*/
