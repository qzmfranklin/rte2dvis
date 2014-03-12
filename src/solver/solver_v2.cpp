#include <stdio.h>
#include <omp.h>
#include <mkl.h>
#include <complex.h>
#include <math.h>
#include <assert.h>
#include "solver_v2.h"
#include "mesh.h" 
#include "quadrules.h"
/**************************************/
#include "utils.h"
/******************************************************************************/
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
/******************************************************************************/
static void init(struct st_solver_v2 *s, const int *ipar, const double *dpar)
{
	assert(s->status==0);

	for (int i = 0; i < 128; i++) {
		s->ipar[i] = ipar[i];
		s->dpar[i] = dpar[i];
	}

	s->M      = 1;
	s->Nd     = s->ipar[1];
	s->pad    = s->ipar[2];
	s->num_threads = s->ipar[7];

	struct st_mesh *q=s->mesh;

	s->Ns     = q->num_trigs;
	s->Nv     = q->num_verts;
	s->Ne     = q->num_edges;
	s->Nt     = s->Nv + (s->M-1)*s->Ne + (s->M-1)*(s->M-2)/2*s->Ns;

	{
	int tmp=2*s->Nd+1;
	int res=tmp%s->pad;
	if (res==0) res=s->pad;
	s->Nm     = tmp + s->pad - res;
	}

	s->Ng     = s->Nt * (2*Nd+1);

	s->g_factor = dpar[0]; 

	s->status = 1;
}
static void alloc(struct st_solver_v2 *s)
{
	assert(s->status==1);

	//s->E    =(double*)mkl_malloc(sizeof(double)*(s->Ns),64);
	s->K    =(double*)mkl_malloc(sizeof(double)*(s->Ns*s->Ns*s->Nm*2),64);
	//assert(s->E);
	assert(s->K);

	// g table [2*Nd+1]
	s->g=(double*)mkl_malloc(sizeof(double)*(2*s->Nd+1),64);
	assert(s->g);
	s->g[s->Nd] = 1.0;
	for (int i = s->Nd+1; i < 2*s->Nd+1; i++) {
		s->g[i] = s->g[i-1] * s->g_factor;
		s->g[2*s->Nd-i] = s->g[i];
	}

	// for matrix-vector multiplication
	s->work[0]=(double _Complex*)mkl_malloc(sizeof(double _Complex)*(s->Ns*s->Nm*2),64);
	s->work[1]=(double _Complex*)mkl_malloc(sizeof(double _Complex)*(s->Ns*s->Nm*2),64);
	assert(s->work[0]);
	assert(s->work[1]);
	const int Nm_times_2 = 2*s->Nm;
	fftw_init_threads();
	fftw_plan_with_nthreads(s->num_threads);
	s->plans[0] = fftw_plan_many_dft(1,&Nm_times_2,s->Ns,
			(fftw_complex*)(s->work[0]),NULL,1,Nm_times_2,
			(fftw_complex*)(s->work[0]),NULL,1,Nm_times_2,
			FFTW_FORWARD ,FFTW_MEASURE|FFTW_PATIENT);
	s->plans[1] = fftw_plan_many_dft(1,&Nm_times_2,s->Ns,
			(fftw_complex*)(s->work[1]),NULL,1,Nm_times_2,
			(fftw_complex*)(s->work[1]),NULL,1,Nm_times_2,
			FFTW_BACKWARD,FFTW_MEASURE|FFTW_PATIENT);

	s->status=2;
}
static void fillE(struct st_solver_v2 *s)
{
	assert(s->status==2);

	s->status=3;
}
/******************************************************************************/
struct st_solver_v2 *sv2_create_solver(struct st_mesh *q, const int *ipar, const double *dpar)
{
	//fprintf(stderr,"sv2_create_solver()\n");

	assert(q);
	assert(q->status==3); // st_mesh is already loaded

	struct st_solver_v2 *s = (struct st_solver_v2*)
		mkl_malloc(sizeof(struct st_solver_v2),64);
	assert(s);
	s->status = 0; 
	s->mesh   = q;

	//init(s,ipar,dpar);
	//alloc(s);
	//fillE(s);
	//fillK(s);

	return s;
}

void sv2_destroy_solver(struct st_solver_v2 *s)
{
	//assert(s->status>=2); // check alloc'd

	//mkl_free(s->E);
	//mkl_free(s->K);

	//mkl_free(s->g);

	//mkl_free(s->work[0]);
	//mkl_free(s->work[1]);
	//fftw_destroy_plan(s->plans[0]);
	//fftw_destroy_plan(s->plans[1]);
	
	//mkl_free(s);
}
