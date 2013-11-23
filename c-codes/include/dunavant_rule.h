#ifndef _DUNAVANT_RULE_H_
#define _DUNAVANT_RULE_H_

#include <stack>
#include <cassert>
#include <mkl.h>
#define MALLOC_ALIGNMENT 64
namespace QuadratureRules {

class DunavantRule {
	private:
		std::stack<double*> 	fxy;
		std::stack<double*> 	fw;
	public:
		DunavantRule() {}
		~DunavantRule() { ReleaseMemory(); }

		/*
		 * Generate Dunavant quadrature rule of given given (int rule).
		 * Allocates memory using mkl_malloc automatically. 64 bytes
		 * aligned. Return the pointers of the abscissas and the weights
		 * *xy and *w through reference.
		 *
		 * Ownership issue:
		 * The memory was allocated from within Generate(). In
		 * principle, one should NOT free the memory on his/her own.
		 * Instead, please call ReleaseMemory() to release the memory,
		 * if must. Even if one does not do anything about those
		 * allocated memory, they will be released (mkl_free()) when the
		 * global DunavantRule instance is destructed.
		 */
		void Generate(int rule, double* &xy, double* &w, int &order_num); 

		int RuleNumber() { return dunavant_rule_num(); }

		int Order(int rule) { return dunavant_order_num(rule); }

		int Degree(int rule) { return dunavant_degree(rule); }

		void ReleaseMemory(); 

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

extern DunavantRule gDunavantRule;


} // namespace QuadratureRules
#endif // End of protection macro _DUNAVANT_RULE_H_
