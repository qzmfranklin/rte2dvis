#include <complex.h>
#include <assert.h>
#include <string.h>
#include "legendre-rule.h"
#include "dunavant-rule.h" 
#include "arcsinh-rule.h" 
#include "utils.h"
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <stdlib.h>

#include "WolframLibrary.h" 
#include "mll.h"
/******************************************************************************/
DLLEXPORT int test( WolframLibraryData libData, mint Argc, MArgument *Args, MArgument Res)
{
	int err = LIBRARY_NO_ERROR; 

	/*Receive from LibraryLink*/
	MTensor const Tv = MArgument_getMTensor(Args[0]);
	double _Complex *v=(double _Complex*)libData->MTensor_getComplexData(Tv);
	mint const *dim=libData->MTensor_getDimensions(Tv);
	const int n=dim[0];

	/*Allocate Memory*/
	MTensor TRes;
	mint dims[2]={8,n};
	mint rank=2;
	libData->MTensor_new(MType_Complex,rank,dims,&TRes);
	double _Complex *a = (double _Complex*)libData->MTensor_getComplexData(TRes);

	/*Send to LibraryLink*/
	MArgument_setMTensor(Res,TRes);

	/*Set*/
	//#pragma omp parallel for \
	//num_threads(8)
	for (int i = 0; i < 8*n; i++)
		a[i] = i-i*I;

	/*Disown MTensor*/
	//libData->MTensor_disown(TRes); 
	//libData->MTensor_disown(Tv); 
	libData->MTensor_free(TRes);
	libData->MTensor_free(Tv);

	return LIBRARY_NO_ERROR;
}

DLLEXPORT int BHomoS_MLL( WolframLibraryData libData, mint Argc, MArgument *Args, MArgument Res)
{
	int err = LIBRARY_NO_ERROR; 

	/*Receive from LibraryLink*/
	const mint     Nd  = MArgument_getInteger(Args[0]);
	const mint     Nm  = MArgument_getInteger(Args[1]);
	const MTensor  Tp0 = MArgument_getMTensor(Args[2]);
	const MTensor  Tq  = MArgument_getMTensor(Args[3]);
	double const  *p0  = libData->MTensor_getRealData(Tp0);
	mint const    *dim = libData->MTensor_getDimensions(Tq);
	const int      M   = dim[1];
	double const  *x   = (double*)libData->MTensor_getRealData(Tq);
	double const  *y   = x + M;
	double const  *w   = y + M;

	/*Allocate Memory*/
	MTensor TRes;
	mint dims[1]={2*Nm};
	mint rank=1;
	libData->MTensor_new(MType_Complex,rank,dims,&TRes);
	double _Complex *b = (double _Complex*)libData->MTensor_getComplexData(TRes);

	/*Compute b*/
	memset(b,0,sizeof(double)*2*2*Nm);
	const int _LWORK=2000;
	assert(_LWORK>=M);
	double _Complex e[_LWORK],wer[_LWORK];
	for (int i = 0; i < M; i++) {
		double dx = p0[0] - x[i];
		double dy = p0[1] - y[i];
		double inv= 1.0/sqrt(dx*dx+dy*dy);
		e[i]   = inv*(dx-dy*I);
		wer[i] = w[i];
	}
	// Fill b[0]
	for (int i = 0; i < M; i++)
		b[0] += wer[i];
	// Fill b[1] -> b[2Nd]
	for (int dm = 1; dm < 2*Nd; dm++) {
		for (int i = 0; i < M; i++)
			wer[i] *= e[i];
		for (int i = 0; i < M; i++)
			b[dm]  += wer[i];
	}
	// Pad b[2Nd+1] -> b[2(Nm-Nd)-1] with zeros
	// Fill b[2(Nm-Nd)] -> b[2Nm-1] with conjugates
	for (int dm = 2*(Nm-Nd); dm <= 2*Nm-1; dm++)
		b[dm] = conj(b[2*Nm-dm]);

	/*Send to LibraryLink*/
	MArgument_setMTensor(Res,TRes);

	/*Disown MTensor*/
	libData->MTensor_disown(TRes);


	return LIBRARY_NO_ERROR;
}

