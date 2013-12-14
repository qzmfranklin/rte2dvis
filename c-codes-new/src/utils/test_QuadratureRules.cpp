#include <stdio.h>
#include "QuadratureRules.h"
#include "utils.h"
/*****************************************************************************/
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
/*****************************************************************************/
using namespace QuadratureRules;
int main(int argc, char const* argv[]);
void test01(int n);
void test02(int n);
void test03(int n);
void test04(int n);
void print_rule_gauss(int order_num, double *x, double *w);
void print_rule(int order_num, double *xy, double *w);
void print_rule(int order_num, double *x, double *y, double *w);
/*****************************************************************************/
int main(int argc, char const* argv[])
{ 
	char fname[FILENAME_MAX];
	verify_dir("DEBUG");
	sprintf(fname,"DEBUG/test_quad_rules.txt");
	link_stdout(fname);

	test01(100);
	test02(100);
	test03(100);
	test04(100);

	unlink_stdout();

	return 0;
} 

void test01( int n )
{
	printf("	Test Dunavant rules.\n");
	n = MIN(n,gDunavantRule.RuleNumber());
	n = MAX(n,1);
	int rule, order_num;
	double *xy, *w, *x, *y;
	for (rule = 1; rule <= n; rule++) {
		printf("rule = %d\n",rule);
		//gDunavantRule.Generate( rule, order_num, xy, w );
		//print_rule( order_num, xy, w ); 
		gDunavantRule.Generate(rule,order_num,x,y,w);
		print_rule(order_num,x,y,w);
	}
}

void test02( int n )
{
	printf("	Test Wandzura rules.\n");
	n = MIN(n,gWandzuraRule.RuleNumber());
	n = MAX(n,1);
	int rule, order_num;
	double *xy, *w, *x, *y;
	for (rule = 1; rule <= n; rule++) {
		printf("rule = %d\n",rule);
		//gWandzuraRule.Generate( rule, order_num, xy, w );
		//print_rule( order_num, xy, w ); 
		gWandzuraRule.Generate(rule,order_num,x,y,w);
		print_rule(order_num,x,y,w);
	}
}

void test03( int n )
{
	printf("	Test Lyness rules.\n");
	n = MIN(n,gLynessRule.RuleNumber());
	n = MAX(n,1);
	int rule, order_num;
	double *xy, *w, *x, *y;
	for (rule = 1; rule <= n; rule++) {
		printf("rule = %d\n",rule);
		//gLynessRule.Generate( rule, order_num, xy, w);
		//print_rule( order_num, xy, w ); 
		gLynessRule.Generate(rule,order_num,x,y,w);
		print_rule(order_num,x,y,w);
	}
}

void test04( int n ) {
	printf("	Test the Legendre rules.\n");
	printf("		A = -1.0\n");
	printf("		B = +1.0\n");
	n = MIN(n,20);
	n = MAX(n,1);
	int order;
	double *x;
	double *w;
	for (order = 1; order <= n; order++) {
		gGaussRule.Generate( x, w, order, -1.0, 1.0 );
		print_rule_gauss( order, x, w ); 
	}
}

void print_rule_gauss(int order_num, double *x, double *w)
{
	printf("	\t\tORDER = %2d\n",order_num);
	printf("	\tx\t\t\tw\n");
	for (int i = 0; i < order_num; i++)  
		printf("	%10.6f\t\t%10.6f\n",
				x[i], w[i] );
	printf("\n");
}

void print_rule(int order_num, double *xy, double *w){
	printf("	\t\tORDER = %2d\n",order_num);
	printf("	\tx\t\ty\t\t\tw\n");
	for (int i = 0; i < order_num; i++)  
		printf("	%10.6f\t%10.6f\t\t%10.6f\n",
				xy[2*i], xy[2*i+1], w[i] );
	printf("\n");
}

void print_rule(int order_num, double *x, double *y, double *w) {
	printf("	\t\tORDER = %2d\n",order_num);
	printf("	\tx\t\ty\t\t\tw\n");
	for (int i = 0; i < order_num; i++)  
		printf("	%10.6f\t%10.6f\t\t%10.6f\n",
				x[i], y[i], w[i] );
	printf("\n");
}
