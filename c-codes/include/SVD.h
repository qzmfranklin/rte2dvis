#ifndef _SVD_H_
#define _SVD_H_
#include <stack>
#include <mkl.h>
#include <cassert>
#define MALLOC_ALIGNMENT 64

class dSVD {
	/*
	 * Calling LAPACK subroutines to compute SVD.
	 * 	DOUBLE PRECISION
	 * Full Col-Major storage:
	 * 	LAPACK_COL_MAJOR
	 * 
	 * Assumed that A, U and VT are packed:
	 * 	LDA = M, LDU = M, LDVT = N
	 * where
	 *	 	A = U S VT
	 * 	A = M*N
	 * 	U = M*M
	 * 	VT= N*N
	 * 	S = MIN(M,N)
	 */
	public:
		int		m;
		int		n;
		double*		a;
		int		info;
	private:
		int		lda; 
		int		ldu;
		int		ldvt;
		double*		work;
		int		lwork;
		std::stack<double*>	fs;
		std::stack<double*>	fu;
		std::stack<double*>	fvt;
		enum EQueryType	{
			kNone		= 0,
			kSingularValue	= 101, // only compute singular values
			kAll 		= 102// compute U and VT as well
		} query_flag;
		enum EResetType {
			kReset,		// is reset
			kSet		// is set
		} reset_flag;		
	public:
		dSVD():	m(0), n(0), 
			a(NULL),
			info(0), 
			lda(0), ldu(0), ldvt(0), 
			work(NULL), lwork(0), 
			query_flag(kNone),
			reset_flag(kReset) {}

		~dSVD() { Free(); }; 

		void Set( int M, int N, double* A ) {
			m=M; 
			n=N; 
			a=A; 
			lda=m; 
			ldu=m; 
			ldvt=n;

			if (query_flag!=kNone) 
				mkl_free(work); 
			query_flag=kNone; 
			reset_flag=kSet;
		}

		void Reset() { reset_flag=kReset; }

		/*
		 * Query for workspace.
		 * Return values:
		 * 	-1	Matrix A not set
		 * 	0	Already queried
		 * 	>0	Query Status
		 */
		int QueryWorkspace( 
				double *restrict s,
				double *restrict u =NULL,
				double *restrict vt=NULL );

		/*
		 * Only get the singular value list. 
		 * Destroy the original matrix A.
		 */
		int SingularValueListX( double *s );
		
		/*
		 * Return the sigular value list and the U and VT.
		 * Destroy the original matix A.
		 */
		int SingularValueDecompositionX( 
				double *restrict s,
				double *restrict u, 
				double *restrict vt );

		/*
		 * Only get the singular value list. 
		 * Not destroy the original matrix A.
		 */
		int SingularValueList( double *s );
		
		/*
		 * Return the sigular value list and the U and VT.
		 * Not destroy the original matrix A.
		 */
		int SingularValueDecomposition( 
				double *restrict s,
				double *restrict u, 
				double *restrict vt );

	private:
		void Copy( int m, int n, double* restrict a, double* restrict b ) {
			mkl_domatcopy ( 'C', 'N', m, n, 1.0, a, m, b, m );
		}

		/*
		 * Free memories.
		 * The design here is to release the programmer from
		 * having to constantly worry about memory management.
		 * Please just use the global object, gdSVD, to do
		 * whatever double precision SVD you need to do. The
		 * memories allocated from within the dSVD class will
		 * be automatically released upon destruction. Or, if
		 * neccessary, one can call gdSVD.Free() to manually
		 * free all existing memories allocated by gdSVD.
		 */
		void Free(); 
};

/*
 * Global instance of dSVD. Used to manage memories with the help of 
 * dSVD.Free(). Usually, the programmer never needs to worry about 
 * freeing memories any more.
 */
extern dSVD gdSVD;
#endif /* End of protection macro _SVD_H_ */
