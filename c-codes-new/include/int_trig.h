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
 * 	p	*double		[6] triangle domain
 * 	f	functor		function being integrated
 * 	work	*double		[?] workspace
 * 				n=q->n is the number of points
 * Output:
 * 	return	double		value of the desired integral
 * Note:
 * 	The functor *f must point to a smooth function on pt.
 * 	Worksapce, i.e., work, must be at least (3*n+4)*sizeof(double) bytes.
 */
double dit_symmetric(const struct st_quadrule *q, const double *restrict p,
		double (*f)(double,double), double *restrict work);

/*
 * Double complex numerical integration over triangle using symmetric
 * quadrature rules.
 * Input:
 * 	q	st_quadrule	symmetric quadrature rule in the standard
 * 				triangle: (0,0), (1,0), (0,1)
 * 	p	*double		[6] triangle domain
 * 	f	functor		function being integrated
 * 	work	*double		[?] workspace
 * Output:
 * 	return	double		value of the desired integral
 * Note:
 * 	The functor *f must point to a smooth function on pt.
 * 	Worksapce, i.e., work, must be at least (4*n+4)*sizeof(double) bytes.
 */
double _Complex zit_symmetric(const struct st_quadrule *q, 
		const double *restrict p,
		double _Complex (*f)(double,double), double *restrict work);

/**************************************/ 
/*
 * Double precision numerical integration over a single triangle with a 1/r
 * singularity at the first node of the input triangle using arcsinh transform.
 * Input:
 * 	qu,qv	*st_quadrule	1D quadrature rules.
 * 	p	*double		[6] triangle domain
 * 	f	functor		function being integrated
 * 	work	*double		[?] workspace
 * Output:
 * 	return	double		value of the desired integral
 * Note:
 * 	The acutal function being integrated is f/r, where r=norm[x-p0].
 * 	The functor *f must point to a smooth function on the triangle p.
 * 	Worksapce, i.e., work, must be at least (4*nu*nv+nu)*sizeof(double) bytes.
 */
double dit_arcsinh_atomic(const struct st_quadrule *restrict qu, 
		const struct st_quadrule *restrict qv,
		const double *restrict p,
		double (*f)(double,double), double *restrict work);

/*
 * Double precision numerical integration over a single triangle with a 1/r
 * singularity at a given point using arcsinh transform.
 * Input:
 * 	qu,qv	*st_quadrule	1D quadrature rules.
 * 	p	*double		[6] triangle domain
 * 	p0	*double		[2] singular point
 * 	f	functor		function being integrated
 * 	work	*double		[?] workspace
 * Output:
 * 	return	double		value of the desired integral
 * Note:
 * 	The acutal function being integrated is f/r, where r=norm[x-p0].
 * 	The functor *f must point to a smooth function on the triangle p.
 * 	Worksapce, i.e., work, must be at least (4*nu*nv+nu+25)*sizeof(double) 
 * bytes.
 */
double dit_arcsinh(const struct st_quadrule *restrict qu, 
		const struct st_quadrule *restrict qv,
		const double *restrict p, const double *restrict p0,
		double (*f)(double,double), double *restrict work);

 /*
  *Worksapce, i.e., work, must be at least (5*nu*nv+nu)*sizeof(double) 
  */
double _Complex zit_arcsinh_atomic(const struct st_quadrule *restrict qu, 
		const struct st_quadrule *restrict qv,
		const double *restrict p,
		double _Complex (*f)(double,double), double *restrict work);

 /*
  *Worksapce, i.e., work, must be at least (5*nu*nv+nu+25)*sizeof(double) 
  */
double _Complex zit_arcsinh(const struct st_quadrule *restrict qu, 
		const struct st_quadrule *restrict qv,
		const double *restrict p, const double *restrict p0,
		double _Complex (*f)(double,double), double *restrict work);
#ifdef __cplusplus
}
#endif
/******************************************************************************/
#endif
