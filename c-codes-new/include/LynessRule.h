#ifndef _LYNESS_H_
#define _LYNESS_H_
/******************************************************************************/
#include <string>
#include <stack>
#include <cassert>
#include <mkl.h>
#include "quad_types.h"
#define MALLOC_ALIGNMENT 64
/******************************************************************************/ 
namespace QuadratureRules {

using namespace std;
class LynessRule {
	private:
		std::stack<double*> 	_fxy;
		std::stack<double*> 	_fw;
	public:
		LynessRule() {}
		~LynessRule() { ReleaseMemory(); }

		void Generate(int rule, struct st_quadrule *q);
		void Generate(int rule, int &order_num, 
				double *&xy, double *&w); 
		void Generate(int rule, int &order_num, 
				double *&x, double *&y, double *&w); 
		int RuleNumber() { return lyness_rule_num()+1; } 
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
		void r8mat_write ( std::string output_filename, int m, int n, 
				double table[] );
		void r8vec_copy ( int n, double a1[], double a2[] );
		double r8vec_sum ( int n, double a[] );
		void timestamp ( );
		int *i4vec_copy_new ( int n, int a1[] );
};


extern LynessRule gLynessRule;
}

/******************************************************************************/
#endif
