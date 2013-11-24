#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <float.h>
#include <string.h>
#include <mkl.h>
#include "file_io.h" 
/******************************************************************************/ 
#define MALLOC_ALIGNMENT 64
/******************************************************************************/ 
void fileio_read_rte2dvis_input_file( const char* filename )
{
	/*
	 * This function is supposed to read 
	 * some kind of input file to the 
	 * rte2dvis machineray to avoid fre-
	 * quent re-compilation.
	 *
	 * Not implemented yet
	 */
}

int fileio_read_info_nodes_triangles( const char * filename_base, 
		double **nodes, int *nodes_num, int **triangles, int *triangles_num )
{
	int err=0;
	printf("--------------------------------------------------\n");
	printf("BEGIN:	FILEIO_READ_INFO_NODES_TRIANGLES\n");
	printf("	|Read the 3 files generated by\n");
	printf("	|FILEIO_DUMP_MSH_FILE into:\n");
	printf("	|arrays of nodes and triangles:\n");
	printf("	|	filename_base.nodes\n");
	printf("	|	filename_base.triangles\n");
	printf("	|	filename_base.info\n");
	printf("	|Inputs:\n");
	printf("	|	const char * filename_base:	base name of ASCII files to read\n");
	printf("	|Outputs:\n");
	printf("	|	double **nodes:			array to store nodes\n");
	printf("	|	int *nodes_num:			number of nodes\n");
	printf("	|	int **triangles:		array to store triangles\n");
	printf("	|	int *triangles_num:		number of triangles\n");
	printf("	|	Notice the passing by pointers.\n");
	printf("	|Ownership:\n");
	printf("	|	Used MKL_MALLOC to allocate memory for\n");
	printf("	|	double * nodes and double * triangles.\n");
	printf("	|	But did NOT free them. Need to free the\n");
	printf("	|	two pointers manually by the user.\n");
	printf("	|	All data are 64 bytes aligned.\n");
	printf("	------------------------------\n");
	printf("\n");

	char *filename_info;
	char *filename_nodes;
	char *filename_triangles;
	filename_info 		= (char*) malloc( FILENAME_MAX * sizeof(char) );
	filename_nodes 		= (char*) malloc( FILENAME_MAX * sizeof(char) );
	filename_triangles 	= (char*) malloc( FILENAME_MAX * sizeof(char) );
	strcpy(filename_info,		filename_base);
	strcpy(filename_nodes,		filename_base);
	strcpy(filename_triangles,	filename_base);
	strcat(filename_info,".info");
	strcat(filename_nodes,".nodes");
	strcat(filename_triangles,".triangles");
	printf("	filename_info      = %s\n",filename_info);
	printf("	filename_nodes     = %s\n",filename_nodes);
	printf("	filename_triangles = %s\n",filename_triangles); 

	// Open fin_info for read
	FILE *fin_info;
	if (  !(fin_info=fopen(filename_info,"r"))  ) { 
		printf("	Cannot open filename_info for read\n");
		printf("	Abort...\n");
		err += 1;
		return err;
	} else { 
		// Scan fin_info
		printf("	Scanning filename_info...\n");
		fscanf(fin_info, "%d\t\t# number of nodes", nodes_num );
		fscanf(fin_info, "%d\t\t# number of triangles", triangles_num );
		printf("		number of nodes:	%d\n", *nodes_num);
		printf("		number of triangles:	%d\n", *triangles_num); 
		fclose(fin_info);
	}
	printf("	Allocating memory...\n");
	*nodes = (double*) mkl_malloc( 2 * (*nodes_num) * sizeof(double), MALLOC_ALIGNMENT );
	*triangles= (int*) mkl_malloc( 3 * (*triangles_num) * sizeof(int), MALLOC_ALIGNMENT );
	assert(*nodes!=NULL);
	assert(*triangles!=NULL);
	// Open fin_nodes for read
	FILE *fin_nodes;
	if (  !(fin_nodes=fopen(filename_nodes,"r"))  ) {
		fprintf(stderr,"	Cannot open filename_nodes for read\n");
		fprintf(stderr,"	Abort...\n");
		err += 2;
		return err;
	} else {
		// Scan fin_nodes
		printf("	Scanning filename_nodes...\n");
		int i=0;
		printf("		\t\tx\t\t\ty\n");
		for (i = 0; i < *nodes_num; i++)  {
			fscanf(fin_nodes,"%lf %lf\n",
					(*nodes)+2*i,
					(*nodes)+2*i+1		);
			if (i<10) { 
				printf("	Ln %3d: %25.19E %25.19E\n",i+1,
						(*nodes)[2*i], (*nodes)[2*i+1]		);
			} else if (i==10) {
				printf("	Further outputs suppressed...\n");
			}
		}
		fclose(fin_nodes);
	}
	// Open fin_triangles for read
	FILE *fin_triangles;
	if (  !(fin_triangles=fopen(filename_triangles,"r"))  ) { 
		fprintf(stderr,"	Cannot open filename_triangles for read\n");
		fprintf(stderr,"	Abort...\n");
		err += 4;
		return err;
	} else {
		// Scan fin_triangles
		printf("	Scanning filename_triangles...\n");
		int i=0;
		for (i = 0; i < *nodes_num; i++)  {
			fscanf(fin_triangles,"%d %d %d\n",
					(*triangles)+3*i,
					(*triangles)+3*i+1,
					(*triangles)+3*i+2	);
			if (i<10) { 
				printf("	Ln %3d: %12d %12d %12d\n", i+1,
						(*triangles)[3*i], 
						(*triangles)[3*i+1], 
						(*triangles)[3*i+2]  );
			} else if (i==10) {
				printf("	Further output suppressed...\n");
			}
		}
		fclose(fin_triangles);
	} 
	printf("	Successfully read out nodes and triangles!\n");
	printf("\n");

	printf("	Free memory.\n");
	free(filename_info);
	free(filename_nodes);
	free(filename_triangles);
	printf("END:	FILEIO_READ_INFO_NODES_TRIANGLES\n");
	printf("--------------------------------------------------\n");
	return err;
}

