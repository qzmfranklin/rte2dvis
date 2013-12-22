#include "QuadratureRules.h"
/******************************************************************************/
namespace QuadratureRules {

void print_rule_1d(int order_num, double *x, double *w)
{
	printf("\t\t\tx\t\t\tw\n");
	for (int i = 0; i < order_num; i++)  
		printf("[%6d]\t%10.6f\t\t%10.6f\n",
				i, x[i], w[i] );
	printf("\n");
}

void print_rule_2d(int order_num, double *xy, double *w)
{
	printf("\t\t\tx\t\ty\t\t\tw\n");
	for (int i = 0; i < order_num; i++)  
		printf("[%6d]\t%10.6f\t%10.6f\t\t%10.6f\n",
				i, xy[2*i], xy[2*i+1], w[i] );
	printf("\n");
}

void print_quadrule(struct st_quadrule *q)
{
	switch (q->dim) {
	case 1:
		print_rule_1d(q->n,q->x,q->w);
		break;
	case 2:
		print_rule_2d(q->n,q->x,q->w);
		break;
	}
}
	
void ReleaseMemory()
{
	gDunavantRule.ReleaseMemory();
	gLynessRule.ReleaseMemory();
	gWandzuraRule.ReleaseMemory();
	gGaussRule.ReleaseMemory();
	gArcSinhMethod.ReleaseMemory();
}

} // namespace QuadratureRules
