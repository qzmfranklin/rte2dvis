#include "franklin-types.h" 
#include <stdio.h>

typedef const char * t_filename;
/******************************************************************************/ 
int fileio_read_rte2dvis_input_file( 	const char * filename );
int fileio_read_array_from_file( 	const char * filename );
int fileio_read_nodes_file( 		const char * filename, double* p );
int fileio_read_triangles_file( 	const char * filename, double* t ); 
/******************************************************************************/ 
/******************************************************************************/ 

int fileio_read_rte2dvis_input_file( t_filename filename ){
	return 0;
}

int fileio_read_array_from_file( t_filename filename ){
	FILE *fp;
	fp = fopen(filename,"r");
		
	fclose(fp);
	return 0;
}

int fileio_read_nodes_file( t_filename filename, double* p ){ 
	return 0;
}

int fileio_read_triangles_file( t_filename filename, double* t ){ 
	return 0;
}
