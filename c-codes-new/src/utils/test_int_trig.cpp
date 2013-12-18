#include "int_trig.h"
#include <complex.h>
#include <string.h>
#include "utils.h"
#include "QuadratureRules.h"
#include <stdio.h>
#include <math.h>
#include <mkl.h>
/******************************************************************************/
#define MALLOC_ALIGNMENT 64
/******************************************************************************/
using namespace QuadratureRules;
static double p[] = {1.,0.,0.,2.,3.,2.};
static double p0[]= {1.,3.};
static double f02(double x, double y) { 
	return log(x*x+exp(x*y));
}
static double _Complex f04(double x, double y) { 
	return cexp(I*( x*y*log(x+3)+y*y));
}
static double f06(double x, double y) { 
	return x*y+x;
}
static double f07(double x, double y) {
	return x;
}
static double _Complex f08(double x, double y) {
	return 2*x*y+(x*x-y*y)*I;
}
static double _Complex f09(double x, double y) {
	return 2*x*y+(x*x-y*y)*I;
}
static double true_val_f02=6.2340491877461960211;
static double _Complex true_val_f04=-0.2986424021019246+0.1503820269098639*I;
static double true_val_f06=6.83251299189152016285147;
static double true_val_f07=2.25338772238102952694357;
static double _Complex true_val_f08=7.0299660409736428635+1.0905215459986228247*I;
static double _Complex true_val_f09=6.5318853202466288449-0.2741522601456360062I;
/******************************************************************************/ 
int test01(void);
int test02(void);
int test03(void);
int test04(void);
int test05(void);
int test06(void);
int test07(void);
int test08(void);
int test09(void);
int testxx(void);
/******************************************************************************/
int main(int argc, char const* argv[])
{
	if (argc>1) link_stdout(argv[1]);

	test01();
	test02();
	test03();
	test04();
	test05();
	test06();
	test07();
	test08();
	test09();
	//testxx();

	if (argc>1) unlink_stdout();

	return 0;
}


int test01(void)
{
	int err=0; 
        printf("TEST01\n");
        printf("	|Test struct st_quadrule\n");

	struct st_quadrule q;
	printf("sizeof(q)\t=%lu\n",sizeof(q));

        printf("END OF TEST01\n");
        printf("\n");
	return err;
}

int test02(void)
{
#define N 20
#define COUNT 20
	int err=0; 
        printf("TEST02\n");
        printf("	|Test dit_symmetric DunavantRule\n");

	double data[3*N];
	double work[400];
	TimeStamp clk(COUNT);

	struct st_quadrule *q;
	q=(struct st_quadrule*)mkl_malloc(sizeof(struct st_quadrule),MALLOC_ALIGNMENT);
	for (int i = 0; i < N; i++) {
		gDunavantRule.Generate(i+1,q);
		data[i]   = q->n;
		data[N+i] = -log( fabs( (dit_symmetric(q,p,&f02,work) 
					 -true_val_f02)/true_val_f02)
				) / log(10);
		clk.flush();
		for (int j = 0; j < COUNT; j++) {
			clk.tic();
			dit_symmetric(q,p,&f02,work);
			clk.toc();
		}
		data[2*N+i] = clk.median()/data[i];
	}
	mkl_free(q);
	
	Table tbl;
	const char *rows[N]={"Dunavant1","Dunavant2","Dunavant3","Dunavant4",
		"Dunavant5","Dunavant6","Dunavant7","Dunavant8","Dunavant9",
		"Dunavant10","Dunavant11","Dunavant12","Dunavant13",
		"Dunavant14","Dunavant15","Dunavant16","Dunavant17",
		"Dunavant18","Dunavant19","Dunavant20"};
	const char *cols[3]={"order","precision in #SD","time in cycles/sample point"};
	tbl.set_width(20);
	tbl.dim(N,3);
	tbl.rows(rows);
	tbl.cols(cols);
	tbl.data(data);
	char banner[BUFSIZ];
	sprintf(banner,"Precision in #SD");
	tbl.print(banner);
	
        printf("END OF TEST02\n");
        printf("\n");
	return err;
#undef COUNT
#undef N
}

