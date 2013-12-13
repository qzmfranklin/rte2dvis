#ifndef _SVD_H_
#define _SVD_H_
#include <stack>
#include <mkl.h>
#include <cassert>
#define MALLOC_ALIGNMENT 64
/******************************************************************************/ 
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
class SVD_D {
	public:
		int32_t		m;
		int32_t		n;
		double*		a;
		int32_t		info;
	private:
		int32_t		_lda; 
		int32_t		_ldu;
		int32_t		_ldvt;
		double*		_work;
		int32_t		_lwork; 
		enum EQueryType	{
			kNone		= 0,
			kSingularValue	= 101,	// only compute singular values
			kAll 		= 102	// compute U and VT as well
		} _query_flag;
		enum EResetType {
			kReset		= 0,	// is reset
			kSet   		= 999	// is set
		} _reset_flag;
	public:
		SVD_D(); 
		~SVD_D(); 
		void Print(); 
		void Set( int32_t M, int32_t N, double* A ); 
		void Reset() { _reset_flag=kReset; }

		/*
		 * Query for workspace.
		 * Return values:
		 * 	-1	Matrix A not set 	Failed
		 * 	0	Already queried		Succeeded
		 * 	>0	Query Status		Succeeded
		 */
		int32_t QueryWorkspace( 
				double *restrict s,
				double *restrict u =NULL,
				double *restrict vt=NULL );

		/*
		 * Only get the singular value list. 
		 * Destroy the original matrix A.
		 * Returns the info from dgesvd().
		 */
		void SingularValueListX( double *restrict s );

		/*
		 * Not destroy the original matrix A.
		 */
		void SingularValueList( double *restrict s );
		
		/*
		 * Return the sigular value list and the U and VT.
		 * Destroy the original matix A.
		 * Returns the info from dgesvd().
		 */
		void SingularValueDecompositionX( 
				double *restrict s,
				double *restrict u, 
				double *restrict vt ); 
		/*
		 * Returns the info from dgesvd().
		 */
		void SingularValueDecomposition( 
				double *restrict s,
				double *restrict u, 
				double *restrict vt );

		/*
		 * Release Memory.
		 */
		void Flush( void ) {
			printf("SVD_D::Flush()\n");
			_Free();
		}

	private:
		/*
		 * Copy a -> b.
		 * Only used by SingularValueList and
		 * SingularValueDecomposition for copying
		 * the contents of a to b.
		 */
		void _Copy( double* restrict a, double* restrict b ) {
			//printf("SVD_D::_Copy()\n");
			mkl_domatcopy ( 'C', 'N', m, n, 1.0, a, m, b, m );
		}

		/*
		 * Release Memory.
		 */
		void _Free( void ) {
			//printf("SVD_D::_Free()\n");
			if (  _query_flag!=kNone  ) {
				//printf("Freeing _work...\n");
				mkl_free(_work);
				_query_flag = kNone;
				_work = NULL;
			}
		}

};
extern SVD_D gSVD_D;
/******************************************************************************/ 
#endif /* End of protection macro _SVD_H_ */
