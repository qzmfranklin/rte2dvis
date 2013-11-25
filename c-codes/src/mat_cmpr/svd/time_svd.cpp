#include "svd.h"
#include "utils.h"
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

	verify_dir("DEBUG/");
	char fname[FILENAME_MAX];
	sprintf(fname, "DEBUG/time_svd_d_%dx%d.txt\n",M,N);
	fprintf(stderr,"Output to %s.",fname);
	link_cout(fname);

	Table tbl;
	tbl.dim(6, 1);
	tbl.rows(rows);
	tbl.cols(cols);
	tbl.data(data);
	char banner[BUFSIZ];
	sprintf(banner,"Matrix size = %d * %d",M,N);
	tbl.print(banner);

	unlink_cout();

	release_mem(a);

	return 0;
}

double time( double* restrict a, enum ETestType type ) { 
#define COUNT 10
	TimeStamp clk(COUNT);
	double s[MIN(M,N)];
	double u[M*M];
	double vt[N*N]; 

	gSVD_D.Set(M,N,a); 
	//gSVD_D.Print(); 

	for (int i = 0; i < COUNT; i++) {
		switch (  type  ) {
		case kQueryS: 
			gSVD_D.Flush();
			clk.tic();
			gSVD_D.QueryWorkspace(s);
			break;
		case kSVLX:
			gSVD_D.QueryWorkspace(s);
			clk.tic();
			gSVD_D.SingularValueListX(s);
			break;
		case kSVL: 
			gSVD_D.QueryWorkspace(s);
			clk.tic();
			gSVD_D.SingularValueList(s);
			break;
		case kQuerySUVT: 
			gSVD_D.Flush();
			clk.tic();
			gSVD_D.QueryWorkspace(s,u,vt);
			break;
		case kSVDX:
			gSVD_D.QueryWorkspace(s,u,vt);
			clk.tic();
			gSVD_D.SingularValueDecompositionX(s,u,vt);
			break;
		case kSVD: 
			gSVD_D.QueryWorkspace(s,u,vt);
			clk.tic();
			gSVD_D.SingularValueDecomposition(s,u,vt);
			break; 
		}
		clk.toc();
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
