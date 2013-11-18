#ifndef _SVD_H_
#define _SVD_H_
#include <stack>
#include <mkl.h>
#include <cassert>
class dSVD {
	/*
	 * Calling LAPACK subroutines to compute SVD
	 * 	DOUBLE PRECISION
	 * Full Col-Major storage:
	 * 	LAPACK_COL_MAJOR
	 */
	private:
		std::stack<double*>	fs;
		std::stack<double*>	fu;
		std::stack<double*>	fvt;
		int	query_flag;	// Whether the query for workspace 
					// is made? 1=Yes, 0=No.
	public:
		double*	a;
		int	m;
		int	n;
		int	lda;
	public:
		dSVD(): a(NULL), m(0), n(0), lda(0), query_flag(0) {}
		~dSVD() { Free(); }; 

		void Set( double* A, int M, int N, int LDA ) {
			memcpy( a, A, sizeof(A) );
			m=M; n=N; lda=LDA; query_flag=0; 
		}


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
		 * 	c) 	If return value == 0, success. Other-
		 * 	wise, failure.
		 * 	d)	int *len stores the length of s.
		 */
		int SingularValueList( double **s, int *len );

		/* 
		 * Only computes the singular value list.
		 * 	a)	The double *s must be allocated beforehand.
		 * 	b)	The LAPACK subroutine assumes the 
		 * 	leading dimension of s =1.
		 * 	c) 	If return value == 0, success. Other-
		 * 	wise, failure.
		 */
		int SingularValueList( double *s );

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
	private:
		/*
		 * Query for workspace.
		 */
		void QueryWorkspace();


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
			printf("dSVD::Free()\n");
			printf("	|fs.size() = %lu \n",fs.size());
			while( !fs.empty() ) {
				//printf("poping fs.top()\n");
				mkl_free(fs.top());
				fs.pop();
			}
			printf("	|fu.size() = %lu \n",fu.size());
			while( !fu.empty() ) {
				//printf("poping fu.top()\n");
				mkl_free(fu.top());
				fu.pop();
			}
			printf("	|fvt.size() = %lu \n",fvt.size());
			while( !fvt.empty() ) {
				//printf("poping fvt.top()\n");
				mkl_free(fvt.top());
				fvt.pop();
			}
		}
}; 

/*
 * Global instance of dSVD. Used to manage memories with the help of 
 * dSVD.Free(). Usually, the programmer never needs to worry about 
 * freeing memories any more.
 */
extern dSVD gdSVD;
#endif /* End of protection macro _SVD_H_ */
