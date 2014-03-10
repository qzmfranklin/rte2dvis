#include <unistd.h>
#include <getopt.h>
#include <limits.h>
#include <float.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "utils.h"
#include "solver_v1.h"
/******************************************************************************/
static void print_usage();
static void test_limits();
static void opt_proc(int argc, char *argv[]);
/*******************************************************************************/
static char fbase[FILENAME_MAX]; // base name of input file 
static char fout_sol[FILENAME_MAX]; // where to save solution, with option -o
static int format=2; // intput format 1=ACSII 2=BINARY(default)
static int verbose_flag=0; // message level 0=BRIEF(default) 1=VERBOSE
// interface from solver_v1.h:
/*
 * ipar[0] = M
 * ipar[1] = Nd
 * ipar[2] = pad
 * ipar[3] = rule1
 * ipar[4] = rule2
 * ipar[5] = nu
 * ipar[6] = nv
 * ipar[7] = num_threads in omp
 */
/*
 * dpar[0] = g factor
 * dpar[1] = mua (absorption coefficient)
 * dpar[2] = mus (scattering coefficient)
 */
static enum  {
	SET_M = 101, // spatial interpolation order, M
	SET_ND,
	SET_PAD,
	SET_RULE1,
	SET_RULE2,
	SET_NU,
	SET_NV,
	SET_NUM_THREADS,
	SET_GFACTOR,
	SET_MUA,
	SET_MUS,
} par_flag;
static int ipar[128];
static double dpar[128]; 
/******************************************************************************/ 
int main(int argc, char *argv[])
{ 
	//test_limits();
	opt_proc(argc,argv);

	return 0;
} 

void opt_proc(int argc, char *argv[])
{
	//opterr=0; // ignore any getopt error

	int *par_flag_address=(int*)(&par_flag);
	static struct option long_options[] = {
		{"ascii",	no_argument,0,'a'},
		{"binary",	no_argument,0,'b'},
		{"file",	required_argument,0,'f'},
		{"help",	no_argument,0,'h'},
		{"set-M",	required_argument,par_flag_address,SET_M},
		{"set-Nd",	required_argument,par_flag_address,SET_ND},
		{"set-pad",	required_argument,par_flag_address,SET_PAD},
		{"set-rule1",	required_argument,par_flag_address,SET_RULE1},
		{"set-rule2",	required_argument,par_flag_address,SET_RULE2},
		{"set-nu",	required_argument,par_flag_address,SET_NU},
		{"set-nv",	required_argument,par_flag_address,SET_NV}, 
		{"set-num_threads",required_argument,0,'j'}, 
		{"set-gfactor",	required_argument,par_flag_address,SET_GFACTOR},
		{"set-mua",	required_argument,par_flag_address,SET_MUA},
		{"set-mus",	required_argument,par_flag_address,SET_MUS},
		{"test",	no_argument,0,'t'},
		{"verbose",	no_argument,0,'v'},
		{0,0,0,0}
	};
	int option_index=0;
	int c;
	while ( (c=getopt_long_only(argc,argv,"abf:hj:o:tv",long_options,&option_index)) != -1) {
		switch (c) {
		case 0:
			//switch (long_options[option_index].flag)
			if (par_flag==SET_M) {
				sscanf(optarg,"%d",ipar);
				printf("M           = %d\n",ipar[0]);
			} else if (par_flag==SET_ND) { 
				sscanf(optarg,"%d",ipar+1); 
				printf("Nd          = %d\n",ipar[1]);
			} else if (par_flag==SET_PAD) { 
				sscanf(optarg,"%d",ipar+2); 
				printf("pad         = %d\n",ipar[2]);
			} else if (par_flag==SET_RULE1) { 
				sscanf(optarg,"%d",ipar+3); 
				printf("rule1       = %d\n",ipar[3]);
			} else if (par_flag==SET_RULE2) { 
				sscanf(optarg,"%d",ipar+4); 
				printf("rule2       = %d\n",ipar[4]);
			} else if (par_flag==SET_NU) { 
				sscanf(optarg,"%d",ipar+5); 
				printf("nu          = %d\n",ipar[5]);
			} else if (par_flag==SET_NV) { 
				sscanf(optarg,"%d",ipar+7); 
				printf("nv          = %d\n",ipar[7]);
			} else if (par_flag==SET_NUM_THREADS) { 
				sscanf(optarg,"%d",ipar+8); 
				printf("num_threads = %d\n",ipar[8]);
			} else if (par_flag==SET_GFACTOR) { 
				sscanf(optarg,"%lf",dpar); 
				printf("gfactor     = %f\n",dpar[0]);
			} else if (par_flag==SET_MUA) { 
				sscanf(optarg,"%lf",dpar+1); 
				printf("mua         = %f\n",dpar[1]);
			} else if (par_flag==SET_MUS) { 
				sscanf(optarg,"%lf",dpar+2); 
				printf("mus         = %f\n",dpar[2]);
			} else {
				// no idea how to get here...
			}
			break;
		case 'a':
			format=1;
			break;
		case 'b':
			format=2;
			break;
		case 'f':
			//printf("read parameters from file %s (not implemented yet)\n",optarg);
			break;
		case 'h':
			print_usage();
			exit(0);
			break;
		case 'j':
			sscanf(optarg,"%d",ipar+8);
			printf("num_threads = %d\n",ipar[8]);
			break;
		case 't':
			test_limits();
			exit(0);
			break;
		case 'v':
			verbose_flag=1;
			break;
		case 'o':
			//strcpy(fname_out,optarg);
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
	//if (optind>=argc) {
		//printf("Requires a MSH file as argument!\n");
		//print_usage();
		//exit(1);
	//}
	//strcpy(fname_in,argv[optind]);

	// If not specified -o [target], use the same file name as the input 
	//if (fname_out[0]=='\0')
		//strcpy(fname_out,fname_in);

	//{// Delete extension ".msh", if any.
		//char * temp= strstr(fname_out,".msh");
		//if (temp) *temp=0;
	//} 

	//if (verbose_flag==1) {
		//printf("From   : %s\n",fname_in);
		//printf("To     : %s\n",fname_out);
		//if (format==1)
			//printf("Format : ASCII\n");
		//else if (format==2) 
			//printf("Format : BINARY\n");
	//}
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

void print_usage()
{
		/******************************************************************************/
	printf("\tPreprocess a Gmsh file into ascii/binary tables\n");
	printf("SYNOPSIS\n");
	printf("	rte2dvisv1 [options] fbase\n");
	printf("\n");
	printf("	where:\n");
	printf("	* 'options' represents zero or more options.\n");
	printf("	* 'fbase' represents the base name of the input files\n");
		/******************************************************************************/
	printf("OPTIONS\n");
	printf("	--ascii/-a	output format is text\n");
	printf("	--binary/-b	output format is binary(default)\n");
	printf("	--help/-h	print this usage message\n");
	printf("	-o [dir]	save outputs to 'dir'\n");
	printf("	--test/-t	print limits of the host architecture\n");
	printf("	--verbose/-t	verbose mode (not default)\n");
		/******************************************************************************/
}