int fileio_dump_msh_file( const char * filename_in, const char * filename_out )
{
	/*
	 * This function is expressibly made suitable 
	 * for being integrated into a standalone 
	 * program that reads an MSH file and dumps 
	 * the nodes and triangles into separate ASCII 
	 * files.
	 */
	int err=0;
	printf("--------------------------------------------------\n");
	printf("BEGIN:	FILEIO_DUMP_MSH_FILE\n");
	printf("	|Read MSH file, dump the nodes and triangles\n");
	printf("	|into two separate files, whose names are\n");
	printf("	|determined by the 2nd parameter. The default\n");
	printf("	|output filenames are:\n");
	printf("	|	filename_in.nodes\n");
	printf("	|	filename_in.triangles\n");
	printf("	|	filename_in.info\n");
	printf("	|Inputs:\n");
	printf("	|	const char * filename_in:	name of the MSH file to read\n");
	printf("	|	const char * filename_out:	base name of ASCII files to write\n");
	printf("	|					NULL to enable default behavior\n");
	printf("	|Outputs:\n"); 
	printf("	|	filename_out.nodes:		ASCII file of nodes coordinates\n");
	printf("	|	filename_out.triangles:		ASCII file of triangles integer pairs\n");
	printf("	|	filename_out.info:		ASCII file of the number of nodes and triangles\n");
	printf("	------------------------------\n");
	printf("\n");

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
	
	// Default filename_out
	if (  !filename_out  ) {
		printf("	filename_out == NULL, proceed with default filename_out:\n");
		printf("		%s\n",filename_in);
		filename_out = filename_in;
	}
	assert( filename_out);
	// Construct ouuput file names
	char *filename_out_nodes;
	char *filename_out_triangles;
	char *filename_out_info;
	filename_out_nodes 	= (char*) malloc( FILENAME_MAX * sizeof(char) );
	filename_out_triangles 	= (char*) malloc( FILENAME_MAX * sizeof(char) );
	filename_out_info 	= (char*) malloc( FILENAME_MAX * sizeof(char) );
	assert(  filename_out_nodes  	);
	assert(  filename_out_triangles );
	assert(  filename_out_info  	);
	strcpy(  filename_out_nodes, 	filename_out  	);
	strcpy(  filename_out_triangles,filename_out  	);
	strcpy(  filename_out_info,	filename_out  	);
	strcat(  filename_out_nodes,	".nodes"  	);
	strcat(  filename_out_triangles,".triangles"  	);
	strcat(  filename_out_info,	".info"  	);

	// Open 4 files:
	// 	fin:		input MSH file
	// 	fout_nodes:	output ASCII nodes file
	// 	fout_triangles:	output ASCII triangles file
	// 	fout_info:	output ASCII info file
	/*printf("	------------------------------\n");*/
	FILE *fin;
	FILE *fout_nodes;
	FILE *fout_triangles;
	FILE *fout_info;
	// open fin for read
	printf("	Opening file for read:\n");
	printf("		%s\n",filename_in);
	if (  !(fin=fopen(filename_in,"r"))  ) {
		printf("	Cannot open file for read:\n");
		printf("	err = err + 1\n");
		err += 1;
	}	
	// open fout_nodes for write
	printf("	Opening file for write:\n");
	printf("		%s\n",filename_out_nodes);
	if (  !(fout_nodes=fopen(filename_out_nodes,"w"))  ) {
		printf("	Cannot open file for write:\n");
		printf("	err = err + 2\n");
		err += 2;
	}	
	// open fout_triangles for write
	printf("	Opening file for write:\n");
	printf("		%s\n",filename_out_triangles);
	if (  !(fout_triangles=fopen(filename_out_triangles,"w"))  ) {
		printf("	Cannot open file for write:\n");
		printf("	err = err + 4\n");
		err += 4;
	}	
	// open fout_info for write
	printf("	Opening file for write:\n");
	printf("		%s\n",filename_out_info);
	if (  !(fout_info=fopen(filename_out_info,"w"))  ) {
		printf("	Cannot open file for write:\n");
		printf("	err = err + 8\n");
		err += 8;
	}	
	// Abort if either occured
	if (  err  ) {
		printf("	Abort with err = %2d\n",err);
		return err;
	} else  
		printf("	Successfully opened all 4 files!\n");
	/*printf("	------------------------------\n");*/

	int nodes_num;
	int num_elements;
	int triangles_num;
	char str_Nodes[]="Nodes";
	char str_EndNodes[]="EndNodes";
	char str_Elements[]="Elements";
	char str_EndElements[]="EndElements";
	char buff[BUFSIZ];
	int i=0;
	// Find nodes
	printf("	Search for \"%s\"...\n",str_Nodes);
	while (  (fgets(buff,BUFSIZ,fin))  ) {
		i++;
		if ( strstr(buff,str_Nodes)  ) {
			printf("		Found \"%s\" at line %d\n",str_Nodes,i);
			break;
		}
	}
	// Scan nodes and write to fout_nodes
	fscanf(fin,"%d",&nodes_num);
	printf("	Number of nodes is\n");
	printf("		%d\n",nodes_num);
	printf("	Start scanning nodes and writing to filename_out_nodes...\n");
	for (i = 0; i < nodes_num; i++) {
		int ip,z_zero;
		double x,y;
		fscanf(fin,"%d %lf %lf %d\n",&ip,&x,&y,&z_zero);
		/*printf("\t\t\t%3d\t%12.8E\t%12.8E\n",ip,x,y);*/
		fprintf(fout_nodes,"%+25.19E %+25.19E\n",x,y);
	}
	printf("	Finished scanning nodes and writing.\n");
	/*printf("	------------------------------\n");*/
	// Find triangles
	rewind(fin);
	i=0;
	printf("	Search for \"%s\"...\n",str_Elements);
	while (  (fgets(buff,BUFSIZ,fin))  ) {
		i++;
		if ( strstr(buff,str_Elements)  ) {
			printf("		Found \"%s\" at line %d\n.",str_Elements,i);
			break;
		}
	}
	// Scan triangles and write to fout_triangles
	fscanf(fin,"%d",&num_elements);
	i++;
	printf("	Number of elements is\n");
	printf("		%d\n",num_elements);
	triangles_num=0;
	printf("	Start Scanning elements and writing triangles to filename_out_triangles...\n");
	int j;
	for ( j = 0; j < num_elements; j++) {
		int tri[5],n1,n2,n3;
		fscanf(fin,"%d %d %d %d %d",&tri[0],&tri[1],&tri[2],&tri[3],&tri[4]);
		fgets(buff,BUFSIZ,fin);
		i++;
		if (  (tri[1]==2) && (tri[4]==6)  ) {
			triangles_num++;
			if (  triangles_num == 1  ) {
				printf("		Found first triangle\n");
				printf("			at line %d\n",i);
				printf("			as element #%d\n",j+1);
			}
			sscanf(buff," %d %d %d\n",&n1,&n2,&n3);
			/*printf("\t\t\t%3d\t%d\t%d\t%d\n",i+1,n1,n2,n3);*/
			fprintf(fout_triangles,"%12d %12d %12d\n",n1,n2,n3);
		}
	}
	printf("	Finished scanning elements and writing triangles.\n");
	printf("	Found %d triangles.\n",triangles_num);
	// Write to fout_info
	/*printf("	------------------------------\n"); */
	printf("	Start writing into filename_out_info...\n");
	fprintf(fout_info,"%d\t\t# number of nodes\n",nodes_num);
	fprintf(fout_info,"%d\t\t# number of triangles\n",triangles_num);
	fprintf(fout_info,"\n");
	fprintf(fout_info,"\n");
	fprintf(fout_info,"\n");
	fprintf(fout_info,"\n");
	fprintf(fout_info,"\n");
	fprintf(fout_info,"\n");
	fprintf(fout_info,"\n");
	fprintf(fout_info,"\n");
	fprintf(fout_info,"# This is the info file for the .nodes\n"); 
	fprintf(fout_info,"# and the .triangles file generated from\n");
	fprintf(fout_info,"# .msh file. The first 10 lines are reserved\n");
	fprintf(fout_info,"# for current or future use. Comments enter\n");
	fprintf(fout_info,"# here:\n");
	fprintf(fout_info,"#\n"); 
	fprintf(fout_info,"# How to read this info file using C fscanf?\n");
	fprintf(fout_info,"# Answer: the format strings are:\n");
	fprintf(fout_info,"# 	.info:		\"%%d\\t\\t# number of nodes\"\n");
	fprintf(fout_info,"# 			\"%%d\\t\\t# number of triangles\"\n");
	fprintf(fout_info,"#	.nodes:		\"%%lf %%lf\"\n");
	fprintf(fout_info,"#	.triangles:	\"%%d %%d %%d\"\n");
	fprintf(fout_info,"#\n");
	fprintf(fout_info,"#\n");
	fprintf(fout_info,"#\n");
	fprintf(fout_info,"#\n");
	fprintf(fout_info,"#\n"); 
	printf("	Finished writing into filename_out_info.\n");

	// Free memory
	/*printf("	------------------------------\n");*/
	printf("	Freeing memory...\n");
	fclose(fin);
	fclose(fout_nodes);
	fclose(fout_triangles);
	fclose(fout_info);
	free(filename_out_nodes);
	free(filename_out_triangles);
	free(filename_out_info);
	/*printf("	------------------------------\n");*/

	printf("\n");
	printf("# How to read the info file using C fscanf?\n");
	printf("# 	.info:		\"%%d\\t\\t# number of nodes\"\n");
	printf("# 			\"%%d\\t\\t# number of triangles\"\n");
	printf("#	.nodes:		\"%%lf %%lf\"\n");
	printf("#	.triangles:	\"%%d %%d %%d\"\n");
	printf("\n");

	printf("END:	FILEIO_DUMP_MSH_FILE\n");
	printf("--------------------------------------------------\n");

	return err;
}

void fileio_help() {
}
