#include "int_trig.h"
#include "utils.h"
#include "QuadratureRules.h"
#include <stdio.h>
#include <math.h>
#include <mkl.h>
/******************************************************************************/
#define MALLOC_ALIGNMENT 64
/******************************************************************************/
double f(double x, double y) { return x*x + 0.1*y + exp(-x-0.5*y*y); }

int test01(void);
int test02(void);

/******************************************************************************/
int main(int argc, char const* argv[])
{
	test01();
	
	return 0;
}


int test01(void)
{
	int err=0; 
        printf("TEST01\n");
        printf("	|Test struct st_quadrule\n");

	struct st_quadrule q;
	q.flag = 1;
	printf("sizeof(q)\t=%lu\n",sizeof(q));

        printf("END OF TEST01\n");
        printf("\n");
	return err;
}

int test02(void)
{
#define N 20
	using namespace QuadratureRules;
	int err=0; 
        printf("TEST02\n");
        printf("	|Test dit_symmetric\n");

	double p[] = {1.,0.,0.,2.,3.,2.};
	double val[N];

	struct st_quadrule *q;
	q=(struct st_quadrule*)mkl_malloc(sizeof(struct st_quadrule),MALLOC_ALIGNMENT);
	for (int i = 0; i < N; i++) {
		gDunavantRule.Generate(i+1,q);
		val[i] = dit_symmetric(q,p,&f);
	}

	print_vector("val",N,val);
	mkl_free(q);
	
        printf("END OF TEST02\n");
        printf("\n");
	return err;
#undef N
}

/******************************************************************************/
