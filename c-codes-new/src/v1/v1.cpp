#include <complex.h>
#include <mkl.h>
#include <fftw3.h>
#include "utils.h"
#include "QuadratureRules.h"
#include "toeplitz.h"
#include "file_io.h"
/******************************************************************************/
#define MALLOC_ALIGNMENT 64
/******************************************************************************/
void print_rte2dvis(struct st_rte2dvis_info &q)
{
	printf("version\t=%d\n",q.version);
	printf("flag\t=%d (1=HOMOGENEOUS 2=INHOMOGENEOUS)\n",q.flag);
	printf("status\t=%d\n",q.status);
	printf("Ns\t=%d\t\n",q.Ns);
	printf("Nd\t=%d\t\n",q.Nd);
	printf("Nm\t=%d\t\n",q.Nm);
	printf("Ng\t=%d\t\n",q.Ng);

	//print_mesh(*q.mesh);
	printf("area\t=%p\n",q.area);

	printf("g\t=%p\n",q.g);
	printf("A\t=%p\n",q.A);
	printf("B\t=%p\n",q.B);
	printf("rhs\t=%p\n",q.rhs);
	printf("sol\t=%p\n",q.sol);
	printf("work\t=%p\n",q.work);
}

/*
 * return	Det[a1-a2,b1-b2]
 */
static double det(const double *a1, const double *a2, 
		const double *b1, const double *b2)
{
	return (a1[0]-a2[0])*(b1[1]-b2[1]) - (a1[1]-a2[1])*(b1[0]-b2[0]);
}

void init_rte2dvis(struct st_rte2dvis_info &q, const char *fbase, 
		const int Nd, const int pad, const int flag)
{
	// version, flag
	q.version = 1;
	q.flag = flag; 

	// mesh
	q.mesh=(struct st_mesh_info*)mkl_malloc(
			sizeof(struct st_mesh_info),MALLOC_ALIGNMENT);
	load_mesh(*q.mesh,fbase);

	// Ns, area
	q.Ns = q.mesh->num_trigs;
	q.area=(double*)mkl_malloc(
			q.Ns*sizeof(double),MALLOC_ALIGNMENT);
	double *p = q.mesh->trigs;
	for (int i = 0; i < q.Ns; i++)
		// area[i] = 0.5*Det[p0-p1,p0-p2]
		q.area[i] = 0.5*det(p+6*i,p+6*i+2,p+6*i,p+6*i+4);

	// Nd, Nm, Ng
	q.Nm = 2*Nd+1+ pad - (2*Nd+1)%pad;
	q.Nd = (q.Nm-1)/2;
	q.Ng = q.Ns * (2*q.Nd+1);


	// A, B, rhs, sol, work
	q.A    = NULL;
	q.B    = NULL;
	q.rhs  = NULL;
	q.sol  = NULL;
	q.work = NULL;


	// status
	q.status = 0;
}

int check_rte2dvis(const struct st_rte2dvis_info &q)
{
	return 0;
}

void destroy_rte2dvis(const struct st_rte2dvis_info &q)
{
	release_mesh(*(q.mesh));
	mkl_free(q.mesh);
	mkl_free(q.area);
	mkl_free(q.g);
}
/******************************************************************************/
void fill_rte2dvis_v1(struct st_rte2dvis_info &solver)
{
}

void solve_rte2dvis_v1(struct st_rte2dvis_info &solver)
{
}

void release_rte2dvis_v1(struct st_rte2dvis_info &solver)
{
}
