#include "c_utils.h"
#include <time.h>
#include <stdio.h>
#include <mkl.h>
/******************************************************************************/ 
#define MALLOC_ALIGNMENT 64 
/******************************************************************************/
int test01(void);
int test02(void);
int test03(int nthreads);
/******************************************************************************/ 
int main(int argc, char const* argv[])
{
	char fname[FILENAME_MAX];
	sprintf(fname,"DEBUG/test_c_utils.txt");
	link_stdout(fname);

	test01();
	test02();
	test03(8);

	unlink_stdout(); 
	return 0;
}

int test01(void)
{
#define M 4
#define N 6
	int err=0; 
        printf("TEST01\n");
        printf("	|Test MATRIX_OUT()\n");

	double a[M*N];
	init_matrix(M,N,a,M);

	array_out("DEBUG/test_array_out.txt",
			M,N,a,M);

	
        printf("END OF TEST01\n");
        printf("\n");
	return err;
#undef M
#undef N
}

int test02(void)
{
#define M 7
#define N 5
	int err=0; 
        printf("TEST02\n");
        printf("	|Test DABS_MAX()\n");

	double a[M*N];
	init_matrix(M,N,a,M);
	print_matrix("orignal matrix a",M,N,a,M);

	int pos[2], r=3, c=4;
	double max;
	max = dabs_max(pos,M,N,a,M);
	printf("Max{|a[i,j]|}=a[%d,%d]=%f\n",pos[0],pos[1],max);
	max = dabs_max(pos,1,N,a+r,M);
	printf("Max{|a[%d,j]|}=a[%d,%d]=%f\n",r,r+pos[0],pos[1],max);
	max = dabs_max(pos,M,1,a+c*M,M);
	printf("Max{|a[i,%d]|}=a[%d,%d]=%f\n",c,pos[0],c+pos[1],max);

        printf("END OF TEST02\n");
        printf("\n");
	return err;
#undef M
#undef N
}


int test03(int nthreads) 
{
#define M 9
#define N 7
	int err=0; 
        printf("TEST03\n");
        printf("	|Test DABS_MAX_OMP()\n");

	double a[M*N];
	init_matrix(M,N,a,M);
	print_matrix("orignal matrix a",M,N,a,M);

	int pos[2], r=3, c=4;
	double max;
	max = dabs_max_omp(pos,M,N,a,M,nthreads);
	printf("Max{|a[i,j]|}=a[%d,%d]=%f\n",pos[0],pos[1],max);
	max = dabs_max_omp(pos,1,N,a+r,M,nthreads);
	printf("Max{|a[%d,j]|}=a[%d,%d]=%f\n",r,r+pos[0],pos[1],max);
	max = dabs_max_omp(pos,M,1,a+c*M,M,nthreads);
	printf("Max{|a[i,%d]|}=a[%d,%d]=%f\n",c,pos[0],c+pos[1],max);

        printf("END OF TEST03\n");
        printf("\n");
	return err;
#undef M
#undef N
}

