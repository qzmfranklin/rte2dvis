#include "int_trig.h"
#include <stdio.h>
#include "utils.h"
#include <math.h>
#include <complex.h>
#include <assert.h>
/******************************************************************************/ 
/*
 * double *p is the triangle:
 * 	x0	p[0]
 * 	y0	p[1]
 * 	x1	p[2]
 * 	y1	p[3]
 * 	x2	p[4]
 * 	y2	p[5]
 */
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

double dit_arcsinh_atomic(const struct st_quadrule *restrict qu, 
		const struct st_quadrule *restrict qv,
		const double *restrict p,
		double (*f)(double,double), double *restrict work)
{
	assert((qu->dim==1)&&(qv->dim==1));
	double p0p1[2],p0p2[2],p1p2[2],invp1p2,up1p2[2],Ah[4],xp[2],h,u0[2];
	p0p1[0] = p[2] - p[0]; // x1-x0
	p0p1[1] = p[3] - p[1]; // y1-y0
	p0p2[0] = p[4] - p[0]; // x2-x0
	p0p2[1] = p[5] - p[1]; // y2-y0
	p1p2[0]	= p[4] - p[2]; // x2-x1
	p1p2[1]	= p[5] - p[3]; // y2-y1
	invp1p2 = 1.0/sqrt( p1p2[0]*p1p2[0] + p1p2[1]*p1p2[1] ); // inverse p1p2
	up1p2[0]= invp1p2*p1p2[0];
	up1p2[1]= invp1p2*p1p2[1];
	h	= invp1p2*( -p1p2[0]*p[1] + p0p2[0]*p[3] - p0p1[0]*p[5]);
	Ah[0]	= up1p2[0]*h;
	Ah[1]	= -up1p2[1]*h;
	Ah[2]	= up1p2[1]*h;
	Ah[3]	= up1p2[0]*h;
	xp[0]	= up1p2[0]*p0p1[0] + up1p2[1]*p0p1[1]; // x1p
	xp[1]	= up1p2[0]*p0p2[0] + up1p2[1]*p0p2[1]; // x2p
	u0[0]	= asinh(xp[0]/h); // u1
	u0[1]	= asinh(xp[1]/h)-u0[0]; // u2-u1

	int nu,nv;
	double *xu,*wu,*xv,*wv;
	nu = qu->n;
	xu = qu->x;
	wu = qu->w;

	nv = qv->n;
	xv = qv->x;
	wv = qv->w;

	double *u,*v,*ww,*qq,*ff;
	u  = work;
	ww = u+nu;
	qq = ww+nu*nv;
	ff = qq+2*nu*nv;

	for (int i = 0; i < nu; i++)
		u[i] = u0[0] + u0[1]*xu[i]; 

	for (int j = 0; j < nv; j++)
		for (int i = 0; i < nu; i++)
			ww[i+nu*j] = wu[i]*wv[j]; 

	for (int j = 0; j < nv; j++)
		for (int i = 0; i < nu; i++) { 
			int k=2*(i+nu*j);
			qq[k]   = p[0] + xv[j]*( Ah[0]*sinh(u[i]) + Ah[1] );
			qq[k+1] = p[1] + xv[j]*( Ah[2]*sinh(u[i]) + Ah[3] );
		} 

	for (int k = 0; k < nu*nv; k++) 
		ff[k] = (*f)(qq[2*k],qq[2*k+1]);

	double val=0.0; 
	for (int k = 0; k < nu*nv; k++)
		val += ww[k]*ff[k]; 
	val *= fabs(h*u0[1]);

	return val;
}


static double vec_diff2(double *restrict u, double *restrict v, double *res) {
	res[0] = u[0] - v[0];
	res[1] = u[1] - v[1];
}

static double det2x2(double *restrict a, double *restrict b) {
	return a[0]*b[1]-a[1]*b[0];
}

double dit_arcsinh(const struct st_quadrule *restrict qu, 
		const struct st_quadrule *restrict qv,
		const double *restrict p, const double *restrict p0,
		double (*f)(double,double), double *restrict work)
{
	double *u,*v;
	u = work;
	v = u + 2;
	double A;


	double val=0.0;

	return val;
}
