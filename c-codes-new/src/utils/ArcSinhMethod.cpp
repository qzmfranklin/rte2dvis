#include "ArcSinhMethod.h"
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <mkl.h>
/******************************************************************************/
#define MALLOC_ALIGNMENT 64
/******************************************************************************/
namespace QuadratureRules {
ArcSinhMethod gArcSinhMethod;

ArcSinhMethod::ArcSinhMethod()
{ 
	//fprintf(stderr,"ArcSinhMethod::ArcSinhMethod()\n");
	_fqu=NULL;
	_fqv=NULL;
	_fsize=0;
	_flwork=0;
	_fwork=NULL;

	status=0;
}

void ArcSinhMethod::Print()
{
	printf("status\t\t=%d (1=Init'd 2=Generated'd)\n",status);
	printf("_fxy.size\t=%lu\n",_fxy.size());
	printf("_fw.size\t=%lu\n",_fw.size());
	printf("_fsize\t\t=%lu\n",_fsize);
	printf("_flwork\t\t=%lu\n",_flwork);
	printf("_fwork\t\t=%p\n",_fwork);
	printf("_fqu\t\t=%p\n",_fqu);
	printf("_fqv\t\t=%p\n",_fqv);
}

void ArcSinhMethod::Init(struct st_quadrule *qu, struct st_quadrule *qv)
{
	//fprintf(stderr,"ArcSinhMethod::Init()\n");
	assert(qu->dim==1);
	assert(qv->dim==1); 

	_fqu = qu;
	_fqv = qv;
	size_t _size = qu->n * qv->n;

	double *_xy=(double*)mkl_malloc(sizeof(double)*6*_size,MALLOC_ALIGNMENT);
	double *_w =(double*)mkl_malloc(sizeof(double)*3*_size,MALLOC_ALIGNMENT);
	assert(_xy);
	assert(_w );

	_fxy.push(_xy);
	_fw.push(_w);

	double *_work;
	if (_size>_flwork) {
		mkl_free(_fwork);
		_work=(double*)mkl_malloc(sizeof(double)*_size,MALLOC_ALIGNMENT);
		assert(_work);
		_fwork = _work;
		_flwork = _size;
	} 
	_fsize = _size;

	status=1;
}

// Det[a1-a2,b1-b2]
static double det(const double *a1, const double *a2, 
		const double *b1, const double *b2)
{
	return (a1[0]-a2[0])*(b1[1]-b2[1]) - (a1[1]-a2[1])*(b1[0]-b2[0]);
}

void ArcSinhMethod::Generate(const double *restrict p0, const double *restrict p,
		struct st_quadrule *q)
{
	//fprintf(stderr,"ArcSinhMethod::Generate()\n");
	if (status==2) Init(_fqu,_fqv);
	assert(status==1);

	q->dim = 2;
	q->n   = 3*_flwork;
	q->x   = _fxy.top();
	q->w   = _fw.top();

	Atomic(p0,p  ,p+2,q->x          ,q->w          );
	Atomic(p0,p+2,p+4,q->x+2*_fsize,q->w+1*_fsize);
	Atomic(p0,p+4,p  ,q->x+4*_fsize,q->w+2*_fsize); 

	double tmp[4];
	tmp[0] = det(p+2,p ,p+4,p  );		// A
	tmp[1] = det(p,  p0,p+2,p  )*tmp[0];	// s1: p0 p1 p2
	tmp[2] = det(p+2,p0,p+4,p+2)*tmp[0];	// s2: p0 p2 p3
	tmp[3] = det(p+4,p0,p,  p+4)*tmp[0];	// s3: p0 p3 p1

	for (int j = 0; j < 3; j++)
		if (tmp[j+1]<0.0)
			for (int i = 0; i < _fsize; i++)
				q->w[j*_fsize+i] *= -1.0;

	status=2;
}

void ArcSinhMethod::GenerateAtomic(const double *restrict p, struct st_quadrule *q)
{
	//fprintf(stderr,"ArcSinhMethod::GenerateAtomic()\n");
	if (status==2) Init(_fqu,_fqv);
	assert(status==1);

	q->dim = 2;
	q->n   = _flwork;
	q->x   = _fxy.top();
	q->w   = _fw.top();

	Atomic(p,p+2,p+4,q->x,q->w);

	status=2;
}
void ArcSinhMethod::Atomic(const double *p0, const double *p1, 
		const double *p2, double *restrict x, double *restrict w)
{
	//fprintf(stderr,"ArcSinhMethod::GenerateAtomic()\n");
	assert(status==1);
	double *u;
	u = _fwork;

	double p0p1[2],p0p2[2],p1p2[2],invp1p2,Ah[4],xp[2],h,u0[2];
	p0p1[0] = p1[0] - p0[0]; // x1-x0
	p0p1[1] = p1[1] - p0[1]; // y1-y0
	p0p2[0] = p2[0] - p0[0]; // x2-x0
	p0p2[1] = p2[1] - p0[1]; // y2-y0
	p1p2[0]	= p2[0] - p1[0]; // x2-x1
	p1p2[1]	= p2[1] - p1[1]; // y2-y1
	invp1p2 = 1.0/sqrt(p1p2[0]*p1p2[0] + p1p2[1]*p1p2[1] ); // inverse p1p2
	p1p2[0]*= invp1p2;
	p1p2[1]*= invp1p2;
	h	= invp1p2*(-p1p2[0]*p0[1] + p0p2[0]*p1[1] - p0p1[0]*p2[1]);
	Ah[0]	= p1p2[0]*h;
	Ah[1]	= -p1p2[1]*h;
	Ah[2]	= p1p2[1]*h;
	Ah[3]	= p1p2[0]*h;
	xp[0]	= p1p2[0]*p0p1[0] + p1p2[1]*p0p1[1]; // x1p
	xp[1]	= p1p2[0]*p0p2[0] + p1p2[1]*p0p2[1]; // x2p
	u0[0]	= asinh(xp[0]/h); // u1
	u0[1]	= asinh(xp[1]/h)-u0[0]; // u2-u1

	int nu,nv;
	double *xu,*xv,*wu,*wv;
	nu = _fqu->n;
	nv = _fqv->n;
	xu = _fqu->x;
	xv = _fqv->x;
	wu = _fqu->w;
	wv = _fqv->w;

	for (int i = 0; i < nu; i++)
		u[i] = u0[0] + u0[1]*xu[i]; 

	for (int j = 0; j < nv; j++)
		for (int i = 0; i < nu; i++)
			// h(u1-u2) is absorbed into weights.
			w[i+nu*j] = fabs(h*u0[1])*wu[i]*wv[j]; 

	for (int j = 0; j < nv; j++)
		for (int i = 0; i < nu; i++) { 
			int k=2*(i+nu*j);
			x[k]   = p0[0] + xv[j]*( Ah[0]*sinh(u[i]) + Ah[1] );
			x[k+1] = p0[1] + xv[j]*( Ah[2]*sinh(u[i]) + Ah[3] );
		} 
}

void ArcSinhMethod::ReleaseMemory() {
	//fprintf(stderr,"ArcSinhMethod::ReleaseMemory()\n");
	while ( !_fxy.empty() ) { 
		mkl_free( _fxy.top() );
		_fxy.pop();
	}
	while ( !_fw.empty() ) { 
		mkl_free( _fw.top() );
		_fw.pop();
	}
	if (!_flwork) {
		mkl_free(_fwork);
		_flwork=0;
	}

	status=-1;
}
	
} // namespace QuadratureRules
/******************************************************************************/
