#include "c_utils.h"
#include <emmintrin.h>
#include <math.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
/******************************************************************************/
/*
 * based on http://stackoverflow.com/questions/5252375/custom-c-assert-macro
 * works outside debug mode too
 */
#define assrt(cond) \
	do \
{ \
	if (!(cond)) \
	{ \
		fprintf(stderr,"ASSRT FAILED\n");		\
		fprintf(stderr,"file: %s\n",__FILE__);		\
		fprintf(stderr,"line: %d\n",__LINE__);		\
		__builtin_trap();				\
	} \
} while(0)  
/******************************************************************************/ 
/*void array_in(const char* fname, const int m, const int n, double *restrict a,*/
		/*const int lda)*/
/*{*/
	/*assrt(lda>=m); */

	/*FILE *fp = fopen(fname,"r");*/
	/*assrt(fp);*/
	/*fprintf(stderr, "Import array from %s\n",fname);*/

	/*for (int i = 0; i < m; i++)*/
		/*for (int i = 0; i < n; i++)*/
			/*[>fscanf();<]*/
			/*// TODO*/
/*}*/

void array_out(const char* fname, const int m, const int n, double *restrict a,
		const int lda)
{
	assrt(lda>=m);

	FILE *fp;
	if(fname == NULL) {
		fp = stdout;
		fprintf(stderr, "Export array to STDOUT\n");
	}
	else {
		fp = fopen(fname, "w");
		fprintf(stderr, "Export array to %s\n",fname);
	}

	for(int i=0; i < m; i++){
		for(int j=0; j < n; j++)
			fprintf(fp, "%f  ", a[i+j*lda]);
		fprintf(fp, "\n");
	}

	if(fname != NULL){
		int rval = fclose(fp);
		assrt(rval !=  -1);
	}
}

void array_flush(double *v, int size)
{
	for(long int i=0; i < size; i++)
		_mm_clflush(v + i);
}

void solve2x2(double *restrict A, double *restrict rhs, double *restrict x)
{
	double a = A[0];
	double b = A[2];
	double c = A[1];
	double d = A[3];
	double det = a*d-b*c;
	x[0] = (rhs[0]*d-rhs[1]*b)/det;
	x[1] = (-rhs[0]*c+rhs[1]*a)/det;

}

void verify_dir(const char *dir)
{
	struct stat sb;
	int rval = stat(dir, &sb);
	if((rval == -1) && (errno == ENOENT)){
		mkdir(dir, 0700);
		stat(dir, &sb);
	}
	assrt(S_ISDIR(sb.st_mode));
}

static int ___linkstd_state___ = 0; 
static FILE *___stdout_backup___;

void link_stdout(const char *fname)
{
	assrt(___linkstd_state___==0);
	___linkstd_state___ = 1; 

	FILE *fp = fopen(fname,"w");
	assrt(fp);
	fflush(stdout);	// clear stdout buffer
	___stdout_backup___ = stdout;
	stdout = fp;

	fprintf(stderr, "STDOUT => %s\n",fname);
} 

void unlink_stdout()
{
	assrt(___linkstd_state___==1);
	___linkstd_state___ = 0; 
	fclose(stdout);
	stdout = ___stdout_backup___;
}

void init_vector(int n, double* v, int incx)
{
	assrt(incx>0);
	for (int i = 0; i < n; i++)
		v[i*incx] = 10.0 * rand()/RAND_MAX;
}

void init_matrix(int m, int n, double* a, int lda)
{
	assrt(lda>=m);
	for (int j = 0; j < n; j++)
		for (int i = 0; i < m; i++)
			a[i+j*lda] = 10.0 * rand()/RAND_MAX;
}

void print_vector(const char* desc, const int n, double* v, int incx)
{
	assrt(incx>0);
	printf("%s = \n",desc);
	for (int i = 0; i < n; i++)
		printf(" %6.2f",v[i*incx]);
	printf("\n");
}

void print_matrix(const char* desc, const int m, const int n, double *a, int lda)
{
	assrt(lda>=m);
	printf("%s = \n",desc);
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++)
			printf(" %6.2f",a[i+j*lda]);
		printf("\n");
	}
}

double dabs_max(int pos[], const int m, const int n, const double* a, 
		const int lda)
{
	pos[0] = 0;
	pos[1] = 0;
	for (int j = 0; j < n; j++)
		for (int i = 0; i < m; i++)
			if (  fabs(a[i+j*lda])
					>fabs(a[pos[0]+pos[1]*lda])  ) {
				pos[0] = i;
				pos[1] = j;
			}
	return a[pos[0]+pos[1]*lda];
}

double dabs_max_omp(int *pos, const int m, const int n, const double* a, 
		const int lda, const int nthreads)
{
	pos[0] = 0;
	pos[1] = 0;
	#pragma omp parallel				\
	num_threads(nthreads)				\
	default(none)					\
	shared(pos,m,n,a,lda)
	{
		int p[2] = {0,0};
		#pragma omp for
		for (int j = 0; j < n; j++)
			for (int i = 0; i < m; i++)
			if (  fabs(a[i+j*lda])
					>fabs(a[p[0]+p[1]*lda])  ) {
				p[0] = i;
				p[1] = j;
			}
		#pragma omp critical
			if (  fabs(a[p[0]+p[1]*lda])
					>fabs(a[pos[0]+pos[1]*lda])  ) {
				pos[0] = p[0];
				pos[1] = p[1];
			}
	}
	return a[pos[0]+pos[1]*lda];
}
