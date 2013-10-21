#ifndef _FILE_IO_H
#define _FILE_IO_H 

EXTERN_C int fileio_read_rte2dvis_input_file( 	t_filename filename );
EXTERN_C int fileio_read_array_from_file( 	t_filename filename );
EXTERN_C int fileio_read_nodes_file( 		t_filename filename, double* p );
EXTERN_C int fileio_read_triangles_file( 	t_filename filename, double* t ); 

#endif /*_FILE_IO_H*/
