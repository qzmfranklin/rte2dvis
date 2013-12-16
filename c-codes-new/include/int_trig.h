#ifndef _INT_TRIG_H_
#define _INT_TRIG_H_
/******************************************************************************/
#include "quad_types.h"
#include <complex.h>
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

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Double precision numerical integration over triangle using symmetric
 * quadrature rules.
 * Input:
 * 	q	st_quadrule	symmetric quadrature rule in the standard
 * 				triangle: (0,0), (1,0), (0,1)
 * 	pt	*double		[6] triangle domain
 * 	f	functor		function being integrated
 * 	work	*double		[?] workspace
 * 				n=q->n is the number of points
 * Output:
 * 	return	double		value of the desired integral
 * Note:
 * 	The functor *f must point to a smooth function on pt.
 * 	Workspace must be larger than (3*n+4)*sizeof(double) bytes long,
 * otherwise might run into run time errors.
 */
double dit_symmetric(const struct st_quadrule *q, const double *restrict p,
		double (*f)(double,double), double *restrict work);

/*
 * Double complex numerical integration over triangle using symmetric
 * quadrature rules.
 * Input:
 * 	q	st_quadrule	symmetric quadrature rule in the standard
 * 				triangle: (0,0), (1,0), (0,1)
 * 	pt	*double		[6] triangle domain
 * 	f	functor		function being integrated
 * 	work	*_Complex	[?] workspace
 * 				n=q->n is the number of points
 * Output:
 * 	return	double		value of the desired integral
 * Note:
 * 	The functor *f must point to a smooth function on pt.
 * 	Workspace must be larger than (4*n+4)*sizeof(double) bytes long,
 * otherwise might run into run time errors.
 */
double _Complex zit_symmetric(const struct st_quadrule *q, 
		const double *restrict p,
		double _Complex (*f)(double,double), double *restrict work);

/**************************************/ 
double dit_arcsinh_atomic(const struct st_quadrule *restrict qu, 
		const struct st_quadrule *restrict qv,
		const double *restrict p,
		double (*f)(double,double), double *restrict work);

double dit_arcsinh(const struct st_quadrule *restrict qu, 
		const struct st_quadrule *restrict qv,
		const double *restrict p, const double *restrict p0,
		double (*f)(double,double), double *restrict work);
	
//double _Complex zit_arcsinh(const struct st_quadrule *q, 
		//const double *restrict p,
		//double _Complex (*f)(double,double), double *restrict work);

#ifdef __cplusplus
}
#endif
/******************************************************************************/
#endif
