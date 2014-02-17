#ifndef _MSH_RAY_TRACING_H_
#define _MSH_RAY_TRACING_H_
/******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

struct st_meshnode {
	int n;		// number of triangles
	double *trigs;	// pointer to the array of triangles
			// not NULL only when the node is a leaf

	// boundary of the square:
	double x1;
	double y1;
	double x2;
	double y2; 
};

struct st_meshtree {
	int status;	// 0=Uninit'd 1=Init'd <0=ERROR
	int n;		// number of packed levels
	struct st_meshnode *root; // pointer to the root of packed levels
};

struct st_meshtree *mshtree_new(const int n, const double *restrict trigs);
void mshtree_destroy(struct st_meshtree *t);

/*
 * Line parameters:
 * 	ln[0]	slope
 * 	ln[1]	y-intercept
 * Point:
 * 	p0[0]	x0
 * 	p0[1]	y0
 * Note:
 * 	ln[0] (slope) must not be inifity or 0.
 */ 
double mshtree_line_seg(struct st_mesh *restrict t, double ln[2]);
double mshtree_ray_seg(struct st_mesh *restrict t, double ln[2], double p0[2]);
	
#ifdef __cplusplus
}
#endif
/******************************************************************************/
#endif
