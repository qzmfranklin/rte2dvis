#include "DunavantRule.h"
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
	test01(100);
	return 0;
}

void test01( int n ){
	n = MIN(n,20);
	n = MAX(n,1);
	int rule;
	int order_num;
	double *xy;
	double *w;
	for (rule = 1; rule <= n; rule++) {
		gDunavantRule.Generate( rule, &xy, &w, &order_num );
		print_rule( xy, w, order_num ); 
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
