#include <unistd.h>
#include <limits.h>
#include <float.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "msh_io.h"
#include "utils.h"
/*******************************************************************************/
void test_limits()
{
	// Read out system-dependent constants
	printf("	FILENAME_MAX	= %d\n",FILENAME_MAX);
	printf("	BUFSIZ		= %d\n",BUFSIZ);

	printf("	INT_MAX		= %d\n",INT_MAX);
	printf("	INT_MIN		= %d\n",INT_MIN);

	printf("	FLT_EPSILON	= %g\n",FLT_EPSILON);
	printf("	FLT_MIN		= %g\n",FLT_MIN);
	printf("	FLT_MAX		= %g\n",FLT_MAX);
	/*printf("	FLT_MIN_EXP	= %d\n",FLT_MIN_EXP);*/
	/*printf("	FLT_MAX_EXP	= %d\n",FLT_MAX_EXP);*/
	printf("	FLT_MIN_10_EXP	= %d\n",FLT_MIN_10_EXP);
	printf("	FLT_MAX_10_EXP	= %d\n",FLT_MAX_10_EXP);

	printf("	DBL_EPSILON	= %g\n",DBL_EPSILON);
	printf("	DBL_MIN		= %g\n",DBL_MIN);
	/*printf("	DBL_MAX		= %g\n",DBL_MAX);*/
	printf("	DBL_MAX		= too many digits...\n");
	/*printf("	DBL_MIN_EXP	= %d\n",DBL_MIN_EXP);*/
	/*printf("	DBL_MAX_EXP	= %d\n",DBL_MAX_EXP);*/
	printf("	DBL_MIN_10_EXP	= %d\n",DBL_MIN_10_EXP);
	printf("	DBL_MAX_10_EXP	= %d\n",DBL_MAX_10_EXP);
	printf("\n");
}

int main(int argc, char *argv[])
{ 
	test_limits();

	int err=0;
	printf("###############################################################################\n");
	/*printf("--------------------------------------------------\n");*/
	printf("BEGIN:	|DUMP_MSH.EXE\n");
	printf("	|This executable aims to read the nodes\n");
	printf("	|and triangles from the given MSH file,\n");
	printf("	|then dump the nodes and triangles into\n");
	printf("	|separate ASCII files.\n");
	printf("	|Calling convention:\n");
	printf("	|	dump_msh.exe [format] [source] [target-base]\n");
	printf("	|\n");   
	printf("	|[format]	1=ASCII 2=BINARY\n");
	printf("	|[source]	source file name\n");
	printf("	|[target-base]	the base name for the output files\n");
	printf("	|		if not given (empty), default is [source]\n");
	printf("	|		without the .msh extension\n");
	printf("	|\n");   
	printf("	|Example:\n");
	printf("	|	If you type\n");
	printf("	|		$ dump_msh.exe 1 example.msh\n");
	printf("	|	The output are three ASCII files:\n");
	printf("	|		example.info\n");
	printf("	|		example.nodes\n");
	printf("	|		example.triangles\n");
	printf("	|	If you type\n");
	printf("	|		$ dump_msh.exe 2 example.msh myname\n");
	printf("	|	The output are one ASCII file:\n");
	printf("	|		example.info\n");
	printf("	|	and two BINARY files:\n");
	printf("	|		myname.nodes\n");
	printf("	|		myname.triangles\n");
	printf("	------------------------------\n");
	printf("\n");


	int format;
	char cwd[FILENAME_MAX];
	char filename_source[FILENAME_MAX];
	char filename_target_base[FILENAME_MAX]; 
	char filename_in[FILENAME_MAX];
	char filename_out[FILENAME_MAX];
	getcwd(cwd,FILENAME_MAX);
	strcpy(filename_in,	cwd);
	strcpy(filename_out,	cwd);
	if (argc<3) {
		// 0 argument
		fprintf(stderr,"	ERROR:		REQUIRES AT LEAST TWO ARGUMENTS:\n");
		fprintf(stderr,"			argv[1] = [format]\n");
		fprintf(stderr,"			argv[2] = [source]\n");
		fprintf(stderr,"			argv[3] = [target-base] (optional)\n"); 
		err += 1;
		return err;
	} else if (argc<4) {
		fprintf(stderr,"	WARNING:	[target-base] undefined,\n");
		fprintf(stderr,"			use [target-base]=[source].\n"); 
		format=atoi(argv[1]);
		strcpy(filename_source,		argv[2]);
		strcpy(filename_target_base,	argv[2]);
	} else if (argc<5) {
		format=atoi(argv[1]);
		strcpy(filename_source,		argv[2]);
		strcpy(filename_target_base,	argv[3]);
	} else {
		fprintf(stderr,"	WARNING:	Only the first two arguments are used.\n");
		fprintf(stderr,"			Proceed as usual...\n");
		format=atoi(argv[1]);
		strcpy(filename_source,		argv[2]);
		strcpy(filename_target_base,	argv[3]);
	} 
	{	// Delete extension ".msh", if any.
		char * temp= strstr(filename_target_base,".msh");
		if (temp) *temp=0;
	} 
	strcat(filename_in,"/");
	strcat(filename_in,filename_source);
	strcat(filename_out,"/");
	strcat(filename_out,filename_target_base); 
	printf("\n");


	printf("	Calling FILEIO_DUMP_MSH_FILE(filename_in,filename_out)...\n");
	printf("		filename_in	= %s\n",filename_in);
	printf("		filename_out	= %s\n",filename_out); 

	// Calling fileio_dump_msh_file
	err += dump_msh(filename_in,filename_out,format); 

	if (!err)
		printf("	Successfully dumped nodes and triangles!\n");
	else
		fprintf(stderr,"	ERROR:		err = %d\n",err);

	printf("END:	DUMP-MSH-FILE.EXE\n");
	printf("###############################################################################\n");
	return 0;
} 