DLLEXPORT int BHomoN_MLL( WolframLibraryData libData, mint Argc, MArgument *Args, MArgument Res)
{
	int err = LIBRARY_NO_ERROR; 

	/*Receive from LibraryLink*/
	const mint     Nd  = MArgument_getInteger(Args[0]);
	const mint     Nm  = MArgument_getInteger(Args[1]);
	const MTensor  Tp0 = MArgument_getMTensor(Args[2]);
	const MTensor  Tq  = MArgument_getMTensor(Args[3]);
	double const  *p0  = libData->MTensor_getRealData(Tp0);
	mint const    *dim = libData->MTensor_getDimensions(Tq);
	const int      M   = dim[1];
	double const  *x   = (double*)libData->MTensor_getRealData(Tq);
	double const  *y   = x + M;
	double const  *w   = y + M;

	/*Allocate Memory*/
	MTensor TRes;
	mint dims[1]={2*Nm};
	mint rank=1;
	libData->MTensor_new(MType_Complex,rank,dims,&TRes);
	double _Complex *b = (double _Complex*)libData->MTensor_getComplexData(TRes);

	/*Compute b*/
	memset(b,0,sizeof(double)*2*2*Nm);
	const int _LWORK=2000;
	assert(_LWORK>=M);
	double _Complex e[_LWORK],wer[_LWORK];
	for (int i = 0; i < M; i++) {
		double dx = p0[0] - x[i];
		double dy = p0[1] - y[i];
		double inv= 1.0/sqrt(dx*dx+dy*dy);
		e[i]   = inv*(dx-dy*I);
		wer[i] = inv*w[i];
	}
	// Fill b[0]
	for (int i = 0; i < M; i++)
		b[0] += wer[i];
	// Fill b[1] -> b[2Nd]
	for (int dm = 1; dm < 2*Nd; dm++) {
		for (int i = 0; i < M; i++)
			wer[i] *= e[i];
		for (int i = 0; i < M; i++)
			b[dm]  += wer[i];
	}
	// Pad b[2Nd+1] -> b[2(Nm-Nd)-1] with zeros
	// Fill b[2(Nm-Nd)] -> b[2Nm-1] with conjugates
	for (int dm = 2*(Nm-Nd); dm <= 2*Nm-1; dm++)
		b[dm] = conj(b[2*Nm-dm]);

	/*Send to LibraryLink*/
	MArgument_setMTensor(Res,TRes);

	/*Disown MTensor*/
	libData->MTensor_disown(TRes);


	return LIBRARY_NO_ERROR;
}

/**************************************/
static void scale(const double scale, const int n, double _Complex *a)
{
	for (int i = 0; i < n; i++)
		a[i] *= scale;
}

