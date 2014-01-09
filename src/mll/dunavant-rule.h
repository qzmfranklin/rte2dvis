#ifndef EXTERN_C                                                                                               
#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C extern
#endif
#endif
EXTERN_C int dunavant_degree ( int rule );
EXTERN_C int dunavant_order_num ( int rule );
EXTERN_C void dunavant_rule ( int rule, int order_num, double xy[], double w[] );
EXTERN_C int dunavant_rule_num ( );
EXTERN_C int *dunavant_suborder ( int rule, int suborder_num );
EXTERN_C int dunavant_suborder_num ( int rule );
EXTERN_C void dunavant_subrule ( int rule, int suborder_num, double suborder_xyz[], double suborder_w[] );
EXTERN_C void dunavant_subrule_01 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
EXTERN_C void dunavant_subrule_02 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
EXTERN_C void dunavant_subrule_03 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
EXTERN_C void dunavant_subrule_04 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
EXTERN_C void dunavant_subrule_05 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
EXTERN_C void dunavant_subrule_06 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
EXTERN_C void dunavant_subrule_07 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
EXTERN_C void dunavant_subrule_08 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
EXTERN_C void dunavant_subrule_09 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
EXTERN_C void dunavant_subrule_10 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
EXTERN_C void dunavant_subrule_11 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
EXTERN_C void dunavant_subrule_12 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
EXTERN_C void dunavant_subrule_13 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
EXTERN_C void dunavant_subrule_14 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
EXTERN_C void dunavant_subrule_15 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
EXTERN_C void dunavant_subrule_16 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
EXTERN_C void dunavant_subrule_17 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
EXTERN_C void dunavant_subrule_18 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
EXTERN_C void dunavant_subrule_19 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
EXTERN_C void dunavant_subrule_20 ( int suborder_num, double suborder_xyz[], double suborder_w[] );
EXTERN_C void file_name_inc ( char *file_name );
EXTERN_C int i4_max ( int i1, int i2 );
EXTERN_C int i4_min ( int i1, int i2 );
EXTERN_C int i4_modp ( int i, int j );
EXTERN_C int i4_wrap ( int ival, int ilo, int ihi );
EXTERN_C double r8_huge ( );
EXTERN_C int r8_nint ( double x );
EXTERN_C void reference_to_physical_t3 ( double t[], int n, double ref[], double phy[] );
EXTERN_C int s_len_trim ( char *s );
EXTERN_C void timestamp ( );
EXTERN_C double triangle_area ( double t[2*3] );
