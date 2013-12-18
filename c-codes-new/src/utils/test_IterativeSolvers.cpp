#include <stdio.h>
#include <string.h>
#include "IterativeSolvers.h"
#include "utils.h"
/*****************************************************************************/
int main(int argc, char const* argv[]);
//void test01(void);
//void test02(void);
//void test03(void);
//void test04(void);
/*****************************************************************************/
int main(int argc, char const* argv[])
{ 
	if (argc>1) link_stdout(argv[1]);

	//test01();

	if (argc>1) unlink_stdout();

	return 0;
}

int test01(void)
{
	int err=0; 
        printf("TEST01\n");
        printf("	|Test MKL fgmres\n");
	
        printf("END OF TEST01\n");
        printf("\n");
	return err;
}
