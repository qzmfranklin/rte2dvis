#ifndef _GAUSS_QUADRATURES_H_
#define _GAUSS_QUADRATURES_H_

#include <stack>
#include <mkl.h>
#include <cassert>
#include <cstdio>

namespace QuadratureRules { 
class GaussRule {
	private: 
		std::stack<double*>	_fx;

	public: 
		GaussRule() {}
		~GaussRule() { ReleaseMemory(); }

		enum {
			kLegendre = 1,
			kChebyshevI,
			kGegenbauer,
			kJacobi,
			kGeneralizedLaguerre,
			kGeneralizedHerite,
			kExponential,
			kRational
		};

/*
 * Input, int KIND, the rule.
 * 1, Legendre,             (a,b)       1.0
 * 2, Chebyshev Type 1,     (a,b)       ((b-x)*(x-a))^-0.5)
 * 3, Gegenbauer,           (a,b)       ((b-x)*(x-a))^alpha
 * 4, Jacobi,               (a,b)       (b-x)^alpha*(x-a)^beta
 * 5, Generalized Laguerre, (a,inf)     (x-a)^alpha*exp(-b*(x-a))
 * 6, Generalized Hermite,  (-inf,inf)  |x-a|^alpha*exp(-b*(x-a)^2)
 * 7, Exponential,          (a,b)       |x-(a+b)/2.0|^alpha
 * 8, Rational,             (a,inf)     (x-a)^alpha*(x+b)^beta
 */
		void Generate(	double* &x, double* &w,
				const int order	= 1, 
				const double a	= 0.0,
				const double b	= 1.0,
				const int kind	= kLegendre, 
				const double alpha= 0.0,
				const double beta= 0.0); 

		void ReleaseMemory();

		/**************************************/ 
	private:
		void cdgqf ( int nt, int kind, double alpha, double beta, double t[], double wts[] );
		void cgqf ( int nt, int kind, double alpha, double beta, double a, double b, double t[], double wts[] );
		double class_matrix ( int kind, int m, double alpha, double beta, double aj[], double bj[] );
		void imtqlx ( int n, double d[], double e[], double z[] );
		void parchk ( int kind, int m, double alpha, double beta );
		double r8_abs ( double x );
		double r8_epsilon ( );
		double r8_gamma ( double x );
		double r8_sign ( double x );
		void r8mat_write ( char *output_filename, int m, int n, double table[] );
		void rule_write ( int order, char *filename, double x[], double w[], double r[] );
		void scqf ( int nt, double t[], int mlt[], double wts[], int nwts, int ndx[], double swts[], double st[], int kind, double alpha, double beta, double a, double b );
		void sgqf ( int nt, double aj[], double bj[], double zemu, double t[], double wts[] );
		void timestamp ( );
};
extern GaussRule gGaussRule;

} // namespace QuadratureRules
#endif // End of protection macro _GAUSS_RULE_H_
