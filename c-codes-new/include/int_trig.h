#ifndef _INT_TRIG_H_
#define _INT_TRIG_H_
/******************************************************************************/
#include "quad_types.h"
#include <complex.h>
/******************************************************************************/
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
 * 	work	*double		[>=3n+4] workspace
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
 * 	work	*_Complex	[>=3n+4] workspace
 * 				n=q->n is the number of points
 * Output:
 * 	return	double		value of the desired integral
 * Note:
 * 	The functor *f must point to a smooth function on pt.
 * 	Workspace must be larger than (3*n+4)*2*2*sizeof(double) bytes long,
 * otherwise might run into run time errors.
 */
double _Complex cit_symmetric(const struct st_quadrule *q, 
		const double *restrict p,
		double _Complex (*f)(double,double), double *restrict work);
	
#ifdef __cplusplus
}
#endif
/******************************************************************************/
#endif