/*
 *
 *DLLEXPORT int HomoMulOMP_MLL( WolframLibraryData libData, mint Argc, MArgument *Args, MArgument Res)
 *{
 *#define NUM_THREAD 8
 *        int err = LIBRARY_NO_ERROR; 
 *
 *        [>Receive from LibraryLink<]
 *        MTensor const   TX = MArgument_getMTensor(Args[0]);	// Ns(2Nd+1)
 *        MTensor const   TA = MArgument_getMTensor(Args[1]);	// Ns
 *        MTensor const   TB = MArgument_getMTensor(Args[2]);	// Ns, Ns, 2Nm
 *        MTensor const   Tg = MArgument_getMTensor(Args[3]);	// 2Nd+1
 *        const double   mut = MArgument_getReal(Args[4]);	// mut=mus(scattering)+mua(absorbing)
 *        const double   mus = MArgument_getReal(Args[5]);	// mus
 *        mint const   *dimA = libData->MTensor_getDimensions(TA);
 *        mint const   *dimB = libData->MTensor_getDimensions(TB);
 *        mint const   *dimg = libData->MTensor_getDimensions(Tg);
 *        const int       Ns = dimA[0];
 *        const int       Nm = dimB[2]/2;
 *        const int       Nd = (dimg[0]-1)/2;
 *        const int       Ng = Ns*(2*Nd+1);
 *        double _Complex *X = (double _Complex*)libData->MTensor_getComplexData(TX);
 *        double          *A = libData->MTensor_getRealData(TA);
 *        double          *B = libData->MTensor_getRealData(TB);
 *        double          *g = libData->MTensor_getRealData(Tg);
 *
 *        [>Allocate Memory/Workspace<]
 *        MTensor TRes;
 *        mint dims[1]={Ng};
 *        mint rank=1;
 *        libData->MTensor_new(MType_Complex,rank,dims,&TRes);
 *        double _Complex *Y=(double _Complex*)libData->MTensor_getComplexData(TRes);
 *
 *        const int _LWORK=4001;
 *        assert(_LWORK>2*Nm);
 *        double _Complex work[_LWORK] __attribute__((aligned(64)));
 *
 *        [>FFTW plans<]
 *        fftw_plan pf=fftw_plan_dft_1d(2*Nm,work,work,
 *                        FFTW_FORWARD ,FFTW_MEASURE|FFTW_PATIENT);
 *        fftw_plan pb=fftw_plan_dft_1d(2*Nm,work,work,
 *                        FFTW_BACKWARD,FFTW_MEASURE|FFTW_PATIENT);
 *        //fftw_execute(p);
 *
 *        [>Compute Y<]
 *        memset(Y,0,sizeof(double)*2*Ng);
 *        // B[Ns,Ns,2*Nm] is row-major
 *        for (int n = 0; n < Ns; n++) {
 *                for (int np = 0; np < Ns; np++) {
 *                        for (int i = 0; i < 2*Nd+1; i++)
 *                                work[i]  = (mut-mus*g[i]) * X[i+(2*Nd+1)*np];
 *                        memset(work+2*Nd+1,0,sizeof(double)*2*(2*Nm-2*Nd-1));
 *                        fftw_execute(pf);
 *                        for (int i = 0; i < 2*Nm; i++)
 *                                work[i] *= B[i+(np+n*Ns)*2*Nm]; // row-major
 *                        fftw_execute(pb);
 *                        for (int i = 0; i < 2*Nd+1; i++)
 *                                Y[i+n*(2*Nd+1)] += work[i];
 *                }
 *        }
 *
 *        [>Send to LibraryLink<]
 *        MArgument_setMTensor(Res,TRes);
 *
 *        [>Disown MTensor<]
 *        libData->MTensor_disown(TRes);
 *
 *        [>Free FFTW plans/Workspace<] 
 *        fftw_destroy_plan(pf);
 *        fftw_destroy_plan(pb);
 *
 *        return LIBRARY_NO_ERROR;
 *#undef NUM_THREAD
 *}
 */

DLLEXPORT int HomoMul_MLL( WolframLibraryData libData, mint Argc, MArgument *Args, MArgument Res)
{
	int err = LIBRARY_NO_ERROR; 

	/*Receive from LibraryLink*/
	MTensor const   TX = MArgument_getMTensor(Args[0]);	// Ns(2Nd+1)
	MTensor const   TA = MArgument_getMTensor(Args[1]);	// Ns
	MTensor const   TB = MArgument_getMTensor(Args[2]);	// Ns, Ns, 2Nm
	MTensor const   Tg = MArgument_getMTensor(Args[3]);	// 2Nd+1
	const double   mut = MArgument_getReal(Args[4]);	// mut=mus(scattering)+mua(absorbing)
	const double   mus = MArgument_getReal(Args[5]);	// mus
	mint const   *dimA = libData->MTensor_getDimensions(TA);
	mint const   *dimB = libData->MTensor_getDimensions(TB);
	mint const   *dimg = libData->MTensor_getDimensions(Tg);
	const int       Ns = dimA[0];
	const int       Nm = dimB[2]/2;
	const int       Nd = (dimg[0]-1)/2;
	const int       Ng = Ns*(2*Nd+1);
	double _Complex *X = (double _Complex*)libData->MTensor_getComplexData(TX);
	double          *A = libData->MTensor_getRealData(TA);
	double          *B = libData->MTensor_getRealData(TB);
	double          *g = libData->MTensor_getRealData(Tg);

	/*Allocate Memory/Workspace*/
	MTensor TRes;
	mint dims[1]={Ng};
	mint rank=1;
	libData->MTensor_new(MType_Complex,rank,dims,&TRes);
	double _Complex *Y=(double _Complex*)libData->MTensor_getComplexData(TRes);
	//memset(Y,0,sizeof(double)*2*Ng);

	const int _LWORK=4001;
	assert(_LWORK>2*Nm);
	double _Complex work[_LWORK] __attribute__((aligned(64)));
	//double _Complex work[_LWORK];

	/*FFTW plans*/
	fftw_plan pf=fftw_plan_dft_1d(2*Nm,work,work,
			FFTW_FORWARD ,FFTW_MEASURE|FFTW_PATIENT);
	fftw_plan pb=fftw_plan_dft_1d(2*Nm,work,work,
			FFTW_BACKWARD,FFTW_MEASURE|FFTW_PATIENT);
	//fftw_execute(p);

	/*Compute Y*/
	for (int n = 0; n < Ns; n++)
		for (int i = 0; i < 2*Nd+1; i++)
			Y[i+n*(2*Nd+1)] = A[n] * X[i+n*(2*Nd+1)];
	// B[Ns,Ns,2*Nm] is row-major
	for (int n = 0; n < Ns; n++) {
		for (int np = 0; np < Ns; np++) {
			for (int i = 0; i < 2*Nd+1; i++)
				work[i]  = (mut-mus*g[i]) * X[i+(2*Nd+1)*np];
			memset(work+2*Nd+1,0,sizeof(double)*2*(2*Nm-2*Nd-1));
			fftw_execute(pf);
			for (int i = 0; i < 2*Nm; i++)
				work[i] *= B[i+(np+n*Ns)*2*Nm]; // row-major
			fftw_execute(pb);
			for (int i = 0; i < 2*Nd+1; i++)
				Y[i+n*(2*Nd+1)] += work[i];
		}
	}

	/*Send to LibraryLink*/
	MArgument_setMTensor(Res,TRes);

	/*Disown MTensor*/
	libData->MTensor_disown(TRes);

	/*Free FFTW plans/Workspace*/ 
	fftw_destroy_plan(pf);
	fftw_destroy_plan(pb);

	return LIBRARY_NO_ERROR;
}
/******************************************************************************/

