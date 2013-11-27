/*
 * Cross approximation with full pivoting of a general double floating precision
 * matrix.
 * Input:
 * 	m,n	int		number of rows and columns of input matrix
 * 	a	double*		[>=m*n] input matrix, col-maj storage
 * 	r	int		maximal rank of a
 * 	lda	int		leading dimension of a
 * 	ldu	int		leading dimension of u
 * 	ldvt	int		leading dimension of vt
 * Output:
 * 	a	double*		upon exit, stores the residual matrix
 * 	r	int		upon exit, stores the actual rank
 * 	u	double*		[>=m*r*ldu] left vectors, col-maj storage
 * 	vt	double*		[>=r*n*ldvt] right vectors, row-maj storage
 */
void dgefca(const int m, const int n, double *restrict a, const int lda,
		const int r, double *restrict u, const int ldu,
		double *restrict vt, const int ldvt)
{
} 
