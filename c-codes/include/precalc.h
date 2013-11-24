#ifndef _PRECALC_H_
#define _PRECALC_H_


#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/



/*
 * Convert triangles and nodes into a single array.
 * In the Mathematica codes:
 * 	p	nodes
 * 	t 	triangles
 * 	pt=p[[#]]&/@t
 * Input:
 * 	nodes_num	int		number of nodes
 * 	nodes		double*		Cartesian coordinates of nodes
 * 	triangles_num	int		number of triangles
 * 	triangles	int*		number of 3 nodes for a triangle
 * Output:
 * 	pt	pt[i] -- pt[i+5] represent the i-th triangle
 */
void precalc_pt(const int triangles_num, const int *restrict triangles, 
		const double *restrict nodes, double *restrict pt);

/*
 * Compute centers of triangles.
 * Input:
 * 	n	int		number of triangles
 * 	pt	double*		[6n] output from pt_precalc()
 * Output:
 * 	cntr	double*		[2n] centers of triangles in Cartesian coords
 */
void precalc_cntr(const int n, const double *restrict pt, 
		double *restrict cntr);

/*
 * Compute the signed area of triangles.
 * Input:
 * 	n	int		number of triangles
 * 	pt	double*		[6n] output from pt_precalc()
 * Output:
 * 	area	double*		[2n] signed areas of triangles
 * Notes:
 * 	Right hand rule.
 */
void precalc_signed_area(const int n, const double *restrict pt, 
		double *restrict area);

/*
 * Convert: Cartesian -> Polar.
 * Input:
 * 	n	int		number of coordinates to convert
 * 	in	double*		[2n] input vector
 * 	out	double*		[2n] output vector
 */
void precalc_cartesian_to_polar(const int n, const double *restrict in,
		double *restrict out);

/*TODO
 * Convert: Polar -> Cartesian.
 * Input:
 * 	n	int		number of coordinates to convert
 * 	in	double*		[2n] input vector
 * 	out	double*		[2n] output vector
 */
void precalc_polar_to_cartesian(const int n, const double *restrict in,
		double *restrict out);

/*
 * Compute out[i] = (*fun)(x[i]]) using Cartesian coords
 * Input:
 * 	fun 	functor		function pointer
 * 	n	int		number of positions
 * 	x	double*		[n] x
 * Output:
 * 	out	double*		[n]
 */
void precalc_fun(double (*fun)(double), const int n, 
		const double *restrict x, double *restrict out);

/*
 * Compute out[i] = (*fun)(xy[2*i],xy[2*i+1]) using Cartesian coords
 * Input:
 * 	fun 	functor		function pointer
 * 	n	int		number of positions
 * 	xy	double*		[2n] (x,y) or (r,phi)
 * Output:
 * 	out	double*		[n]
 */
void precalc_fun2(double (*fun)(double,double), const int n, 
		const double *restrict xy, double *restrict out);

/*TODO
 * Tell whether a point is inside a triangle or not.
 * Input:
 * 	p	double*		[2] point
 * 	pt	double*		[6] triangle
 * Output:
 * 	return	int		1=inside,  0=outside
 * Note:
 * 	If the point is _on_ the side of the triangle, return 0.
 */
int is_in_triangle(const double *restrict p, const double *restrict pt);

/*TODO
 * Tell which triangle does a point fall into.
 * Input:
 * 	p	double*		[2] point
 * 	n	int		number of triangles
 * 	pt	double*		[6n] triangles
 * Output:
 * 	return	int		number of the triangle where the 
 * 				given point falls into;
 * Note:
 * 	If the point does not fall into any of the given triangles, return -1.
 */
int in_which_triangle(const double *restrict p, 
		const int n, const double *restrict pt);

/*TODO
 * Measure the length of segment of a ray passing a triangle.
 * Input:
 * 	p0	double*		[2] point
 * 	phi	double		angle [radian]
 * 	pt	double*		[6] triangle
 * Output:
 * 	return	double		length of intersection
 * Note:
 * 	If the ray does not intersect with the triangle, return 0.0.
 */
double ray_triangle_seg_len(const double *restrict p0, const double phi,
		const double *restrict pt);

/*TODO
 * Measure the length of segment of a finite line passing a triangle.
 * Input:
 * 	p1	double*		[2] starting point
 * 	p2	double*		[2] ending point
 * 	phi	double		angle [radian]
 * 	pt	double*		[6] triangle
 * Output:
 * 	return	double		length of intersection
 * Note:
 * 	If the line does not intersect with the triangle, return 0.0.
 */
double line_triangle_seg_len(const double *restrict p1, 
		const double *restrict p2, const double *restrict pt);
/******************************************************************************/
#ifdef __cplusplus
}
#endif


#endif
