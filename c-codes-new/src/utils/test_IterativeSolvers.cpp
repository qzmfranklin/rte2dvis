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
	char fname[FILENAME_MAX];
	if (!argc)
		sprintf(fname,"DEBUG/test_IterativeSolvers.txt");
	else
		strcpy(fname,argv[1]);

	link_stdout(fname);

	//test01();

	unlink_stdout();

	return 0;
}
