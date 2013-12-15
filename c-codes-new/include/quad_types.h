#ifndef _QUAD_TYPES_H_
#define _QUAD_TYPES_H_
/******************************************************************************/
/*
 * This header file defines the basic data structure for all quadrature rules
 * and related routines. This header file should only be included by other
 * header files and shall never be directly used by an external user.
 */ 

/*
 * Structure of quadrature rules.
 * 	dim:
 * 		1	1D line
 * 		2	2D triangle
 * 		3	3D tetrahedron
 */
struct st_quadrule {
	int dim;	// dimension
	int n;		// number of points
	double *x;	// abscissa
	double *w;	// weights
};

/******************************************************************************/
#endif
