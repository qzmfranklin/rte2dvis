#include "arcsinh-rule.h"
#include <math.h>
/******************************************************************************/

void arcsinh_rule_atomic(double *x, double *y, double *w, 
		double const *p0, double const *p1, double const *p2,
		const int nu, double const *xu, double const *wu,
		const int nv, double const *xv, double const *wv,
		double *work)
{
	double *u = work;

	double p0p1[2],p0p2[2],p1p2[2],invp1p2,Ah[4],xp[2],h,u0[2];
	p0p1[0] = p1[0] - p0[0]; // x1-x0
	p0p1[1] = p1[1] - p0[1]; // y1-y0
	p0p2[0] = p2[0] - p0[0]; // x2-x0
	p0p2[1] = p2[1] - p0[1]; // y2-y0
	p1p2[0]	= p2[0] - p1[0]; // x2-x1
	p1p2[1]	= p2[1] - p1[1]; // y2-y1
	invp1p2 = 1.0/sqrt(p1p2[0]*p1p2[0] + p1p2[1]*p1p2[1] ); // inverse p1p2
	p1p2[0]*= invp1p2;
	p1p2[1]*= invp1p2;
	h	= -p1p2[0]*p0[1] + invp1p2*(p0p2[0]*p1[1] - p0p1[0]*p2[1]);
	Ah[0]	= p1p2[0]*h;
	Ah[1]	= -p1p2[1]*h;
	Ah[2]	= p1p2[1]*h;
	Ah[3]	= p1p2[0]*h;
	xp[0]	= p1p2[0]*p0p1[0] + p1p2[1]*p0p1[1]; // x1p
	xp[1]	= p1p2[0]*p0p2[0] + p1p2[1]*p0p2[1]; // x2p
	u0[0]	= asinh(xp[0]/h); // u1
	u0[1]	= asinh(xp[1]/h)-u0[0]; // u2-u1

	for (int i = 0; i < nu; i++)
		u[i] = u0[0] + u0[1]*xu[i]; 

	for (int j = 0; j < nv; j++)
		for (int i = 0; i < nu; i++)
			//// h(u1-u2) is absorbed into weights.
			w[i+nu*j] = fabs(h*u0[1])*wu[i]*wv[j]; 

	for (int j = 0; j < nv; j++)
		for (int i = 0; i < nu; i++) { 
			x[i+nu*j] = p0[0] + xv[j]*( Ah[0]*sinh(u[i]) + Ah[1] );
			y[i+nu*j] = p0[1] + xv[j]*( Ah[2]*sinh(u[i]) + Ah[3] );
		} 
}
