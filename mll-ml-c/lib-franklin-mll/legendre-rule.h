#ifndef EXTERN_C                                                                                               
#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C extern
#endif
#endif
EXTERN_C int main_old ( int argc, char *argv[] );
EXTERN_C void cdgqf ( int nt, int kind, double alpha, double beta, double t[], double wts[] );
EXTERN_C void cgqf ( int nt, int kind, double alpha, double beta, double a, double b, double t[], double wts[] );
EXTERN_C double class_matrix ( int kind, int m, double alpha, double beta, double aj[], double bj[] );
EXTERN_C void imtqlx ( int n, double d[], double e[], double z[] );
EXTERN_C void parchk ( int kind, int m, double alpha, double beta );
EXTERN_C double r8_abs ( double x );
EXTERN_C double r8_epsilon ( );
EXTERN_C double r8_gamma ( double x );
EXTERN_C double r8_sign ( double x );
EXTERN_C void r8mat_write ( char *output_filename, int m, int n, double table[] );
EXTERN_C void rule_write ( int order, char *filename, double x[], double w[], double r[] );
EXTERN_C void scqf ( int nt, double t[], int mlt[], double wts[], int nwts, int ndx[], double swts[], double st[], int kind, double alpha, double beta, double a, double b );
EXTERN_C void sgqf ( int nt, double aj[], double bj[], double zemu, double t[], double wts[] );
