#ifndef _WANDZURA_RULE_H_
#define _WANDZURA_RULE_H_

#include <vector.h>
#include <cassert>
#include <mkl.h>
#define MALLOC_ALIGNMENT 64

namespace QuadratureRules {

	class WandzuraRule {
		private:
			std::vector<double*> 	fxy;
			std::vector<double*> 	fw;
		public:
			WandzuraRule() : fxy(0), fw(0) {}
			~WandzuraRule() { Free(); }

			void Generate( int rule, double **xy, double **w, int *order_num ) {
				assert( 1<=rule && rule<=wandzura_rule_num() );
				*order_num = wandzura_order_num ( rule ); 
				*xy = (double*) mkl_malloc( 2 * (*order_num) * sizeof(double), MALLOC_ALIGNMENT );
				*w = (double*) mkl_malloc( (*order_num) * sizeof(double), MALLOC_ALIGNMENT); 
				assert( (*xy) && (*w) );
				wandzura_rule ( rule, *order_num, *xy, *w );
				fxy.push_back(*xy);
				fw.push_back(*w);
			} 

			/*
			 * Free memory when destructed or explicitly called Free().
			 * The goal of this design is to keep the programmer free
			 * from freeing memory. Usually, one NEVER needs to call
			 * Free() explicitly.
			 */
			void Free() {
				assert( fxy.size() == fw.size() );
				int i, n = fw.size();
				for (i = 0; i < n; i++) {
					mkl_free( fxy[i] );
					mkl_free( fw[i] );
				}
			}

		public: 
			void file_name_inc ( char *file_name );
			int i4_max ( int i1, int i2 );
			int i4_min ( int i1, int i2 );
			int i4_modp ( int i, int j );
			int i4_wrap ( int ival, int ilo, int ihi );
			double r8_huge ( void );
			int r8_nint ( double x );
			void reference_to_physical_t3 ( double t[], int n, double ref[], double phy[] );
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

	// Declaration of a global WandzuraRule object gWandzuraRule.
	// gWandzuraRule is meant to be the ONLY WandzuraRule object
	// in the entire program.
	extern WandzuraRule gWandzuraRule;
} // namespace QuadratureRules
#endif // End of protection macro _WANDZURA_RULE_H_
