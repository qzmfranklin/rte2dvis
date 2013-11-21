#include "SVD.h"
#include "Utils.h"
#include <stdio.h>
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
using namespace Utils;
/*******************************************************************************/
int main(int argc, char const* argv[]);
int test01( void );
/*******************************************************************************/
int main(int argc, char const* argv[])
{ 
	test01();
	return 0;
}

int test01( void ) {
#define M 7
#define N 5
	int err=0; 
        printf("TEST01\n");
        printf("	|Test dSVD::QueryWorkspace()\n");

	double a[M*N], u[M*M], vt[N*N], s[MIN(M,N)];
	init_matrix(M,N,a);
	//init_matrix(M,M,u);
	//init_matrix(N,N,vt);
	//init_vector(MIN(M,N),s);
	
	gdSVD.Set(M,N,a);
	assert(   gdSVD.QueryWorkspace(s) > 0   );



        printf("END OF TEST01\n");
        printf("\n");
	return err;
#undef M
#undef N
}

