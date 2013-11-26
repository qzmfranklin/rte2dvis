#include "cross_approx.h"
#include <mkl.h>
#include <math.h>
/******************************************************************************/
#define MALLOC_ALIGNMENT 64
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#ifndef MAX_NUM_THREADS
	/*
	 * Use many threads to search for max/min values
	 */
	#define MAX_NUM_THREADS 1
#endif
/******************************************************************************/
/*
 * Finding the position of the maximum in a double floating precision vector.
 * Input:
 * 	n	int		number of elements in the input vector
 * 	v	double*		[>=n*incx] input vector
 * 	incx	int		increment in v
 * Output:
 * 	pos	int*		[1] such that max value of v = v[*pos]
 * 	return	double		max(abs(v))
 */
static double dvmaxpos(int *pos, const int n, const double *restrict v, 
		const int incx);

/*
 * Finding the position of the maximum in a double floating precision matrix.
 * Input:
 * 	m,n	int		number of row/col in the input matrix
 * 	a	double*		[>=m*n*incx] input matrix
 * 	lda	int		leading dimension of the input matrix
 * Output:
 * 	pos	int[]		[2] such that max value of a = a[pos[0],pos[1]]
 * 	return	double		max(abs(a))
 */
static double dmmaxpos(int pos[], const int m, const int n, const double* a, 
		const int lda);

/*
 * Finding the position of the maximum in a double floating precision vector.
 * Input:
 * 	n	int		number of elements in the input vector
 * 	v	double*		[>=n*incx] input vector
 * 	incx	int		increment in v
 * Output:
 * 	pos	int*		[1] such that max value of v = v[*pos]
 * 	return	double		max(abs(v))
 */
static double dvmaxpos_omp(int *pos, const int n, const double *restrict v, 
		const int incx, const int nthreads);

/*
 * Finding the position of the maximum in a double floating precision matrix.
 * Input:
 * 	m,n	int		number of row/col in the input matrix
 * 	a	double*		[>=m*n*incx] input matrix
 * 	lda	int		leading dimension of the input matrix
 * Output:
 * 	pos	int[]		[2] such that max value of a = a[pos[0],pos[1]]
 * 	return	double		max(abs(a))
 */
static double dmmaxpos_omp(int pos[], const int m, const int n, const double* a, 
		const int lda, const int nthreads);
/*****************************************************************************/

void dgefca(const int m, const int n, double *restrict a, const int lda,
		const int r, double *restrict u, const int ldu,
		double *restrict vt, const int ldvt)
{
}

void dgepca(const int m, const int n, double *restrict a, const int lda,
		const int r, double *restrict u, const int ldu,
		double *restrict vt, const int ldvt)
{
}

void dgeaca(const int m, const int n, double *restrict a, const int lda,
		const int r, double *restrict u, const int ldu,
		double *restrict vt, const int ldvt)
{
}

double dvmaxpos_omp(int *pos, const int n, const double *restrict v, 
		const int incx, const int nthreads)
{
	*pos = 0;
	#pragma omp parallel				\
	num_threads(nthreads)				\
	default(none)					\
	shared(pos,n,v,incx)
	{
		int p=0;
		#pragma omp for
		for (int i = 1; i < n; i++)
			if (  fabs(v[i*incx]) > fabs(v[p])  )
				p = i;
		#pragma omp critical
		if (  fabs(v[p]) > fabs(v[*pos])  )
			*pos = p;
	}
	return v[*pos];
}

double dmmaxpos_omp(int pos[], const int m, const int n, const double* a, 
		const int lda, const int nthreads)
{
	pos[0] = pos[1] = 0;
	#pragma omp parallel				\
	num_threads(nthreads)			\
	default(none)					\
	shared(pos,m,n,a,lda)
	{
		int p[2] = {0,0};
		#pragma omp for
		for (int j = 0; j < n; j++)
			for (int i = 0; i < m; i++)
				if (fabs(a[i+j*lda])
					>fabs(a[p[0]+p[1]*lda])) {
					p[0] = i;
					p[1] = j;
				}	
		#pragma omp critical
		if (fabs(a[p[0]+p[1]*lda]) 
				>fabs(a[pos[0]+pos[1]*lda])) {
			pos[0] = p[0];
			pos[1] = p[1];
		}	
	}
	return a[pos[0]+pos[1]*lda];
}
/******************************************************************************/
