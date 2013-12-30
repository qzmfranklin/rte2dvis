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
	printf("mem\t=%luKB\n",q.mem>>10);

	//print_mesh(*q.mesh);
	printf("area\t=%p\n",q.area);
	printf("cntr\t=%p\n",q.cntr);

	printf("g\t=%p\n",q.f);
	printf("A\t=%p\n",q.A);
	printf("B\t=%p\n",q.B);
	printf("rhs\t=%p\n",q.rhs);
	printf("sol\t=%p\n",q.sol);
	printf("work\t=%p\n",q.work);

	printf("\n");
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
	fprintf(stderr,"init_rte2dvis\n");

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
	q.area=(double*)mkl_malloc(sizeof(double)*q.Ns,MALLOC_ALIGNMENT);
	double *p = q.mesh->trigs;
	for (int i = 0; i < q.Ns; i++)
		// area[i] = 0.5*Det[p0-p1,p0-p2]
		q.area[i] = 0.5*det(p+6*i,p+6*i+2,p+6*i,p+6*i+4);

	// cntr
	q.cntr=(double*)mkl_malloc(sizeof(double)*2*q.Ns,MALLOC_ALIGNMENT);
	double xx = 1.0/3.0;
	for (int i = 0; i < q.Ns; i++) {
		double *pp=p+6*i;
		q.cntr[2*i  ] = xx * (pp[0]+pp[2]+pp[4]);
		q.cntr[2*i+1] = xx * (pp[1]+pp[3]+pp[5]);
	}

	// Nd, Nm, Ng
	if (pad) {
		q.Nm = 2*Nd+1+ pad - (2*Nd+1)%pad;
		q.Nd = (q.Nm-1)/2;
	} else {
		q.Nd = Nd;
		q.Nm = 2*Nd+1;
	}
	q.Ng = q.Ns * (2*q.Nd+1);

	// plans
	create_fftw_plans(2*q.Nm,q.plans);

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
		+sizeof(double)*q.Ns
		+sizeof(fftw_plan)*4
		+sizeof(double)*2*q.Ns;

	// status
	q.status = 0;
}

int check_rte2dvis(const struct st_rte2dvis_info &q)
{
	fprintf(stderr,"check_rte2dvis\n");
	return 0;
}

void destroy_rte2dvis(struct st_rte2dvis_info &q)
{
	fprintf(stderr,"destroy_rte2dvis\n");
	destroy_fftw_plans(q.plans);
	release_mesh(*(q.mesh));
	mkl_free(q.mesh);
	mkl_free(q.area); 

	q.mem -= sizeof(struct st_mesh_info)
		+sizeof(double)*q.Ns
		+sizeof(fftw_plan)*4
		+sizeof(double)*2*q.Ns;

	q.status=-1;
}
/******************************************************************************/
/******************************************************************************/

/*
 * Allocate g,A,B,rhs,sol,work
 */
void alloc_rte2dvis_v1(struct st_rte2dvis_info &q)
{
	fprintf(stderr,"alloc_rte2dvis_v1\n");
	assert(q.status==0);

	// f(m)=g^|m|
	// A=2*pi*area(n)*delta(n,np)*delta(m,mp)
	// B is very complicated. See the derivation documentation.
	// rhs[Ns*(2*Nd+1)]
	// sol[Ns*(2*Nd+1)]
	// work[Nm] is the tmp workspace in FFT convolution.
	int flag=q.flag, Ns=q.Ns, Nd=q.Nd, Nm=q.Nm, Ng=q.Ng;
	q.f   =(double*)mkl_malloc(sizeof(double)*(2*Nd+1),MALLOC_ALIGNMENT);
	q.A   =(double*)mkl_malloc(sizeof(double)*Ns,MALLOC_ALIGNMENT);
	q.B   =(double*)mkl_malloc(sizeof(double)*2*flag*Ns*Ns*2*Nm,MALLOC_ALIGNMENT);
	q.rhs =(double*)mkl_malloc(sizeof(double)*2*Ng,MALLOC_ALIGNMENT);
	q.sol =(double*)mkl_malloc(sizeof(double)*2*Ng,MALLOC_ALIGNMENT);
	q.work=(double*)mkl_malloc(sizeof(double)*2*2*Nm,MALLOC_ALIGNMENT); 

	q.mem+=sizeof(double)*(
			2*Nd+1
			+Ns
			+flag*2*Ns*Ns*2*Nm
			+2*Ng
			+2*Ng
			+2*2*Nm
			);
	q.status=1;
}

