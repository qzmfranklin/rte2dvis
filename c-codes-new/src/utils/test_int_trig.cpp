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
static double f02(double x, double y) { 
	return log(x*x+exp(x*y));
}
static double true_val_f02 = 6.2340491877461960211;

static double _Complex f03(double x, double y) { 
	return sqrt(fabs(x))+sqrt(fabs(y))*x*I; 
}

int test01(void);
int test02(void);
int test03(void);
/******************************************************************************/
int main(int argc, char const* argv[])
{
	char fname[FILENAME_MAX];
	if (!argc)
		sprintf(fname,"DEBUG/test_int_trig.txt");
	else
		strcpy(fname,argv[1]);

	link_stdout(fname);
	test01();
	unlink_stdout(); 

	link_cout(fname);
	test02();
	test03();
	unlink_cout();
	
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
	printf("sizeof(*q)\t=%lu\n",sizeof(*q));
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
			for (int k = 0; k < 1000; k++) {
				int i=1;
			}
			clk.toc();
		}
		data[2*N+i] = clk.median();
	}
	mkl_free(q);
	
	Table tbl;
	const char *rows[N]={"Dunavant1","Dunavant2","Dunavant3","Dunavant4",
		"Dunavant5","Dunavant6","Dunavant7","Dunavant8","Dunavant9",
		"Dunavant10","Dunavant11","Dunavant12","Dunavant13",
		"Dunavant14","Dunavant15","Dunavant16","Dunavant17",
		"Dunavant18","Dunavant19","Dunavant20"};
	const char *cols[3]={"order","precision in #SD","time in cycles"};
	tbl.set_width(20);
	tbl.dim(N,3);
	tbl.rows(rows);
	tbl.cols(cols);
	tbl.data(data);
	char banner[BUFSIZ];
	sprintf(banner,"Double Precision Numerical Integration on \n\tTriangle using Symmetric Quadrature Rules\n\t\tTrue value = %20.15f",true_val_f02);
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
	printf("sizeof(*q)\t=%lu\n",sizeof(*q));
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
			for (int k = 0; k < 1000; k++) {
				int i=1;
			}
			clk.toc();
		}
		data[2*N+i] = clk.median();
	}
	mkl_free(q);
	
	Table tbl;
	const char *rows[N]={"Wandzura1","Wandzura2","Wandzura3","Wandzura4",
		"Wandzura5","Wandzura6"};
	const char *cols[3]={"order","precision in #SD","time in cycles"};
	tbl.set_width(20);
	tbl.dim(N,3);
	tbl.rows(rows);
	tbl.cols(cols);
	tbl.data(data);
	char banner[BUFSIZ];
	sprintf(banner,"Double Precision Numerical Integration on \n\tTriangle using Symmetric Quadrature Rules\n\t\tTrue value = %20.15f",true_val_f02);
	tbl.print(banner);
        printf("END OF TEST03\n");
        printf("\n");
	return err;
#undef COUNT
#undef N
}
/******************************************************************************/
