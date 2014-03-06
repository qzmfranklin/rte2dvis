#include <stdio.h>
#include <omp.h>
#include <mkl.h>
#include <complex.h>
#include <math.h>
#include <assert.h>
#include "solver_v1.h"
#include "mesh.h" 
#include "quadrules.h"
/**************************************/
#include "utils.h"
/******************************************************************************/
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
/******************************************************************************/
static void init(struct st_solver_v1 *s, const int *ipar, const double *dpar)
{

	assert(s->status==0);

	for (int i = 0; i < 128; i++) {
		s->ipar[i] = ipar[i];
		s->dpar[i] = dpar[i];
	}

	s->M      = s->ipar[0]; // unused in v1
	s->Nd     = s->ipar[1];
	s->pad    = s->ipar[2];

	struct st_mesh *q=s->mesh;

	s->Ns     = q->num_trigs;
	s->Nv     = q->num_verts;
	s->Ne     = q->num_edges;
	s->Nt     = s->Nv + (s->M-1)*s->Ne + (s->M-1)*(s->M-2)/2*s->Ns; // unused in v1

	{
	int tmp=2*s->Nd+1;
	int res=tmp%s->pad;
	if (res==0) res=s->pad;
	s->Nm     = tmp + s->pad - res;
	}

	s->Ng     = s->Ns * (2*s->Nd+1);
	//s->Ng     = s->Nt * (2*Nd+1); // unused in v1

	s->status = 1;
}
static void alloc(struct st_solver_v1 *s)
{
	assert(s->status==1);

	s->E    = (double*)mkl_malloc(sizeof(double)*(s->Ns),64);
	s->K    = (double*)mkl_malloc(sizeof(double)*(s->Ns*s->Ns*s->Nm*2),64);
	//s->tmp  = (double*)mkl_malloc(sizeof(double)*(3*s->ipar[5]*s->ipar[6]),64); //TODO
	//s->work = (double _Complex*)mkl_malloc(
			//sizeof(double _Complex)*(s->Ns*s->Ns*s->Nm*2),64);
	assert(s->E);
	assert(s->K);
	//assert(s->tmp);
	//assert(s->work);

	s->status=2;
}
static void fillE(struct st_solver_v1 *s)
{
	assert(s->status==2);

	const double xx = 2.0*M_PI; 
	for (int i = 0; i < s->Ns; i++)
		s->E[i] = xx * s->mesh->a[i];

	s->status=3;
}
/**************************************/
static double r2rd(const double *p1, const double *p2)
{
	const double a=p1[0]-p2[0];
	const double b=p1[1]-p2[1];
	return sqrt(a*a+b*b);
}
static void fillK(struct st_solver_v1 *s)
{
	assert(s->status==3);

/*
 * ipar[0] = M
 * ipar[1] = Nd
 * ipar[2] = pad
 * ipar[3] = rule1
 * ipar[4] = rule2
 * ipar[5] = nu
 * ipar[6] = nv
 */
/*
 * dpar[0] = g
 * dpar[1] = phis (planewave incident)
 * dpar[2] = mua (absorption coefficient)
 * dpar[3] = mus (scattering coefficient)
 */
	//const int      nu = (int)MArgument_getInteger(Args[5]); // source
	//const int      nv = (int)MArgument_getInteger(Args[6]); // source
	const int nu = s->ipar[5]; // source
	const int nv = s->ipar[6]; // source

	//double         *p = libData->MTensor_getRealData(Tp);
	//const mint  *dimp = libData->MTensor_getDimensions(Tp);
	//const int      Ns = (int)(dimp[0]);
	//const int      Ng = Ns*(2*Nd+1);
	const int Ns = s->Ns;
	const int Ng = s->Ng;
	const int Nd = s->Nd;
	const int Nm = s->Nm;
	const double *p = s->mesh->p;

	//const int     nn1 = wandzura_order_num(rule1);
	//const int     nn2 = dunavant_order_num(rule2);
	//const int      ns = 3*nu*nv;
	const int rule1 = s->ipar[3];
	const int rule2 = s->ipar[4];
	const int nn1 = wandzura_order_num(rule1);
	const int nn2 = dunavant_order_num(rule2);
	const int  ns = 3*nu*nv;

	//MTensor TRes;
	//mint dims[]={Ns,Ns,2*Nm};
	//libData->MTensor_new(MType_Real,3,dims,&TRes);
	//double *B = libData->MTensor_getRealData(TRes);
	// s->K = B

	//double *area=(double*)mkl_malloc(sizeof(double)*Ns,64);
	//double *cntr=(double*)mkl_malloc(sizeof(double)*2*Ns,64);
	// s->a = area, s->c = cntr
	const double *area = s->mesh->a;
	const double *cntr = s->mesh->c; 

	double *xy01=(double*)mkl_malloc(sizeof(double)*2*nn1,64);
	double  *wn1=(double*)mkl_malloc(sizeof(double)*nn1,64);
	double *xy02=(double*)mkl_malloc(sizeof(double)*2*nn2,64);
	double  *wn2=(double*)mkl_malloc(sizeof(double)*nn2,64);

	double *xu =(double*)mkl_malloc(sizeof(double)*nu,64);
	double *wu =(double*)mkl_malloc(sizeof(double)*nu,64);
	double *xv =(double*)mkl_malloc(sizeof(double)*nv,64);
	double *wv =(double*)mkl_malloc(sizeof(double)*nv,64);

	double _Complex *Kwork=(double _Complex*)mkl_malloc(sizeof(double _Complex)*Ns*Ns*2*Nm,64);

	// init non-sigular quad rules
	wandzura_rule(rule1,nn1,xy01,wn1);
	dunavant_rule(rule2,nn2,xy02,wn2);
	// init 1D quad rules
	//                     xmin,  xmax
	cgqf(nu,1.0,0.0,0.0,    0.0,  1.0,   xu,wu);
	cgqf(nv,1.0,0.0,0.0,    0.0,  1.0,   xv,wv);

	// create FFTW plan
	//fftw_plan plan= fftw_plan_dft_1d(2*Nm,
			//(fftw_complex*)Kwork,
			//(fftw_complex*)Kwork,
			//FFTW_FORWARD, FFTW_ESTIMATE);
	const int Nm_times_2 = 2*Nm;
	fftw_plan plan = fftw_plan_many_dft(1,&Nm_times_2,Ns*Ns,
			(fftw_complex*)Kwork,NULL,1,Nm_times_2,
			(fftw_complex*)Kwork,NULL,1,Nm_times_2,
			FFTW_FORWARD, FFTW_ESTIMATE);

	// Note: s->K [Ns,Ns,2Nm] is real row-major rank-3 tensor
	memset(Kwork,0,sizeof(double _Complex)*Ns*Ns*2*Nm);

	#pragma omp parallel	\
	num_threads(s->ipar[7])	\
	default(none)		\
	shared(nu,nv,Nd,Ns,Ng,Nm,rule1,rule2,nn1,nn2,p,ns,area,cntr, \
			xy01,wn1,xy02,wn2,xu,wu,xv,wv,Kwork, \
			stdout,stdin,stderr)
	{
	// testing
	//double *xy01=(double*)mkl_malloc(sizeof(double)*2*nn1,64);
	double *xyn1=(double*)mkl_malloc(sizeof(double)*2*nn1,64);
	//double  *wn1=(double*)mkl_malloc(sizeof(double)*nn1,64);

	// source
	//double *xy02=(double*)mkl_malloc(sizeof(double)*2*nn2,64);
	double *xyn2=(double*)mkl_malloc(sizeof(double)*2*nn2,64);
	//double  *wn2=(double*)mkl_malloc(sizeof(double)*nn2,64);

	double *xys=(double*)mkl_malloc(sizeof(double)*2*ns,64);
	double *ws =(double*)mkl_malloc(sizeof(double)*ns,64); 

	double _Complex *b  =(double _Complex*)mkl_malloc(sizeof(double)*2*2*Nm,64);
	//double _Complex *bb =(double _Complex*)mkl_malloc(sizeof(double)*2*2*Nm,64);

	double _Complex *e  =(double _Complex*)mkl_malloc(sizeof(double)*2*MAX(nn1,MAX(nn2,ns)),64);
	double _Complex *wer=(double _Complex*)mkl_malloc(sizeof(double)*2*MAX(nn1,MAX(nn2,ns)),64); 

	double *work=(double*)mkl_malloc(sizeof(double)*3*nu*nv,64);
	//double *work = s->tmp;

	const int num_threads=omp_get_num_threads();
	const int tid=omp_get_thread_num();
	fprintf(stderr,"thread_id = %-5d\n",tid);
	int blk=Ns/num_threads;
	int res=Ns%num_threads;
	int start, end;
	if (!res) blk++;
	start = blk * tid;
	if (tid==num_threads-1)
		end = Ns;
	else
		end = blk * (tid+1);
	//for (int n = 0; n < Ns; n++) {
	for (int n = start; n < end; n++) {
		reference_to_physical_t3(nn1,p+6*n,xy01,xyn1);
		for (int np = 0; np < Ns; np++) {
			reference_to_physical_t3(nn2,p+6*np,xy02,xyn2);
			// Compute one block of s->K with (n,np)
			//memset(bb,0,sizeof(double)*2*2*Nm);
			for (int j = 0; j < nn1; j++) {
				const double *p0=xyn1+2*j;
				int MM;
				memset(b,0,sizeof(double _Complex)*2*Nm);
				if (r2rd(p0,cntr+2*np)>6*0.8774*sqrt(area[np])) {
					// Non-singular
					MM = nn2;
					for (int i = 0; i < MM; i++) {
						const double dx = p0[0] - xyn2[2*i  ];
						const double dy = p0[1] - xyn2[2*i+1];
						const double inv= 1.0/sqrt(dx*dx+dy*dy);
						e[i]   = inv*(dx-dy*I);
						wer[i] = inv*wn2[i]*area[np];
					}
				} else {
					// Singular, near-singular
					MM = 3*nu*nv;
					arcsinh_rule_xy(xys,ws, p0,p+6*np,
							nu,xu,wu, nv,xv,wv,
							work);
					for (int i = 0; i < MM; i++) {
						const double dx = p0[0] - xys[2*i  ];
						const double dy = p0[1] - xys[2*i+1];
						const double inv= 1.0/sqrt(dx*dx+dy*dy);
						e[i]   = inv*(dx-dy*I);
						wer[i] = ws[i];
					}
				}
				// Fill b[0]
				for (int i = 0; i < MM; i++)
					b[0] += wer[i];
				// Fill b[1] -> b[2Nd]
				for (int dm = 1; dm < 2*Nd; dm++) {
					for (int i = 0; i < MM; i++)
						wer[i] *= e[i];
					for (int i = 0; i < MM; i++)
						b[dm]  += wer[i];
				}
				// Pad b[2Nd+1] -> b[2(Nm-Nd)-1] with zeros
				// Fill b[2(Nm-Nd)] -> b[2Nm-1] with complex conjugates
				for (int i = 2*(Nm-Nd); i <= 2*Nm-1; i++)
					b[i] = conj(b[2*Nm-i]);

				//// copy b to bb
				//for (int i = 0; i < 2*Nm; i++)
					//bb[i] += b[i]*wn1[j]; // testing weights
				for (int i = 0; i < 2*Nm; i++)
					Kwork[i+(np+n*Ns)*2*Nm] += b[i] * wn1[j];
			}
			//[>FFTW bb to B<]
			//fftw_execute(plan);	// in-place bb
			//{
				//const double xx=area[n]*0.5/Nm;
				//for (int i = 0; i < 2*Nm; i++)
					//B[i+(np+n*Ns)*2*Nm] += xx * creal(bb[i]);
			//}
		}
	}
	// Free thread-specific memories
	//mkl_free(xy01);
	mkl_free(xyn1);
	//mkl_free(wn1 );

	//mkl_free(xy02);
	mkl_free(xyn2);
	//mkl_free(wn2 );

	//mkl_free(xu );
	//mkl_free(wu );
	//mkl_free(xv );
	//mkl_free(wv );
	mkl_free(xys);
	mkl_free(ws );

	mkl_free(b  );
	//mkl_free(bb );
	mkl_free(e  );
	mkl_free(wer);

	mkl_free(work);
	}

	// FFTW s->work to s->K
	fftw_execute(plan); // in-place s->K
	for (int n = 0; n < Ns; n++) {
		const double xx=area[n]*0.5/Nm;
		for (int np = 0; np < Ns; np++)
			for (int i = 0; i < 2*Nm; i++)
				s->K[i+(np+n*Ns)*2*Nm] += xx * creal(Kwork[i+(np+n*Ns)*2*Nm]);
	}

	/*Release Memory*/
	mkl_free(xy01);
	mkl_free(wn1 );
	mkl_free(xy02);
	mkl_free(wn2 );

	mkl_free(xu );
	mkl_free(wu );
	mkl_free(xv );
	mkl_free(wv );

	mkl_free(Kwork);

	/*Destroy FFTW plan*/
	fftw_destroy_plan(plan);

	//{ const int n=1, np=3;
	//for (int i = 0; i < 2*Nm; i++)
		//printf("[%d] %.4E\n",i,s->K[i+(np+n*Ns)*2*Nm]);
	//}

	s->status=4;
}
/******************************************************************************/
struct st_solver_v1 *sv1_create_solver(struct st_mesh *q, const int *ipar, const double *dpar)
{
	fprintf(stderr,"sv1_create_solver()\n");

