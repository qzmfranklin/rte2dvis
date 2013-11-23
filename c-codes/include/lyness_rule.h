#ifndef _LYNESS_H_
#define _LYNESS_H_
#include <string.h>
#include <stack>
#include <cassert>
#include <mkl.h>
#define MALLOC_ALIGNMENT 64
/******************************************************************************/



namespace QuadratureRules {

using namespace std;
class LynessRule {
	private:
		std::stack<double*> 	fxy;
		std::stack<double*> 	fw;
	public:
		LynessRule() {}
		~LynessRule() { ReleaseMemory(); }

		/*
		 * Generate Lyness quadrature rule of given given (int rule).
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
		 * global LynessRule instance is destructed.
		 */
		void Generate(int rule, double* &xy, double* &w, int &order_num); 

		int RuleNumber() { return lyness_rule_num(); }

		int Order(int rule) { return lyness_order(rule); }

		int Degree(int rule) { return lyness_precision(rule); }

		void ReleaseMemory(); 
	private:
		int i4_max ( int i1, int i2 );
		int i4_min ( int i1, int i2 );
		int i4_modp ( int i, int j );
		int i4_wrap ( int ival, int ilo, int ihi );
		int lyness_order ( int rule );
		int lyness_precision ( int rule );
		void lyness_rule ( int rule, int order, double w[], 
				double x[] );
		int lyness_rule_num ( );
		int *lyness_suborder ( int rule, int suborder_num );
		int lyness_suborder_num ( int rule );
		void lyness_subrule ( int rule, int suborder_num, 
				double sub_xyz[], 
				double sub_w[] );
		double r8_abs ( double x );
		void r8mat_copy ( int m, int n, double a1[], double a2[] );
		void r8mat_write ( string output_filename, int m, int n, 
				double table[] );
		void r8vec_copy ( int n, double a1[], double a2[] );
		double r8vec_sum ( int n, double a[] );
		void timestamp ( );
		int *i4vec_copy_new ( int n, int a1[] );
};

extern LynessRule gLynessRule;

}

#endif
