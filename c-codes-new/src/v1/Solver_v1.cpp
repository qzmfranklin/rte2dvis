#include <complex.h>
#include <math.h>
#include <mkl.h>
#include <fftw3.h>
#include "utils.h"
#include "QuadratureRules.h"
#include "toeplitz.h"
#include "file_io.h"
#include "Solver_v1.h"
/******************************************************************************/
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
/******************************************************************************/ 
Solver_v1::Solver_v1(const char* fbase, const int ND, const int PAD)
{
	fprintf(stderr,"Solver_v1::Solver_v1()\n");

	// version, flag
	version=1;
	flag=1;

	// Nd, Nm
	if (PAD) {
		Nm = 2*ND+1+ PAD - (2*ND+1)%PAD;
		Nd = (Nm-1)/2;
	} else {
		Nd = ND;
		Nm = 2*ND+1;
	}
	status=1;

	// Ns, Ng
	Fill_mesh(fbase); 

	assert(status==2);
	create_fftw_plans(2*Nm,plans);
	Fill_area();
	Fill_cntr();
	Fill_f();
	Fill_A();
	Fill_B();
	Fill_rhs();

	work=(double _Complex*)mkl_malloc(sizeof(double)*2*2*Nm,64);

	status=3;
}

void Solver_v1::Check()
{
	fprintf(stderr,"Solver_v1::Check()\n");
	assert(plans);
	assert(mesh);
	assert(area);
	assert(cntr);
	assert(f);
	assert(A);
	assert(B);
	assert(rhs);
	assert(work);
}

void Solver_v1::Debug()
{
}

void Solver_v1::Solve(double _Complex *restrict sol)
{
}

void Solver_v1::mul(const double *restrict in, double *restrict out)
{
}

void Solver_v1::Print()
{
	printf("version\t=%d\n",version);
	printf("flag\t=%d (1=HOMOGENEOUS 2=INHOMOGENEOUS)\n",flag);
	printf("status\t=%d\n",status);
	printf("Ns\t=%d\n",Ns);
	printf("Nd\t=%d\n",Nd);
	printf("Nm\t=%d\n",Nm);
	printf("Ng\t=%d\n",Ng);

	//print_mesh(*mesh);
	printf("area\t=%p\n",area);
	printf("cntr\t=%p\n",cntr);

	printf("g\t=%p\n",f);
	printf("A\t=%p\n",A);
	printf("B\t=%p\n",B);
	printf("rhs\t=%p\n",rhs);
	printf("work\t=%p\n",work);

	printf("\n");
}

void Solver_v1::Fill_mesh(const char* fbase)
{
	assert(status==1);
	fprintf(stderr,"Solver_v1::Fill_mesh()\n");
	mesh=(struct st_mesh_info*)mkl_malloc(sizeof(struct st_mesh_info),64);
	load_mesh(*mesh,fbase);
	Ns = mesh->num_trigs;
	Ng = Ns * (2*Nd+1);
	status=2;
}

// Det[a1-a2,b1-b2]
static double det(const double *a1, const double *a2, 
		const double *b1, const double *b2) {
	return (a1[0]-a2[0])*(b1[1]-b2[1]) - (a1[1]-a2[1])*(b1[0]-b2[0]);
}
void Solver_v1::Fill_area()
{
	assert(status==2);
	fprintf(stderr,"Solver_v1::Fill_area()\n");
	area=(double*)mkl_malloc(sizeof(double)*Ns,64);
	double *p=mesh->trigs;
	for (int i = 0; i < Ns; i++)
		// area[i] = 0.5*Det[p0-p1,p0-p2]
		area[i] = 0.5*det(p+6*i,p+6*i+2,p+6*i,p+6*i+4);
}
void Solver_v1::Fill_cntr()
{
	assert(status==2);
	fprintf(stderr,"Solver_v1::Fill_cntr()\n");
	cntr=(double*)mkl_malloc(sizeof(double)*2*Ns,64);
	double xx = 1.0/3.0;
	for (int i = 0; i < Ns; i++) {
		double *pp=mesh->trigs+6*i;
		cntr[2*i  ] = xx * (pp[0]+pp[2]+pp[4]);
		cntr[2*i+1] = xx * (pp[1]+pp[3]+pp[5]);
	}
}
void Solver_v1::Fill_f()
{
	// HG phase function: f(m)=g^|m|
	const double g=0.6;

	assert(status==2);
	fprintf(stderr,"Solver_v1::Fill_f()\n");
	f=(double*)mkl_malloc(sizeof(double)*(2*Nd+1),64);
	f[Nd] = 1.0;
	for (int i = 1; i <= Nd; i++) {
		f[Nd+i] = f[Nd+i-1] * g;
		f[Nd-i] = f[Nd+i];
	}
}
void Solver_v1::Fill_A()
{
	assert(status==2);
	fprintf(stderr,"Solver_v1::Fill_A()\n");
	A=(double*)mkl_malloc(sizeof(double)*Ns,64);
	for (int i = 0; i < Ns; i++)
		A[i] = 2*M_PI*fabs(area[i]);
}

static double distance(double p1[2], double p2[2])
{
	double a,b;
	a=p1[0]-p2[0];
	b=p1[1]-p2[1];
	return sqrt(a*a+b*b);
}
static void scale(const double a, const int n, double _Complex *v)
{
	for (int i = 0; i < n; i++)
		v[i] *= a;
}
void Solver_v1::Fill_B()
{
	assert(status==2);
	fprintf(stderr,"Solver_v1::Fill_B()\n");
	B=(double _Complex*)mkl_malloc(sizeof(double)*2*Ns*Ns*2*Nm,64);
}
void Solver_v1::Fill_rhs()
{
	assert(status==2);
	fprintf(stderr,"Solver_v1::Fill_rhs()\n");
	rhs=(double _Complex*)mkl_malloc(sizeof(double)*2*Ng,64);
}
void Solver_v1::ReleaseMemory()
{
	destroy_fftw_plans(plans);
	release_mesh(*mesh);

	mkl_free(mesh);
	mkl_free(area);
	mkl_free(cntr);
	mkl_free(f);
	mkl_free(A);
	mkl_free(B);
	mkl_free(rhs);
	mkl_free(work);
}
