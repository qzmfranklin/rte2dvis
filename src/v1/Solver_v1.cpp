#include <complex.h>
#include <omp.h>
#include <math.h>
#include <mkl.h>
#include <fftw3.h>
#include "utils.h"
#include "QuadratureRules.h"
#include "file_io.h"
#include "Solver_v1.h"
/******************************************************************************/
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
/******************************************************************************/ 
// maximal number of threads
static const int NUM_THREAD_MAX=8;
// mu_t and mu_s
static const double mut=0.4, mus=0.3;
// HG phase function: f(m)=g^|m|
static const double g=0.6; 
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
	work=(double _Complex*)mkl_malloc(sizeof(double)*2*2*Nm*NUM_THREAD_MAX,64);
	create_fftw_plans(2*Nm,plans);
	Fill_area();
	Fill_cntr();
	Fill_f();
	Fill_A();
	Fill_B();
	Fill_rhs();


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
	assert(work);
	assert(rhs);
}

void Solver_v1::Debug()
{
	const int N=MIN(Ns,30);
	assert(status>=3);
	for (int i = 0; i < N; i++)
		printf("pt[%3d]=(%f,%f)(%f,%f)(%f,%f)\n",
				i+1,
				p[6*i+0],p[6*i+1],
				p[6*i+2],p[6*i+3],
				p[6*i+4],p[6*i+5]);
	for (int i = 0; i < N; i++)
		printf("area[%3d]=%f\n",i+1,area[i]);
	for (int i = 0; i < N; i++)
		printf("cntr[%3d]=(%f,%f)\n",
				i+1,
				cntr[2*i+0],
				cntr[2*i+1]); 

	//mul(rhs,rhs);

	for (int n = 0; n < Ns; n++) {
		for (int m = 0; m < 2*Nd+1; m++) {
			printf("rhs[%4d,%4d]=%20.10f + %20.10fI\n",
					n+1,m-Nd,
					creal(rhs[m+n*(2*Nd+1)]),
					cimag(rhs[m+n*(2*Nd+1)]));
		}
	}
}

int Solver_v1::Solve(double _Complex *restrict sol)
{
	fprintf(stderr,"Solver_v1::Solve\n");
	assert(status>=3);

#define MAX_ITR_NUM 6000
#define PAR_SIZE 128
	 /*
	  * Prepare the inputs.
	  */
	double _Complex *expected_sol=
		(double _Complex*)mkl_malloc(sizeof(double)*2*Ng,64);
	for (int i = 0; i < Ng; i++)
		expected_sol[i] = 1.0/Ns + 2.0/Ns*I;
	mul(expected_sol,rhs);
	print_cvector("rhs",10,rhs);
	
	/*
	 * Allocate storage for the iterative solver.
	 */
	double _Complex *solution=
		(double _Complex*)mkl_malloc(sizeof(double)*2*Ng,64);
	for (int i = 0; i < Ng; i++)
		solution[i] = 0.05;
	//init_vector(2*Ng,(double*)solution); 
	print_cvector("initial guess for the solution",10,solution);

	int ipar[PAR_SIZE],RCI_request,ivar,itercount;
	double dpar[PAR_SIZE],*tmp;
	ivar = 2*Ng;
	{ size_t ltmp=(2*MAX_ITR_NUM+1)*ivar+MAX_ITR_NUM*(MAX_ITR_NUM+9)/2+1;
	tmp=(double*)mkl_malloc(sizeof(double)*ltmp,64); }

	/*
	 * Initialize the solver.
	 */
	dfgmres_init(&ivar,(double*)solution,(double*)rhs,&RCI_request,ipar,dpar,tmp);
	if (RCI_request) { MKL_Free_Buffers(); return RCI_request; }

	/*
	 * Set parameters.
	 * LOGICAL parameters:
	 *	 ipar[4]:  max num_iteration, default min(n,150)
	 *	 ipar[8]:  do automatic residual stopping test
	 *	 ipar[9]:  do residual stopping test: dpar[5]<dpar[4]
	 *	 ipar[11]: automatic check norm of next generated vector
	 *	 ipar[14]: max non-restart num_iteration, default min(n,150)
	 * DOUBLE parameters:
	 *	 dpar[0]:  set the relative tolerance to 1.0E-5
	 */
	ipar[4]		= MAX_ITR_NUM;
	ipar[8]		= 1;
	ipar[9]		= 0;
	ipar[11]	= 1;
	ipar[14]	= MAX_ITR_NUM;
	dpar[0]		= 1e-5;

	/*
	 * Check consistency of newly assigned parameters.
	 */
	dfgmres_check(&ivar,(double*)solution,(double*)rhs,&RCI_request,ipar,dpar,tmp);
	if (RCI_request) { MKL_Free_Buffers(); return RCI_request; }

	/*
	 * Solve!
	 * RCI_request:
	 *	 0	completed
	 *	 1	go on iterating
	 */
	dfgmres(&ivar,(double*)solution,(double*)rhs,&RCI_request,ipar,dpar,tmp);
	while (RCI_request==1) {
		mul_omp((double _Complex*)(tmp+ipar[21]-1),
				(double _Complex*)(tmp+ipar[22]-1),
				8);
		dfgmres(&ivar,(double*)solution,(double*)rhs,&RCI_request,ipar,dpar,tmp); 
	}

	/*
	 * Extract solution, print, clear buffers.
	 */
	dfgmres_get(&ivar,(double*)solution,(double*)rhs,
			&RCI_request,ipar,dpar,tmp,&itercount);
	printf("Solver finished after %d iterations.\n",itercount);
	printf("Exit code (RCI_request) = %d\n",RCI_request);
	print_cvector("expected_sol",10,expected_sol);
	print_cvector("solution",10,solution);
	MKL_Free_Buffers(); 

#undef PAR_SIZE
#undef N
	status=4;
	mkl_free(expected_sol);
	mkl_free(solution);
	mkl_free(tmp);

	return RCI_request;
}

