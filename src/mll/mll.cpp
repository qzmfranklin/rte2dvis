#include "WolframLibrary.h" 
#include "mll.h"
#include "legendre-rule.h"
#include "dunavant-rule.h" 
#include <stdio.h>
#include <fftw3.h>
#include <malloc.h>
#include <math.h>
#include <stdlib.h>
/******************************************************************************/
/* Input:	
 * 	order		Integer
 * 	xmin		Real
 * 	xmax		Real
 * Output:
 * 	RES		{Real,2,"Shared"}
 */
DLLEXPORT int ArcSinhRule_MLL( WolframLibraryData libData, mint Argc, 
		MArgument *Args, MArgument Res)
{
	int err = LIBRARY_NO_ERROR; 

	/*Receive from LibraryLink*/
	mint rule;
	MTensor NODE_XY2;
	rule = MArgument_getInteger(Args[0]);
	NODE_XY2 = MArgument_getMTensor(Args[1]);
	double *node_xy2;
	node_xy2 = libData->MTensor_getRealData(NODE_XY2);

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
		res[i+2*order_num] = (mreal) w[i]; // normalized to 1.0
		/**res[i+2*order_num] = (mreal) w[i]*fabs(area2);*/
	}
	MArgument_setMTensor(Res,RES);

	/*Disown MTensor*/
	libData->MTensor_disown(RES);

	return LIBRARY_NO_ERROR;
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

/*
 * Input:
 * 	rule		Integer
 * 	NODE_XY2	{Real,2,"Constatn"}
 * Output:
 * 	RES		{Real,2,"Shared"}
 */
DLLEXPORT int DunavantRule_MLL( WolframLibraryData libData, mint Argc, 
		MArgument *Args, MArgument Res)
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
		res[i+2*order_num] = (mreal) w[i]; // normalized to 1.0
		/**res[i+2*order_num] = (mreal) w[i]*fabs(area2);*/
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
