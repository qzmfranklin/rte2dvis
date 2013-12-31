#include <complex.h>
#include <stdio.h>
#include <mkl.h>
#include "utils.h"
#include "file_io.h"
#include "QuadratureRules.h"
#include "Solver_v1.h"
/******************************************************************************/
int test01(void);
/******************************************************************************/
int main(int argc, char const* argv[])
{
	if (argc>1) link_stdout(argv[1]);

	test01();
	if (argc>1) unlink_stdout();



	return 0;
}

int test01(void)
{
	int err=0; 
        printf("TEST01\n");
        printf("	|Test work flow of rte2dvis\n");

	__declspec(align(64)) double _Complex sol[2000];
	const char *fbase="example/square162.binary";
	const int Nd=127, pad=8;
	Solver_v1 s(fbase,Nd,pad);
	s.Print();
	s.Check();
	s.Debug();

	s.Solve(sol);

	printf("END OF TEST01\n");
	printf("\n");
	return err;
}
