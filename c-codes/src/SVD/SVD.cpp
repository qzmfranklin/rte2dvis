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
	//printf("dSVD::QueryWorkspace()\n");

	if (  _reset_flag == kReset  ) {
		fprintf(stderr, "The matrix is not set yet.\n");
		fprintf(stderr, "Call dSVD::Set() first.\n");
		return -1;
	}

	enum EQueryType type;
	if ( !(u||vt) )
		type = kSingularValue;
	else
		type = kAll;

	if (  type == _query_flag  ) {
		fprintf(stderr, "Workspace is already queried.\n");
		fprintf(stderr, "Do nothing this time.\n");
		return 0; 
	}

	//TODO
	double __work[1];
	int _ldu, _ldvt;
	mkl_free(_work); // Free previous workspace.
	switch (  type  ) {
		case kSingularValue: 
			_ldu = 1; _ldvt = 1; _lwork=-1;
			dgesvd("N","N",	&m,&n,a,&_lda, s,u,&_ldu,vt,&_ldvt, __work,&_lwork,&info);
			assert(!info); // Query for optimal workspace size.
			_lwork = (int) *__work;
			_work = (double*) mkl_malloc( _lwork * sizeof(double), MALLOC_ALIGNMENT );
			assert(_work);
			_query_flag = type;
			break;
		case kAll: 
			break;
		default:
			fprintf(stderr,"\tArgument 'type' is not one of kSingularValue, kAll.\n");
			return NULL;
			break;
	}

	printf("_work = %p\n",_work);
	printf("_query_flag = %d\n",(int)_query_flag);
	return _query_flag;
} 
