#ifndef _FILE_IO_H
#define _FILE_IO_H 

extern int fileio_read_rte2dvis_input_file( 	const char * filename );
extern int fileio_read_array_from_file( 	const char * filename );
extern int fileio_read_nodes_file( 		const char * filename, double* p );
extern int fileio_read_triangles_file( 	const char * filename, double* t ); 

#endif /*_FILE_IO_H*/
