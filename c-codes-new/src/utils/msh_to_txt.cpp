#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "file_io.h"
/*******************************************************************************/
int main(int argc, char *argv[])
{ 
	int err=0;
	printf("###############################################################################\n");
	/*printf("--------------------------------------------------\n");*/
	printf("BEGIN:	|MSH_TO_TXT.EXE\n");
	printf("	|This executable aims to read the nodes\n");
	printf("	|and triangles from the given MSH file,\n");
	printf("	|then dump the nodes and triangles into\n");
	printf("	|separate ASCII files.\n");
	printf("	|Calling convention:\n");
	printf("	|	msh_to_data.exe  [source] [target-base]\n");
	printf("	|\n");   
	printf("	|[source]	source file name\n");
	printf("	|[target-base]	the base name for the output files\n");
	printf("	|		if not given (empty), default is [source]\n");
	printf("	|\n");   
	printf("	|Example:\n");
	printf("	|	If you type\n");
	printf("	|		$ msh_to_data.exe example.msh\n");
	printf("	|	The output three files are:\n");
	printf("	|		example.info\n");
	printf("	|		example.nodes\n");
	printf("	|		example.triangles\n");
	printf("	------------------------------\n");
	printf("\n");


	char cwd[FILENAME_MAX];
	char filename_source[FILENAME_MAX];
	char filename_target_base[FILENAME_MAX]; 
	char filename_in[FILENAME_MAX];
	char filename_out[FILENAME_MAX];
	getcwd(cwd,FILENAME_MAX);
	strcpy(filename_in,	cwd);
	strcpy(filename_out,	cwd);
	if (argc<2) {	
		// 0 argument
		fprintf(stderr,"	ERROR:		REQUIRES ONE OR TWO ARGUMENTS:\n");
		fprintf(stderr,"			argv[1] = [source]\n");
		fprintf(stderr,"			argv[2] = [target-base] (optional)\n"); 
		err += 1;
		return err;
		// 1 argument
	} else if (argc<3) {
		fprintf(stderr,"	WARNING:	[target-base] undefined,\n");
		fprintf(stderr,"			use [target-base]=[source].\n"); 
		strcpy(filename_source,		argv[1]);
		strcpy(filename_target_base,	argv[1]);
	} else if (argc<4) {
		// 2 argument
		strcpy(filename_source,		argv[1]);
		strcpy(filename_target_base,	argv[2]);
	} else {
		fprintf(stderr,"	WARNING:	Only the first two arguments are used.\n");
		fprintf(stderr,"			Proceed as usual...\n");
		strcpy(filename_source,		argv[1]);
		strcpy(filename_target_base,	argv[2]);
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
	err += msh_to_txt(filename_in,filename_out); 

	if (!err) {
		printf("	Successfully dumped nodes and triangles!\n");
	} else {
		fprintf(stderr,"	ERROR:		err = %d\n",err);
	}

	printf("END:	DUMP-MSH-FILE.EXE\n");
	printf("###############################################################################\n");
	return 0;
} 
