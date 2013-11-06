#ifndef _DUNAVANT_RULE_H_
#define _DUNAVANT_RULE_H_

#include <vector.h>
#include <cassert>
#include <mkl.h>
#define MALLOC_ALIGNMENT 64

class DunavantRule {
	private:
		std::vector<double*> 	fxy;
		std::vector<double*> 	fw;
	public:
		DunavantRule() : fxy(0), fw(0) {}
		~DunavantRule() { Free(); }

		void Generate( int rule, double **xy, double **w, int *order_num ) {
			assert(1<=rule && rule<=dunavant_rule_num() );
			*order_num = dunavant_order_num ( rule ); 
			*xy = (double*) mkl_malloc( 2 * (*order_num) * sizeof(double), MALLOC_ALIGNMENT );
			*w = (double*) mkl_malloc( (*order_num) * sizeof(double), MALLOC_ALIGNMENT); 
			assert( (*xy) && (*w) );
			dunavant_rule ( rule, *order_num, *xy, *w );
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

		
	private:
		int dunavant_degree ( int rule );
		int dunavant_order_num ( int rule );
		void dunavant_rule ( int rule, int order_num, double xy[], double w[] );
		int dunavant_rule_num ( );
		int *dunavant_suborder ( int rule, int suborder_num );
		int dunavant_suborder_num ( int rule );
		void dunavant_subrule ( int rule, int suborder_num, double suborder_xyz[], double suborder_w[] );
		void dunavant_subrule_01 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
		void dunavant_subrule_02 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
		void dunavant_subrule_03 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
		void dunavant_subrule_04 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
		void dunavant_subrule_05 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
		void dunavant_subrule_06 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
		void dunavant_subrule_07 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
		void dunavant_subrule_08 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
		void dunavant_subrule_09 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
		void dunavant_subrule_10 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
		void dunavant_subrule_11 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
		void dunavant_subrule_12 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
		void dunavant_subrule_13 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
		void dunavant_subrule_14 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
		void dunavant_subrule_15 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
		void dunavant_subrule_16 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
		void dunavant_subrule_17 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
		void dunavant_subrule_18 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
		void dunavant_subrule_19 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
		void dunavant_subrule_20 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
		void file_name_inc ( char *file_name );
		int i4_max ( int i1, int i2 );
		int i4_min ( int i1, int i2 );
		int i4_modp ( int i, int j );
		int i4_wrap ( int ival, int ilo, int ihi );
		double r8_huge ( );
		int r8_nint ( double x );
		void reference_to_physical_t3 ( double t[], int n, double ref[], double phy[] );
		int s_len_trim ( char *s );
		double triangle_area ( double t[2*3] );
		void triangle_points_plot ( char *file_name, double node_xy[], int node_show, int point_num, double point_xy[], int point_show );
		void timestamp ( );
}; // class DunavantRule

// Declaration of a global DunavantRule object gDunavantRule.
// gDunavantRule is meant to be the ONLY DunavantRule object
// in the entire program.
extern DunavantRule gDunavantRule;

#endif // End of protection macro _DUNAVANT_RULE_H_
