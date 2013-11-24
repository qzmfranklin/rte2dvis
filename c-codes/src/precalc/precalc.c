#include "precalc.h"
#include <math.h> 

void precalc_pt(const int triangles_num, const int *restrict triangles, 
		const double *restrict nodes, double *restrict pt) {
	for (int i = 0; i < triangles_num*3; i++) { 
		pt[i+0] = nodes[triangles[i]+0];
		pt[i+1] = nodes[triangles[i]+1];
	}
}

static void scale(const double alpha, const int n, double *restrict a) {
	for (int i = 0; i < n; i++)
		a[i] *= alpha;
}

void precalc_cntr(const int n, const double *restrict pt, 
		double *restrict cntr) {
	for (int i = 0; i < n; i++) {
		cntr[2*i+0] = pt[6*i+0] + pt[6*i+2] + pt[6*i+4];
		cntr[2*i+1] = pt[6*i+1] + pt[6*i+3] + pt[6*i+5];
	}
	scale(1.0/3.0, n, cntr);
}

void precalc_signed_area(const int n, const double *restrict pt, 
		double *restrict area) {
	for (int i = 0; i < n; i++)
		area[i] = 0.5*(  (pt[6*i+2] - pt[6*i+0])
				*(pt[6*i+3] - pt[6*i+1])
				-(pt[6*i+4] - pt[6*i+0])
				*(pt[6*i+5] - pt[6*i+1]));
}

void precalc_cartesian_to_polar(const int n, const double *restrict in,
		double *restrict out) {
	for (int i = 0; i < n; i++)
		out[2*i+0] = sqrt( in[2*i+0]*in[2*i+0] 
				+ in[2*i+1]*in[2*i+1] );
	for (int i = 0; i < n; i++)
		out[2*i+1] = atan2( in[2*i+1], in[2*i+0] );
}

/*TODO
 * Convert: Polar -> Cartesian.
 * Input:
 * 	n	int		number of coordinates to convert
 * 	in	double*		[2n] input vector
 * 	out	double*		[2n] output vector
 */
void precalc_polar_to_cartesian(const int n, const double *restrict in,
		double *restrict out) {
	double c, s;
	for (int i = 0; i < n; i++) {
		sincos(in[2*i+1],&c,&s);
		out[2*i+0] = in[2*i] * c;
		out[2*i+1] = in[2*i] * s;
	}
}

void precalc_fun(double (*fun)(double), const int n, 
		const double *restrict x, double *restrict out) {
	for (int i = 0; i < n; i++)
		out[i] = (*fun)(x[i]);
}

void precalc_fun2(double (*fun)(double,double), const int n, 
		const double *restrict xy, double *restrict out) {
	for (int i = 0; i < n; i++)
		out[i] = (*fun)(xy[2*i],xy[2*i+1]);
}

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
