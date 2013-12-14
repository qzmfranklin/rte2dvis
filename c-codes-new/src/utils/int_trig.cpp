#include "int_trig.h"
#include <math.h>
#include <assert.h>
/******************************************************************************/
		val[i] = dit_symmetric(q,p,&f);
double dit_symmetric(struct st_quadrule *q, const double *restrict p,
		double (*f)(double,double))
{
	assert(q->flag == 2); // quadrature rule for 2D triangle
	double val=0.0;

	// A matrix, col-major
	int n;
	double A[4], *w, *xy;
	A[0] = p[2]-p[0];
	A[1] = p[3]-p[1];
	A[2] = p[4]-p[0];
	A[3] = p[5]-p[1];
	n    = q->n;
	w    = q->w;
	xy   = q->x;
	
	for (int i = 0; i < n; i++)
		val += w[i] * (*f)( 
				A[0]*xy[2*i]   + A[2]*xy[2*i]   + p[0],
				A[1]*xy[2*i+1] + A[3]*xy[2*i+1] + p[1] );

	val *= 0.5 * fabs(A[0]*A[3]-A[1]*A[2]);

	return val;
}
