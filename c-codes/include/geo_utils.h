
#ifndef _GEO_UTILS_H_
#define _GEO_UTILS_H_


#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/ 

/*
 * Tell whether a point is inside a triangle or not.
 * Input:
 * 	p	double*		[2] point
 * 	x	double*		[3] x coords of the triangle
 * 	y	double*		[3] y coords of the triangle
 * Output:
 * 	return	int		1=inside,  0=outside
 * Note:
 * 	If the point is _on_ the side of the triangle, return 0.
 */
int geoutils_is_in_triangle(const double *restrict p, 
		const double *restrict x, const double *restrict y);

/*
 * Tell which triangle does a point fall into.
 * Input:
 * 	p	double*		[2] point
 * 	n	int		number of triangles
 * 	x	double*		[3n] x coords of the triangle
 * 	y	double*		[3n] y coords of the triangle
 * Output:
 * 	return	int		number of the triangle where the 
 * 				given point falls into;
 * Note:
 * 	If the point does not fall into any of the given triangles, return -1.
 */
int geoutils_in_which_triangle(const double *restrict p, const int n, 
		const double *restrict x, const double *restrict y);

/*
 * Measure the length of segment of a ray passing a triangle.
 * Input:
 * 	p0	double*		[2] point
 * 	phi	double		angle [radian]
 * 	x	double*		[3] x coords of the triangle
 * 	y	double*		[3] y coords of the triangle
 * Output:
 * 	return	double		length of intersection
 * Note:
 * 	If the ray does not intersect with the triangle, return 0.0.
 */
double geoutils_ray_triangle_seg_len(
		const double *restrict p0, const double phi,
		const double *restrict x, const double *restrict y);

/*
 * Measure the length of segment of a finite line passing a triangle.
 * Input:
 * 	p1	double*		[2] starting point
 * 	p2	double*		[2] ending point
 * 	phi	double		angle [radian]
 * 	x	double*		[3] x coords of the triangle
 * 	y	double*		[3] y coords of the triangle
 * Output:
 * 	return	double		length of intersection
 * Note:
 * 	If the line does not intersect with the triangle, return 0.0.
 */
double geoutils_line_triangle_seg_len(
		const double *restrict p1, const double *restrict p2, 
		const double *restrict x, const double *restrict y);
/******************************************************************************/
#ifdef __cplusplus
}
#endif


#endif
