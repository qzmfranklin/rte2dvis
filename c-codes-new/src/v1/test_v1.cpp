#include <complex.h>
#include <stdio.h>
#include <mkl.h>
#include "utils.h"
#include "file_io.h"
#include "QuadratureRules.h"
#include "toeplitz.h"
#include "v1.h"
/******************************************************************************/
int test01(void);
/******************************************************************************/
int main(int argc, char const* argv[])
{
	if (argc>1) link_stdout(argv[1]);

	if (argc>1) unlink_stdout();

	test01();

	return 0;
}

int test01(void)
{
	int err=0; 
        printf("TEST01\n");
        printf("	|Test work flow of rte2dvis\n");

	struct st_rte2dvis_info solver;
	char *fbase="example/square162.binary";
	const int Nd=0, pad=0, flag=1;

	init_rte2dvis(solver,fbase,Nd,pad,flag);
	print_vector("pt",50,solver.mesh->trigs);
	//print_vector("area",50,solver.area);
	//print_vector("cntr",50,solver.cntr);

	assert(!check_rte2dvis(solver));

	alloc_rte2dvis_v1(solver);
	//print_rte2dvis(solver);

	fill_rte2dvis_v1(solver);
	print_cvector("B[0]",20,(double _Complex*)solver.B);
	print_cvector("rhs",20,(double _Complex*)solver.rhs);
	print_cvector("expected solution",40,(double _Complex*)solver.sol);

	for (int i = 0; i < solver.Ng; i++) {
		solver.sol[2*i  ] = 2.;
		solver.sol[2*i+1] = 3.;
	}
	print_cvector("initial guess",40,(double _Complex*)solver.sol);
	solve_rte2dvis_v1(solver);
	print_cvector("solution",solver.Ng,(double _Complex*)solver.sol);

	release_rte2dvis_v1(solver);

	destroy_rte2dvis(solver);
	//print_rte2dvis(solver);

	printf("END OF TEST01\n");
	printf("\n");
	return err;
}
