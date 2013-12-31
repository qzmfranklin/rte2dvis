#ifndef _ARCSINH_METHOD_H_
#define _ARCSINH_METHOD_H_
/******************************************************************************/
#include <stack>
#include "quad_types.h"
#include <stddef.h>
/******************************************************************************/
namespace QuadratureRules {

class ArcSinhMethod {
	private: 
		int status;
		std::stack<double*> _fxy;
		std::stack<double*> _fw;
		size_t _fsize;
		size_t _flwork;
		double *_fwork;
		struct st_quadrule *_fqu;
		struct st_quadrule *_fqv;
	public:
		ArcSinhMethod();
		~ArcSinhMethod() { ReleaseMemory(); }

		void Print();
		void Init(struct st_quadrule *qu, struct st_quadrule *qv);
		void Generate(	struct st_quadrule *q, 
				const double *restrict p, 
				const double *restrict p0);
		void GenerateAtomic(struct st_quadrule *q, 
				const double *restrict p);
		int Order() { return 3 * _fqu->n * _fqv->n; }
		void ReleaseMemory();
	private:
		void Atomic(	const double *p0, const double *p1, 
				const double *p2, double *restrict x, 
				double *restrict w);
};

extern ArcSinhMethod gArcSinhMethod;

} // namespace QuadratureRules
/******************************************************************************/
#endif /* End of protection macro _ARCSINH_METHOD_H_ */
