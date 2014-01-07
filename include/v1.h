#ifndef _RTE2DVIS_V1_H_
#define _RTE2DVIS_V1_H_
/******************************************************************************/
/*
 * This header file defines the top level interface of the RTE solver.
 * Work flow of rte2dvis:
 *
 * 	1.	Call init_rte2dvis().
 *
 * 	2.	Fill up g,A,B,rhs manually.
 *
 * 	3.	Solve (A+B)sol=rhs for sol using some linear solver.
 *
 * 	4.	Release g,A,B,rhs accordingly.
 *
 * 	5.	Call destroy_rte2dvis().
 *
 * Among the above five steps, only the first and the last are provided in the
 * common interface. All the rest are too general to be contained inside a
 * interface.
 */
/******************************************************************************/
struct st_rte2dvis_info {
	/*
	 * Top level information of the RTE solver
	 * Must not be modified by the user.
	 */
	int                   version;		// 1=v1
	int                   flag;		// 1=HOMOGENEOUS 2=INHOMOGENEOUS
	int                   status;		// rte2dvis internal status
	int                   pad;		// padding mod for Nd Nm
	int                   Ns;		// number of triangles
	int                   Nd;		// max angular component index
	int                   Nm;		// 2(Nd+1)
	int                   Ng;		// Ns*Nm
	size_t                mem;		// memory used in bytes
	fftw_plan            *plans;		// fftw plans: IFWD IBWD OFWD OBWD

	/*
	 * The following buffers are to be filled up by init_rte2dvis.
	 * Must not be modified by the user.
	 */
	struct st_mesh_info  *mesh;		// mesh, defined in file_io.h
	double               *area;		// [Ns] signed area, right-handed 
	double               *cntr;		// [2Ns]centers of triangles

	/*
	 * The following buffers are to be supplied/filled up by the user.
	 * May be modified by the user when appropriate.
	 */
	double               *f;		// phase function components f(m)
	double               *A;		// identity matrix, block-wise diagonal
	double               *B;		// interaction matrix, block-wise Toeplitz-like
	double               *rhs;		// input vector, right hand side
	double               *sol;		// solution vector
	double               *work;		// workspace
};

/*
 * flag:
 * 	0=CONCISE	 	1=VERBOSE
 */
void print_rte2dvis(struct st_rte2dvis_info &q);

/*
 * Fill up q from version to area.
 * flag:
 * 	1=HOMOGENEOUS 2=INHOMOGENEOUS
 * About padding:
 * 	The default padding is 8. See the source file for details.
 */
void init_rte2dvis(struct st_rte2dvis_info &q, const char *fbase, 
		const int Nd, const int pad, const int flag);

/*
 * Check the consistency of the data.
 */
int check_rte2dvis(const struct st_rte2dvis_info &q);

/*
 * Release buffers initialized by init_rte2dvis.
 */
void destroy_rte2dvis(struct st_rte2dvis_info &sovler);
/******************************************************************************/
/*
 * The following routines are v1-specific, i.e., not part of the common
 * interface.
 */

/*
 * Allocate g,A,B,rhs,sol,work
 */
void alloc_rte2dvis_v1(struct st_rte2dvis_info &q);

/*
 * Fill up g,A,B,rhs
 */
void fill_rte2dvis_v1(struct st_rte2dvis_info &q);

/*
 * Solve (A+B)sol=rhs for sol using some linear solver.
 */
int solve_rte2dvis_v1(struct st_rte2dvis_info &q);

/*
 * Release g,A,B,rhs accordingly.
 */
void release_rte2dvis_v1(struct st_rte2dvis_info &q);

/******************************************************************************/
#endif
