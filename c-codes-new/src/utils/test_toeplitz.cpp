#include <complex.h>
#include <string.h>
#include <math.h>
#include <fftw3.h>
#include <stdio.h>
#include "toeplitz.h"
#include "utils.h"
/******************************************************************************/
/******************************************************************************/
int test01(void);
int test02(void);
/******************************************************************************/
int main(int argc, char const* argv[])
{ 
	char fname[FILENAME_MAX];
	if (!argc)
		sprintf(fname,"DEBUG/test_test_toeplitz.txt");
	else
		strcpy(fname,argv[1]); 
	link_stdout(fname);

	test01(); 
	test02(); 

	unlink_stdout(); 
	return 0; 
} 


static void cscale(fftw_complex scale, const int n, fftw_complex *restrict v)
{
	for (int i = 0; i < n; i++)
		v[i] *= scale;
}

static void cdot(const int n, fftw_complex *a,  
		fftw_complex *b, fftw_complex *dest)
{
	for (int i = 0; i < n; i++) 
		dest[i] = a[i]*b[i];
}

int test01(void)
{
#define N 8
	int err=0; 
        printf("TEST01\n");
        printf("	|Test FFTW\n");

	printf("z is the first column of circulant matrix Z.\n");
	printf("x is the input vector.\n");
	printf("y is the output vector: y=Z.x\n");

	fftw_complex z[N]={1.,2.,3.,4.,0.,8.,7.,6.};
	fftw_complex x[N]={1.,2.,3.,4.,5.,6.,7.,8.};
	fftw_complex y[N],xf[N],yf[N],zf[N];

	fftw_plan po_fwd, po_bwd, pi_fwd, pi_bwd;
	po_fwd = fftw_plan_dft_1d(N,y,yf,FFTW_FORWARD,
			FFTW_MEASURE|FFTW_PATIENT);
	po_bwd = fftw_plan_dft_1d(N,y,yf,FFTW_BACKWARD,
			FFTW_MEASURE|FFTW_PATIENT);
	pi_fwd = fftw_plan_dft_1d(N,y,y,FFTW_FORWARD,
			FFTW_MEASURE|FFTW_PATIENT);
	pi_bwd = fftw_plan_dft_1d(N,y,y,FFTW_BACKWARD,
			FFTW_MEASURE|FFTW_PATIENT);

	printf("\n");
	print_vector("x",2*N,(double*)x);
	print_vector("z",2*N,(double*)z);

	fftw_execute_dft(po_fwd,x,xf);
	fftw_execute_dft(po_fwd,z,zf);

	printf("\n");
	print_vector("xf(unscaled)",2*N,(double*)xf);
	print_vector("zf(unscaled)",2*N,(double*)zf);

	cdot(N,xf,zf,yf);
	cscale(1.0/N,N,yf);
	printf("\n");
	print_vector("yf(scaled)",2*N,(double*)yf);

	fftw_execute_dft(po_bwd,yf,y);
	printf("\n");
	print_vector("y",2*N,(double*)y);

	fftw_destroy_plan(po_fwd);
	fftw_destroy_plan(pi_fwd);
	fftw_destroy_plan(po_bwd);
	fftw_destroy_plan(pi_bwd);


        printf("END OF TEST01\n");
        printf("\n");
	return err;
#undef N
}

int test02(void)
{
#define N 8192
#define COUNT 20
	int err=0; 
        printf("TEST02\n");
        printf("	|Test zicrfmv\n");

	fftw_plan *plans;
	create_fftw_plans(N,plans);

	__declspec(align(64)) double _Complex z[N];
	__declspec(align(64)) double _Complex v[N];

	fftw_execute_dft(plans[IFWD],(fftw_complex*)z,(fftw_complex*)z);

	TimeStamp clk(COUNT);
	for (int i = 0; i < COUNT; i++) {
		clk.tic();
		zicrfmv(N,z,v,plans);
		clk.toc();
	}
	printf("N=%d\n",N);
	printf("Cycles/(N*logN)=%f\n",clk.median()/N*log(N));

	destroy_fftw_plans(plans);

        printf("END OF TEST02\n");
        printf("\n");
	return err;
#undef COUNT
#undef N
}
