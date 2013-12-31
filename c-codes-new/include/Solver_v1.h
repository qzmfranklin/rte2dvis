#ifndef _SOLVER_V1_H_
#define _SOLVER_V1_H_
/******************************************************************************/
#include <stdio.h>
#include <math.h>
#include "QuadratureRules.h"
#include "file_io.h"
#include "complex.h"
#include "fftw3.h"
/******************************************************************************/
/*
 * Homogeneous only
 */
class Solver_v1 {
	public:
		int                   version;	// 1=v1
		int                   flag;	// 1=HOMOGENEOUS 2=INHOMOGENEOUS
		int                   status;	// rte2dvis internal status
		int                   pad;	// padding mod for Nd Nm
		int                   Ns;	// number of triangles
		int                   Nd;	// max angular component index
		int                   Nm;	// 2*Nd+1 plus some padding
		int                   Ng;	// Ns*Nm
		fftw_plan            *plans;	// fftw plans: IFWD IBWD OFWD OBWD

		struct st_mesh_info  *mesh;	// mesh, defined in file_io.h
		double               *p;	// mesh->trigs, convenience pointer
		double               *area;	// [Ns] signed area, right-handed 
		double               *cntr;	// [2Ns]centers of triangles

		double               *f;	// phase function components f(m)
		double               *A;	// identity matrix, block-wise diagonal
		double _Complex      *B;	// interaction matrix, block-wise Toeplitz-like
		double _Complex      *rhs;	// input vector, right hand side, packed
		double _Complex      *work;	// workspace when doing mulplication
	public:
		Solver_v1(const char* fbase, const int Nd, const int pad);
		~Solver_v1() { ReleaseMemory(); }
		void Check();
		void Debug();
		void Solve(double _Complex *restrict sol);
		void mul(const double *restrict in, double *restrict out);

		void Print();
	private:
		void Fill_mesh(const char* fbase);
		void Fill_area();
		void Fill_cntr();
		void Fill_f();
		void Fill_A();
		void Fill_B();
		void Fill_rhs();

		void ReleaseMemory();
};
/******************************************************************************/
#endif /* End of protection macro _SOLVER_V1_H_ */
