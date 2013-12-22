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
void test01(int n);
void test02(int n);
void test03(int n);
void test04(int n);

int test05(void);
int test06(void);
/******************************************************************************/ 
using namespace QuadratureRules;
/******************************************************************************/ 
int main(int argc, char const* argv[])
{ 
	if (argc>1) link_stdout(argv[1]);

	test01(100);
	test02(100);
	test03(100);
	test04(100);

	if (argc>1) unlink_stdout();

	test05();
	test06();

	return 0;
} 

void test01( int n )
{
	printf("	Test Dunavant rules.\n");
	n = MIN(n,gDunavantRule.RuleNumber());
	n = MAX(n,1);
	struct st_quadrule *q;
	q=(struct st_quadrule*)mkl_malloc(sizeof(struct st_quadrule),MALLOC_ALIGNMENT);
	for (int rule = 1; rule <= n; rule++) {
		printf("rule = %d\n",rule);
		gDunavantRule.Generate(rule,q);
		print_quadrule(q);
	}
	mkl_free(q);
}

void test02( int n )
{
	printf("	Test Wandzura rules.\n");
	n = MIN(n,gWandzuraRule.RuleNumber());
	n = MAX(n,1);
	struct st_quadrule *q;
	q=(struct st_quadrule*)mkl_malloc(sizeof(struct st_quadrule),MALLOC_ALIGNMENT);
	for (int rule = 1; rule <= n; rule++) {
		printf("rule = %d\n",rule);
		gWandzuraRule.Generate(rule,q);
		print_quadrule(q);
	}
	mkl_free(q);
}

void test03( int n )
{
	printf("	Test Lyness rules.\n");
	n = MIN(n,gLynessRule.RuleNumber());
	n = MAX(n,1);
	struct st_quadrule *q;
	q=(struct st_quadrule*)mkl_malloc(sizeof(struct st_quadrule),MALLOC_ALIGNMENT);
	for (int rule = 1; rule <= n; rule++) {
		printf("rule = %d\n",rule);
		gLynessRule.Generate(rule,q);
		print_quadrule(q);
	}
	mkl_free(q);
}

void test04( int n ) 
{
	printf("	Test the Legendre rules.\n");
	printf("		A = -1.0\n");
	printf("		B = +1.0\n");
	n = MIN(n,20);
	n = MAX(n,1);
	struct st_quadrule *q;
	q=(struct st_quadrule*)mkl_malloc(sizeof(struct st_quadrule),MALLOC_ALIGNMENT);
	for (int order = 1; order <= n; order++) {
		gGaussRule.Generate(order,q); 
		print_quadrule(q);
	}
	mkl_free(q);
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

	double p[] = {1.,0.,0.,2.,3.,2.};
	double valf05=3.514983020486826813699771;

	struct st_quadrule q[3];
	gGaussRule.Generate(NU,q  );
	gGaussRule.Generate(NV,q+1);
	gArcSinhMethod.Init(q,q+1);
	gArcSinhMethod.GenerateAtomic(p,q+2);

	double f[3*NU*NV], val=0.0;
	for (int i = 0; i < q[2].n; i++)
		val += f05(q[2].x+2*i) * q[2].w[i];

	printf("true value\t=%19.16f\n",valf05);
	printf("my value\t=%19.16f\n"  ,val   );
	printf("NU\t\t=%3d\n",NU);
	printf("NV\t\t=%3d\n",NV);
	printf("#SD\t\t=%5.1f\n"       ,-log(fabs((val-valf05)/valf05))/log(10.0));

	ReleaseMemory();
	
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
	gGaussRule.Generate(NU,q  );
	gGaussRule.Generate(NV,q+1);

	gArcSinhMethod.Init(q,q+1);
	gArcSinhMethod.Generate(p0,p,q+2);

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
