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
void array_diff(double *restrict v, double *restrict w, int n); 
void array_copy(double *restrict v, double *restrict w, int n); 
void array_out(const char* fname, const int m, const int n, double *restrict a,
		const int lda); 
void array_in(double *v, int size,  const char* fname); 
void array_flush(double *v, int size); 
void solve2x2(double *restrict A, double *restrict rhs, double *restrict x); 
void verify_dir(const char *dir); 
void link_cout(const char *fname); 
void unlink_cout(); 
void link_stdout(const char *fname); 
void unlink_stdout(); 
void box_file(const char* fname, const char*mesg); 
void print_proc_status(const char* mesg); 
void init_vector(int n, double* v, int incx);
void init_matrix(int m, int n, double* a, int lda); 
void print_vector(const char* desc, const int n, double* v, int incx); 
void print_matrix(const char* desc, const int m, const int n, double *a, int lda); 
/******************************************************************************/
#endif
