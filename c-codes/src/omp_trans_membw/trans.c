#include <stdio.h>
#include <mkl.h>
#include <omp.h>
#include "trans.h"
#include <assert.h>
/******************************************************************************/
#define B 128
#define MALLOC_ALIGNMENT 64
/******************************************************************************/
static void swap(double *a, double *b)
{
	double c=*a;
	*a=*b;
	*b=c;
}

void trans_easy(const int n,double *restrict a)
{
	for (int j = 0; j < n; j++) 
		for (int i = 0; i < j; i++)
			swap(a+i+j*n,a+j+i*n);
} 

void trans_mkl(const int n,double *restrict a)
{
	mkl_dimatcopy('C','T',n,n,1.0,a,n,n);
} 

void trans_seq(const int n,double * restrict a)
{
	trans_omp(n,a,1);
}

static void transB_diag(const int n, double *restrict a, const int lda)
{
	for (int j = 0; j < n; j++)
		for (int i = 0; i < j; i++)
			swap(a+i+j*lda,a+j+i*lda);
	//mkl_dimatcopy('C','T',B,B,1.0,a,lda,lda);
}

static void transB_offdiag(const int m, const int n,
		double *restrict a,double *restrict b,const int lda)
{
	for (int j = 0; j < n; j++)
		for (int i = 0; i < m; i++)
			swap(a+i+j*lda,b+j+i*lda); 
	//static double Btmp[B*B]; 
	//mkl_domatcopy('C','T',B,B,1.0,a,lda,Btmp,B);
	//mkl_domatcopy('C','T',B,B,1.0,b,lda,a,lda);
	//mkl_domatcopy('C','N',B,B,1.0,Btmp,B,b,lda);
}

void trans_omp(const int n,double * restrict a,const int nthreads)
{
	int B_num = n/B;
	int res_row = n - B_num*B; 

	/*
	 * right-lower corner
	 */
	transB_diag(res_row,a+B_num*B+B_num*B*n,n);

	/*
	 * diagonal blocks, right side and down side
	 */
#pragma omp parallel for			\
	num_threads(nthreads)			\
	default(none)				\
	shared(n,a,B_num,res_row)
	for(int i=0; i < B_num; i++) {
		transB_diag(B,a+i*B+i*B*n,n);
		transB_offdiag(res_row,B,
				a+B_num*B+i*B*n,
				a+i*B+B_num*B*n,
				n);
	}

	/*
	 * off-diagonal blocks
	 */
	int Boff_num=(B_num*(B_num-1))/2;
	int *ki, *kj;
	ki = (int*)mkl_malloc( Boff_num*sizeof(int),MALLOC_ALIGNMENT );
	kj = (int*)mkl_malloc( Boff_num*sizeof(int),MALLOC_ALIGNMENT );
	assert(ki&&kj); 
	ki[0]=0;
	kj[0]=1;
	for (int k = 1; k < Boff_num; k++) { 
		ki[k] = ki[k-1] + 1;
		kj[k] = kj[k-1];
		if (ki[k]==kj[k]) {
			ki[k]=0;
			kj[k]++;
		}
	} 
#pragma omp parallel for			\
	num_threads(nthreads)			\
	default(none)				\
	shared(n,a,B_num,Boff_num,ki,kj)
	for (int k = 0; k < Boff_num; k++)
		transB_offdiag(B,B,
				a+kj[k]*B+ki[k]*B*n,
				a+ki[k]*B+kj[k]*B*n,
				n);

	mkl_free(ki);
	mkl_free(kj);
} 
/******************************************************************************/