int test03(void)
{
#define N 6
#define COUNT 20
	int err=0; 
        printf("TEST03\n");
        printf("	|Test dit_symmetric WandzuraRule\n");
	
	double data[3*N];
	double work[1000];
	TimeStamp clk(COUNT);

	struct st_quadrule *q;
	q=(struct st_quadrule*)mkl_malloc(sizeof(struct st_quadrule),MALLOC_ALIGNMENT);
	for (int i = 0; i < N; i++) {
		gWandzuraRule.Generate(i+1,q);
		data[i]   = q->n;
		data[N+i] = -log( fabs( (dit_symmetric(q,p,&f02,work) 
					 -true_val_f02)/true_val_f02)
				) / log(10);
		clk.flush();
		for (int j = 0; j < COUNT; j++) {
			clk.tic();
			dit_symmetric(q,p,&f02,work);
			clk.toc();
		}
		data[2*N+i] = clk.median()/data[i];
	}
	mkl_free(q);
	
	Table tbl;
	const char *rows[N]={"Wandzura1","Wandzura2","Wandzura3","Wandzura4",
		"Wandzura5","Wandzura6"};
	const char *cols[3]={"order","precision in #SD","time in cycles/sample point"};
	tbl.set_width(20);
	tbl.dim(N,3);
	tbl.rows(rows);
	tbl.cols(cols);
	tbl.data(data);
	char banner[BUFSIZ];
	sprintf(banner,"Precision in #SD");
	tbl.print(banner);
        printf("END OF TEST03\n");
        printf("\n");
	return err;
#undef COUNT
#undef N
}

int test04(void)
{
#define N 20
#define COUNT 20
	int err=0; 
        printf("TEST04\n");
        printf("	|Test zit_symmetric DunavantRule\n");
	double data[3*N];
	double work[400];
	TimeStamp clk(COUNT);

	struct st_quadrule *q;
	q=(struct st_quadrule*)mkl_malloc(sizeof(struct st_quadrule),MALLOC_ALIGNMENT);
	for (int i = 0; i < N; i++) {
		gDunavantRule.Generate(i+1,q);
		data[i]   = q->n;
		data[N+i] = -log( cabs( (zit_symmetric(q,p,&f04,work) 
					 -true_val_f04)/true_val_f04)
				) / log(10);
		clk.flush();
		for (int j = 0; j < COUNT; j++) {
			clk.tic();
			zit_symmetric(q,p,&f04,work);
			clk.toc();
		}
		data[2*N+i] = clk.median()/data[i];
	}
	mkl_free(q);
	
	Table tbl;
	const char *rows[N]={"Dunavant1","Dunavant2","Dunavant3","Dunavant4",
		"Dunavant5","Dunavant6","Dunavant7","Dunavant8","Dunavant9",
		"Dunavant10","Dunavant11","Dunavant12","Dunavant13",
		"Dunavant14","Dunavant15","Dunavant16","Dunavant17",
		"Dunavant18","Dunavant19","Dunavant20"};
	const char *cols[3]={"order","precision in #SD","time in cycles/sample point"};
	tbl.set_width(20);
	tbl.dim(N,3);
	tbl.rows(rows);
	tbl.cols(cols);
	tbl.data(data);
	char banner[BUFSIZ];
	sprintf(banner,"Precision in #SD");
	tbl.print(banner);
	
        printf("END OF TEST04\n");
        printf("\n");
	return err; 
#undef COUNT
#undef N
}

