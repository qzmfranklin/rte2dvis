#include "precalc.h"
#include <assert.h>
#include <mkl.h>
#include <math.h>
#define MALLOC_ALIGNMENT 64


//TODO: test, time, vectorization pragmas

/******************************************************************************/

void precalc_pt(const int n, const int *restrict triangles, 
		const double *restrict nodes, 
		double *restrict x, double *restrict y)
{
	#pragma vector always
	for (int i = 0; i < 3*n; i++) {
		x[i] = nodes[triangles[i]+0];
		y[i] = nodes[triangles[i]+1];
	}
}

void precalc_cntr(const int n, 
		const double *restrict x, const double *restrict y,
		double *restrict cx, double *restrict cy)
{
	for (int i = 0; i < n; i++) {
		cx[i] = x[3*i] + x[3*i+1] + x[3*i+2];
		cy[i] = y[3*i] + y[3*i+1] + y[3*i+2];
	}
}

void precalc_signed_area(const int n,
		const double *restrict x, const double *restrict y,
		double *restrict area)
{
	/*
	double a[2],b[2];
	for (int i = 0; i < n; i++) {
		a[0] = x[3*i+1] - x[3*i];
		a[1] = x[3*i+2] - x[3*i];
		b[0] = y[3*i+1] - y[3*i];
		b[1] = y[3*i+2] - y[3*i];
		area[i] = 0.5*( a[0]*b[1] - a[1]*b[0] );
	}
	*/
	for (int i = 0; i < n; i++)
		area[i] = 0.5*(  (x[3*i+1]-x[3*i])
				*(y[3*i+2]-y[3*i]) 
				-(x[3*i+2]-x[3*i])
				*(y[3*i+1]-y[3*i]) );
}


void precalc_cartesian_to_polar(const int n,
		const double *restrict inx, const double *restrict iny,
		double *restrict outr, double *restrict outphi)
{
	/*double *tmp;*/
	/*tmp = (double*)mkl_malloc(n*sizeof(double),MALLOC_ALIGNMENT);*/
	/*assert(tmp);*/
	/*vmdMul(n,inx,inx,outr,VML_LA);*/
	/*vmdMul(n,iny,iny,tmp,VML_LA);*/
	/*vmdAdd(n,outr,tmp,outr,VML_LA);*/
	/*vmdSqr(n,outr,outr,VML_LA);*/
	/*mkl_free(tmp);*/
	for (int i = 0; i < n; i++)
		outr[i] = sqrt(  inx[i]*inx[i] + iny[i]*iny[i]  );
	/*vmdAtan2(n,iny,inx,outphi,VML_LA);*/
	for (int i = 0; i < n; i++) {
		outphi[i]=atan2( iny[i], inx[i] );
	}
}

void precalc_polar_to_cartesian(const int n,
		const double *restrict inr, const double *restrict inphi,
		double *restrict outx, double *restrict outy)
{
	vmdSinCos(n,inphi,outx,outy,VML_LA);
	vmdMul(n,inr,outx,outx,VML_LA);
	vmdMul(n,inr,outy,outy,VML_LA);
	/*for (int i = 0; i < n; i++) {*/
		/*outx[i] = inr[i] * cos(inphi[i]);*/
		/*outy[i] = inr[i] * sin(inphi[i]);*/
	/*}*/
}

void precalc_fun(double (*fun)(double), const int n, 
		const double *restrict in, double *restrict out)
{
	for (int i = 0; i < n; i++)
		out[i] = (*fun)(in[i]);
}

void precalc_fun2(double (*fun)(double,double), const int n, 
		const double *restrict in1, const double *restrict in2,
		double *restrict out)
{
	for (int i = 0; i < n; i++)
		out[i] = (*fun)(in1[i],in2[i]);
}
