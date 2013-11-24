#include "precalc.h"
#include <math.h>


void precalc_pt(const int triangles_num, const int *restrict triangles, 
		const double *restrict nodes, double *restrict pt) {
	for (int i = 0; i < triangles_num*3; i++) { 
		pt[i+0] = nodes[triangles[i]+0];
		pt[i+1] = nodes[triangles[i]+1];
	}
}

static void scale(const double alpha, const int n, double *restrict a) {
	for (int i = 0; i < n; i++)
		a[i] *= alpha;
}

void precalc_cntr(const int n, const double *restrict pt, 
		double *restrict cntr) {
	for (int i = 0; i < n; i++) {
		cntr[2*i+0] = pt[6*i+0] + pt[6*i+2] + pt[6*i+4];
		cntr[2*i+1] = pt[6*i+1] + pt[6*i+3] + pt[6*i+5];
	}
	scale(1.0/3.0, n, cntr);
}

void precalc_signed_area(const int n, const double *restrict pt, 
		double *restrict area) {
	for (int i = 0; i < n; i++)
		area[i] = 0.5*(  (pt[6*i+2] - pt[6*i+0])
				*(pt[6*i+3] - pt[6*i+1])
				-(pt[6*i+4] - pt[6*i+0])
				*(pt[6*i+5] - pt[6*i+1]));
}

void precalc_cartesian_to_polar(const int n, const double *restrict in,
		double *restrict out) {
	for (int i = 0; i < n; i++)
		out[2*i+0] = sqrt( in[2*i+0]*in[2*i+0] 
				+ in[2*i+1]*in[2*i+1] );
	for (int i = 0; i < n; i++)
		out[2*i+1] = atan2( in[2*i+1], in[2*i+0] );
}

void precalc_fun(double (*fun)(double), const int n, 
		const double *restrict x, double *restrict out) {
	for (int i = 0; i < n; i++)
		out[i] = (*fun)(x[i]);
}

void precalc_fun2(double (*fun)(double,double), const int n, 
		const double *restrict xy, double *restrict out) {
	for (int i = 0; i < n; i++)
		out[i] = (*fun)(xy[2*i],xy[2*i+1]);
}
