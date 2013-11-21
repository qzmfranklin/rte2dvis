#include "SVD.h"
#include "Utils.h"
#include <stdio.h>
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
using namespace Utils;
/*******************************************************************************/
int main(int argc, char const* argv[]);
int test01( void );
int test02( void );
int test03( void );
/*******************************************************************************/
int main(int argc, char const* argv[])
{ 
	test01();
	test02();
	test03();

	return 0;
}

#define M 50
#define N 11
int test01( void ) {
	int err=0; 
        printf("TEST01\n");
        printf("	|Test dSVD::QueryWorkspace()\n");

	double a[M*N], u[M*M], vt[N*N], s[MIN(M,N)];
	init_matrix(M,N,a);
	//init_matrix(M,M,u);
	//init_matrix(N,N,vt);
	//init_vector(MIN(M,N),s);
	
	gdSVD.Set(M,N,a); 
	gdSVD.Print(); 
	gdSVD.QueryWorkspace(s); 
	gdSVD.QueryWorkspace(s); 
	gdSVD.QueryWorkspace(s,u,vt);

        printf("END OF TEST01\n");
        printf("\n");
	return err;
}

int test02( void ) {
	int err=0; 
        printf("TEST02\n");
        printf("	|Test dSVD::SingularValueList?()\n");

	double a[M*N], u[M*M], vt[N*N], sX[MIN(M,N)], s[MIN(M,N)];
	init_matrix(M,N,a);
	
	gdSVD.Set(M,N,a); 
	gdSVD.QueryWorkspace(s); 

	gdSVD.SingularValueList(s);
	print_vector("Singular Value List s",MIN(M,N),s);
	gdSVD.SingularValueListX(sX);
	print_vector("Singular Value List sX",MIN(M,N),sX);

        printf("END OF TEST02\n");
        printf("\n");
	return err;
}


int test03( void ) {
	int err=0; 
        printf("TEST03\n");
        printf("	|Test dSVD::SingularValueDecomposition?()\n");

	double a[M*N], uX[M*M], vtX[N*N] ,u[M*M], vt[N*N], sX[MIN(M,N)], s[MIN(M,N)];
	init_matrix(M,N,a);
	
	gdSVD.Set(M,N,a); 
	gdSVD.QueryWorkspace(s,u,vt); 

	gdSVD.SingularValueDecomposition(s,u,vt);
	print_vector("Singular Value List s",MIN(M,N),s);
	gdSVD.SingularValueDecompositionX(sX,uX,vtX);
	print_vector("Singular Value List sX",MIN(M,N),sX);

        printf("END OF TEST03\n");
        printf("\n");
	return err;
}



#undef M
#undef N
