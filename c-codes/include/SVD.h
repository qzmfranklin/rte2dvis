#ifndef _SVD_H_
#define _SVD_H_
#include <stack>
#include <mkl.h>
#include <cassert>
class dSVD {
	/*
	 * Calling LAPACKE subroutines to compute SVD
	 * 	DOUBLE PRECISION
	 * Full Col-Major storage:
	 * 	LAPACK_COL_MAJOR
	 */
	private:
		std::stack<double*>	fs;
		std::stack<double*>	fu;
		std::stack<double*>	fvt;
	public:
		double*	a;
		int	m;
		int	n;
		int	lda;
		int	reset_flag;	// 1=IsReset, 0=IsNotReset
	public:
		dSVD(): a(NULL), m(0), n(0), lda(0), reset_flag(1) {}
		~dSVD() { Free(); }; 
		/* 
		 * Only computes the singular value list.
		 * 	a)	The double *s must NOT be allocated 
		 * 	beforehand. This function allocates memory 
		 * 	for double *s. If you use the global instance 
		 * 	gdSVD, there is NO need to manually free the 
		 * 	memory allocated in this function. All 
		 * 	memories allocated by global object will be 
		 * 	released automatically upon destruction.
		 * 	b)	The LAPACK subroutine assumes the
		 * 	leading dimension of s =1.
		 */
		void SingularValueList( double **s );
		/* 
		 * Only computes the singular value list.
		 * 	a)	The double *s must be allocated beforehand.
		 * 	b)	The LAPACK subroutine assumes the 
		 * 	leading dimension of s =1.
		 */
		void SingularValueList( double *s );
		/* 
		 * 1.	Computes all singular values, store them in 
		 * double *s_temp.
		 * 2.	Truncate the singular value list to the relative 
		 * error of eps. If, however, eps==0.0 (default), no 
		 * truncation will be made. Assume length(s_temp)=N.
		 * 3.	Return the first N left and right vectors in 
		 * u and vt, respectively. Return the first N singualr 
		 * values in s.
		 */
		//void SingularValueDecomposition( double *s, 
						//double *u,  int ldu, 
						//double *vt, int ldvt,
						//double eps=0.0 ); 


		/*
		 * The design here is to release the programmer from
		 * having to constantly worry about memory management.
		 * Please just use the global object, gdSVD, to do
		 * whatever double precision SVD you need to do. The
		 * memories allocated from within the dSVD class will
		 * be automatically released upon destruction. Or, if
		 * neccessary, one can call gdSVD.Free() to manually
		 * free all existing memories allocated by gdSVD.
		 */
		void Free() {
			while( !fs.empty() ) {
				mkl_free(fs.top());
				fs.pop();
			}
			while( !fu.empty() ) {
				mkl_free(fu.top());
				fu.pop();
			}
			while( !fvt.empty() ) {
				mkl_free(fvt.top());
				fvt.pop();
			}
		}
		void Set( double* A, int M, int N, int LDA ) {
			a=A; m=M; n=N; lda=LDA; reset_flag=0; }
		void Reset() { Set(NULL,0,0,0); reset_flag=1; }
}; 

/*
 * Global instance of dSVD. Used to manage memories with the help of 
 * dSVD.Free(). Usually, the programmer never needs to worry about 
 * freeing memories any more.
 */
extern dSVD gdSVD;
#endif /* End of protection macro _SVD_H_ */
