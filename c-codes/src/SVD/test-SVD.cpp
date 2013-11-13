#include "SVD.h"
#include "Utils.h"
#include <stdio.h>
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
#define M 6
#define N 4
	int err=0; 
        printf("TEST01\n");
        printf("	|Test dSVD\n");

	// Prepare the input matrix.
	double a[M*N];
	int m=M;
	int n=N;
	int lda=M;
	init_matrix(a,m,n,lda);
	printf("A = \n");
	print_matrix(a,m,n,lda);
	gdSVD.Set(a,m,n,lda);

	// Compute the singular value list.
	double s[M];
	gdSVD.SingularValueList(s);
	printf("SVL = \n");
	print_vector(s,M);

        printf("END OF TEST01\n");
        printf("\n");
	return err;
#undef M
#undef N
}
