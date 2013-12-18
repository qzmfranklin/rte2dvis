#ifndef _UTILS_H_
#define _UTILS_H_
/******************************************************************************/
#include <complex.h>
#include "Table.h"
#include "Random.h"
#include "StatVector.h"
#include "TimeStamp.h"
/******************************************************************************/
void cscale(const double _Complex scale, 
		const int n, double _Complex *restrict v);
void cdot(const int n, double _Complex *a,  
		double _Complex *b, double _Complex *dest);
void array_abs(const int n, double *v); 
double array_max(const  int n, double *v); 
void array_diff(const int n, double *restrict v, double *restrict w); 
void array_copy(const int n, double *restrict v, double *restrict w); 
void array_out(const char* fname, const int m, const int n, double *restrict a,
		int lda=0);
void array_in(const char* fname, const int n, double *v);
void array_flush(const int n, double *v); 
void solve2x2(double *restrict A, double *restrict rhs, double *restrict x); 
void verify_dir(const char *dir); 
void link_stdout(const char *fname); 
void unlink_stdout(); 
void box_file(const char* fname, const char*mesg); 
void print_proc_status(const char* mesg); 
void init_vector(const int n, double* v, int incx=1);
void init_matrix(const int m, const int n, double* a, int lda=1); 
void print_vector(const char* desc, const int n, double* v, int incx=1);
void print_matrix(const char* desc, const int m, const int n, double *a, 
		int lda=0);
void print_cvector(const char* desc, const int n, 
		double _Complex *restrict v, int incx=1);
void print_cmatrix(const char* desc, const int m, const int n, 
		double _Complex *restrict a, int lda=0);
/******************************************************************************/
#endif
