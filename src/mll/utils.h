#ifndef _UTILS_H_
#define _UTILS_H_
/******************************************************************************/
#include <fftw3.h>
#ifdef __cplusplus
extern "C" {
#endif
	
enum fftw_plan_kind {
	IFWD	= 0,	// in-place forward
	IBWD,		// in-place backward
	OFWD,		// out-of-place forward
	OBWD,		// out-of-place backward
}; 
void create_fftw_plans(const int n, fftw_plan plans[4], 
		const int flag);
		//const int flag=FFTW_MEASURE|FFTW_PATIENT);
void destroy_fftw_plans(fftw_plan *plans); 

#ifdef __cplusplus
}
#endif
/******************************************************************************/
#endif