void Solver_v1::mul(const double _Complex *in, double _Complex *out)
{
	assert(status>=2);
	// mu_t and mu_s

	//fprintf(stderr,"Solver_v1::mul\n");

	// Identity term A
	for (int n = 0; n < Ns; n++)
		for (int m = 0; m < 2*Nd+1; m++)
			out[m+n*(2*Nd+1)] = in[m+n*(2*Nd+1)] * A[n];

	// Interaction term B
	for (int n = 0; n < Ns; n++) {
		fftw_complex *wk=work;
		for (int np = 0; np < Ns; np++) {
			// copy into workspace
			for (int i = 0; i < 2*Nd+1; i++)
				wk[i] = (mut-mus*f[i])*in[i+np*(2*Nd+1)];
			// padding zeros
			memset(wk+2*Nd+1,0,sizeof(double)*(2*Nm-2*Nd-1));
			// transform wkspace
			fftw_execute_dft(plans[IFWD],wk,wk);
			// convolution in frequency domain
			for (int i = 0; i < 2*Nm; i++)
				wk[i]*= B[i+(n+np*Ns)*2*Nm];
			// FFT back, normalization is incorporated in B
			fftw_execute_dft(plans[IBWD],wk,wk);
			// copy out
			for (int i = 0; i < 2*Nd+1; i++)
				out[i+n*(2*Nd+1)] += wk[i];
		}
	}
}

