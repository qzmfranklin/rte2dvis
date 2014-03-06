#ifndef _SOLVER_V1_H_
#define _SOLVER_V1_H_
/*
 * Version 1
 * 	solve (I+K).X=V for X
 * 	pulse spatial basis function
 * 	I = purely diagonal, only store diagonal elements
 * 	K = blockwise Toeplitz, store the DFT of the first col+row
 */
/******************************************************************************/
#include <fftw3.h>
#include <unistd.h>
#include <stdio.h>
#include <complex.h>
#include "mesh.h"
/******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif 
struct st_solver_v1 {
	int status; // internal status

	int M; // spatial interpolation order
	int pad; // padding for Nd Nm

	int Ns; // #triangles
	int Nv; // #vertices
	int Ne; // #edeges
	int Nt; // #total spatial d.o.f.

	int Nd; // #highest angular harmonic
	int Nm;	// #padded angular number >=2*Nd+1

	int Ng; // #total d.o.f.

	struct st_mesh *mesh; // raw mesh

	double *E; // I, avoid _Complex_I
	double *K; // K, DFT, row-major, [Ns,Ns,2*Nm]

	double _Complex *work; // complex workspace
	double *tmp; // real workspace

	fftw_plan *plans; // fftw_plan, unused so far

	int ipar[128]; // integer parameters
	double dpar[128]; // double precision parameters 
};

/*
 * ipar[0] = M
 * ipar[1] = Nd
 * ipar[2] = pad
 * ipar[3] = rule1
 * ipar[4] = rule2
 * ipar[5] = nu
 * ipar[6] = nv
 * ipar[7] = num_threads in omp
 */
/*
 * dpar[0] = g
 * dpar[1] = phis (planewave incident)
 * dpar[2] = mua (absorption coefficient)
 * dpar[3] = mus (scattering coefficient)
 */
struct st_solver_v1 *sv1_create_solver(struct st_mesh *q, const int *ipar, const double *dpar);
void sv1_solve(double _Complex *solution);
void sv1_mul(struct st_solver_v1 *s, const double _Complex *in, double _Complex *out);
void sv1_print_solver(struct st_solver_v1 *s);
void sv1_destroy_solver(struct st_solver_v1 *s);
#ifdef __cplusplus
}
#endif
/******************************************************************************/
#endif /* end of include guard */
