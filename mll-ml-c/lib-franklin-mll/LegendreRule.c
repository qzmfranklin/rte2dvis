#include <stdio.h>
#include "mathlink.h"
#include "legendre-rule.h"

int main(int argc, char*argv[]);
void LegendreRule(void);

/**********************************************************************************************************************************************************************/
int main(int argc, char *argv[])
{
	/*main_old(argc,argv);*/
	return MLMain(argc,argv);
}

void LegendreRule(void)
{
	/*Receive from MathLink*/
	int order;
	double a;
	double b;
	MLGetInteger(stdlink,&order);
	MLGetReal(stdlink,&a);
	MLGetReal(stdlink,&b);

	/*Construct Legendre Rule*/
	int kind;
	double alpha;
	double beta;
	double *x;
	double *w;
	alpha = 0.0;
	beta = 0.0;
	kind = 1; 
	x = (double *) malloc( order * sizeof(double) );
	w = (double *) malloc( order * sizeof(double) );
	cgqf ( order, kind, alpha, beta, a, b, x, w );

	/*Send to MathLink*/
	double *res;
	int dims[2]={2,order};
	int d=2;
	res = (double *) malloc( 2 * order * sizeof(double) );
	int i;
	for (i = 0; i < order; i++) {
		res[i] = x[i];
	}
	for (i = 0; i < order; i++) {
		res[i+order] = w[i];
	}
	MLPutReal64Array(stdlink,res,dims,NULL,d);

	/*Free Memory*/
	free ( x );
	free ( w );
	free ( res );
}