static double det(const double *a1, const double *a2, 
		const double *b1, const double *b2)
{
	return (a1[0]-a2[0])*(b1[1]-b2[1]) - (a1[1]-a2[1])*(b1[0]-b2[0]);
}

DLLEXPORT int ArcSinhRule_MLL( WolframLibraryData libData, mint Argc, MArgument *Args, MArgument Res)
{
	int err = LIBRARY_NO_ERROR; 

	/*Receive from LibraryLink*/
	MTensor const Tp  = MArgument_getMTensor(Args[0]);	// triangle
	MTensor const Tp0 = MArgument_getMTensor(Args[1]);	// singular point
	MTensor const Tqu = MArgument_getMTensor(Args[2]);
	MTensor const Tqv = MArgument_getMTensor(Args[3]);
	double const *p   = libData->MTensor_getRealData(Tp );
	double const *p0  = libData->MTensor_getRealData(Tp0);
	double const *xu  = libData->MTensor_getRealData(Tqu);
	double const *xv  = libData->MTensor_getRealData(Tqv);
	mint const   *dimu= libData->MTensor_getDimensions(Tqu);
	mint const   *dimv= libData->MTensor_getDimensions(Tqv);
	const int     nu  = (int)dimu[1];
	const int     nv  = (int)dimv[1];
	double const *wu  = xu+nu;
	double const *wv  = xv+nv;

	/*Allocate Memory*/
	MTensor RES;
	const int N=nu*nv;
	mint dims[2]={3,3*N};
	mint rank=2;
	libData->MTensor_new(MType_Real,rank,dims,&RES);
	double *res = libData->MTensor_getRealData(RES);
	double *y   = res + 3*N;
	double *w   = res + 6*N;

	/*Construct ArcSinh Rule*/
	const int _LWORK=2000;
	assert(_LWORK>=N);
	double work[_LWORK];
	arcsinh_rule_atomic(res    ,y    ,w    ,p0,p  ,p+2,nu,xu,wu,nv,xv,wv,work);
	arcsinh_rule_atomic(res+N  ,y+N  ,w+N  ,p0,p+2,p+4,nu,xu,wu,nv,xv,wv,work);
	arcsinh_rule_atomic(res+2*N,y+2*N,w+2*N,p0,p+4,p  ,nu,xu,wu,nv,xv,wv,work);

	double tmp[4];
	tmp[0] = det(p+2,p ,p+4,p  );		// A
	tmp[1] = det(p,  p0,p+2,p  )*tmp[0];	// s1: p0 p1 p2
	tmp[2] = det(p+2,p0,p+4,p+2)*tmp[0];	// s2: p0 p2 p3
	tmp[3] = det(p+4,p0,p,  p+4)*tmp[0];	// s3: p0 p3 p1

	for (int j = 0; j < 3; j++)
		if (tmp[j+1]<0.0)
			for (int i = 0; i < N; i++)
				w[j*N+i] *= -1.0;

	/*Send to LibraryLink*/
	MArgument_setMTensor(Res,RES);

	/*Disown MTensor*/
	libData->MTensor_disown(RES);

	return LIBRARY_NO_ERROR;
}