	assert(q);
	assert(q->status==3); // st_mesh is already loaded

	struct st_solver_v1 *s = (struct st_solver_v1*)
		mkl_malloc(sizeof(struct st_solver_v1),64);
	assert(s);
	s->status = 0; 
	s->mesh   = q;

	init(s,ipar,dpar);
	alloc(s);
	fillE(s);
	fillK(s);

	return s;
}
void sv1_destroy_solver(struct st_solver_v1 *s)
{
	assert(s->status>=2); // check alloc'd

	mkl_free(s->E);
	mkl_free(s->K);
	//mkl_free(s->work);
	//mkl_free(s->tmp); 

	// fftw_destroy_plan();
	
	mkl_free(s);
}
void sv1_mul(struct st_solver_v1 *s, const double _Complex *in, double _Complex *out)
{
}
//void sv1_solve(double _Complex *solution)
//{
	//const int N=100;
	//int err=0; 

	//// Prepare the inputs.
	//double A[N*N],expected_sol[N],rhs[N];
	//init_matrix(N,N,A);
	//for (int i = 0; i < N; i++)
		//expected_sol[i] = i;
	//m_mul_v(N,A,expected_sol,rhs);

	//printf("N=%d\n",N);
	//print_vector("expected_sol",20,expected_sol);
	
	//// Allocate storage for the iterative solver.
	//double sol[N];
	//init_vector(N,sol); 
	//print_vector("initial guess",20,sol); 

	//int ipar[128],RCI_request,ivar,itercount;
	//double dpar[128],tmp[N*(5*N+11)/2+1];
	//ivar    = N;

	//print_mkl_dfgmres_pars(ipar,dpar);
	//dfgmres_init(&ivar,sol,rhs,&RCI_request,ipar,dpar,tmp);
	//assert(!RCI_request);

	/*
	 * LOGICAL parameters:
	 *   ipar[3]:  current iteration count
	 *   ipar[4]:  max numbuer of iterations
	 *   ipar[7]:  !0 = max iteration check [1]
	 *   ipar[8]:  !0 = residual stopping check dpar[3]<dpar[4] [0]
	 *   ipar[9]:  !0 = user-defined stopping check by setting RCI_request=2 [1]
	 *   ipar[10]: !0 = requests precondition by setting RCI_request=3 [0]
	 *   ipar[11]: !0 = check zero norm of current vector dpar[6]<=dpar[7] [0]
	 *              0 = requests user-defined check by setting RCI_request=4
	 *              (must set to 1 for some unknown reasons)
	 *   ipar[12]: solution vector storage flag [0]
	 *   ipar[13]: internal iteration counts before restart
	 *   ipar[14]: max number of non-restarted version
	 */
	//ipar[4]  = 100;
	//ipar[7]  = 1;
	//ipar[8]	 = 1;
	//ipar[9]	 = 0;
	//ipar[11] = 1;

	/*
	 * DOUBLE parameters:
	 *   dpar[0]:  relative tolerance [1.0D-6]
	 *   dpar[1]:  absolute tolerance [0.0D-0]
	 *   dpar[2]:  L2 norm of initial residual
	 *   dpar[3]:  service variable, dpar[0]*dpar[2]+dpar[1]
	 *   dpar[4]:  L2 norm of current residual
	 *   dpar[5]:  L2 norm of previous residual, if available
	 *   dpar[6]:  norm of generated vector
	 *   dpar[7]:  tolerance for zero norm of current vector [1.0D-12]
	 */
	//dpar[0]	 = 1.0E-5;
	//// Check consistency of parameters
	//dfgmres_check(&ivar,sol,rhs,&RCI_request,ipar,dpar,tmp);
	//assert(!RCI_request);

	/*
	 * RCI_request:
	 * 	0	completed
	 * 	1	go on iterating
	 * 	2	requests user-defined stopping check, ipar[9]
	 * 	3	requests preconditoning, ipar[10]
	 * 	4	requests zero norm check, ipar[11]
	 */
	//double bnorm=0.0;
	//for (int i = 0; i < N; i++)
		//bnorm += rhs[i] * rhs[i];
	//bnorm = sqrt(bnorm);
	//do {
		//dfgmres(&ivar,sol,rhs,&RCI_request,ipar,dpar,tmp); 
		//switch (RCI_request) {
		//case 1: // user-defined matrix-vector multiplication comes here...
			//m_mul_v(N,A,tmp+ipar[21]-1,tmp+ipar[22]-1); 
		//case 2: // requests user-defined stopping check
			//break;
		//case 3: // requests preconditioning
			//break;
		//case 4: // requests zero norm check
			//break;
		//default:// no idea how to get here...
			//break;
		//}
		//printf("[  %5d,  %5d,  %.2E,  %-.2E]\n",ipar[3],ipar[13],dpar[4],dpar[4]/bnorm); 
	//} while(RCI_request>0);

	//// Extract solution, print, clear buffers.
	//dfgmres_get(&ivar,sol,rhs,&RCI_request,ipar,dpar,tmp,&itercount);
	//printf("\n");
	//printf("Solver finished after %d iterations.\n",itercount);
	//print_vector("sol",20,sol);
	//MKL_Free_Buffers(); 
//}
void sv1_print_solver(struct st_solver_v1 *s)
{
	//fprintf(stderr,"mshio_print_mesh()\n"); 

	printf("    status = %d  ", s->status);
	printf("(0=uninit'd 4=ready)\n");

	printf("SPA INT  M = %d\n", s->M);
	printf("ANG    pad = %d\n", s->pad);
	printf("TRIGS   Ns = %d\n", s->Ns);
	printf("VERTS   Nv = %d\n", s->Nv);
	printf("EDGES   Ne = %d\n", s->Ne);
	printf("SPAT    Nt = %d\n", s->Nt);
	printf("HARM    Nd = %d\n", s->Nd);
	printf("ANG     Nm = %d\n", s->Nm);
	printf("TOTAL   Ng = %d\n", s->Ng);

	printf("RAW   mesh = %p\n", s->mesh);

	printf("IDENTITY E = %p\n", s->E);
	printf("INTERACT K = %p\n", s->K);
	printf("CPLX  work = %p\n", s->work);
	printf("REAL   tmp = %p\n", s->tmp);
}
