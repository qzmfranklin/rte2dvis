#ifndef _TOEPLITZ_VEC_MUL_H_
#define _TOEPLITZ_VEC_MUL_H_
/******************************************************************************/ 
/*
 * Nameing convenction:
 * 	D	double precision
 * 	Z	double complex
 * 	I	in-place
 * 	O	out-of-place
 * 	CR	circulant matrix
 * 	TL	Toeplitz matrix
 * 	MV	matrix-vector multiplication
 */

#ifdef __cplusplus
extern "C" {
#endif
	//TODO
struct st_plans { 
	int             kind;
	int             plan_num;
	fftw_plan     **plan;
	void           *work;
}

/*
 * Auxilliary routine to connect the first column and the first row and the
 * Toeplitz matrix to form a single vector, z, that is the input of the
 * multiplication routine.
 */
void d_toeplitz_to_circulant(const double *col, const double *row, 
		double *restrict z);

/*
 * Double precision out-of-place fast M-V multiplication:
 * 		Z.Vin => Vout
 * Input:
 * 	n	int		dimension of the Toeplitz matrix
 * 	z	*double		[n] col and row representing the matrix
 * 	in	*double		[n] input vector
 * 	work	*double		[?] workspace
 * Output:
 * 	out	*double		[n] first n elements form the output vector
 * Note:
 * 	n, z, and in are all kept constant during operation.
 * 	z, in, and out must point to different arrays.
 * 	Workspace, i.e., work, must be at least sizeof(double)*(TODO)
 * bytes.
 */
void zocrmv(const int n, void *z, void *in, void *out, 
		fftw_plan **plans, void *work);
	
#ifdef __cplusplus
}
#endif 
/******************************************************************************/
#endif
