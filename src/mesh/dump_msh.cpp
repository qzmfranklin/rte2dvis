#include <unistd.h>
#include <getopt.h>
#include <limits.h>
#include <float.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "msh_io.h"
#include "utils.h"
/******************************************************************************/
static void print_usage();
static void test_limits();
static void opt_proc(int argc, char *argv[]);
/*******************************************************************************/
static char fname_in[FILENAME_MAX];
static char fname_out[FILENAME_MAX];
static int format=2; // 1=ACSII 2=BINARY(default)
static int verbose_flag=0; // 0=UNSET(default) 1=SET
/******************************************************************************/ 
int main(int argc, char *argv[])
{ 
	//test_limits();
	opt_proc(argc,argv);
	int err=dump_msh(fname_in,fname_out,format); 

	return err;
} 
void print_usage()
{
		/******************************************************************************/
	printf("SYNOPSIS\n");
	printf("	dump [options] file\n");
	printf("\n");
	printf("	where:\n");
	printf("	* 'options' represents zero or more dump options.\n");
	printf("	* 'file' represents a Gmsh format file.\n");
		/******************************************************************************/
	printf("OPTIONS\n");
	printf("	--ascii/-a	output format is text\n");
	printf("	--binary/-b	output format is binary(default)\n");
	printf("	--help/-h	print this usage message\n");
	printf("	-o [target]	direct the output to 'target'\n");
	printf("	--test/-t	print limits of the host architecture\n");
	printf("	--verbose/-t	verbose mode(not default)\n");
		/******************************************************************************/
}

void opt_proc(int argc, char *argv[])
{
	opterr=0; // ignore any getopt error

	fname_out[0]='\0';

	while (1) {
		static struct option long_options[] = {
			{"ascii",no_argument,0,'a'},
			{"binary",no_argument,0,'b'},
			{"help",no_argument,0,'h'},
			{"test",no_argument,0,'t'},
			{"verbose",no_argument,0,'v'},
			{0,0,0,0}
		};
		// getopt_long stores the option index here.
		int option_index = 0;

		int c = getopt_long (argc, argv, "abho:tv",
				long_options, &option_index);

		// Detect the end of the options.
		if (c == -1) break;

		switch (c) {
		case 0:
			// If this option set a flag, do nothing else now. 
			if (long_options[option_index].flag != 0)
				break;
			else {
				//printf("%s\n",long_options[option_index].name);
			}
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
		case 't':
			test_limits();
			exit(0);
			break;
		case 'v':
			verbose_flag=1;
			break;
		case 'o':
			strcpy(fname_out,optarg);
			printf("%s\n",optarg);
			break;
		case '?':
			printf("%s\n",optarg);
			print_usage();
			exit(1);
			break;
		default:
			abort();
		}
	}

	// The remaining command line arguments is the GSM file. 
	if (optind>=argc) {
		printf("Requires a MSH file as argument!\n");
		print_usage();
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

	if (verbose_flag==1) {
		printf("From   : %s\n",fname_in);
		printf("To     : %s\n",fname_out);
		if (format==1)
			printf("Format : ASCII\n");
		else if (format==2) 
			printf("Format : BINARY\n");
	}

	printf("\n"); 
}

void test_limits()
{
	// Read out system-dependent constants
	printf("	FILENAME_MAX	= %d\n",FILENAME_MAX);
	printf("	PIPE_BUF	= %d\n",PIPE_BUF);
	printf("	FOPEN_MAX	= %d\n",FOPEN_MAX);
	printf("	TMP_MAX		= %d\n",TMP_MAX);
	printf("	BUFSIZ		= %d\n",BUFSIZ);
	printf("	CHAR_MAX	= %d\n",CHAR_MAX);
	printf("	CHAR_MIN	= %d\n",CHAR_MIN);
	printf("	INT_MAX		= %d\n",INT_MAX);
	printf("	INT_MIN		= %d\n",INT_MIN);
	printf("	LONG_MAX	= %ld\n",LONG_MAX);
	printf("	LONG_MIN	= %ld\n",LONG_MIN);
	printf("	FLT_EPSILON	= %g\n",FLT_EPSILON);
	printf("	FLT_MIN		= %g\n",FLT_MIN);
	printf("	FLT_MAX		= %g\n",FLT_MAX);
	printf("	FLT_MIN_10_EXP	= %d\n",FLT_MIN_10_EXP);
	printf("	FLT_MAX_10_EXP	= %d\n",FLT_MAX_10_EXP);
	printf("	DBL_EPSILON	= %g\n",DBL_EPSILON);
	printf("	DBL_MIN		= %g\n",DBL_MIN);
	printf("	DBL_MAX		= %g\n",DBL_MAX);
	printf("	DBL_MIN_10_EXP	= %d\n",DBL_MIN_10_EXP);
	printf("	DBL_MAX_10_EXP	= %d\n",DBL_MAX_10_EXP);
}

