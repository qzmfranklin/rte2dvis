#include "SVD.h"
#include <stdio.h>
#include <mkl.h>
/*******************************************************************************/
dSVD gdSVD; 
/*******************************************************************************/
int LAPACKE_dgesdd( int matrix_order, char jobz, int m,
                    int n, double* a, int lda, double* s,
                    double* u, int ldu, double* vt,
                    int ldvt );
//int LAPACKE_zgesdd( int matrix_order, char jobz, int m,
                    //int n, double complex* a,
                    //int lda, double* s, double complex* u,
                    //int ldu, double complex* vt,
                    //int ldvt );
/*******************************************************************************/

void dSVD::SingularValueList( double *s ) { 
	LAPACKE_dgesdd( LAPACK_COL_MAJOR, 'N', m, n, a, lda, s, NULL, 1, NULL, 1 );
}

//void dSVD::SingularValueDecomposition( 	double *s, 
					//double *u,  int ldu, 
					//double *vt, int ldvt,
					//double eps ) {
//}
