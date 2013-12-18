#ifndef _TOEPLITZ_VEC_MUL_H_
#define _TOEPLITZ_VEC_MUL_H_
/******************************************************************************/ 
#include <complex.h>
#include <fftw3.h>
/******************************************************************************/ 
/*
 * Nameing convenction:
 * 	D	double precision
 * 	Z	double complex
 *
 * 	I	in-place
 * 	O	out-of-place
 *
 * 	CR	circulant matrix, pass the first column
 * 	CRF	circulant matrix, pass the Fourier transform of the first column
 *
 * 	TL	Toeplitz matrix, pass the first column and the first row
 *
 * 	MV	matrix-vector multiplication
 */

#ifdef __cplusplus
extern "C" {
#endif 

enum fftw_plan_kind {
	IFWD	= 0,	// in-place forward
	IBWD,		// in-place backward
	OFWD,		// out-of-place forward
	OBWD,		// out-of-place backward
}; 

void create_fftw_plans(const int n, fftw_plan *restrict &plans, 
		int flag = FFTW_MEASURE|FFTW_PATIENT);
void destroy_fftw_plans(fftw_plan *restrict plans);

/*
 * In-place circulant matrix - vector multiplication.
 * Input:
 * 	n		size of matrix Z
 * 	zf		DFT of the first column of Z
 * 	v		vector V
 * 	plans		fftw_plan *, created by create_fftw_plans
 * Output:
 * 	v		upon exit, stores Z.V
 * Note:
 * 	The fftw_plan pointer, plans, must be created by calling the
 * create_fftw_plans routine with the same dimension n.
 * 	The input vectors, zf and v, must be 16bytes aligned.
 */
void zicrfmv(const int n, const double _Complex *restrict zf,
		double _Complex *restrict v, const fftw_plan *plans);
#ifdef __cplusplus
}
#endif 
/******************************************************************************/
#endif
