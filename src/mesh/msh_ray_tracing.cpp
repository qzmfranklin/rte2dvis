#include "msh_ray_tracing.h"
/******************************************************************************/
struct st_meshtree *mshtree_new(const int n, const double *restrict trigs)
{
}

void mshtree_destroy(struct st_meshtree *t);
double mshtree_line_seg(struct st_mesh *restrict t, double ln[2]);
double mshtree_ray_seg(struct st_mesh *restrict t, double ln[2], double p0[2]);
