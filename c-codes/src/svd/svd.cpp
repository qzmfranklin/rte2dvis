#include "svd.h"
#include <stdio.h>
#include <mkl.h>
#define MALLOC_ALIGNMENT 64
/*******************************************************************************/
SVD_D gSVD_D; 
/*******************************************************************************/
void dgesvd( const char* jobu, const char* jobvt, const int32_t* m, 
		const int32_t* n, double* a, const int32_t* lda, double* s,
		double* u, const int32_t* ldu, double* vt, const int32_t* ldvt,
		double* work, const int32_t* lwork, int32_t* info );
/*******************************************************************************/
SVD_D::SVD_D():m(0), 
	n(0), 
	a(NULL),
	info(0), 
	_lda(0),
	_ldu(0), 
	_ldvt(0), 
	_work(NULL), 
	_lwork(0), 
	_query_flag(kNone),
	_reset_flag(kReset) {
		//printf("SVD_D::SVD_D()\n");
	}

SVD_D::~SVD_D() { 
	//printf("SVD_D::~SVD_D()\n");
	_Free(); 
}; 

void SVD_D::Set( int32_t M, int32_t N, double* A ) {
	//printf("SVD_D::Set()\n");
	m=M; 
	n=N; 
	a=A; 
	_lda=m; 
	_ldu=m; 
	_ldvt=n; 
	_Free();
	_query_flag=kNone; 
	_reset_flag=kSet;
}
/**************************************/ 
int32_t SVD_D::QueryWorkspace( 
		double *restrict s,
		double *restrict u, 
		double *restrict vt ) {
	//printf("SVD_D::QueryWorkspace()\n");
	if (  _reset_flag == kReset  ) {
		fprintf(stderr, "\tThe matrix is not set yet. Call SVD_D::Set() first.\n");
		return -1;
	}

	enum EQueryType type;
	if ( u==NULL && vt==NULL )
		type = kSingularValue;
	else
		type = kAll;

	if (  type == _query_flag  ) {
		fprintf(stderr, "\tWorkspace is already queried. Do nothing this time\n");
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
	_lwork = (int32_t) (*__work + 0.001);
	_work = (double*) mkl_malloc( _lwork * sizeof(double), MALLOC_ALIGNMENT );
	assert(_work);
	_query_flag = type;

#ifdef NDEBUG
	char *str = "------  ";
	printf("Completed the query for workspace:\n");
	printf("%s*__work\t= %f\n",str,*__work);
	printf("%s_lwork\t= %d\n",str,_lwork);
	printf("%s_work\t= %p\n",str,_work);
	printf("%s_query_flag\t= %d\n",str,(int32_t)_query_flag);
#endif
	return _query_flag;
} 
/**************************************/ 
void SVD_D::SingularValueListX( double *restrict s ) {
	//printf("SVD_D::SingularValueListX()\n");
	dgesvd("N","N",&m,&n,a,&_lda,s,NULL,&_ldu,
			NULL,&_ldvt,_work,&_lwork,&info);
}
void SVD_D::SingularValueList( double *restrict s ) {
	//printf("SVD_D::SingularValueList()\n");
	double *b;
	b = (double*) mkl_malloc( m*n*sizeof(double), MALLOC_ALIGNMENT );
	assert(b);
	_Copy(a,b);
	dgesvd("N","N",&m,&n,b,&_lda,s,NULL,&_ldu,
			NULL,&_ldvt,_work,&_lwork,&info);
	mkl_free(b);
}
/**************************************/ 
void SVD_D::SingularValueDecompositionX( 
		double *restrict s,
		double *restrict u, 
		double *restrict vt ) {
	//printf("SVD_D::SingularValueDecompositionX()\n"); 
	dgesvd("A","A",&m,&n,a,&_lda,s,u,&_ldu,
			vt,&_ldvt,_work,&_lwork,&info);
} 
void SVD_D::SingularValueDecomposition( 
		double *restrict s,
		double *restrict u, 
		double *restrict vt ) {
	//printf("SVD_D::SingularValueDecomposition()\n"); 
	double *b;
	b = (double*) mkl_malloc( m*n*sizeof(double), MALLOC_ALIGNMENT );
	assert(b);
	_Copy(a,b);
	dgesvd("A","A",&m,&n,b,&_lda,s,u,&_ldu,
			vt,&_ldvt,_work,&_lwork,&info);
	mkl_free(b);
} 
/**************************************/ 
void SVD_D::Print() {
	//printf("SVD_D::Print()\n"); 
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
	printf("%s_query_flag\t= %d\n",s,(int32_t)_query_flag);
	printf("%s_reset_flag\t= %d\n",s,(int32_t)_reset_flag);
}
