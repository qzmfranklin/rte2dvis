#include "QuadratureRules.h"
/******************************************************************************/
namespace QuadratureRules {

static void print_rule_1d(int order_num, double *x, double *w)
{
	printf("\t\t\tx\t\t\tw\n");
	for (int i = 0; i < order_num; i++)  
		printf("[%6d]\t%10.6f\t\t%10.6f\n",
				i, x[i], w[i] );
}

static void print_rule_2d(int order_num, double *xy, double *w)
{
	printf("\t\t\tx\t\ty\t\t\tw\n");
	for (int i = 0; i < order_num; i++)  
		printf("[%6d]\t%10.6f\t%10.6f\t\t%10.6f\n",
				i, xy[2*i], xy[2*i+1], w[i] );
}

void print_quadrule(struct st_quadrule *q)
{
	double ww=0.0;
	for (int i = 0; i < q->n; i++)
		ww += q->w[i];
	printf("total weight = %f\n",ww);
	printf("q->a (scale) = %f\n",q->a);
	//printf("q->x         = %p\n",q->x);
	//printf("q->w         = %p\n",q->w);

	switch (q->dim) {
	case 1:
		print_rule_1d(q->n,q->x,q->w);
		break;
	case 2:
		print_rule_2d(q->n,q->x,q->w);
		break;
	} 

	printf("\n");
}

} // namespace QuadratureRules
