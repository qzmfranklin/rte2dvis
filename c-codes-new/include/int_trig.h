#ifndef _INT_TRIG_H_
#define _INT_TRIG_H_
/******************************************************************************/
#include "quad_types.h"
#include <complex.h>
/******************************************************************************/
/*
 * Double precision numerical integration over triangle using symmetric
 * quadrature rules.
 * Input:
 * 	q	st_quadrule	symmetric quadrature rule being employed
 * 	pt	*double		[6]triangle domain
 * 	f	functor		function being integrated
 * Output:
 * 	return	double		value of the desired integral
 * Note:
 * 	The functor *f must point to a smooth function on pt.
 */
double dit_symmetric(const struct st_quadrule *q, const double *restrict p,
		double (*f)(double,double));

/*
 * Double precision numerical integration over triangle with 1/r
 * singularity using arcsinh transform.
 * Input:
 * 	q	st_quadrule	symmetric quadrature rule being employed
 * 	pt	*double		[6]triangle domain
 * 	r0	*double		[2]singular point
 * 	f	functor		function being integrated
 * Output:
 * 	return	double		value of the desired integral
 * Note:
 * 	The value returned is integral of 
 * 		f(r)/norm(r-r0)
 * 	where the function f(r) is smooth on the pt.
 */
//double dit_arcsinh(struct st_quadrule *restrict qu, 
		//struct st_quadrule *restrict qv, 
		//const double *restrict pt, 
		//const double *restrict r0,
		//double (*f)(double,double));

/*
 * Complex valued integral routines.
 */
//double _Complex cit_symmetric(struct st_quadrule *q, 
		//const double *pt,
		//double _Complex (*f)(double,double)); 
//double _Complex dit_arcsinh(struct st_quadrule *restrict qu, 
		//struct st_quadrule *restrict qv, 
		//const double *restrict pt, 
		//const double *restrict r0,
		//double _Complex (*f)(double,double));
/******************************************************************************/
#endif
