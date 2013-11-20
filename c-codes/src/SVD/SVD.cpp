#include "SVD.h"
#include <stdio.h>
#include <mkl.h>
#define MALLOC_ALIGNMENT 64
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
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

	if (  reset_flag == kReset  ) {
		fprintf(stderr, "A matrix is not set.\n");
		return -1;
	}

	enum EQueryType type;
	if ( !(u||vt) )
		type = kSingularValue;
	else
		type = kAll;

	if (  type == query_flag  ) {
		fprintf(stderr, "Already queried.\n");
		return 0; 
	}

	//TODO
	char *_jobu="N", *_jobvt="N";
	double _work[1]={0.0};
	int _ldu=1, _ldvt=1;
	mkl_free(work);	// Free previous workspace.
	switch (  type  ) {
		case kSingularValue: 
			lwork=-1;
			// Query for optimal workspace size.
			//dgesvd(_jobu,_jobvt,&m,&n,a,&lda,s,u,&ldu,vt,&ldvt,_work,&lwork,info);
			assert(info==0);
			lwork = (int) *_work;
			work = (double*) mkl_malloc( lwork * sizeof(double), MALLOC_ALIGNMENT );
			assert(work);
			query_flag = type;
			break;
		case kAll: 
			break;
		default:
			fprintf(stderr,"\tArgument 'type' is not one of kSingularValue, kAll.\n");
			return NULL;
			break;
	}

	return query_flag;
}

void dSVD::Free() {
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
