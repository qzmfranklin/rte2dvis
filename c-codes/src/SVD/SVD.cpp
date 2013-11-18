#include "SVD.h"
#include <stdio.h>
#include <mkl.h>
#define MALLOC_ALIGNMENT 64
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
/*******************************************************************************/
dSVD gdSVD; 
/*******************************************************************************/
void dgesvd( const char* jobu, const char* jobvt, const int* m, 
             const int* n, double* a, const int* lda, double* s,
             double* u, const int* ldu, double* vt, const int* ldvt,
             double* work, const int* lwork, int* info );
/*******************************************************************************/
void dSVD::QueryWorkspace(const char* jobu, const char* jobvt) {
	// TODO
}

int dSVD::SingularValueList( double *s ) { 
	printf("dSVD::SingularValueList(double *s)\n");
	// TODO

	return info;
}

//void dSVD::SingularValueDecomposition( 	double *s, 
					//double *u,  int ldu, 
					//double *vt, int ldvt,
					//double eps ) {
					//// TODO
//}
