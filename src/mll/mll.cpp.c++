#include "QuadratureRules.h"
#include "WolframLibrary.h" 
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include "mll.h"
/******************************************************************************/
/*
 * Input:
 * 	in		Real
 * Output:
 * 	out		Real
 */
DLLEXPORT int test( WolframLibraryData libData, mint Argc, 
		MArgument *Args, MArgument Res)
{
	double in,out;
	in = MArgument_getReal(Args[0]);
	out = in*in;

	MArgument_setReal(Res,out);

	return 0; 
}

/* Input:	
 * 	order		Integer
 * 	xmin		Real
 * 	xmax		Real
 * Output:
 * 	RES		{Real,2,"Shared"}
 */
DLLEXPORT int LegendreRule_MLL( WolframLibraryData libData, mint Argc, 
		MArgument *Args, MArgument Res)
{
	int err = LIBRARY_NO_ERROR; 
	/*Receive from LibraryLink*/
	mint order;
	double xmin;
	double xmax;
	order = MArgument_getInteger(Args[0]);
	xmin = MArgument_getReal(Args[1]);
	xmax = MArgument_getReal(Args[2]);

	/*Construct Legendre Rule*/
	struct st_quadrule q;
	using namespace QuadratureRules;
	GaussRule g((int)order);
	g.Generate(&q,xmin,xmax);

	/*Send to LibraryLink*/
	double *res, *x=q.x, *w=q.w;
	MTensor RES;
	mint dims[2]={2,order};
	mint rank=2;
	err = libData->MTensor_new( MType_Real, rank, dims, &RES );
	res = libData->MTensor_getRealData(RES);
	for (int i = 0; i < order; i++) {
		res[i]		= 2.0* (double) x[i];
		res[i+order]	= 2.0* (double) w[i];
	}
	MArgument_setMTensor(Res,RES);

	/*Disown MTensor*/
	libData->MTensor_disown(RES);

	return LIBRARY_NO_ERROR;
}

/*
 * Input:
 * 	rule		Integer
 * 	mp		{Real,2,"Constant"}
 * Output:
 * 	RES		{Real,2,"Shared"}
 */
DLLEXPORT int WandzuraRule_MLL( WolframLibraryData libData, mint Argc, 
		MArgument *Args, MArgument Res)
{
	int err = LIBRARY_NO_ERROR;

	/*Receive from LibraryLink*/
	mint rule;
	MTensor mp;
	double *p;
	rule = MArgument_getInteger(Args[0]);
	mp   = MArgument_getMTensor(Args[1]);
	p    = libData->MTensor_getRealData(mp);

	/*Construct Dunavant Rule*/
	struct st_quadrule q;
	using namespace QuadratureRules;
	WandzuraRule g((int)rule);
	g.Generate(&q,p);

	/*Send to LibraryLink*/
	int N=g.Order();
	double *res, *xy=q.x, *w=q.w;
	MTensor RES;
	mint dims[2]={3,N};
	mint rank=2;
	libData->MTensor_new( MType_Real, rank, dims, &RES );
	res = libData->MTensor_getRealData(RES);
	for (int i = 0; i < N; i++) {
		res[i    ] = (double) xy[0+i*2];
		res[i+1*N] = (double) xy[1+i*2];
		res[i+2*N] = (double) w[i]; // normalized to 1.0
		/**res[i+2*order_num] = (mreal) w[i]*fabs(area2);*/
	}
	MArgument_setMTensor(Res,RES);

	/*Disown MTensor*/
	libData->MTensor_disown(RES);

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
