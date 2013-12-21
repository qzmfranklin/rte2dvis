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
	const int Nd=10, pad=8, flag=1;

	init_rte2dvis(solver,fbase,Nd,pad,flag);
	assert(!check_rte2dvis(solver));
	//print_rte2dvis(solver);
	//printf("\n");

	alloc_rte2dvis_v1(solver);
	//print_rte2dvis(solver);
	//printf("\n");

	fill_rte2dvis_v1(solver);
	//print_rte2dvis(solver);
	//printf("\n");

	solve_rte2dvis_v1(solver);
	//print_rte2dvis(solver);
	//printf("\n");

	release_rte2dvis_v1(solver);
	//print_rte2dvis(solver);
	//printf("\n");

	destroy_rte2dvis(solver);
	//print_rte2dvis(solver);
	//printf("\n");

        printf("END OF TEST01\n");
        printf("\n");
	return err;
}