int test05(void)
{
#define N 6
#define COUNT 20
	int err=0; 
        printf("TEST05\n");
        printf("	|Test zit_symmetric WandzuraRule\n");
	
	double data[3*N];
	double work[1000];
	TimeStamp clk(COUNT);

	struct st_quadrule *q;
	q=(struct st_quadrule*)mkl_malloc(sizeof(struct st_quadrule),MALLOC_ALIGNMENT);
	for (int i = 0; i < N; i++) {
		gWandzuraRule.Generate(i+1,q);
		data[i]   = q->n;
		data[N+i] = -log( cabs( (zit_symmetric(q,p,&f04,work) 
					 -true_val_f04)/true_val_f04)
				) / log(10);
		clk.flush();
		for (int j = 0; j < COUNT; j++) {
			clk.tic();
			zit_symmetric(q,p,&f04,work);
			clk.toc();
		}
		data[2*N+i] = clk.median()/data[i];
	}
	mkl_free(q);
	
	Table tbl;
	const char *rows[N]={"Wandzura1","Wandzura2","Wandzura3","Wandzura4",
		"Wandzura5","Wandzura6"};
	const char *cols[3]={"order","precision in #SD","time in cycles/sample point"};
	tbl.set_width(20);
	tbl.dim(N,3);
	tbl.rows(rows);
	tbl.cols(cols);
	tbl.data(data);
	char banner[BUFSIZ];
	sprintf(banner,"Precision in #SD");
	tbl.print(banner);
        printf("END OF TEST05\n");
        printf("\n");
	return err;
#undef COUNT
#undef N
}

int test06(void)
{
#define NU 10
#define NV 6
#define COUNT 20
	int err=0; 
        printf("TEST06\n");
        printf("	|Test dit_arcsinh_atomic Legendre X Legendre\n");
	double precision[NU*NV];
	double time[NU*NV];
	TimeStamp clk(COUNT);

	struct st_quadrule *qu[NU],*qv[NV]; 
	for (int i = 0; i < NU; i++) { 
		qu[i]=(struct st_quadrule*)mkl_malloc(
				sizeof(struct st_quadrule),MALLOC_ALIGNMENT);
		gGaussRule.Generate(i+1,qu[i]);
	}
	for (int i = 0; i < NV; i++) {
		qv[i]=(struct st_quadrule*)mkl_malloc(
				sizeof(struct st_quadrule),MALLOC_ALIGNMENT);
		gGaussRule.Generate(i+1,qv[i]);
	}

	double work[2000]; // workspace
	for (int j = 0; j < NV; j++)
		for (int i = 0; i < NU; i++) {
			precision[i+j*NU] = -log( fabs(
						(dit_arcsinh_atomic(qu[i],qv[j],p,&f06,work) 
						 -true_val_f06)/true_val_f06)
					) / log(10);
			//precision[i+j*NU] = dit_arcsinh_atomic(qu[i],qv[j],p,&f06,work);
			clk.flush();
			for (int k = 0; k < COUNT; k++) {
				clk.tic();
				dit_arcsinh_atomic(qu[i],qv[j],p,&f06,work);
				clk.toc();
			}
			time[i+j*NU] = clk.median()/(i*j);
		}

	for (int i = 0; i < NU; i++)
		mkl_free(qu[i]);
	for (int i = 0; i < NV; i++)
		mkl_free(qv[i]);
	
	Table tbl[2];
	char banner[BUFSIZ];
	const char *rows[NU]={"1","2","3","4",
		"5","6","7","8","9","10"};
	const char *cols[NV]={"1","2","3","4","5","6"};
	tbl[0].set_width(12);
	tbl[0].dim(NU,NV);
	tbl[0].rows(rows);
	tbl[0].cols(cols);
	tbl[0].data(precision);
	sprintf(banner,"Precision in #SD");
	tbl[0].print(banner);


	tbl[1].set_width(12);
	tbl[1].dim(NU,NV);
	tbl[1].rows(rows);
	tbl[1].cols(cols);
	tbl[1].data(time);
	sprintf(banner,"Time in cycles/sample point");
	tbl[1].print(banner);
	
        printf("END OF TEST06\n");
        printf("\n");
	return err; 
#undef COUNT
#undef NV
#undef NU
}

