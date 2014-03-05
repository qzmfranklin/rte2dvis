#ifndef _WANDZURA_H_
#define _WANDZURA_H_
/******************************************************************************/
#include "quadrules_internal.h"
/******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif 
int wandzura_degree ( int rule );
int wandzura_order_num ( int rule );
void wandzura_rule ( int rule, int order_num, double xy[], double w[] );
int wandzura_rule_num ( void );
int *wandzura_suborder ( int rule, int suborder_num );
int wandzura_suborder_num ( int rule );
void wandzura_subrule ( int rule, int suborder_num, double suborder_xyz[], 
  double suborder_w[] );
void wandzura_subrule_1 ( int suborder_num, double suborder_xyz[], 
  double suborder_w[] );
void wandzura_subrule_2 ( int suborder_num, double suborder_xyz[], 
  double suborder_w[] );
void wandzura_subrule_3 ( int suborder_num, double suborder_xyz[], 
  double suborder_w[] );
void wandzura_subrule_4 ( int suborder_num, double suborder_xyz[], 
  double suborder_w[] );
void wandzura_subrule_5 ( int suborder_num, double suborder_xyz[], 
  double suborder_w[] );
void wandzura_subrule_6 ( int suborder_num, double suborder_xyz[], 
  double suborder_w[] );
//
//  Versions of the rule using the nonstandard reference triangle:
//
void wandzura_subrule2_1 ( int suborder_num, double suborder_xy[], 
  double suborder_w[] );
void wandzura_subrule2_2 ( int suborder_num, double suborder_xy[], 
  double suborder_w[] );
void wandzura_subrule2_3 ( int suborder_num, double suborder_xy[], 
  double suborder_w[] );
void wandzura_subrule2_4 ( int suborder_num, double suborder_xy[], 
  double suborder_w[] );
void wandzura_subrule2_5 ( int suborder_num, double suborder_xy[], 
  double suborder_w[] );
void wandzura_subrule2_6 ( int suborder_num, double suborder_xy[], 
  double suborder_w[] );	
#ifdef __cplusplus
}
#endif
/******************************************************************************/
#endif /* end of include guard */
