#include "int_trig.h"
#include "utils.h"
#include "QuadratureRules.h"
#include <stdio.h>
#include <math.h>
#include <mkl.h>
/******************************************************************************/
#define MALLOC_ALIGNMENT 64
/******************************************************************************/
static double f(double x, double y) { return x*x + 0.1*y + exp(-x-0.5*y*y); }

int test01(void);
int test02(void);

/******************************************************************************/
int main(int argc, char const* argv[])
{
	test01();
	test02();
	
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
	double val[2*N];

	struct st_quadrule *q;
	q=(struct st_quadrule*)mkl_malloc(sizeof(struct st_quadrule),MALLOC_ALIGNMENT);
	printf("sizeof(*q)\t=%lu\n",sizeof(*q));
	for (int i = 0; i < N; i++) {
		gDunavantRule.Generate(i+1,q);
		val[i]   = q->n;
		val[N+i] = dit_symmetric(q,p,&f);
	}
	mkl_free(q);
	
	Table tbl;
	const char *rows[N]={"1","2","3","4","5","6","7","8","9","10",
		"11","12","13","14","15","16","17","18","19","20"};
	const char *cols[2]={"order","value"};
	tbl.set_width(20);
	tbl.dim(N,2);
	tbl.rows(rows);
	tbl.cols(cols);
	tbl.data(val);
	char banner[BUFSIZ]=
		"Double Precision Numerical Integration on \n\tTriangle using Symmetric Quadrature Rules";
	tbl.print(banner);
	
        printf("END OF TEST02\n");
        printf("\n");
	return err;
#undef N
}

/******************************************************************************/
