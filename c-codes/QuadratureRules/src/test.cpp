#include "../include/DunavantRule.h"
#include "../include/WandzuraRule.h"
#include "../include/GaussQuadratures.h"
/*****************************************************************************/
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
/*****************************************************************************/
using namespace QuadratureRules;
int main(int argc, char const* argv[]);
void test01(int n);
void test02(int n);
void test03(int n);
void print_rule( double *xy, double *w, int order_num );
/*****************************************************************************/
int main(int argc, char const* argv[])
{ 
	test01(100);
	test02(100);
	test03(100);
	return 0;
} 

void test01( int n ){
	printf("	Test Dunavant rules.\n");
	n = MIN(n,20);
	n = MAX(n,1);
	int rule;
	int order_num;
	double *xy;
	double *w;
	for (rule = 1; rule <= n; rule++) {
		printf("rule = %d\n",rule);
		gDunavantRule.Generate( rule, &xy, &w, &order_num );
		print_rule( xy, w, order_num ); 
	}
}

void test02( int n ){
	printf("	Test Wandzura rules.\n");
	n = MIN(n,6);
	n = MAX(n,1);
	int rule;
	int order_num;
	double *xy;
	double *w;
	for (rule = 1; rule <= n; rule++) {
		printf("rule = %d\n",rule);
		gWandzuraRule.Generate( rule, &xy, &w, &order_num );
		print_rule( xy, w, order_num ); 
	}
}

void test03( int n ) {
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

void print_rule( double *xy, double *w, int order_num ){
	printf("	\t\tORDER = %2d\n",order_num);
	printf("	\tx\t\ty\t\t\tw\n");
	int i;
	for (i = 0; i < order_num; i++)  
		printf("	%10.6f\t%10.6f\t\t%10.6f\n",
				xy[2*i], xy[2*i+1], w[i] );
	printf("\n");
}
