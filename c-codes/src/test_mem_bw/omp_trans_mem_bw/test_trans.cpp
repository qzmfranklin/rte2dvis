#include <stdio.h>
#include <assert.h>
#include "trans.h"
#include "utils.h"
/******************************************************************************/
#define N 15671
#define N_THREADS 8
#define MALLOC_ALIGNMENT 64
/*******************************************************************************/
using namespace utils;
enum ETestType { kEASY, kMKL, kSEQ, kOMP };
void init(const int n,double *restrict &a,double *restrict &b);
void release(double *restrict a, double *restrict b);
void matcpy(const int n,const double *restrict a,double *restrict b);
double test(const int n,const double *restrict a,double *restrict b,
	double (*fun)(const int,const double *restrict,const double *restrict),
	enum ETestType type,const int nthreads=1);
double time(const int n,const double *restrict a,double *restrict b,
		enum ETestType type,const int nthreads=1);
double diffnorm(const int n,const double *restrict a,const double *restrict b);
double maxdiff(const int n,const double *restrict a,const double *restrict b);
/*******************************************************************************/
int main(int argc, char const* argv[])
{
	const char* rows[10] = {"trans_easy","trans_mkl","trans_seq",
		"trans_omp_2","trans_omp_3","trans_omp_4","trans_omp_5",
		"trans_omp_6","trans_omp_7","trans_omp_8"};
	const char* cols[2] = {"diffnorm","maxdiff"};
	double data[10*2];


	double *a,*b; 
	init(N,a,b);
	////print_matrix("original a",N,N,a); 
	//matcpy(N,a,b); 
	//print_matrix("original b",N,N,b); 
	//matcpy(N,a,b); 
	//trans_easy(N,b);
	////print_matrix("trans_easy(b)",N,N,b); 
	//matcpy(N,a,b); 
	//trans_mkl(N,b);
	////print_matrix("trans_mkl(b)",N,N,b);
	//matcpy(N,a,b); 
	//trans_omp(N,b,2);
	////print_matrix("trans_omp(b,2)",N,N,b);
	
	data[0+0]	= test(N,a,b,&diffnorm,kEASY);
	data[10+0]	= test(N,a,b,&maxdiff,kEASY);
	data[0+1]	= test(N,a,b,&diffnorm,kMKL);
	data[10+1]	= test(N,a,b,&maxdiff,kMKL);
	data[0+2]	= test(N,a,b,&diffnorm,kSEQ);
	data[10+2]	= test(N,a,b,&maxdiff,kSEQ);
	for (int i = 1; i < 8; i++) {
		data[0+2+i]	= test(N,a,b,&diffnorm,kOMP,1+i);
		data[10+2+i]	= test(N,a,b,&maxdiff,kOMP,1+i);
	}

	release(a,b);


	Table tbl;
	tbl.dim(10,2);
	tbl.rows(rows);
	tbl.cols(cols);
	tbl.data(data);

	char fname[FILENAME_MAX];
	verify_dir("OUTPUT");
	sprintf(fname,"OUTPUT/test_trans_%d.txt",N);
	link_cout(fname);

	char banner[BUFSIZ];
	sprintf(banner,"verify: N=%d\n",N);
	tbl.print(banner);


	unlink_cout();
	fprintf(stdout,"Output to %s\n",fname); 

	return 0;
}

/*
 * Verify the validity of each method
 */
double test(const int n,const double *restrict a,double *restrict b,
	double (*fun)(const int, const double *restrict, const double *restrict),
	enum ETestType type,const int nthreads)
{
	double err;

	matcpy(n,a,b);

	switch (  type  ) {
	case kEASY: 
		trans_easy(n,b);
		err = (*fun)(n,a,b);
		break;
	case kMKL: 
		trans_mkl(n,b);
		err = (*fun)(n,a,b);
		break;
	case kSEQ: 
		trans_seq(n,b);
		err = (*fun)(n,a,b);
		break;
	case kOMP: 
		trans_omp(n,b,nthreads);
		err = (*fun)(n,a,b);
		break;
	}

	return err;
}

/*
 * Time each method
 */
double time(const int n,const double *restrict a,double *restrict b,
		enum ETestType type,const int nthreads)
{
#define COUNT 10
	matcpy(n,a,b);

	TimeStamp clk(COUNT);
	for (int i = 0; i < COUNT; i++) { 
		clk.tic(); 
		switch (  type  ) {
		case kEASY: 
			trans_easy(n,b);
			break;
		case kMKL: 
			trans_mkl(n,b);
			break;
		case kSEQ: 
			trans_seq(n,b);
			break;
		case kOMP: 
			trans_omp(n,b,nthreads);
			break;
		}
		clk.toc();
	}

	return (8*n*n-8*n)/clk.median();
#undef COUNT
}

/*
 * Return the Euclidean Norm[aT-b]
 */
double diffnorm(const int n,const double *restrict a,const double *restrict b)
{
	double err=0.0;
	#pragma omp parallel 				\
	num_threads(N_THREADS)				\
	default(none)					\
	shared(n,a,b)					\
	reduction(+:err)
	{
		double xx, sum=0.0;
		#pragma omp for
		for (int j = 0; j < n; j++) {
			for (int i = 0; i < n; i++) { 
				xx = a[j+i*n] - b[i+j*n];
				sum += xx*xx;
			} 
		}
		err += sum;
	}
	return sqrt(err);
}

/*
 * Return the Max[aT-b]
 */
double maxdiff(const int n,const double *restrict a,const double *restrict b)
{
	double res=0.0;
	#pragma omp parallel				\
	num_threads(N_THREADS)				\
	default(none)					\
	shared(n,a,b,res)
	{
		double tmp=0.0,xx;
		#pragma omp for
		for (int j = 0; j < n; j++) {
			for (int i = 0; i < n; i++)
				if (    (xx=fabs(a[j+i*n]-b[i+j*n])) > tmp    )
					tmp = xx;
		}
		#pragma omp critical
		if (tmp>res) res=tmp;
	}
	return res;
}

/*
 * Only running on my own laptop.
 */
void init(const int n,double *restrict &a,double *restrict &b)
{
	a=(double*)mkl_malloc(n*n*sizeof(double),MALLOC_ALIGNMENT);
	b=(double*)mkl_malloc(n*n*sizeof(double),MALLOC_ALIGNMENT);
	assert(a);
	assert(b);
	init_matrix(n,n,a,n);
	init_matrix(n,n,b,n);
}

/*
 * Lazy release
 */
void release(double *restrict a, double *restrict b)
{
	mkl_free(a);
	mkl_free(b);
	a=NULL;
	b=NULL;
}

/*
 * Copy memory content a->b
 */
void matcpy(const int n,const double *restrict a,double *restrict b)
{
	mkl_domatcopy('C','N',n,n,1.0,a,n,b,n);
} 