static void fill_solver_f(struct st_rte2dvis_info &q)
{
	fprintf(stderr,"fill_solver_f\n");
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
	fprintf(stderr,"fill_solver_A\n");
	// A=2*pi*area(n)*delta(n,np)*delta(m,mp)
	// Note that q.area stores the signed areas.
	for (int i = 0; i < q.Ns; i++)
		q.A[i] = 2*M_PI*fabs(q.area[i]);
	//for (int i = 0; i < q.Ns; i++)
		//fprintf(stderr, "q.A[%4d]=%f\n",i,q.A[i]);
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

static void fill_solver_B_homo(struct st_rte2dvis_info &solver)
{
	fprintf(stderr,"fill_solver_B_homo\n");
	// quadrature rules
	const int RULE=6, NU=100, NV=3, MAX_NUM=1500;
	// critical distance
	const double dmin=0.25;

	// Prepare quadrature rules.
	using namespace QuadratureRules;
	struct st_quadrule q[4];
	WandzuraRule  g0(RULE);
	ArcSinhMethod g1;
	GaussRule     g2(NU);
	GaussRule     g3(NV);
	g2.Generate(q+2,0.0,1.0);
	g3.Generate(q+3,0.0,1.0);
	g1.Init(q+2,q+3);

	assert(MAX_NUM>=g0.Order());
	assert(MAX_NUM>=3*NU*NV);

	// Fill in elements. 1-point testing, multi-point source.
	double *p=solver.mesh->trigs;
	double *area=solver.area;
	double *cntr=solver.cntr;
	fftw_plan *plans=solver.plans;
	int Ns=solver.Ns, Nd=solver.Nd, Nm=solver.Nm, arcsinh_count=0;
	__declspec(align(64)) double _Complex E[MAX_NUM], wER[MAX_NUM];
	for (int np = 0; np < Ns; np++) {
		g0.Generate(q,p+6*np);
		for (int n = 0; n < Ns; n++) {
			double _Complex *z;
			z=((double _Complex*)solver.B)+(n+np*Ns)*2*Nm;
			//printf("B[%3d,%3d]=%p\n",n,np,z);
			int M;
			if ( distance(cntr+2*n,cntr+2*np)>dmin ) {
				// Non-singular, Wandzura
				//printf("[%4d,%4d] symmetric\n",n,np);
				double dx,dy,r;
				M = g0.Order();
				for (int i = 0; i < g0.Order(); i++) {
					dx  = cntr[2*n  ] - q->x[2*i  ];
					dy  = cntr[2*n+1] - q->x[2*i+1];
					r   = sqrt(dx*dx+dy*dy);
					E[i]= (dx-dy*I)/r;
					wER[i] = q->w[i]/r;
				}
			} else {
				// Singular/Near-singular, ArcSinh
				//printf("[%4d,%4d] arcsinh\n",n,np);
				g1.Generate(q+1,p+6*np,cntr+2*n);
				M = g1.Order();
				for (int i = 0; i < M; i++) {
					double dx,dy,r;
					dx  = cntr[2*n  ] - q[1].x[2*i  ];
					dy  = cntr[2*n+1] - q[1].x[2*i+1];
					r   = sqrt(dx*dx+dy*dy);
					E[i]= (dx-dy*I)/r;
					wER[i]= q[1].w[i];
				}
				arcsinh_count++;
			}
			// Fill z
			memset(z,0,sizeof(double)*2*Nm);
			for (int i = 0; i < M; i++)
				z[0] += wER[i];
			for (int dm = 1; dm < 2*Nd+1; dm++) {
				for (int i = 0; i < M; i++)
					wER[i] *= E[i];
				for (int i = 0; i < M; i++)
					z[dm]  += wER[i];
			}
			for (int dm = 2*Nd+1; dm <= 2*(Nm-Nd)-1; dm++)
				z[dm] = 0.0;
			for (int dm = 2*(Nm-Nd); dm <= 2*Nm-1; dm++)
				z[dm] = conj(z[2*Nm-dm]);
			// in-place FFT z, divide by 2*Nm, multiply by S(n)
			//fftw_execute_dft(plans[IFWD],(fftw_complex*)z,(fftw_complex*)z);
			scale(0.5/Nm*fabs(solver.area[n])*1.0E5,2*Nm,z);
			//for (int i = 0; i <= 2*Nm-1; i++)
				//printf("z[%3d] = %7.4f + %7.4f*I\n",i,creal(z[i]),cimag(z[i]));
		}
	}
	printf("arcsinh_count/(Ns*Ns) = %f\n",(double)arcsinh_count/(Ns*Ns));
}


/*
 * Out-of-place multiplication for homogeneous problem.
 */
static void mul_homo(struct st_rte2dvis_info &q, 
		const double _Complex *restrict in, 
		double _Complex *restrict out)
{
	// mu_t and mu_s
	const double mut=0.4, mus=0.3;

	int Ns=q.Ns, Nm=q.Nm, Nd=q.Nd;
	double *f=q.f, *A=q.A;
	double _Complex *B=(double _Complex*)q.B;
	fftw_complex *work=(fftw_complex *)q.work;
	memset(out,0,Ns*(2*Nd+1));
	for (int n = 0; n < Ns; n++)
		for (int m = 0; m < 2*Nd+1; m++)
			out[m+n*(2*Nd+1)] += in[m+n*(2*Nd+1)] * A[n];
	for (int np = 0; np < Ns; np++) {
		for (int n = 0; n < Ns; n++) {
			// copy into workspace
			for (int i = 0; i < 2*Nd+1; i++)
				work[i] = (mut-mus*f[i])*in[i+np*(2*Nd+1)];
			// padding zeros
			memset(work+2*Nd+1,0,sizeof(double)*2*(Nm-2*Nd-1));
			// transform workspace
			fftw_execute_dft(q.plans[IFWD],work,work);
			// convolution in frequency domain
			for (int i = 0; i < 2*Nm; i++)
				work[i]*= B[i+(n+np*Ns)*2*Nm];
			// FFT back
			fftw_execute_dft(q.plans[IBWD],work,work);
			// copy to out
			for (int i = 0; i < 2*Nd+1; i++)
				out[i+np*(2*Nd+1)] += work[i];
		}
	}
}

static void fill_solver_rhs(struct st_rte2dvis_info &q)
{
	fprintf(stderr,"fill_solver_rhs\n");
	// rhs[Ns*(2*Nd+1)] TODO
	double _Complex *rhs=(double _Complex*)q.rhs;
	double _Complex *sol=(double _Complex*)q.sol;
	for (int i = 0; i < q.Ng; i++)
		sol[i] = i;
	mul_homo(q,sol,rhs);
}

/*
 * Fill up g,A,B,rhs
 */
void fill_rte2dvis_v1(struct st_rte2dvis_info &q)
{
	fprintf(stderr,"fill_rte2dvis_v1\n");
	assert(q.status==1);
	
	fill_solver_f(q);
	fill_solver_A(q);
	fill_solver_B_homo(q);
	fill_solver_rhs(q);

	q.status=2;
}

int solve_rte2dvis_v1(struct st_rte2dvis_info &q)
{
#define MAX_ITR_NUM 50
#define PAR_SIZE 128
	fprintf(stderr,"solve_rte2dvis_v1\n");
	assert(q.status==2);

	int Nd=q.Nd, Nm=q.Nm, Ns=q.Ns, Ng=q.Ng;
	double *sol=q.sol, *rhs=q.rhs;

	/*
	 * Allocate storage for the iterative solver.
	 */
	int ipar[PAR_SIZE],RCI_request,ivar=2*Ng,itercount;
	double dpar[PAR_SIZE], *tmp;
	size_t ltmp=(2*MAX_ITR_NUM+1)*ivar+MAX_ITR_NUM*((MAX_ITR_NUM+9)/2+1);
	tmp=(double*)mkl_malloc(sizeof(double)*ltmp,MALLOC_ALIGNMENT);

	/*
	 * Initialize the solver.
	 */
	dfgmres_init(&ivar,sol,rhs,&RCI_request,ipar,dpar,tmp);
	if (RCI_request) { MKL_Free_Buffers(); return RCI_request; }

	/*
	 * Set the desired parameters:
	 * LOGICAL parameters:
	 * 	ipar[8]:  do automatic residual stopping test
	 * 	ipar[9]:  do residual stopping test: dpar[5]<dpar[4]
	 * 	ipar[11]: automatic check norm of next generated vector??
	 * 	ipar[14]: max number of non-restart iteration, default 150
	 * DOUBLE parameters:
	 * 	dpar[0]:  set the relative tolerance to 1.0E-5
	 */
	ipar[8]		= 1;
	ipar[9]		= 0;
	ipar[11]	= 1;
	ipar[14]	= MAX_ITR_NUM;
	dpar[0]		= 1.0E-10;

	/*
	 * Check consistency of newly assigned parameters.
	 */
	dfgmres_check(&ivar,sol,rhs,&RCI_request,ipar,dpar,tmp);
	if (RCI_request) { MKL_Free_Buffers(); return RCI_request; }

	/*
	 * Solve!
	 * RCI_request:
	 * 	0	completed
	 * 	1	go on iterating
	 */
	dfgmres(&ivar,sol,rhs,&RCI_request,ipar,dpar,tmp);
	while (RCI_request==1) {
		//printf("tmp+ipar[21]-1=%p\n",tmp+ipar[21]-1);
		//printf("        [22]  =%p\n",tmp+ipar[22]-1);
		mul_homo(q,(double _Complex*)(tmp+ipar[21]-1),
				(double _Complex*)(tmp+ipar[22]-1));
		dfgmres(&ivar,sol,rhs,&RCI_request,ipar,dpar,tmp); 
	}
	printf("RCI_request=%d\n",RCI_request);

	/*
	 * Extract solution, print, clear buffers.
	 */
	dfgmres_get(&ivar,sol,rhs,&RCI_request,ipar,dpar,tmp,&itercount);
	printf("\n");
	printf("Solver finished after %d iterations.\n",itercount);
	MKL_Free_Buffers();

	mkl_free(tmp);

	q.status=3;

	return 0;
#undef MAX_ITR_NUM
#undef PAR_SIZE
}

void release_rte2dvis_v1(struct st_rte2dvis_info &q)
{
	fprintf(stderr,"release_rte2dvis_v1\n");
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
			+q.flag*2*q.Ns*q.Ns*2*q.Nm
			+2*q.Ng
			+2*q.Ng
			+2*2*q.Nm
			);
	q.status=-2;
}
