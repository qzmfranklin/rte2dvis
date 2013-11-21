#include "SVD.h"
#include <stdio.h>
#include <mkl.h>
#define MALLOC_ALIGNMENT 64
/*******************************************************************************/
dSVD gdSVD; 
/*******************************************************************************/
void dgesvd( const char* jobu, const char* jobvt, const int* m, 
             const int* n, double* a, const int* lda, double* s,
             double* u, const int* ldu, double* vt, const int* ldvt,
             double* work, const int* lwork, int* info );
/*******************************************************************************/
int dSVD::QueryWorkspace( 
		double *restrict s,
		double *restrict u, 
		double *restrict vt ) {
	printf("dSVD::QueryWorkspace()\n");

	if (  _reset_flag == kReset  ) {
		fprintf(stderr, "\tThe matrix is not set yet.\n");
		fprintf(stderr, "\tCall dSVD::Set() first.\n");
		return -1;
	}

	enum EQueryType type;
	if ( !(u||vt) )
		type = kSingularValue;
	else
		type = kAll;

	if (  type == _query_flag  ) {
		fprintf(stderr, "\tWorkspace is already queried.\n");
		fprintf(stderr, "\tDo nothing this time.\n");
		return 0; 
	}

	double __work[1];
	int _ldu, _ldvt;
	mkl_free(_work); // Free previous workspace.
	switch (  type  ) {
		case kSingularValue: 
			_ldu = 1; _ldvt = 1; _lwork=-1;
			dgesvd("N","N",&m,&n,a,&_lda,s,u,&_ldu,
					vt,&_ldvt,__work,&_lwork,&info);
			break;
		case kAll: 
			_ldu = m; _ldvt = n; _lwork=-1;
			dgesvd("A","A",&m,&n,a,&_lda,s,u,&_ldu,
					vt,&_ldvt,__work,&_lwork,&info);
			break;
	} 
	assert(!info);
	_lwork = (int) (*__work + 0.001);
	_work = (double*) mkl_malloc( _lwork * sizeof(double), MALLOC_ALIGNMENT );
	assert(_work);
	_query_flag = type;

	//printf("*__work = %f\n",*__work);
	//printf("_lwork = %d\n",_lwork);
	//printf("_work = %p\n",_work);
	//printf("_query_flag = %d\n",(int)_query_flag);
	return _query_flag;
} 

void dSVD::Print() {
	int		m;
	int		n;
	double*		a;
	int		info;
	int		_lda; 
	int		_ldu;
	int		_ldvt;
	double*		_work;
	int		_lwork; 
	_query_flag;
	_reset_flag;
	printf("\n");
}
