#include <fftw3.h>
#include <mkl.h>
#include <stdio.h>
#include "utils.h"
/******************************************************************************/
int test01(void);
/******************************************************************************/

int main(int argc, char const* argv[])
{
	if (argc>1) link_stdout(argv[1]);
	test01();

	if (argc>1) unlink_stdout();
	
	return 0;
}

int test01(void)
{
#define N 11
	int err=0; 
        printf("TEST01\n");
        printf("	|Test fftw3\n");

	__declspec(align(64)) double _Complex a[N],b[N],c[N];

	for (int i = 0; i < N; i++)
		a[i]=i; 
	print_cvector("a(ORIG)",N,a);

	fftw_plan plans[4];
	create_fftw_plans(N,plans,FFTW_ESTIMATE);

	fftw_execute_dft(plans[OFWD],(fftw_complex*)a,(fftw_complex*)b);
	fftw_execute_dft(plans[OBWD],(fftw_complex*)a,(fftw_complex*)c); 
	print_cvector("b(OFWD)",N,b);
	print_cvector("c(OBWD)",N,c);

	destroy_fftw_plans(plans); 

        printf("END OF TEST01\n");
        printf("\n");
	return err;
#undef N
}
