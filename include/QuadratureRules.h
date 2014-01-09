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

void print_quadrule(struct st_quadrule *q);

} // namespace QuadratureRules

#endif /* End of protection macro _QUADRATURE_RULES_H_ */
