#include <stdio.h>
#include <string.h>
#include "GaussQuadratures.h"

/*****************************************************************************/
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
/*****************************************************************************/
int main(int argc, char const* argv[]);
void test01(int n);
void print_rule( double *xy, double *w, int order_num );

/*****************************************************************************/
int main(int argc, char const* argv[])
{ 
	test01(10);
	return 0;
}

void test01( int n ) {
	printf("	Test the Legendre rules.\n");
	printf("		A = -1.0\n");
	printf("		B = +1.0\n");
	n = MIN(n,50);
	n = MAX(n,1);
	int order;
	double *x;
	double *w;
	for (order = 1; order <= n; order++) {
		gGaussQuadratures.Generate( &x, &w, order, -1.0, 1.0 );
		print_rule( x, w, order ); 
	}
}

void print_rule( double *x, double *w, int order) {
	printf("\t\tORDER = %2d\n",order);
	printf("\t\tx\t\tw\n");
	int i;
	for (i = 0; i < order; i++)  
		printf("\t%10.6f\t%10.6f\n", x[i], w[i] );
}