int test07(void)
{
#define NU 10
#define NV 6
#define COUNT 20
	int err=0; 
        printf("TEST07\n");
        printf("	|Test dit_arcsinh Legendre X Legendre\n");
	double precision[NU*NV];
	double time[NU*NV];
	TimeStamp clk(COUNT);

	struct st_quadrule *qu[NU],*qv[NV]; 
	for (int i = 0; i < NU; i++) { 
		qu[i]=(struct st_quadrule*)mkl_malloc(
				sizeof(struct st_quadrule),MALLOC_ALIGNMENT);
		gGaussRule.Generate(i+1,qu[i]);
	}
	for (int i = 0; i < NV; i++) {
		qv[i]=(struct st_quadrule*)mkl_malloc(
				sizeof(struct st_quadrule),MALLOC_ALIGNMENT);
		gGaussRule.Generate(i+1,qv[i]);
	}

	double work[2000]; // workspace
	for (int j = 0; j < NV; j++)
		for (int i = 0; i < NU; i++) {
			precision[i+j*NU] = -log( fabs(
						(dit_arcsinh(qu[i],qv[j],p,p0,&f07,work) 
						 -true_val_f07)/true_val_f07)
					) / log(10);
			//precision[i+j*NU] = dit_arcsinh(qu[i],qv[j],p,p0,&f07,work);
			clk.flush();
			for (int k = 0; k < COUNT; k++) {
				clk.tic();
				dit_arcsinh(qu[i],qv[j],p,p0,&f07,work);
				clk.toc();
			}
			time[i+j*NU] = clk.median()/(3*i*j);
		}

	for (int i = 0; i < NU; i++)
		mkl_free(qu[i]);
	for (int i = 0; i < NV; i++)
		mkl_free(qv[i]);
	
	Table tbl[2];
	char banner[BUFSIZ];
	const char *rows[NU]={"1","2","3","4",
		"5","6","7","8","9","10"};
	const char *cols[NV]={"1","2","3","4","5","6"};
	tbl[0].set_width(12);
	tbl[0].dim(NU,NV);
	tbl[0].rows(rows);
	tbl[0].cols(cols);
	tbl[0].data(precision);
	sprintf(banner,"Precision in #SD");
	tbl[0].print(banner);


	tbl[1].set_width(12);
	tbl[1].dim(NU,NV);
	tbl[1].rows(rows);
	tbl[1].cols(cols);
	tbl[1].data(time);
	sprintf(banner,"Time in cycles/sample point");
	tbl[1].print(banner);
	
        printf("END OF TEST07\n");
        printf("\n");
	return err; 
#undef COUNT
#undef NV
#undef NU
}

int test08(void)
{
#define NU 10
#define NV 6
#define COUNT 20
	int err=0; 
        printf("TEST08\n");
        printf("	|Test zit_arcsinh_atomic Legendre X Legendre\n");
	double precision[NU*NV];
	double time[NU*NV];
	TimeStamp clk(COUNT);

	struct st_quadrule *qu[NU],*qv[NV]; 
	for (int i = 0; i < NU; i++) { 
		qu[i]=(struct st_quadrule*)mkl_malloc(
				sizeof(struct st_quadrule),MALLOC_ALIGNMENT);
		gGaussRule.Generate(i+1,qu[i]);
	}
	for (int i = 0; i < NV; i++) {
		qv[i]=(struct st_quadrule*)mkl_malloc(
				sizeof(struct st_quadrule),MALLOC_ALIGNMENT);
		gGaussRule.Generate(i+1,qv[i]);
	}

	double work[2000]; // workspace
	for (int j = 0; j < NV; j++)
		for (int i = 0; i < NU; i++) {
			precision[i+j*NU] = -log( cabs(
						(zit_arcsinh_atomic(qu[i],qv[j],p,&f08,work) 
						 -true_val_f08)/true_val_f08)
					) / log(10);
			//precision[i+j*NU] = dit_arcsinh_atomic(qu[i],qv[j],p,&f08,work);
			clk.flush();
			for (int k = 0; k < COUNT; k++) {
				clk.tic();
				zit_arcsinh_atomic(qu[i],qv[j],p,&f08,work);
				clk.toc();
			}
			time[i+j*NU] = clk.median()/(i*j);
		}

	for (int i = 0; i < NU; i++)
		mkl_free(qu[i]);
	for (int i = 0; i < NV; i++)
		mkl_free(qv[i]);
	
	Table tbl[2];
	char banner[BUFSIZ];
	const char *rows[NU]={"1","2","3","4",
		"5","6","7","8","9","10"};
	const char *cols[NV]={"1","2","3","4","5","6"};
	tbl[0].set_width(12);
	tbl[0].dim(NU,NV);
	tbl[0].rows(rows);
	tbl[0].cols(cols);
	tbl[0].data(precision);
	sprintf(banner,"Precision in #SD");
	tbl[0].print(banner);


	tbl[1].set_width(12);
	tbl[1].dim(NU,NV);
	tbl[1].rows(rows);
	tbl[1].cols(cols);
	tbl[1].data(time);
	sprintf(banner,"Time in cycles/sample point");
	tbl[1].print(banner);
	
        printf("END OF TEST08\n");
        printf("\n");
	return err; 
#undef COUNT
#undef NV
#undef NU
}

