#include <stdio.h>
#include <math.h>
#include <mkl.h>
#include "Utils.h"
using namespace Utils;
/*******************************************************************************/
int main(int argc, char const* argv[]);
int test01(void);
int test02(void);
int test03(void);
int test04(void);
//void init_vector( double *v, int n, int incx=1 );
//void init_matrix( double *a, int m, int n, int lda );
//void print_vector( double *a, int n, int incx=1 );
//void print_matrix( double *a, int m, int n, int lda );

/*******************************************************************************/
int main(int argc, char const* argv[])
{ 
	test01();
	test02();
	test03(); 
	test04(); 
	return 0;
} 

int test01(void) {
	int err=0; 
        printf("TEST01\n");
        printf("	|Test MKL_TYPES.H\n"); 
	printf("int		= %lu\n",sizeof(int));
	printf("MKL_INT		= %lu\n",sizeof(MKL_INT));
	printf("MKL_INT64	= %lu\n",sizeof(MKL_INT64));
	printf("MKL_Complex8	= %lu\n",sizeof(MKL_Complex8));
	printf("MKL_Complex16	= %lu\n",sizeof(MKL_Complex16)); 
        printf("END OF TEST01\n");
        printf("\n");
	return err;
}

int test02(void) { 
	int err=0; 
        printf("TEST02\n");
        printf("	|Test ddot\n"); 

	double x[5];
	double y[5];
	int n=5; 
	int incx=1;
	for ( int i = 0; i < 5; i++) {
		x[i] = 5.0 * rand()/RAND_MAX;
		y[i] = 5.0 * rand()/RAND_MAX;
	} 
	printf("x=  ");
	print_vector(x,n);
	printf("y=  ");
	print_vector(y,n); 
	double res;
	res = ddot(&n,x,&incx,y,&incx);
	printf("res = x*y = %lf\n",res);

        printf("END OF TEST02\n");
        printf("\n");
	return err; 
}

int test03( void ) {
	int err=0; 
        printf("TEST03\n");
        printf("	|Test dgemm\n");


#define M 500
#define N 300
#define K 400
	double a[M*K], b[K*N], c[M*N];
	int m, n, k, lda, ldb, ldc; 
	char transa, transb;
	double alpha, beta;
	transa	= 'N';
	transb	= 'N';
	m	= M;
	n	= N;
	k	= K;
	lda	= m;
	ldb	= k;
	ldc	= m;
	alpha	= 1.0;
	beta	= 0.0;
	init_matrix( a, m, k, lda );
	init_matrix( b, k, n, ldb );
	init_matrix( c, m, n, ldc );

	double flops = ( (double)K-1.0 )*M*N;
#undef M
#undef N
#undef K
	//printf("a = \n");
	//print_matrix( a, m, k, lda );
	//printf("b = \n");
	//print_matrix( b, k, n, ldb );
	//printf("c = \n");
	//print_matrix( c, m, n, ldc );
	//printf("alpha=%lf\n",alpha);
	//printf("beta=%lf\n",beta); 

#define COUNTER 50
	StatVector stat(COUNTER);
	TimeStamp clk;
	for (int i = 0; i < COUNTER; i++) {
		double cycles;
		clk.tic();
		dgemm( &transa, &transb, &m, &n, &k, &alpha, a, &lda, b, &ldb, &beta, c, &ldc ); 
		cycles = clk.toc();
		stat.insert(cycles);
	} 
#undef COUNTER

	printf("\n");
	printf("FLOPS/cycle = %lf\n",flops/stat.median());
	//printf("\n");
	//printf("alpha*(a.b)+beta*c= \n");
	//print_matrix( c, m, n, ldc );
	
        printf("END OF TEST03\n");
        printf("\n");
	return err;
}

int test04( void ) {
	/* Parameters */
#define M 6
#define N 4
#define LDA M
#define LDU M
#define LDVT N
	int err=0; 
        printf("TEST04\n");
        printf("	|Test dgesdd\n"); 
	/* Locals */
        int m = M, n = N, lda = LDA, ldu = LDU, ldvt = LDVT, info, lwork;
        double wkopt;
        double* work;
	/* Local arrays */
	/* iwork dimension should be at least 8*min(m,n) */
	int iwork[8*N];
	double s[N], u[LDU*M], vt[LDVT*N];
	double a[LDA*N];
	init_matrix( a, m, n, lda );
	/* Executable statements */
	printf( " DGESDD Example Program Results\n" );
	/* Query and allocate the optimal workspace */
	lwork = -1;
	dgesdd( "Singular vectors", &m, &n, a, &lda, s, u, &ldu, vt, &ldvt, &wkopt,
			&lwork, iwork, &info );
	lwork = (int)wkopt;
	work = (double*)malloc( lwork*sizeof(double) );
	/* Compute SVD */
	dgesdd( "Singular vectors", &m, &n, a, &lda, s, u, &ldu, vt, &ldvt, work,
			&lwork, iwork, &info );
	/* Check for convergence */
	if( info > 0 ) {
		printf( "The algorithm computing SVD failed to converge.\n" );
		exit( 1 );
	}
	printf("Singular Value List = \n");
	print_vector( s, n );
	printf("Left Singular Vectors = \n");
	print_matrix( u, m, n, ldu );
	printf("Right Singular Vectors = \n");
	print_matrix( vt, n, n, ldvt );
	/* Free workspace */
	free( (void*)work );
	exit( 0 );
	printf("END OF TEST04\n");
	printf("\n");
	return err;
	/* Parameters */
#undef M 6
#undef N 4
#undef LDA M
#undef LDU M
#undef LDVT N
}

//void init_vector( double *v, int n, int incx ) {
	//srand(time(NULL));
	//for ( int i = 0; i < n; i++)
		//v[i*incx] = 10.0 * rand()/RAND_MAX;
//}

//void init_matrix( double *a, int m, int n, int lda ) {
	//srand(time(NULL));
	//for ( int j = 0; j < n; j++)
		//for ( int i = 0; i < m; i++)
			//a[i+j*lda] = 10.0 * rand()/RAND_MAX;
//}

//void print_vector( double *a, int n, int incx ) {
	//for ( int i = 0; i < n; i++) 
		//printf(" %6.2f",a[i*incx]);
	//printf("\n");
//}

//void print_matrix( double *a, int m, int n, int lda ) {
	//for ( int i = 0; i < m; i++){
		//for ( int j = 0; j < n; j++) 
			//printf(" %6.2f",a[i+j*lda]);
		//printf("\n");
	//}
//} 
