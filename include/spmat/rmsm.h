#ifndef _RMSM_H_
#define _RMSM_H_
/******************************************************************************/
/*
 * Row Major Spase Matrix
 * Only be included indirectly through including spmat. through including
 * spmat.h. Not in the user space, i.e., not for direct use.
 */
#include <complex.h>
#include <vector>
/******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
// row-major sparse matrix, good for A.x operation
struct st_rmsm {
	int dim;// dimension
	int *n;	// n[i] stores the #elements in a[i]
	double **a;
	double *data; // used only when packed
	std::vector<double> *tmp; // used only when unpacked
};
/*
 * Usage:
 * struct st_rmsm *m=rmsm_create(100);
 * // adding values to elements at [i,j] though
 * // rmsm_add(m,i,j,val);
 * // Note that values add up.
 * rmsm_pack(m); // pack storage for maximal A.x performance
 * rmsm_printinfo(m);
 * // define and init double v[100] here...
 * // define double u[100] here...
 * rmsm_mul(m,v,u);
 * rmsm_destroy(m);
 */
rmsm_create(const int size);
rmsm_add(struct st_rmsm *m, const int row, const int col, const double val);
rmsm_pack(struct st_rmsm *m);
//rmsm_unpack(struct st_rmsm *m);
rmsm_printinfo(struct st_rmsm *m);
rmsm_destroy(struct st_rmsm *m);
#ifdef __cplusplus
}
#endif 
rmsm_mul(struct st_rmsm *m, const double *restrict in, double *restrict out);
rmsm_mul(struct st_rmsm *m, const double _Complex *restrict in, double _Complex *restrict out);
/******************************************************************************/
#endif /* end of include guard */
