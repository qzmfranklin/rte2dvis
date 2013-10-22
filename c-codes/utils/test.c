#include "file-io.h"
#include <stdio.h>
#include <string.h>


static int test01(int argc, char *argv[]);
static int test02(int argc, char *argv[]);
/*static int test03(int argc, char *argv[]);*/
/******************************************************************************/
int main(int argc, char *argv[])
{
	int err=0;
	printf("###############################################################################\n");
	printf("START TESTING\n");
	printf("	FILE IO\n");
	printf("\n");
	err += test01(argc,argv);
	err += test02(argc,argv);
	/*err += test03(argc,argv);*/
	if (!err)
		printf("NORMAL END OF EXECUTION\n");
	else {
		printf("ERROR CODE = %d\n",err);
		printf("END OF EXECUTION\n");
	}
	printf("###############################################################################\n");
	printf("\n");
	return err;
}

/******************************************************************************/
static int test01(int argc, char *argv[])
{
	int err = 0;
	printf("TEST01\n");
	printf("	Read out the system-dependent constant\n");
	printf("		FILENAME_MAX\n");
	printf("	Read the first augument and the\n");
	printf("	current working directory (cwd).\n");
	printf("	Join them into a full path filename\n");
	printf("\n");
	char *cwd;
	cwd = (char*) malloc ( FILENAME_MAX * sizeof(char) );
	getcwd(cwd,FILENAME_MAX);
	int n1=strlen(cwd);
	int n2=strlen(argv[1]);
	printf("	FILENAME_MAX = %d\n",FILENAME_MAX);
	printf("	The lengths of cwd and argv[1] are\n");
	printf("			%d	%d\n",n1,n2);
	printf("	The last letters of cwd and argv[1] are\n");
	printf("			%c	%c\n",cwd[n1-1],argv[1][n2-1]);
	strcat(cwd,"/");
	strcat(cwd,argv[1]);
	printf("	The file indicated was:\n");
	printf("		%s\n",cwd);
	free(cwd);
	printf("END OF TEST01\n");
	printf("\n");
	return err;
}

static int test02(int argc, char *argv[]){
	int err=0;
	printf("TEST02\n");
	printf("	This test reads the second augument\n");
	printf("	as the filename. Create the file,\n");
	printf("	write \"Hello World\" into it.\n");
	char *cwd;
	cwd = (char*) malloc ( FILENAME_MAX * sizeof(char) );
	getcwd(cwd,FILENAME_MAX);
	strcat(cwd,"/");
	strcat(cwd,argv[2]);
	printf("	Open the following file for write:\n");
	printf("		%s\n",cwd); 
	FILE *fp;
	if (  fp=fopen(argv[1],"w+")  ) { 
		printf("	Cannot open file for write:\n");
		printf("		%s\n",cwd);
		printf("	Abort this test...\n");
		err = 2;
	}	
	else {
		printf("	Write to the file.\n");
		printf("	Close file.\n");
		fclose(fp); 
	}
	free(cwd);
	printf("END OF TEST02\n");
	printf("\n");
	return err;
}

