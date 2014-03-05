#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <mkl.h>
#include <fftw3.h>
#include <string.h>
#include "utils.h"
/*****************************************************************************/
int main(int argc, char const* argv[]);
int test01(void);
/*****************************************************************************/
int main(int argc, char const* argv[])
{ 
	if (argc>1) link_stdout(argv[1]); 
	test01(); 
	if (argc>1) unlink_stdout(); 
	return 0;
}

static void m_mul_v(const int n, double *restrict A, double *restrict in, double *restrict out)
{
	for (int i = 0; i < n; i++)
		out[i] = 0.0;
	for (int j = 0; j < n; j++)
		for (int i = 0; i < n; i++)
			out[i] += A[i+j*n] * in[j];
}

static void print_mkl_dfgmres_pars(const int *restrict ipar, const double *restrict dpar)
{
}

int test01(void)
{
	const int N=100;
	int err=0; 
        printf("TEST01\n");
        printf("	|Test real MKL FGMRES calling convention\n");

	// Prepare the inputs.
	double A[N*N],expected_sol[N],rhs[N];
	init_matrix(N,N,A);
	for (int i = 0; i < N; i++)
		expected_sol[i] = i;
	m_mul_v(N,A,expected_sol,rhs);

	printf("N=%d\n",N);
	print_vector("expected_sol",20,expected_sol);
	
	// Allocate storage for the iterative solver.
	double sol[N];
	init_vector(N,sol); 
	print_vector("initial guess",20,sol); 

	int ipar[128],RCI_request,ivar,itercount;
	double dpar[128],tmp[N*(5*N+11)/2+1];
	ivar    = N;

	print_mkl_dfgmres_pars(ipar,dpar);
	dfgmres_init(&ivar,sol,rhs,&RCI_request,ipar,dpar,tmp);
	assert(!RCI_request);

	/*
	 * LOGICAL parameters:
	 *   ipar[3]:  current iteration count
	 *   ipar[4]:  max numbuer of iterations
	 *   ipar[7]:  !0 = max iteration check [1]
	 *   ipar[8]:  !0 = residual stopping check dpar[3]<dpar[4] [0]
	 *   ipar[9]:  !0 = user-defined stopping check by setting RCI_request=2 [1]
	 *   ipar[10]: !0 = requests precondition by setting RCI_request=3 [0]
	 *   ipar[11]: !0 = check zero norm of current vector dpar[6]<=dpar[7] [0]
	 *              0 = requests user-defined check by setting RCI_request=4
	 *              (must set to 1 for some unknown reasons)
	 *   ipar[12]: solution vector storage flag [0]
	 *   ipar[13]: internal iteration counts before restart
	 *   ipar[14]: max number of non-restarted version
	 */
	ipar[4]  = 100;
	ipar[7]  = 1;
	ipar[8]	 = 1;
	ipar[9]	 = 0;
	ipar[11] = 1;

	/*
	 * DOUBLE parameters:
	 *   dpar[0]:  relative tolerance [1.0D-6]
	 *   dpar[1]:  absolute tolerance [0.0D-0]
	 *   dpar[2]:  L2 norm of initial residual
	 *   dpar[3]:  service variable, dpar[0]*dpar[2]+dpar[1]
	 *   dpar[4]:  L2 norm of current residual
	 *   dpar[5]:  L2 norm of previous residual, if available
	 *   dpar[6]:  norm of generated vector
	 *   dpar[7]:  tolerance for zero norm of current vector [1.0D-12]
	 */
	dpar[0]	 = 1.0E-5;
	// Check consistency of parameters
	dfgmres_check(&ivar,sol,rhs,&RCI_request,ipar,dpar,tmp);
	assert(!RCI_request);

	/*
	 * RCI_request:
	 * 	0	completed
	 * 	1	go on iterating
	 * 	2	requests user-defined stopping check, ipar[9]
	 * 	3	requests preconditoning, ipar[10]
	 * 	4	requests zero norm check, ipar[11]
	 */
	dfgmres(&ivar,sol,rhs,&RCI_request,ipar,dpar,tmp); 
	double bnorm=0.0;
	for (int i = 0; i < N; i++)
		bnorm += rhs[i] * rhs[i];
	bnorm = sqrt(bnorm);
	while (RCI_request>0) {
		// user-defined matrix-vector multiplication comes here...
		m_mul_v(N,A,tmp+ipar[21]-1,tmp+ipar[22]-1);
		dfgmres(&ivar,sol,rhs,&RCI_request,ipar,dpar,tmp); 
		switch (RCI_request) {
		case 2: // requests user-defined stopping check
			break;
		case 3: // requests preconditioning
			break;
		case 4: // requests zero norm check
			break;
		default:// no idea how to get here...
			break;
		}
		printf("[  %5d,  %5d,  %.2E,  %-.2E]\n",ipar[3],ipar[13],dpar[4],dpar[4]/bnorm); 
	}

	// Extract solution, print, clear buffers.
	dfgmres_get(&ivar,sol,rhs,&RCI_request,ipar,dpar,tmp,&itercount);
	printf("\n");
	printf("Solver finished after %d iterations.\n",itercount);
	print_vector("sol",20,sol);
	MKL_Free_Buffers(); 

        printf("END OF TEST01\n");
        printf("\n");
	return err;
}
