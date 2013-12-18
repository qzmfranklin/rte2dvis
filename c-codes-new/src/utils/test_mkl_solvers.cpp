#include <stdio.h>
#include <mkl.h>
#include <fftw3.h>
#include <string.h>
#include "utils.h"
/*****************************************************************************/
int main(int argc, char const* argv[]);
int test01(void);
int test02(void);
/*****************************************************************************/
int main(int argc, char const* argv[])
{ 
	if (argc>1) link_stdout(argv[1]); 
	test01(); 
	test02();
	if (argc>1) unlink_stdout(); 
	return 0;
}

static void m_mul_v(const int n, double *restrict A, 
		double *restrict in, 
		double *restrict out)
{
	for (int i = 0; i < n; i++)
		out[i] = 0.0;
	for (int j = 0; j < n; j++)
		for (int i = 0; i < n; i++)
			out[i] += A[i+j*n] * in[j];
}

int test01(void)
{
#define N 100
#define PAR_SIZE 128
	int err=0; 
        printf("TEST01\n");
        printf("	|Test real MKL FGMRES calling convention\n");

	 /*
	  * Prepare the inputs.
	  */
	double A[N*N],expected_sol[N],rhs[N];
	init_matrix(N,N,A);
	for (int i = 0; i < N; i++)
		expected_sol[i] = i;
	m_mul_v(N,A,expected_sol,rhs);

	printf("N=%d\n",N);
	print_vector("expected_sol",N,expected_sol);
	
	/*
	 * Allocate storage for the iterative solver.
	 */
	double sol[N];
	init_vector(N,sol); 
	print_vector("initial guess",N,sol); 

	int ipar[PAR_SIZE],RCI_request,ivar,itercount;
	double dpar[PAR_SIZE],tmp[N*(5*N+11)/2+1];
	ivar    = N;

	/*
	 * Initialize the solver.
	 */
	dfgmres_init(&ivar,sol,rhs,&RCI_request,ipar,dpar,tmp);
	if (RCI_request) { MKL_Free_Buffers(); return RCI_request; }

	/*
	 * Set the desired parameters:
	 * LOGICAL parameters:
	 * 	ipar[8]:  do automatica residual stopping test
	 * 	ipar[9]:  do residual stopping test: dpar[5]<dpar[4]
	 * 	ipar[11]: automatic check norm of next generated vector??
	 * DOUBLE parameters:
	 * 	dpar[0]:  set the relative tolerance to 1.0E-5
	 */
	ipar[8]		= 1;
	ipar[9]		= 0;
	ipar[11]	= 1;
	dpar[0]		= 1.0E-5;

	/*
	 * Check consistency of newly assigned parameters.
	 */
	dfgmres_check(&ivar,sol,rhs,&RCI_request,ipar,dpar,tmp);
	if (RCI_request) { MKL_Free_Buffers(); return RCI_request; }

	/*
	 * Solve!
	 * RCI_request:
	 * 	0	completed
	 * 	1	go on iterating
	 */
	dfgmres(&ivar,sol,rhs,&RCI_request,ipar,dpar,tmp);
	while (RCI_request==1) {
		//printf("tmp+ipar[21]-1=%p\n",tmp+ipar[21]-1);
		//printf("        [22]  =%p\n",tmp+ipar[22]-1);
		m_mul_v(N,A,tmp+ipar[21]-1,tmp+ipar[22]-1);
		dfgmres(&ivar,sol,rhs,&RCI_request,ipar,dpar,tmp); 
	}

	/*
	 * Extract solution, print, clear buffers.
	 */
	dfgmres_get(&ivar,sol,rhs,&RCI_request,ipar,dpar,tmp,&itercount);
	printf("\n");
	printf("Solver finished after %d iterations.\n",itercount);
	print_vector("sol",N,sol);
	MKL_Free_Buffers();


        printf("END OF TEST01\n");
        printf("\n");
	return err;
#undef PAR_SIZE
#undef N
}

static void cm_mul_v(const int n, double _Complex *restrict A,
		double _Complex *restrict in,
		double _Complex *restrict out)
{
	for (int i = 0; i < n; i++)
		out[i] = 0.0;
	for (int j = 0; j < n; j++) {
		for (int i = 0; i < n; i++)
			out[i] += A[i+j*n] * in[j];
	}
}

int test02(void)
{
#define N 20
#define PAR_SIZE 128
	int err=0; 
        printf("TEST02\n");
        printf("	|Test complex MKL FGMRES calling convention\n");

	 /*
	  * Prepare the inputs.
	  */
	double _Complex A[N*N],expected_sol[N],rhs[N];
	init_vector(2*N*N,(double*)A);
	for (int i = 0; i < N; i++)
		expected_sol[i] = i;
	cm_mul_v(N,A,expected_sol,rhs);

	printf("N=%d\n",N);
	print_cvector("expected_sol",N,expected_sol);
	//print_cmatrix("A",N,N,A);
	//print_cvector("rhs",N,rhs);
	
	/*
	 * Allocate storage for the iterative solver.
	 */
	double _Complex sol[N];
	init_vector(2*N,(double*)sol); 
	print_cvector("initial sol",N,sol);


	int ipar[PAR_SIZE],RCI_request,ivar,itercount;
	double dpar[PAR_SIZE],tmp[N*(10*N+11)+1];
	ivar    = 2*N;

	/*
	 * Initialize the solver.
	 */
	dfgmres_init(&ivar,(double*)sol,(double*)rhs,&RCI_request,ipar,dpar,tmp);
	if (RCI_request) { MKL_Free_Buffers(); return RCI_request; }

	/*
	 * Set the desired parameters:
	 * LOGICAL parameters:
	 * 	ipar[8]:  do automatica residual stopping test
	 * 	ipar[9]:  do residual stopping test: dpar[5]<dpar[4]
	 * 	ipar[11]: automatic check norm of next generated vector??
	 * DOUBLE parameters:
	 * 	dpar[0]:  set the relative tolerance to 1.0E-5
	 */
	ipar[8]		= 1;
	ipar[9]		= 0;
	ipar[11]	= 1;
	dpar[0]		= 1.0e-5;

	/*
	 * Check consistency of newly assigned parameters.
	 */
	dfgmres_check(&ivar,(double*)sol,(double*)rhs,&RCI_request,ipar,dpar,tmp);
	if (RCI_request) { MKL_Free_Buffers(); return RCI_request; }

	/*
	 * Solve!
	 * RCI_request:
	 * 	0	completed
	 * 	1	go on iterating
	 */
	dfgmres(&ivar,(double*)sol,(double*)rhs,&RCI_request,ipar,dpar,tmp);
	while (RCI_request==1) {
		cm_mul_v(N,A,(double _Complex*)(tmp+ipar[21]-1),
				(double _Complex*)(tmp+ipar[22]-1));
		dfgmres(&ivar,(double*)sol,(double*)rhs,&RCI_request,ipar,dpar,tmp); 
	}

	/*
	 * Extract solution, print, clear buffers.
	 */
	dfgmres_get(&ivar,(double*)sol,(double*)rhs,&RCI_request,ipar,dpar,tmp,&itercount);
	printf("Solver finished after %d iterations.\n",itercount);
	print_cvector("sol",N,sol);
	MKL_Free_Buffers(); 

        printf("END OF TEST02\n");
        printf("\n");
	return err;
#undef PAR_SIZE
#undef N
}
