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
		fprintf(stderr, "\tThe matrix is not set yet.\n");
		fprintf(stderr, "\tCall dSVD::Set() first.\n");
		return -1;
	}

	enum EQueryType type;
	if ( u==NULL && vt==NULL )
		type = kSingularValue;
	else
		type = kAll;

	if (  type == _query_flag  ) {
		//fprintf(stderr, "\tWorkspace is already queried.");
		//fprintf(stderr, "\tDo nothing this time.\n");
		return 0; 
	}

	double __work[1];
	_Free();
	_lwork = -1;
	switch (  type  ) {
		case kSingularValue: 
			dgesvd("N","N",&m,&n,a,&_lda,s,u,&_ldu,
					vt,&_ldvt,__work,&_lwork,&info);
			break;
		case kAll: 
			dgesvd("A","A",&m,&n,a,&_lda,s,u,&_ldu,
					vt,&_ldvt,__work,&_lwork,&info);
			break;
	} 
	assert(!info);
	_lwork = (int) (*__work + 0.001);
	_work = (double*) mkl_malloc( _lwork * sizeof(double), MALLOC_ALIGNMENT );
	assert(_work);
	_query_flag = type;

#ifdef NDEBUG
	char *str = "------  ";
	printf("Completed the query for workspace:\n");
	printf("%s*__work\t= %f\n",str,*__work);
	printf("%s_lwork\t= %d\n",str,_lwork);
	printf("%s_work\t= %p\n",str,_work);
	printf("%s_query_flag\t= %d\n",str,(int)_query_flag);
#endif
	return _query_flag;
} 
/**************************************/ 
void dSVD::SingularValueListX( double *restrict s ) {
	//printf("dSVD::SingularValueListX()\n");
	dgesvd("N","N",&m,&n,a,&_lda,s,NULL,&_ldu,
			NULL,&_ldvt,_work,&_lwork,&info);
}
void dSVD::SingularValueList( double *restrict s ) {
	//printf("dSVD::SingularValueList()\n");
	double *b;
	b = (double*) mkl_malloc( m*n*sizeof(double), MALLOC_ALIGNMENT );
	assert(b);
	_Copy(a,b);
	dgesvd("N","N",&m,&n,b,&_lda,s,NULL,&_ldu,
			NULL,&_ldvt,_work,&_lwork,&info);
	mkl_free(b);
}
/**************************************/ 
void dSVD::SingularValueDecompositionX( 
		double *restrict s,
		double *restrict u, 
		double *restrict vt ) {
	//printf("dSVD::SingularValueDecompositionX()\n"); 
	dgesvd("A","A",&m,&n,a,&_lda,s,u,&_ldu,
			vt,&_ldvt,_work,&_lwork,&info);
} 
void dSVD::SingularValueDecomposition( 
		double *restrict s,
		double *restrict u, 
		double *restrict vt ) {
	//printf("dSVD::SingularValueDecomposition()\n"); 
	double *b;
	b = (double*) mkl_malloc( m*n*sizeof(double), MALLOC_ALIGNMENT );
	assert(b);
	_Copy(a,b);
	dgesvd("A","A",&m,&n,b,&_lda,s,u,&_ldu,
			vt,&_ldvt,_work,&_lwork,&info);
	mkl_free(b);
} 
/**************************************/ 
void dSVD::Print() {
	//printf("dSVD::Print()\n");

	char *s = "------  ";
	printf("%sm\t= %d\n",s,m);
	printf("%sn\t= %d\n",s,n);
	printf("%sa\t= %p\n",s,a);
	printf("%sinfo\t= %d\n",s,info);
	printf("%s_lda\t= %d\n",s,_lda);
	printf("%s_ldu\t= %d\n",s,_ldu);
	printf("%s_ldvt\t= %d\n",s,_ldvt);
	printf("%s_work\t= %p\n",s,_work);
	printf("%s_lwork\t= %d\n",s,_lwork);
	printf("%s_query_flag\t= %d\n",s,(int)_query_flag);
	printf("%s_reset_flag\t= %d\n",s,(int)_reset_flag);
}
