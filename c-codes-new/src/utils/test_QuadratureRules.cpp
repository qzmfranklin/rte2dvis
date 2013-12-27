#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <string.h>
#include "QuadratureRules.h"
#include "utils.h"
/*****************************************************************************/
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
/*****************************************************************************/
int main(int argc, char const* argv[]);
int test01(void);
int test02(void);


int test05(void);
int test06(void);
/******************************************************************************/ 
using namespace QuadratureRules;
/******************************************************************************/ 
int main(int argc, char const* argv[])
{ 
	if (argc>1) link_stdout(argv[1]);

	test01();
	test02();
	test05();
	test06();

	if (argc>1) unlink_stdout();



	return 0;
} 

double f01(double *xy) { double x=xy[0], y=xy[1]; return pow(x,y); }
int test01(void)
{
	int err=0; 
        printf("TEST01\n");
        printf("	|Test WandzuraRule\n");

	const double p[]={1.,0.,0.,2.,3.,2.};
	const double valf01=5.259453011331279104629427;
	double f[200], val[6];
	struct st_quadrule q;

	WandzuraRule g1(6);
	for (int j = 0; j < 6; j++) {
		val[j]=0.0;
		g1.Reset(j+1);
		g1.Generate(&q,p); 
		print_quadrule(&q);
		for (int i = 0; i < q.n; i++)
			val[j] += f01(q.x+2*i) * q.w[i];
		val[j] *= q.a;
		val[j] = -log(fabs((val[j]-valf01)/valf01))/log(10);
	}
	printf("true value=%18.16f\n",valf01);
	print_vector("#SD",6,val);

        printf("END OF TEST01\n");
        printf("\n");
	return err;
}

double f02(double x) { return pow(x,15); }
int test02(void)
{
#define N 10
	int err=0; 
        printf("TEST02\n");
        printf("	|Test GaussRule\n");

	const double a=1.3,b=2.7;
	const double valf02=498536.110376646;

	struct st_quadrule q;
	GaussRule g1(50);
	g1.Print();
	g1.Generate(&q,a,b);
	print_quadrule(&q);

	double data[N];
	for (int j = 0; j < N; j++) {
		g1.Reset(j+1);
		g1.Generate(&q,a,b);
		double val=0.0;
		for (int i = 0; i < q.n; i++)
			val += f02(q.x[i]) * q.w[i];
		val *= q.a;
		data[j] = -log(fabs((val-valf02)/valf02));
	}

	printf("valf02\t=%f\n",valf02);
	print_vector("#SD",N,data); 



        printf("END OF TEST02\n");
        printf("\n");
	return err;
#undef N
}
/*****************************************************************************/
double f05(double *xy) { double x=xy[0],y=xy[1]; return x*y; }
int test05(void)
{
#define NU 10
#define NV 2
	int err=0; 
        printf("TEST05\n");
        printf("	|Test ArcSinhMethod::GenerateAtomic\n");

	const double p[] = {1.,0.,0.,2.,3.,2.};
	const double valf05=3.514983020486826813699771;

	struct st_quadrule q[3];

	GaussRule g1(NU),g2(NV);
	g1.Generate(q  ,0.0,1.0);
	g2.Generate(q+1,0.0,1.0);

	ArcSinhMethod g3;
	g3.Init(q,q+1);
	g3.GenerateAtomic(q+2,p);

	double f[3*NU*NV], val=0.0;
	for (int i = 0; i < q[2].n; i++)
		val += f05(q[2].x+2*i) * q[2].w[i];

	printf("true value\t=%19.16f\n",valf05);
	printf("my value\t=%19.16f\n"  ,val   );
	printf("NU\t\t=%3d\n",NU);
	printf("NV\t\t=%3d\n",NV);
	printf("#SD\t\t=%5.1f\n"       ,-log(fabs((val-valf05)/valf05))/log(10.0));

        printf("END OF TEST05\n");
        printf("\n");
	return err;
#undef NV
#undef NU
}

double f06(double *xy) { double x=xy[0],y=xy[1]; return x*y; }
int test06(void)
{
#define NU 10
#define NV 2
	int err=0; 
	printf("TEST06\n");
	printf("	|Test ArcSinhMethod::Generate\n");

	double p[] = {1.,0.,0.,2.,3.,2.};
	double p0[]= {0.,0.};
	double valf06=2.528273711783832823373392;

	struct st_quadrule q[3];

	GaussRule g1(NU),g2(NV);
	g1.Generate(q  ,0.0,1.0);
	g2.Generate(q+1,0.0,1.0);

	ArcSinhMethod g3;
	g3.Init(q,q+1);
	g3.Generate(q+2,p,p0);

	double val=0.0;
	for (int i = 0; i < q[2].n; i++)
		val += q[2].w[i] *  f06( q[2].x+2*i );

	printf("true value\t=%19.16f\n",valf06);
	printf("my value\t=%19.16f\n"  ,val   );
	printf("NU\t\t=%3d\n",NU);
	printf("NV\t\t=%3d\n",NV);
	printf("#SD\t\t=%5.1f\n"       ,-log(fabs((val-valf06)/valf06))/log(10.0)); 

	printf("END OF TEST06\n");
	printf("\n");
	return err;
#undef NU
#undef NV
}
/******************************************************************************/
