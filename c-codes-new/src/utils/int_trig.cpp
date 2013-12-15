#include "int_trig.h"
#include <math.h>
#include <assert.h>
/******************************************************************************/ 
double dit_symmetric(const struct st_quadrule *q, const double *restrict p,
		double (*f)(double,double), double *restrict work)
{
	assert(q->dim == 2); // quadrature rule for 2D triangle
	double val=0.0;

	int n;
	double *w, *xy;
	n    = q->n;
	w    = q->w;
	xy   = q->x;
	double *A, *qq, *ff;
	A  = work;
	qq = A + 4;
	ff = qq + 2*n;
	// A matrix, row-major
	A[0] = p[2]-p[0];
	A[1] = p[4]-p[0];
	A[2] = p[3]-p[1];
	A[3] = p[5]-p[1];

	#pragma vector always
	for (int i = 0; i < n; i++) {
		qq[2*i]   = A[0]*xy[2*i] + A[1]*xy[2*i+1] + p[0];
		qq[2*i+1] = A[2]*xy[2*i] + A[3]*xy[2*i+1] + p[1]; 
	} 

	for (int i = 0; i < n; i++)
		ff[i] = (*f)(qq[2*i],qq[2*i+1]);

	#pragma vector always
	for (int i = 0; i < n; i++)
		val += w[i] * ff[i];

	val *= 0.5 * fabs(A[0]*A[3]-A[1]*A[2]);

	return val;
}

double _Complex zit_symmetric(const struct st_quadrule *q, 
		const double *restrict p,
		double _Complex (*f)(double,double), double *restrict work)
{
	assert(q->dim == 2); // quadrature rule for 2D triangle
	double _Complex val=0.0;

	int n;
	double *w, *xy;
	n    = q->n;
	w    = q->w;
	xy   = q->x;
	double *A, *qq;
	double _Complex *ff;
	A  = work;
	qq = A + 4;
	ff = (double _Complex*) (qq + 2*n);
	// A matrix, row-major
	A[0] = p[2]-p[0];
	A[1] = p[4]-p[0];
	A[2] = p[3]-p[1];
	A[3] = p[5]-p[1];

	#pragma vector always
	for (int i = 0; i < n; i++) {
		qq[2*i]   = A[0]*xy[2*i] + A[1]*xy[2*i+1] + p[0];
		qq[2*i+1] = A[2]*xy[2*i] + A[3]*xy[2*i+1] + p[1]; 
	} 

	for (int i = 0; i < n; i++)
		ff[i] = (*f)(qq[2*i],qq[2*i+1]);

	#pragma vector always
	for (int i = 0; i < n; i++)
		val += w[i] * ff[i];

	val *= 0.5 * fabs(A[0]*A[3]-A[1]*A[2]);

	return val;
}
