#include <complex.h>
#include <math.h>
#include <mkl.h>
#include <fftw3.h>
#include "utils.h"
#include "QuadratureRules.h"
#include "toeplitz.h"
#include "file_io.h"
#include "int_trig.h"
#include "v1.h"
/******************************************************************************/
#define MALLOC_ALIGNMENT 64
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
/******************************************************************************/
void print_rte2dvis(struct st_rte2dvis_info &q)
{
	printf("version\t=%d\n",q.version);
	printf("flag\t=%d (1=HOMOGENEOUS 2=INHOMOGENEOUS)\n",q.flag);
	printf("status\t=%d\n",q.status);
	printf("Ns\t=%d\n",q.Ns);
	printf("Nd\t=%d\n",q.Nd);
	printf("Nm\t=%d\n",q.Nm);
	printf("Ng\t=%d\n",q.Ng);
	printf("mem\t=%luKB=%luMB\n",q.mem>>10,q.mem>>20);

	//print_mesh(*q.mesh);
	printf("area\t=%p\n",q.area);

	printf("g\t=%p\n",q.f);
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

void init_rte2dvis(struct st_rte2dvis_info &q, const char *fabse, 
		const int Nd, const int pad, const int flag)
{
	// version, flag
	q.version = 1;
	q.flag = flag; 

	// mesh
	q.mesh=(struct st_mesh_info*)mkl_malloc(
			sizeof(struct st_mesh_info),MALLOC_ALIGNMENT);
	load_mesh(*q.mesh,fabse);

	// Ns
	q.Ns = q.mesh->num_trigs;

	// area
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


	// f, A, B, rhs, sol, work
	q.f    = NULL;
	q.A    = NULL;
	q.B    = NULL;
	q.rhs  = NULL;
	q.sol  = NULL;
	q.work = NULL;

	// mem
	q.mem  = sizeof(struct st_rte2dvis_info)
		+sizeof(struct st_mesh_info)
		+sizeof(double)*q.Ns;

	// status
	q.status = 0;
}

int check_rte2dvis(const struct st_rte2dvis_info &q)
{
	return 0;
}

void destroy_rte2dvis(struct st_rte2dvis_info &q)
{
	release_mesh(*(q.mesh));
	mkl_free(q.mesh);
	mkl_free(q.area); 

	q.mem -= sizeof(struct st_mesh_info)
		+sizeof(double)*q.Ns;
	q.status=-1;
}
/******************************************************************************/
/******************************************************************************/

/*
 * Allocate g,A,B,rhs,sol,work
 */
void alloc_rte2dvis_v1(struct st_rte2dvis_info &q)
{
	assert(q.status==0);

	// f(m)=g^|m|
	// A=2*pi*area(n)*delta(n,np)*delta(m,mp)
	// B is very complicated. See the derivation documentation.
	// rhs[Ns*(2*Nd+1)]
	// sol[Ns*(2*Nd+1)]
	// work[Nm] is the tmp workspace in FFT convolution.
	q.f   =(double*)mkl_malloc(sizeof(double)*(2*q.Nd+1),MALLOC_ALIGNMENT);
	q.A   =(double*)mkl_malloc(sizeof(double)*q.Ns,MALLOC_ALIGNMENT);
	q.B   =(double*)mkl_malloc(sizeof(double)*q.flag*2*q.Ns*q.Ns*q.Nm,MALLOC_ALIGNMENT);
	q.rhs =(double*)mkl_malloc(sizeof(double)*q.Ns*(2*q.Nd+1),MALLOC_ALIGNMENT);
	q.sol =(double*)mkl_malloc(sizeof(double)*q.Ns*(2*q.Nd+1),MALLOC_ALIGNMENT);
	q.work=(double*)mkl_malloc(sizeof(double)*q.Nm,MALLOC_ALIGNMENT); 

	q.mem+=sizeof(double)*(
			2*q.Nd+1
			+q.Ns
			+q.Ns*(2*q.Nd+1)
			+q.Ns*(2*q.Nd+1)
			+q.Nm
			+q.flag*2*q.Ns*q.Ns*q.Nm
			);
	q.status=1;
}

static void fill_solver_g(struct st_rte2dvis_info &q)
{
	// Modify the following line to change the g.
	double g=0.6;
	// HG phase function: f(m)=g^|m|
	int Nd=q.Nd;
	q.f[Nd] = 1.0;
	for (int i = 1; i <= q.Nd; i++) {
		q.f[Nd+i] = q.f[Nd+i-1] * g;
		q.f[Nd-i] = q.f[Nd+i];
	}
	//for (int i = 0; i < 2*q.Nd+1; i++)
		//fprintf(stderr,"q.f[%4d]=%f\n",i-Nd,q.f[i]);
}

static void fill_solver_A(struct st_rte2dvis_info &q)
{
	// A=2*pi*area(n)*delta(n,np)*delta(m,mp)
	// Note that q.area stores the signed areas.
	for (int i = 0; i < q.Ns; i++)
		q.A[i] = 2*M_PI*fabs(q.area[i]);
	//for (int i = 0; i < q.Ns; i++)
		//fprintf(stderr, "q.A[%4d]=%f\n",i,q.A[i]);
}

/**************************************/
#define RULE 6
#define NU 50
#define NV 3

/*
 * Fill in matrix block B(n,np)
 * Homogeneous material, single-point testing, multi-point source
 */
static void fill_solver_B_homo_atomic(struct st_rte2dvis_info &q, 
		const int n, const int np,
		struct st_quadrule *quadrules,
		double *restrict x, double *restrict w,
		double _Complex *restrict f)
{
	//TODO
	//qw_arcsinh(quadrule+1,quadrule+2,p,x,w,work);
}

static void fill_solver_B_homo(struct st_rte2dvis_info &q)
{
	// Prepare quadrature rules.
	using namespace QuadratureRules;
	struct st_quadrule quadrules[3];
	gWandzuraRule.Generate(RULE,quadrules);
	gGaussRule.Generate(NU,quadrules+1);
	gGaussRule.Generate(NV,quadrules+2);

	// Prepare workspace.
	double *x,*w;
	double _Complex *f;
	x=(double*)mkl_malloc(sizeof(double)*MAX(200,NU*NV),MALLOC_ALIGNMENT);
	w=(double*)mkl_malloc(sizeof(double)*MAX(200,NU*NV),MALLOC_ALIGNMENT);
	f=(double _Complex*)mkl_malloc(sizeof(double)*2*MAX(200,NU*NV),MALLOC_ALIGNMENT);

	// Fill in elements.
	for (int n = 0; n < q.Ns; n++)
		for (int np = 0; np < q.Ns; np++)
			fill_solver_B_homo_atomic(q,n,np,quadrules,x,w,f);

	// Release memories.
	mkl_free(x);
	mkl_free(w);
	mkl_free(f);
	gWandzuraRule.ReleaseMemory();
	gGaussRule.ReleaseMemory();
}
#undef RULE
#undef NU
#undef NV
/**************************************/

static void fill_solver_rhs(struct st_rte2dvis_info &q)
{
	// rhs[Ns*(2*Nd+1)]
}


/*
 * Fill up g,A,B,rhs
 */
void fill_rte2dvis_v1(struct st_rte2dvis_info &q)
{
	assert(q.status==1);
	
	fill_solver_g(q);
	fill_solver_A(q);
	fill_solver_B_homo(q);


	fill_solver_rhs(q);

	q.status=2;
}

void solve_rte2dvis_v1(struct st_rte2dvis_info &q)
{
	assert(q.status==2);
	q.status=3;
}

void release_rte2dvis_v1(struct st_rte2dvis_info &q)
{
	assert(q.status>=1); 

	mkl_free(q.f   );
	mkl_free(q.A   );
	mkl_free(q.B   );
	mkl_free(q.rhs );
	mkl_free(q.sol );
	mkl_free(q.work);

	q.mem-=sizeof(double)*(
			2*q.Nd+1
			+q.Ns
			+q.Ns*(2*q.Nd+1)
			+q.Ns*(2*q.Nd+1)
			+q.Nm
			+q.flag*2*q.Ns*q.Ns*q.Nm
			);
	q.status=-2;
}
