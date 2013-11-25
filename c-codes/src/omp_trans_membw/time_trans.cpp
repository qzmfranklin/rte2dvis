#include <stdio.h>
#include <assert.h>
#include "trans.h"
#include "utils.h"
/******************************************************************************/
#define N 24525
#define MALLOC_ALIGNMENT 64
/******************************************************************************/
using namespace utils;
enum ETestType { kEASY, kMKL, kSEQ, kOMP };
void init(const int n,double *restrict &a,double *restrict &b);
void release(double *restrict a, double *restrict b);
void matcpy(const int n,const double *restrict a,double *restrict b);
double time(const int n,const double *restrict a,double *restrict b,
		enum ETestType type,const int nthreads=1);
/******************************************************************************/
int main(int argc, char const* argv[])
{
	const char* rows[10] = {"trans_easy","trans_mkl","trans_seq",
		"trans_omp_2","trans_omp_3","trans_omp_4","trans_omp_5",
		"trans_omp_6","trans_omp_7","trans_omp_8"};
	const char* cols[1] = {"mem-bw"};
	double data[10*1];

	double *a,*b; 
	init(N,a,b);
	
	data[0]	= time(N,a,b,kEASY); 
	data[1]	= time(N,a,b,kMKL); 
	data[2]	= time(N,a,b,kSEQ); 
	for (int i = 1; i < 8; i++)
		data[2+i] = time(N,a,b,kOMP,1+i); 

	release(a,b);


	Table tbl;
	tbl.dim(10, 1);
	tbl.rows(rows);
	tbl.cols(cols);
	tbl.data(data);

	char fname[FILENAME_MAX];
	verify_dir("OUTPUT");
	sprintf(fname,"OUTPUT/time_trans_%d.txt",N);
	link_cout(fname);

	char banner[BUFSIZ];
	sprintf(banner,"memory bandwidth in bytes/cycle: N=%d\n",N);
	tbl.print(banner);


	unlink_cout();
	fprintf(stdout,"Output to %s\n",fname); 

	return 0;
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

	return (8l*n*n-8*n)/clk.median();
#undef COUNT
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
