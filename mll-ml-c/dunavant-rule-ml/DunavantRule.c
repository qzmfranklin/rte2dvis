#include <stdio.h>
#include "mathlink.h"
#include "dunavant-rule.h"

int main(int argc, char*argv[]);
void DunavantRule(void);

/**********************************************************************************************************************************************************************/
int main(int argc, char *argv[])
{
	return MLMain(argc,argv);
}

void DunavantRule(void){
	/*Receive from MathLink*/
	int rule;
	double *node_xy2;
	int *dims;
	char **heads;
	int d;
	MLGetInteger(stdlink,&rule);
	MLGetReal64Array(stdlink,&node_xy2,&dims,&heads,&d); 
	/*MLPutReal64List(stdlink,node_xy2,6);*/

	/*Construct Dunavant Rule*/
	double area2;
	int node;
	int order;
	double node_xy[6] = {
		0.0, 0.0,
		1.0, 0.0,
		0.0, 1.0
	};
	int order_num;
	double *w;
	double *xy;
	double *xy2;
	area2 = triangle_area ( node_xy2 );
	order_num = dunavant_order_num ( rule );
	xy = (double*)malloc(sizeof(double)*2*order_num);
	xy2 = (double*)malloc(sizeof(double)*2*order_num);
	w = (double*)malloc(sizeof(double)*order_num);
	dunavant_rule ( rule, order_num, xy, w );
	reference_to_physical_t3 ( node_xy2, order_num, xy, xy2 );

	/*Disown Data*/
	MLReleaseReal64Array(stdlink,node_xy2,dims,heads,d);

	/*Send to MathLink*/
	double *res;
	res = (double*) malloc (3*order_num*sizeof(double));
	int i;
	for (i = 0; i < order_num; i++) {
		res[i+0*order_num] = xy2[0+i*2];
		res[i+1*order_num] = xy2[1+i*2];
		res[i+2*order_num] = w[i]*fabs(area2);
	}
	dims = (int*) malloc (2*sizeof(int));
	dims[0]=3;
	dims[1]=order_num;
	d=2;
	MLPutReal64Array(stdlink,res,dims,NULL,d);

	/*Free Memory*/
	free(w);
	free(xy);
	free(xy2);
	free(dims);
	free(res);

	return;
}