int test09(void)
{
#define NU 10
#define NV 6
#define COUNT 20
	int err=0; 
        printf("TEST09\n");
        printf("	|Test zit_arcsinh Legendre X Legendre\n");
	double precision[NU*NV];
	double time[NU*NV];
	TimeStamp clk(COUNT);

	struct st_quadrule *qu[NU],*qv[NV]; 
	for (int i = 0; i < NU; i++) { 
		qu[i]=(struct st_quadrule*)mkl_malloc(
				sizeof(struct st_quadrule),MALLOC_ALIGNMENT);
		gGaussRule.Generate(i+1,qu[i]);
	}
	for (int i = 0; i < NV; i++) {
		qv[i]=(struct st_quadrule*)mkl_malloc(
				sizeof(struct st_quadrule),MALLOC_ALIGNMENT);
		gGaussRule.Generate(i+1,qv[i]);
	}

	double work[2000]; // workspace
	for (int j = 0; j < NV; j++)
		for (int i = 0; i < NU; i++) {
			precision[i+j*NU] = -log( cabs(
						(zit_arcsinh(qu[i],qv[j],p,p0,&f09,work) 
						 -true_val_f09)/true_val_f09)
					) / log(10);
			//precision[i+j*NU] = dit_arcsinh(qu[i],qv[j],p,p0,&f09,work);
			clk.flush();
			for (int k = 0; k < COUNT; k++) {
				clk.tic();
				zit_arcsinh(qu[i],qv[j],p,p0,&f09,work);
				clk.toc();
			}
			time[i+j*NU] = clk.median()/(3*i*j);
		}

	for (int i = 0; i < NU; i++)
		mkl_free(qu[i]);
	for (int i = 0; i < NV; i++)
		mkl_free(qv[i]);
	
	Table tbl[2];
	char banner[BUFSIZ];
	const char *rows[NU]={"1","2","3","4",
		"5","6","7","8","9","10"};
	const char *cols[NV]={"1","2","3","4","5","6"};
	tbl[0].set_width(12);
	tbl[0].dim(NU,NV);
	tbl[0].rows(rows);
	tbl[0].cols(cols);
	tbl[0].data(precision);
	sprintf(banner,"Precision in #SD");
	tbl[0].print(banner);


	tbl[1].set_width(12);
	tbl[1].dim(NU,NV);
	tbl[1].rows(rows);
	tbl[1].cols(cols);
	tbl[1].data(time);
	sprintf(banner,"Time in cycles/sample point");
	tbl[1].print(banner);
	
        printf("END OF TEST09\n");
        printf("\n");
	return err; 
#undef COUNT
#undef NV
#undef NU
}

int testxx(void)
{
#define NU 5
#define NV 2
	int err=0; 
        printf("TESTxx\n");
        printf("	|Test zit_arcsinh_atomic Legendre X Legendre\n");

	struct st_quadrule *qu,*qv; 
	qu=(struct st_quadrule*)mkl_malloc(
			sizeof(struct st_quadrule),MALLOC_ALIGNMENT);
	qv=(struct st_quadrule*)mkl_malloc(
			sizeof(struct st_quadrule),MALLOC_ALIGNMENT);
	gGaussRule.Generate(NU,qu);
	gGaussRule.Generate(NV,qv);

	double work[2000]; // workspace
	double _Complex val;
	val = zit_arcsinh_atomic(qu,qv,p,&f08,work);

	fprintf(stderr, "true val= %f + %f*I\n",creal(true_val_f08),cimag(true_val_f08));
	fprintf(stderr, "val     = %f + %f*I\n",creal(val),cimag(val));
	
        printf("END OF TESTxx\n");
        printf("\n");
	return err; 
#undef NV
#undef NU
}
/******************************************************************************/
