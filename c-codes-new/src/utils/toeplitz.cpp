#include "utils.h"
#include <mkl.h>
#include <complex.h>
#include "toeplitz.h"
/******************************************************************************/
#define MALLOC_ALIGNMENT 64
/******************************************************************************/ 

void create_fftw_plans(const int n, fftw_plan *restrict &plans, int flag)

{
	//fprintf(stderr, "create_fftw_plans(n=%d,plans,flag)\n",n);
	fftw_complex *work1,*work2;
	work1 =(fftw_complex*)mkl_malloc(
			n*sizeof(fftw_complex),MALLOC_ALIGNMENT);
	work2 =(fftw_complex*)mkl_malloc(n*sizeof(
				fftw_complex),MALLOC_ALIGNMENT);

	plans       = (fftw_plan*)mkl_malloc(
			4*sizeof(fftw_plan),MALLOC_ALIGNMENT);
	plans[IFWD] = fftw_plan_dft_1d(n,
			(fftw_complex*)work1,
			(fftw_complex*)work1,
			FFTW_FORWARD,flag);
	plans[IBWD] = fftw_plan_dft_1d(n,
			(fftw_complex*)work1,
			(fftw_complex*)work1,
			FFTW_BACKWARD,flag);
	plans[OFWD] = fftw_plan_dft_1d(n,
			(fftw_complex*)work1,
			(fftw_complex*)work2,
			FFTW_FORWARD,flag);
	plans[OBWD] = fftw_plan_dft_1d(n,
			(fftw_complex*)work1,
			(fftw_complex*)work2,
			FFTW_BACKWARD,flag);
	mkl_free(work1);
	mkl_free(work2);
}

void destroy_fftw_plans(fftw_plan *restrict plans)
{
	//fprintf(stderr, "destroy_fftw_plans(plans)\n");
	for (int i = 0; i < 4; i++)
		fftw_destroy_plan(plans[i]);
}

void zicrfmv(const int n, const double _Complex *restrict zf,
		double _Complex *restrict v, const fftw_plan *plans)
{ 
	fftw_execute_dft(plans[IFWD],(fftw_complex*)v,(fftw_complex*)v);
	// convolution in freq-domain
	for (int i = 0; i < n; i++)
		v[i] = v[i]*zf[i];
	// normalize
	double xx = 1.0/n;
	for (int i = 0; i < n; i++)
		v[i]*= xx;
	fftw_execute_dft(plans[IBWD],(fftw_complex*)v,(fftw_complex*)v); 
}
