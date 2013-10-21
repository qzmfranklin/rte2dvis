#include "file-io.h"
#include <stdio.h>


static int test01(int argc, char *argv[]);
/*static int test02(int argc, char *argv[]);*/
/*static int test03(int argc, char *argv[]);*/
/******************************************************************************/
int main(int argc, char *argv[])
{
	printf("START TESTING\n");
	test01(argc,argv);
	/*test02(argc,argv);*/
	/*test03(argc,argv);*/
	printf("NORMAL END OF EXECUTION\n");
	return 0;
}

/******************************************************************************/
static int test01(int argc, char *argv[])
{
	printf("TEST01\n");
	printf("	This test reads the first augument\n");
	printf("	and output the full path using the \n");
	printf("	current working directory\n");
	char *cwd;
	cwd = (char*) malloc ( FILENAME_MAX * sizeof(char) );
	getcwd(cwd,FILENAME_MAX);
	printf("%s/%s\n",cwd,argv[1]); 
	free(cwd);
	printf("END OF TEST01\n");
	return 0;
}
