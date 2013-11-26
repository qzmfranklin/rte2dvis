#include <stdio.h>
#include <assert.h>
#include "trans.h"
#include "utils.h"
/******************************************************************************/
#define N 24525
#define MALLOC_ALIGNMENT 64
/******************************************************************************/
enum ETestType { kEASY, kMKL, kSEQ, kOMP };
void init(const int n,double *restrict &a);
void release(double *restrict &a);
double time(const int n,double *restrict a,
		enum ETestType type,const int nthreads=1);
/******************************************************************************/
int main(int argc, char const* argv[])
{
	const char* rows[10] = {"trans_easy","trans_mkl","trans_seq",
		"trans_omp_2","trans_omp_3","trans_omp_4","trans_omp_5",
		"trans_omp_6","trans_omp_7","trans_omp_8"};
	const char* cols[1] = {"mem-bw"};
	double data[10*1];

	double *a; 
	init(N,a);
	
	data[0]	= time(N,a,kEASY); 
	data[1]	= time(N,a,kMKL); 
	data[2]	= time(N,a,kSEQ); 
	for (int i = 1; i < 8; i++)
		data[2+i] = time(N,a,kOMP,1+i); 

	release(a);


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
double time(const int n,double *restrict a,
		enum ETestType type,const int nthreads)
{
#define COUNT 10
	TimeStamp clk(COUNT);
	for (int i = 0; i < COUNT; i++) { 
		clk.tic(); 
		switch (  type  ) {
		case kEASY: 
			trans_easy(n,a);
			break;
		case kMKL: 
			trans_mkl(n,a);
			break;
		case kSEQ: 
			trans_seq(n,a);
			break;
		case kOMP: 
			trans_omp(n,a,nthreads);
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
void init(const int n,double *restrict &a)
{
	a=(double*)mkl_malloc(n*n*sizeof(double),MALLOC_ALIGNMENT);
	assert(a);
	init_matrix(n,n,a,n);
}

/*
 * Lazy release
 */
void release(double *restrict &a)
{
	mkl_free(a);
	a=NULL;
}
