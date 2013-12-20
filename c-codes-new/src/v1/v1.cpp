#include <complex.h>
#include <mkl.h>
#include <fftw3.h>
#include "utils.h"
#include "QuadratureRules.h"
#include "toeplitz.h"
#include "file_io.h"
/******************************************************************************/
struct st_rte2dvis_v1_data{
	int flag;		// 1=HOMOGENEOUS 2=INHOMOGENEOUS
	int Ns;			// number of triangles
	int Nd;			// max angular component index
	int Nm;			// 2(Nd+1)
	int Ng;			// Ns*Nm
	double *area;		// signed area, right-handed
	double *g;		// array of g^|m|, m=-Nd to +Nd. g[Nd]=1.0
	double _Complex *A;	// identity matrix, block-wise diagonal
				// store the block-wise diagonal lines
	double _Complex *B;	// interaction matrix, block-wise Toeplitz-like
				// store the DFT of block-wise Toeplitz columns
	double _Complex *rhs;	// input vector, right hand side
};
