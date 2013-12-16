#ifndef _UTILS_H_
#define _UTILS_H_
/******************************************************************************/
#include "Table.h"
#include "Random.h"
#include "StatVector.h"
#include "TimeStamp.h"
/******************************************************************************/
void array_abs(double *v, int n); 
double array_max(double *v, int n); 
void array_diff(const int n, double *restrict v, double *restrict w); 
void array_copy(const int n, double *restrict v, double *restrict w); 
void array_out(const char* fname, const int m, const int n, double *restrict a,
		int lda=0);
void array_in(const char* fname, const int n, double *v);
void array_flush(double *v, int size); 
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
/******************************************************************************/
#endif
