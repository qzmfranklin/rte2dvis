#include "SVD.h"
#include "Utils.h"
#include <stdio.h>
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define MALLOC_ALIGNMENT 64
using namespace utils;
/*******************************************************************************/
int main(int argc, char const* argv[]);
enum ETestType {
	kQueryS		= 101,
	kSVLX		= 102,
	kSVL		= 103, 
	kQuerySUVT	= 201,
	kSVDX		= 202,
	kSVD		= 203
};
double time( double* restrict a, enum ETestType type );
static void init_mem( double* &a );
static void release_mem( double*  a );
/*******************************************************************************/
#define M 1001
#define N 151
int main(int argc, char const* argv[])
{ 

	const char* rows[6] = {"QueryS","SVLX","SVL","QuerySUVT","SVDX","SVD"};
	const char* cols[1] = {"CYCLES"};
	double *a; 
	init_mem(a);
	
	double data[6];
	data[0] = time(a,kQueryS);
	data[1] = time(a,kSVLX);
	data[2] = time(a,kSVL);
	data[3] = time(a,kQuerySUVT);
	data[4] = time(a,kSVDX);
	data[5] = time(a,kSVD);

	//verify_dir("DEBUG/");
	//char fname[200];
	//sprintf(fname, "DEBUG/time-sc-stride-%d.txt", STR);
	//link_cout(fname);

	Table tbl;
	tbl.dim(6, 1);
	tbl.rows(rows);
	tbl.cols(cols);
	tbl.data(data);
	char banner[BUFSIZ];
	sprintf(banner,"Matrix size = %d * %d",M,N);
	tbl.print(banner);

	//unlink_cout();

	release_mem(a);

	return 0;
}

double time( double* restrict a, enum ETestType type ) { 
#define COUNT 10
	TimeStamp clk(COUNT);
	double s[MIN(M,N)];
	double u[M*M];
	double vt[N*N]; 
	
	gdSVD.Set(M,N,a); 
	//gdSVD.Print(); 

	for (int i = 0; i < COUNT; i++) {
		switch (  type  ) {
			case kQueryS: 
				gdSVD.Flush();
				clk.tic();
				gdSVD.QueryWorkspace(s);
				clk.toc();
				break;
			case kSVLX:
				gdSVD.QueryWorkspace(s);
				clk.tic();
				gdSVD.SingularValueListX(s);
				clk.toc();
				break;
			case kSVL: 
				gdSVD.QueryWorkspace(s);
				clk.tic();
				gdSVD.SingularValueList(s);
				clk.toc();
				break;
			case kQuerySUVT: 
				gdSVD.Flush();
				clk.tic();
				gdSVD.QueryWorkspace(s,u,vt);
				clk.toc();
				break;
			case kSVDX:
				gdSVD.QueryWorkspace(s,u,vt);
				clk.tic();
				gdSVD.SingularValueDecompositionX(s,u,vt);
				clk.toc();
				break;
			case kSVD: 
				gdSVD.QueryWorkspace(s,u,vt);
				clk.tic();
				gdSVD.SingularValueDecomposition(s,u,vt);
				clk.toc();
				break; 
		}
	}

	printf("\n");
	return clk.median();
#undef COUNT
}

static void init_mem( double* &a ) {
	a = (double*) mkl_malloc( M*N*sizeof(double), MALLOC_ALIGNMENT );
	assert(a);
	init_matrix(M,N,a);
}

static void release_mem( double*  a ) { 
	if (!a) {
		mkl_free(a);
		a=NULL;
	}
}

#undef M
#undef N
