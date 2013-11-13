#ifndef _SVD_H_
#define _SVD_H_
#include <mkl.h>
#include <cassert>

lapack_int LAPACKE_dgesdd( int matrix_order, char jobz, lapack_int m,
                           lapack_int n, double* a, lapack_int lda, double* s,
                           double* u, lapack_int ldu, double* vt,
                           lapack_int ldvt );
lapack_int LAPACKE_zgesdd( int matrix_order, char jobz, lapack_int m,
                           lapack_int n, lapack_complex_double* a,
                           lapack_int lda, double* s, lapack_complex_double* u,
                           lapack_int ldu, lapack_complex_double* vt,
                           lapack_int ldvt );
class dSVD {
	/*
	 * Calling lapack subroutines to compute SVD
	 * 	DOUBLE PRECISION
	 * Full Col-Major storage:
	 * 	LAPACK_COL_MAJOR
	 */
	public:
		double*	a;
		int	m;
		int	n;
		int	lda;
		int	reset_flag;	// 1=IsReset, 0=IsNotReset
	public:
		dSVD() a(NULL), m(0), n(0), lda(0), reset_flag(1) {}
		~dSVD() {}; 
		void Set( double* A, int M, int N, int LDA ) {
			a=A; m=M; n=N; lda=LDA; reset_flag=0; }
		void Reset() { Set(NULL,0,0,0); reset_flag=1; }
		/* 
		 * Only computes the singular value list.
		 * 	a)	The double *s must be allocated beforehand.
		 * 	b)	The LAPACK subroutine assumes the leading dimension of s =1.
		 */
		int SingularValueList( double *s );
		/* 
		 * 1.	Computes all singular values, store them in double *s_temp.
		 * 2.	Truncate the singular value list to the relative error of eps. By
		 * 	default, the eps=1.0E-15 is virtually zero-tolerance for double precision.
		 * 	Assume length(s_temp)=N.
		 * 3.	Return the first N left and right vectors in u and vt, respectively.
		 * 	Return the first N singualr values in s.
		 */
		int SingularValueDecomposition( double *s, 
						double *u,  int ldu, 
						double *vt, int ldvt,
						double eps=1.0E-15 ); 
		enum ESVDMethod {
			DevideAndConquere = 1,
			DirectAndIterative
		}; 
}; 
#endif /* End of protection macro _SVD_H_ */
