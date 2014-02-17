#include <unistd.h>
#include <getopt.h>
#include <limits.h>
#include <float.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "msh_io.h"
#include "utils.h"
/*******************************************************************************/
static char fname_in[FILENAME_MAX];
static char fname_out[FILENAME_MAX];
static int format=2; // 1=ACSII 2=BINARY(default)

static void print_usage();
static void test_limits();
static void opt_proc(int argc, char *argv[]);
/******************************************************************************/ 
int main(int argc, char *argv[])
{ 
	//test_limits();
	opt_proc(argc,argv);

	return dump_msh(fname_in,fname_out,format); 
} 

void opt_proc(int argc, char *argv[])
{
	fname_out[0]='\0';

	while (1) {
		static struct option long_options[] = {
			{"ascii",no_argument,0,'a'},
			{"binary",no_argument,0,'b'},
			{"help",no_argument,0,'h'},
			{0,0,0,0}
		};
		/* getopt_long stores the option index here. */
		int option_index = 0;

		int c = getopt_long (argc, argv, "abho:",
				long_options, &option_index);

		/* Detect the end of the options. */
		if (c == -1) break;

		switch (c) {
		case 0:
			/* If this option set a flag, do nothing else now. */
			if (long_options[option_index].flag != 0)
				break;
			printf ("option %s", long_options[option_index].name);
			if (optarg)
				printf (" with arg %s", optarg);
			printf ("\n");
			break;
		case 'a':
			format=1;
			break;
		case 'b':
			format=2;
			break;
		case 'h':
			print_usage();
			exit(0);
			break;
		case 'o':
			//if (!optarg) {
				//printf("option -o requires an argument!\n");
				//exit(1);
			//}
			strcpy(fname_out,optarg);
			printf("%s\n",optarg);
			break;
		case '?':
			/* getopt_long already printed an error message. */
			printf("%s\n",optarg);
			break;
		default:
			abort();
		}
	}

	// The remaining command line arguments is the GSM file. 
	if (optind>=argc) {
		printf("Requires a MSH file as argument!\n");
		exit(1);
	}
	strcpy(fname_in,argv[optind]);

	// If not specified -o [target], use the same file name as the input 
	if (fname_out[0]=='\0')
		strcpy(fname_out,fname_in);

	{// Delete extension ".msh", if any.
		char * temp= strstr(fname_out,".msh");
		if (temp) *temp=0;
	} 

	// Output format
	if (format==1)
		printf("Output to ASCII...\n");
	else if (format==2) 
		printf("Output to BINARY...\n");

	printf("\n"); 
}

void print_usage()
{
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
}

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

