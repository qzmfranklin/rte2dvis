#ifndef _TRANS_H_QZM_
#define _TRANS_H_QZM_ 
/******************************************************************************/
/*
 * All matrices to be transposed with the following functions are assumed
 * to be COLUMN-MAJOR and SQUARE-SIZE. There is NO internal check for these
 * two assumption.
 */ 
#ifdef __cplusplus
	extern "C" {
#endif
void trans_easy(const int n,double *restrict a); 
void trans_mkl(const int n,double *restrict a); 
void trans_seq(const int n,double * restrict a); 
void trans_omp(const int n,double * restrict a,const int nthreads); 
#ifdef __cplusplus
	}
#endif
/******************************************************************************/ 
#endif /* End of protection macro _TRANS_H_QZM_ */
