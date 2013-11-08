#ifndef _GAUSS_QUADRATURES_H_
#define _GAUSS_QUADRATURES_H_

#include <vector.h>
#include <mkl.h>
#include <cassert>
#include <cstdio>
#define MALLOC_ALIGNMENT 64

namespace QuadratureRules {


	/*
	 * Parameters:
	 * 
	 * Input, int ORDER, the number of knots.
	 * 
	 * Input, int KIND, the rule.
	 * 1, Legendre,             (a,b)       1.0
	 * 2, Chebyshev Type 1,     (a,b)       ((b-x)*(x-a))^-0.5)
	 * 3, Gegenbauer,           (a,b)       ((b-x)*(x-a))^alpha
	 * 4, Jacobi,               (a,b)       (b-x)^alpha*(x-a)^beta
	 * 5, Generalized Laguerre, (a,inf)     (x-a)^alpha*exp(-b*(x-a))
	 * 6, Generalized Hermite,  (-inf,inf)  |x-a|^alpha*exp(-b*(x-a)^2)
	 * 7, Exponential,          (a,b)       |x-(a+b)/2.0|^alpha
	 * 8, Rational,             (a,inf)     (x-a)^alpha*(x+b)^beta
	 * 
	 * Input, double ALPHA, the value of Alpha, if needed.
	 * 
	 * Input, double BETA, the value of Beta, if needed.
	 * 
	 * Input, double A, B, the interval endpoints.
	 * 
	 * Output, double X[ORDER], the knots.
	 * 
	 * Output, double W[ORDER], the weights.
	 * 
	 * Example of calling:
	 * cgqf ( order, kind, alpha, beta, a, b, x, w );
	 *
	 * */

	class GaussQuadratures {
		private: 
			std::vector<double*>	fx;
			std::vector<double*>	fw;

		public: 
			GaussQuadratures(): fx(0), fw(0) {};
			~GaussQuadratures() { Free(); }

			void Generate(	
					double **x, double **w,
					int	order	= 1, 
					double	a	= 0.0,
					double	b	= 1.0,
					int	kind	= kLegendre, 
					double	alpha	= 0.0,
					double	beta	= 0.0
				     ) {
				assert( (order>0) && (a<b) && ((int)kind>0) && ((int)kind<9) );
				*x = (double*) mkl_malloc( order * sizeof(double), MALLOC_ALIGNMENT );
				*w = (double*) mkl_malloc( order * sizeof(double), MALLOC_ALIGNMENT );
				assert( (*x) && (*w) ); 
				cgqf ( order, kind, alpha, beta, a, b, *x, *w ); 
				fx.push_back(*x);
				fw.push_back(*w);
			}

			/*
			 * Free memory when destructed or explicitly called Free().
			 * The goal of this design is to keep the programmer free
			 * from freeing memory. Usually, one NEVER needs to call
			 * Free() explicitly.
			 */
			void Free() {
				assert( fx.size() == fw.size() );
				int i , n = fx.size();
				for (i = 0; i < n; i++) {
					mkl_free( fx[i] );
					mkl_free( fw[i] );
				}
			}

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


	// The purpose of this global object is to free
	// the programmer from having to constantly worry
	// about memory leak.
	extern GaussQuadratures gGaussQuadratures;

} // namespace QuadratureRules
#endif // End of protection macro _GAUSS_QUADRATURES_H_
