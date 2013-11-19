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
#define M 6
#define N 4
	int err=0; 
        printf("TEST01\n");
        printf("	|Test dSVD::SingularValueList\n");

	//// Prepare the input matrix.
	//double a[M*N];
	//int m=M;
	//int n=N;
	//int lda=M;
	//init_matrix(a,m,n,lda);
	//print_matrix("A",a,m,n,lda);
	//gdSVD.Set(a,m,n,lda);

	//// Compute the singular value list.
	//double *s1;
	//int len1;
	//gdSVD.SingularValueList(&s1,&len1);
	//print_vector("SVL(auto mem control)",s1,len1);
	
	//// Compute the singular value list.
	//double s2[MIN(M,N)];
	//int len2=MIN(M,N);
	//gdSVD.SingularValueList(s2);
	//print_vector("SVL2(manual mem control)",s2,len2);

        printf("END OF TEST01\n");
        printf("\n");
	return err;
#undef M
#undef N
}

