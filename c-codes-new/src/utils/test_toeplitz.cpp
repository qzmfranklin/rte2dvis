#include <complex.h>
#include <stdio.h>
#include "toeplitz.h"
#include "utils.h"
/******************************************************************************/
/******************************************************************************/
int testxx(void);
/******************************************************************************/
int main(int argc, char const* argv[])
{ 
	testxx();
	return 0;
} 


int testxx(void)
{
#define N 10
	int err=0; 
        printf("TESTxx\n");
        printf("	|Test xxxxxx\n");

	printf("z is the first column of circulant matrix Z.\n");
	printf("x is the input vector.\n");
	printf("y is the output vector: y=Z.x\n");

	double _Complex x[N],y[N],z[N];
	init_vector(2*N,(double*)x);
	init_vector(2*N,(double*)y);
	init_vector(2*N,(double*)z);

	double work[2000];
	/*
	 *zocrmv(N,z,x,y,work);
	 */

	print_vector("z",2*N,(double*)z);
	print_vector("x",2*N,(double*)x);
	print_vector("y",2*N,(double*)y); 

        printf("END OF TESTxx\n");
        printf("\n");
	return err;
#undef N
}
