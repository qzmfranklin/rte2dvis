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
 * 	n		int		number of triangles
 * 	triangles	int*		nodes of triangles
 * 	nodes		double*		Cartesian coordinates of nodes
 * Output:
 * 	x		double*		[3n] x coordinates of triangles
 * 	y		double*		[3n] y coordinates of triangles
 */
void precalc_pt(const int n, const int *restrict triangles, 
		const double *restrict nodes, 
		double *restrict x, double *restrict y);

/*
 * Compute centers of triangles.
 * Input:
 * 	n	int		number of triangles
 * 	x	double*		[3n] x coordinates of triangles
 * 	y	double*		[3n] y coordinates of triangles
 * Output:
 * 	cx	double*		[n] centers of triangles in Cartesian coords
 * 	cy	double*		[n] centers of triangles in Cartesian coords
 */
void precalc_cntr(const int n, 
		const double *restrict x, const double *restrict y,
		double *restrict cx, double *restrict cy);

/*
 * Compute the signed area of triangles.
 * Input:
 * 	n	int		number of triangles
 * 	x	double*		[3n] x coordinates of triangles
 * 	y	double*		[3n] y coordinates of triangles
 * Output:
 * 	area	double*		[n] signed areas of triangles
 * Notes:
 * 	Right hand rule.
 */
void precalc_signed_area(const int n,
		const double *restrict x, const double *restrict y,
		double *restrict area);


/*
 * Convert: Cartesian -> Polar.
 * Input:
 * 	n	int		number of coordinates to convert
 * 	inx	double*		[n] X of input vector
 * 	iny	double*		[n] Y of input vector
 * Output:
 * 	outr	double*		[n] R of output vector
 * 	outphi	double*		[n] PHI of output vector
 */
void precalc_cartesian_to_polar(const int n,
		const double *restrict inx, const double *restrict iny,
		double *restrict outr, double *restrict outphi);

/*TODO
 * Convert: Polar -> Cartesian.
 * Input:
 * 	n	int		number of coordinates to convert
 * 	inr	double*		[n] R of input vector
 * 	inphi	double*		[n] PHI of input vector
 * Output:
 * 	outx	double*		[n] X of output vector
 * 	outy	double*		[n] Y of output vector
 */
void precalc_polar_to_cartesian(const int n,
		const double *restrict inr, const double *restrict inphi,
		double *restrict outx, double *restrict outy);

/*
 * Map fun: R->R on in->out
 * Input:
 * 	fun 	functor		function pointer
 * 	n	int		number of positions
 * 	in	double*		[n] input vector
 * Output:
 * 	out	double*		[n] output vector
 */
void precalc_fun(double (*fun)(double), const int n, 
		const double *restrict in, double *restrict out);

/*
 * Map fun: RxR->R on in->out
 * Input:
 * 	fun 	functor		function pointer
 * 	n	int		number of positions
 * 	in1	double*		[n] 1st coord of input vector
 * 	in2	double*		[n] 2nd coord of input vector
 * Output:
 * 	out	double*		[n] output vector
 */
void precalc_fun2(double (*fun)(double,double), const int n, 
		const double *restrict in1, const double *restrict in2,
		double *restrict out);
/******************************************************************************/
#ifdef __cplusplus
}
#endif


#endif
