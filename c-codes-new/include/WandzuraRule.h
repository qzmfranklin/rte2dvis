#ifndef _WANDZURA_RULE_H_
#define _WANDZURA_RULE_H_
/******************************************************************************/

#include <stack>
#include <cassert>
#include <mkl.h>
#include "quad_types.h"
/******************************************************************************/
namespace QuadratureRules {

class WandzuraRule {
	private:
		int status;
		int _frule;
		double *_fxy;
		double *_fw;
		double *_fwork;
		size_t _flwork;
		size_t _fsize;
	public:
		size_t mem;	// memory consumption in bytes

		WandzuraRule(const int rule);
		~WandzuraRule();
		void Print();
		void Reset(const int rule);
		void Generate(struct st_quadrule *q, 
				const double *restrict p=NULL);
		int Order() { return wandzura_order_num(_frule); } 

	private:
		void AllocateMemory(const size_t size);
		void ReleaseMemory();

		/**************************************/


	private: 
		void file_name_inc ( char *file_name );
		int i4_max ( int i1, int i2 );
		int i4_min ( int i1, int i2 );
		int i4_modp ( int i, int j );
		int i4_wrap ( int ival, int ilo, int ihi );
		double r8_huge ( void );
		int r8_nint ( double x );
		void reference_to_physical_t3 ( const double t[], int n, double ref[], double phy[] );
		int s_len_trim ( char *s );
		void timestamp ( );
		double triangle_area ( double t[2*3] );
		void triangle_points_plot ( char *file_name, double node_xy[], int node_show, int point_num, double point_xy[], int point_show );
		int wandzura_degree ( int rule );
		int wandzura_order_num ( int rule );
		void wandzura_rule ( int rule, int order_num, double xy[], double w[] );
		int wandzura_rule_num ( void );
		int *wandzura_suborder ( int rule, int suborder_num );
		int wandzura_suborder_num ( int rule );
		void wandzura_subrule ( int rule, int suborder_num, double suborder_xyz[], double suborder_w[] );
		void wandzura_subrule_1 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
		void wandzura_subrule_2 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
		void wandzura_subrule_3 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
		void wandzura_subrule_4 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
		void wandzura_subrule_5 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
		void wandzura_subrule_6 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
		//
		//  Versions of the rule using the nonstandard reference triangle:
		//
		void wandzura_subrule2_1 ( int suborder_num, double suborder_xy[], double suborder_w[] );
		void wandzura_subrule2_2 ( int suborder_num, double suborder_xy[], double suborder_w[] );
		void wandzura_subrule2_3 ( int suborder_num, double suborder_xy[], double suborder_w[] );
		void wandzura_subrule2_4 ( int suborder_num, double suborder_xy[], double suborder_w[] );
		void wandzura_subrule2_5 ( int suborder_num, double suborder_xy[], double suborder_w[] );
		void wandzura_subrule2_6 ( int suborder_num, double suborder_xy[], double suborder_w[] );

};

/*
 * By defaut, uses the highest order quadrature rule
 * 	RULE = 6, ORDER = 175
 */
extern WandzuraRule gWandzuraRule;

} // namespace QuadratureRules
/******************************************************************************/
#endif // End of protection macro _WANDZURA_RULE_H_
