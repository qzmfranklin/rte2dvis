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
		enum EQueryType	{
			kNone		= 0,
			kSingularValue	= 101, // only compute singular values
			kAll 		= 102// compute U and VT as well
		};
		enum EResetType {
			kReset,		// is reset
			kSet		// is set
		};		
		int		m;
		int		n;
		double*		a;
		int		info;
	private:
		int		_lda; 
		int		_ldu;
		int		_ldvt;
		double*		_work;
		int		_lwork; 
		enum EQueryType _query_flag;
		enum EResetType _reset_flag;
	public:
		dSVD():	m(0), 
			n(0), 
			a(NULL),
			info(0), 
			_lda(0),
			_ldu(0), 
			_ldvt(0), 
			_work(NULL), 
			_lwork(0), 
			_query_flag(kNone),
			_reset_flag(kReset) {}

		~dSVD() { _Free(); }; 

		void Set( int M, int N, double* A ) {
			m=M; 
			n=N; 
			a=A; 
			_lda=m; 
			_ldu=m; 
			_ldvt=n; 
			if (_query_flag!=kNone) 
				mkl_free(_work); 
			_query_flag=kNone; 
			_reset_flag=kSet;
		}

		void Reset() { _reset_flag=kReset; }

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
		/*
		 * Copy a -> b.
		 * Only used by SingularValueList and
		 * SingularValueDecomposition for copying
		 * the contents of a to b.
		 */
		void _Copy( double* restrict a, double* restrict b ) {
			mkl_domatcopy ( 'C', 'N', m, n, 1.0, a, m, b, m );
		}

		/*
		 * Release Memory.
		 */
		void _Free( void ) {
			if (  _query_flag!=kNone  )
				free(_work);
		}
};

/*
 * Global instance of dSVD. Used to manage memories with the help of 
 * dSVD.Free(). Usually, the programmer never needs to worry about 
 * freeing memories any more.
 */
extern dSVD gdSVD;
#endif /* End of protection macro _SVD_H_ */
