#include "franklin-types.h"
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <float.h>
#include <mkl.h>

typedef const char * t_filename;
/******************************************************************************/ 
void fileio_read_rte2dvis_input_file( 	const char * filename );
void fileio_read_array_from_file( 	const char * filename, double* array );
void fileio_read_nodes_file( 		const char * filename, double* nodes );
void fileio_read_triangles_file( 	const char * filename, double* triangles); 
void fileio_dump_msh_file(		const char * filename_msh,		\
					const char * filename_dat	);
/******************************************************************************/ 
/******************************************************************************/ 

void fileio_read_rte2dvis_input_file( t_filename filename ){
}

void fileio_read_array_from_file( t_filename filename, double *array ){
	FILE *fp;
	fp = fopen(filename,"r");
		
	fclose(fp);
}

void fileio_read_nodes_file( t_filename filename, double* p ){ 
}

void fileio_read_triangles_file( t_filename filename, double* t ){ 
}

void fileio_dump_msh_file( const char * filename_in, const char * filename_out ) {
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
	printf("	|	const char * filename_in:	name of MSH file to read\n");
	printf("	|	const char * filename_out:	root of ASCII file to write\n");
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
	strcpy(  filename_out_nodes, 	(void*)filename_out  	);
	strcpy(  filename_out_triangles,(void*)filename_out  	);
	strcpy(  filename_out_info,	(void*)filename_out  	);
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
		return;
	} else  
		printf("	Successfully opened all 4 files!\n");
	/*printf("	------------------------------\n");*/

	int num_nodes;
	int num_elements;
	int num_triangles;
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
	fscanf(fin,"%d",&num_nodes);
	printf("	Number of nodes is\n");
	printf("		%d\n",num_nodes);
	printf("	Start scanning nodes and writing to filename_out_nodes...\n");
	for (i = 0; i < num_nodes; i++) {
		int ip,z_zero;
		double x,y;
		fscanf(fin,"%d %lf %lf %d\n",&ip,&x,&y,&z_zero);
		/*printf("\t\t\t%3d\t%12.8E\t%12.8E\n",ip,x,y);*/
		fprintf(fout_nodes,"%25.21E %25.21E\n",x,y);
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
	num_triangles=0;
	printf("	Start Scanning elements and writing triangles to filename_out_triangles...\n");
	int j;
	for ( j = 0; j < num_elements; j++) {
		int tri[5],n1,n2,n3;
		fscanf(fin,"%d %d %d %d %d",&tri[0],&tri[1],&tri[2],&tri[3],&tri[4]);
		fgets(buff,BUFSIZ,fin);
		i++;
		if (  (tri[1]==2) && (tri[4]==6)  ) {
			num_triangles++;
			if (  num_triangles == 1  ) {
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
	printf("	Found %d triangles.\n",num_triangles);
	// Write to fout_info
	/*printf("	------------------------------\n"); */
	printf("	Start writing into filename_out_info...\n");
	fprintf(fout_info,"%d\t\t# number of nodes\n",num_nodes);
	fprintf(fout_info,"%d\t\t# number of triangles\n",num_triangles);
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
	fprintf(fout_info,"#	.nodes:		\"%%25.21E %%25.21E\"\n");
	fprintf(fout_info,"#	.triangles:	\"%%12d %%12d %%12d\"\n");
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
	printf("#	.nodes:		\"%%25.21E %%25.21E\"\n");
	printf("#	.triangles:	\"%%12d %%12d %%12d\"\n");
	printf("\n");

	printf("END:	FILEIO_DUMP_MSH_FILE\n");
	printf("--------------------------------------------------\n");
}
