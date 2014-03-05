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
#include <complex.h>
#include "mesh.h"
/******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif 
struct st_solver_v1 {
	int status; // internal status

	struct st_mesh *mesh; // raw mesh

	int Ns; // #triangles
	int Nd; // #highest angular harmonic
	int Nv; // #vertices
	int Ne; // #edeges
	int Nt; // #total d.o.f.

	int M; // spatial interpolation order


	double _Complex *work; // complex workspace
	double *tmp; // real workspace
};
#ifdef __cplusplus
}
#endif
/******************************************************************************/
#endif /* end of include guard */
