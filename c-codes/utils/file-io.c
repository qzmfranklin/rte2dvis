#include "franklin-types.h" 
/******************************************************************************/ 
int fileio_read_rte2dvis_input_file( 	t_filename filename );
int fileio_read_array_from_file( 	t_filename filename );
int fileio_read_nodes_file( 		t_filename filename, double* p );
int fileio_read_triangles_file( 	t_filename filename, double* t ); 
/******************************************************************************/ 
/******************************************************************************/ 

int fileio_read_rte2dvis_input_file( t_filename filename ){
}

int fileio_read_array_from_file( t_filename filename ){
	FILE *fp;
	fp = fopen(filename,"r");
		
	fclose(fp);
}

int fileio_read_nodes_file( t_filename filename, double* p ){ 
}

int fileio_read_triangles_file( t_filename filename, double* t ){ 
}
