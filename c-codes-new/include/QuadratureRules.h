#ifndef _QUADRATURE_RULES_H_
#define _QUADRATURE_RULES_H_
/******************************************************************************/
/*
 * Top level header file for all quadrature rules and routines to compute
 * various kinds of numerical integrals - everything in the namespace
 * Quadraturerules.
 */ 
#include "quad_types.h"
#include "DunavantRule.h"
#include "LynessRule.h"
#include "WandzuraRule.h" 
#include "GaussRule.h" 
#include "ArcSinhMethod.h"
/******************************************************************************/
namespace QuadratureRules {

void print_rule_1d(int order_num, double *x, double *w);
void print_rule_2d(int order_num, double *xy, double *w);
void print_quadrule(struct st_quadrule *q);

void ReleaseMemory();

extern const double std_ref_trig[6];	// standard reference triangle

/*
 *  Standard Reference Triangle:
 *    |
 *    1  3
 *    |  |\
 *    |  | \
 *    S  |  \
 *    |  |   \
 *    |  |    \
 *    0  1-----2
 *    |
 *    +--0--R--1-->
 * Input:
 * 	n		number of sampling points
 * 	phy_trig	[6] the destination triangle
 * 	in		[2n]the abscissas to be transformed
 * Output:
* 	out		[2n]the abscissas in the physical triangle
 */ 
void ref_to_phy_trig(const int n, double *phy_trig, double *in, double *out);
} // namespace QuadratureRules

#endif /* End of protection macro _QUADRATURE_RULES_H_ */