void Solver_v1::mul_omp(const double _Complex *in, double _Complex *out,
		const int nthreads)
{
	assert(nthreads<=NUM_THREAD_MAX);
	assert(status>=2);
	//fprintf(stderr,"Solver_v1::mul_omp\n");

	// Identity term A
	for (int n = 0; n < Ns; n++)
		for (int m = 0; m < 2*Nd+1; m++)
			out[m+n*(2*Nd+1)] = in[m+n*(2*Nd+1)] * A[n];

	// Interaction term B
	#pragma omp parallel for 		\
	num_threads(nthreads)			\
	default(shared)
	for (int n = 0; n < Ns; n++) {
		fftw_complex *wk=work+2*Nm*omp_get_thread_num();
		for (int np = 0; np < Ns; np++) {
			// copy into workspace
			for (int i = 0; i < 2*Nd+1; i++)
				wk[i] = (mut-mus*f[i])*in[i+np*(2*Nd+1)];
			// padding zeros
			memset(wk+2*Nd+1,0,sizeof(double)*(2*Nm-2*Nd-1));
			// transform wkspace
			fftw_execute_dft(plans[IFWD],wk,wk);
			// convolution in frequency domain
			for (int i = 0; i < 2*Nm; i++)
				wk[i]*= B[i+(n+np*Ns)*2*Nm];
			// FFT back, normalization is incorporated in B
			fftw_execute_dft(plans[IBWD],wk,wk);
			// copy out
			for (int i = 0; i < 2*Nd+1; i++)
				out[i+n*(2*Nd+1)] += wk[i];
		}
	}
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
	p  = mesh->trigs;
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
		double *pp=p+6*i;
		cntr[2*i  ] = xx * (pp[0]+pp[2]+pp[4]);
		cntr[2*i+1] = xx * (pp[1]+pp[3]+pp[5]);
	}
}
void Solver_v1::Fill_f()
{
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
	// quadrature rules, critical distance
	const int RULE=3, NU=12, NV=3, MAX_NUM=1500;
	const double dmin=0.35;
	__declspec(align(64)) double _Complex E[MAX_NUM], wER[MAX_NUM];

	assert(status==2);
	fprintf(stderr,"Solver_v1::Fill_B()\n");
	B=(double _Complex*)mkl_malloc(sizeof(double)*2*Ns*Ns*2*Nm,64); 

	struct st_quadrule q[4];
	using namespace QuadratureRules;
	WandzuraRule  g0(RULE);
	ArcSinhMethod g1;
	GaussRule     g2(NU);
	GaussRule     g3(NV);

	g2.Generate(q+2,0.0,1.0);
	g3.Generate(q+3,0.0,1.0);
	g1.Init(q+2,q+3);

	int arcsinh_count=0;
	for (int np = 0; np < Ns; np++) {
		g0.Generate(q,p+6*np);
		for (int n = 0; n < Ns; n++) {
			int M;
			if ( distance(cntr+2*n,cntr+2*np)>dmin ) {
				// Non-singular
				M = g0.Order();
				for (int i = 0; i < M; i++) {
					double dx,dy,invr;
					dx     = cntr[2*n  ] - q->x[2*i  ];
					dy     = cntr[2*n+1] - q->x[2*i+1];
					invr   = 1.0/sqrt(dx*dx+dy*dy);
					E[i]   = invr*(dx-dy*I);
					wER[i] = invr*q->w[i];
				}
			} else {
				// Singular, near-singular
				g1.Generate(q+1,p+6*np,cntr+2*n);
				M = g1.Order();
				for (int i = 0; i < M; i++) {
					double dx,dy,invr;
					dx    = cntr[2*n  ] - q[1].x[2*i  ];
					dy    = cntr[2*n+1] - q[1].x[2*i+1];
					invr  = 1.0/sqrt(dx*dx+dy*dy);
					E[i]  = invr*(dx-dy*I);
					wER[i]= q[1].w[i];
				}
				arcsinh_count++;
			}
			// Fill one B block
			double _Complex *z=B+(n+np*Ns)*2*Nm;
			memset(z,0,sizeof(double)*2*2*Nm);
			for (int i = 0; i < M; i++)
				z[0] += wER[i];
			for (int dm = 1; dm <= 2*Nd; dm++) {
				for (int i = 0; i < M; i++)
					wER[i] *= E[i];
				for (int i = 0; i < M; i++)
					z[dm]  += wER[i];
			}
			for (int dm = 2*Nd+1; dm <= 2*(Nm-Nd)-1; dm++)
				z[dm] = 0.0;
			for (int dm = 2*(Nm-Nd); dm <= 2*Nm-1; dm++)
				z[dm] = conj(z[2*Nm-dm]);
			// divide by 2*Nm, multiply by S(n), in-place FFT
			fftw_execute_dft(plans[IFWD],z,z);
			scale(fabs(area[n]/(2*Nm)),2*Nm,z);
		}
	}
	printf("arcsinh_count/(Ns*Ns) = %f\n",(double)arcsinh_count/(Ns*Ns));

	//for (int np = 0; np < Ns; np++)
		//for (int n = 0; n < Ns; n++)
			//for (int dm = 0; dm < 2*Nm; dm++)
				//printf("B[%3d,%3d,%3d]=%20.10f + %20.10fI\n",
						//n+1,np+1,dm,
						//creal(B[dm+(n+np*Ns)*2*Nm]),
						//cimag(B[dm+(n+np*Ns)*2*Nm]));

}
void Solver_v1::Fill_rhs()
{
	assert(status==2);
	fprintf(stderr,"Solver_v1::Fill_rhs()\n");
	rhs=(double _Complex*)mkl_malloc(sizeof(double)*2*Ng,64);
	//TODO
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
