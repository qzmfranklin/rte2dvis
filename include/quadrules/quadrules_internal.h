#ifndef _QUADRULES_INTERNAL_H_
#define _QUADRULES_INTERNAL_H_
/******************************************************************************/
/*
 * This header file is NOT in the user space.
 */
/******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
void file_name_inc ( char *file_name );
int i4_max ( int i1, int i2 );
int i4_min ( int i1, int i2 );
int i4_modp ( int i, int j );
int i4_wrap ( int ival, int ilo, int ihi );
double r8_huge ( void );
int r8_nint ( double x );
void reference_to_physical_t3 ( double t[], int n, double ref[], double phy[] );
int s_len_trim ( char *s );
void timestamp ( );
double triangle_area ( double t[2*3] );
void triangle_points_plot ( char *file_name, double node_xy[], int node_show, 
  int point_num, double point_xy[], int point_show );
#ifdef __cplusplus
}
#endif
/******************************************************************************/
#endif /* end of include guard */
