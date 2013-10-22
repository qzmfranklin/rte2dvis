#include "franklin-types.h" 
#include <stdio.h>

typedef const char * t_filename;
/******************************************************************************/ 
void fileio_read_rte2dvis_input_file( 	const char * filename );
void fileio_read_array_from_file( 	const char * filename );
void fileio_read_nodes_file( 		const char * filename, double* p );
void fileio_read_triangles_file( 	const char * filename, double* t ); 
/*void fileio_join_file_name(		const*/
/******************************************************************************/ 
/******************************************************************************/ 

void fileio_read_rte2dvis_input_file( t_filename filename ){
}

void fileio_read_array_from_file( t_filename filename ){
	FILE *fp;
	fp = fopen(filename,"r");
		
	fclose(fp);
}

void fileio_read_nodes_file( t_filename filename, double* p ){ 
}

void fileio_read_triangles_file( t_filename filename, double* t ){ 
}