DLLEXPORT int LegendreRule_MLL( WolframLibraryData libData, mint Argc, MArgument *Args, MArgument Res)
{
	int err = LIBRARY_NO_ERROR; 
	/*Receive from LibraryLink*/
	mint order;
	mreal xmin;
mreal xmax;
	order = MArgument_getInteger(Args[0]);
	xmin = MArgument_getReal(Args[1]);
	xmax = MArgument_getReal(Args[2]);

	/*Construct Legendre Rule*/
	int kind;
	double alpha;
	double beta;
	double *x;
	double *w;
	alpha = 0.0;
	beta = 0.0;
	kind = 1; 
	x = (double *) malloc( order * sizeof(double) );
	w = (double *) malloc( order * sizeof(double) );
	cgqf( order, kind, alpha, beta, xmin, xmax, x, w );

	/*Send to LibraryLink*/
	MTensor RES;
	mint dims[2]={2,order};
	mint rank=2;
	err = libData->MTensor_new( MType_Real, rank, dims, &RES );
	mreal *res;
	res = libData->MTensor_getRealData(RES);
	mint i;
	for (i = 0; i < order; i++) {
		res[i]		= (mreal) x[i];
		res[i+order]	= (mreal) w[i];
	}
	MArgument_setMTensor(Res,RES);

	/*Disown MTensor*/
	libData->MTensor_disown(RES);

	/*Free Memory*/
	free ( x );
	free ( w );

	return LIBRARY_NO_ERROR;
}

DLLEXPORT int DunavantRule_MLL( WolframLibraryData libData, mint Argc, MArgument *Args, MArgument Res)
{
	int err = LIBRARY_NO_ERROR;

	/*Receive from LibraryLink*/
mint rule;
	MTensor NODE_XY2;
	rule = MArgument_getInteger(Args[0]);
	NODE_XY2 = MArgument_getMTensor(Args[1]);
	double *node_xy2;
	node_xy2 = libData->MTensor_getRealData(NODE_XY2);

	/*Construct Dunavant Rule*/
	double area2;
	int node;
	int order;
	double node_xy[6] = {
		0.0, 0.0,
		1.0, 0.0,
		0.0, 1.0
	};
	int order_num;
	double *w;
	double *xy;
	double *xy2;
	area2 = triangle_area ( node_xy2 );
	order_num = dunavant_order_num ( rule );
	xy = (double*)malloc(sizeof(double)*2*order_num);
	xy2 = (double*)malloc(sizeof(double)*2*order_num);
	w = (double*)malloc(sizeof(double)*order_num);
	dunavant_rule ( rule, order_num, xy, w );
	reference_to_physical_t3 ( node_xy2, order_num, xy, xy2 );

	/*Send to LibraryLink*/
	MTensor RES;
	mint dims[2]={3,order_num};
	mint rank=2;
	libData->MTensor_new( MType_Real, rank, dims, &RES );
	mreal *res;
	res = libData->MTensor_getRealData(RES);
	mint i;
	for (i = 0; i < order_num; i++) {
		res[i+0*order_num] = (mreal) xy2[0+i*2];
		res[i+1*order_num] = (mreal) xy2[1+i*2];
		//res[i+2*order_num] = (mreal) w[i]; // normalized to area
		res[i+2*order_num] = (mreal) w[i]*fabs(area2);
	}
	MArgument_setMTensor(Res,RES);

	/*Disown MTensor*/
	libData->MTensor_disown(RES);

	/*Free Memory*/
	free(w);
	free(xy);
	free(xy2);

	return LIBRARY_NO_ERROR;
}
/******************************************************************************/
/*
 * Return the version of Library Link
 */
DLLEXPORT mint WolframLibrary_getVersion( ) 
{
	return WolframLibraryVersion;
}

/* 
 * Initialize Library
 */
DLLEXPORT int WolframLibrary_initialize( WolframLibraryData libData)
{
	return LIBRARY_NO_ERROR;
}

/* 
 * Uninitialize Library
 */
DLLEXPORT void WolframLibrary_uninitialize( WolframLibraryData libData)
{
	return;
} 
/******************************************************************************/
