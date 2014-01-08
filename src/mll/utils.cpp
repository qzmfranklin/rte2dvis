#include "utils.h"
#include <fftw3.h>
/******************************************************************************/ 
void create_fftw_plans(const int n, fftw_plan **plans, int flag)
{
	//fprintf(stderr, "create_fftw_plans(n=%d,plans,flag)\n",n);
	fftw_complex *work1,*work2;
	work1        = fftw_alloc_complex(n);
	work2        = fftw_alloc_complex(n);

	*plans       = (fftw_plan*)fftw_malloc(sizeof(fftw_plan)*4);
	*plans[IFWD] = fftw_plan_dft_1d(n,
			(fftw_complex*)work1,
			(fftw_complex*)work1,
			FFTW_FORWARD,flag);
	*plans[IBWD] = fftw_plan_dft_1d(n,
			(fftw_complex*)work1,
			(fftw_complex*)work1,
			FFTW_BACKWARD,flag);
	*plans[OFWD] = fftw_plan_dft_1d(n,
			(fftw_complex*)work1,
			(fftw_complex*)work2,
			FFTW_FORWARD,flag);
	*plans[OBWD] = fftw_plan_dft_1d(n,
			(fftw_complex*)work1,
			(fftw_complex*)work2,
			FFTW_BACKWARD,flag);
	fftw_free(work1);
	fftw_free(work2);
}

void destroy_fftw_plans(fftw_plan *plans)
{
	//fprintf(stderr, "destroy_fftw_plans(plans)\n");
	for (int i = 0; i < 4; i++)
		fftw_destroy_plan(plans[i]);
	//mkl_free(plans);
	fftw_free(plans);
}
