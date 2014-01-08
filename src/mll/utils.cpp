#include "utils.h"
#include <complex.h>
#include <malloc.h>
#include <fftw3.h>
/******************************************************************************/ 
void create_fftw_plans(const int n, fftw_plan plans[4], const int flag)
{
	//fprintf(stderr, "create_fftw_plans(n=%d,plans,flag)\n",n);
	//fftw_complex *work1 = fftw_alloc_complex(n);
	//fftw_complex *work2 = fftw_alloc_complex(n);
	double _Complex *work1 = (double _Complex*)malloc(sizeof(double)*2*n);
	double _Complex *work2 = (double _Complex*)malloc(sizeof(double)*2*n);

	plans[IFWD] = fftw_plan_dft_1d(n,(fftw_complex*)work1,(fftw_complex*)work1,FFTW_FORWARD ,flag);
	plans[IBWD] = fftw_plan_dft_1d(n,(fftw_complex*)work1,(fftw_complex*)work1,FFTW_BACKWARD,flag);
	plans[OFWD] = fftw_plan_dft_1d(n,(fftw_complex*)work1,(fftw_complex*)work2,FFTW_FORWARD ,flag);
	plans[OBWD] = fftw_plan_dft_1d(n,(fftw_complex*)work1,(fftw_complex*)work2,FFTW_BACKWARD,flag);

	//fftw_free(work1);
	//fftw_free(work2);
	free(work1);
	free(work2);
}

void destroy_fftw_plans(fftw_plan *plans)
{
	//fprintf(stderr, "destroy_fftw_plans(plans)\n");
	for (int i = 0; i < 4; i++)
		fftw_destroy_plan(plans[i]);
}

