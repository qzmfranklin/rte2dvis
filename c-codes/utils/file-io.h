#ifndef _FILE_IO_H
#define _FILE_IO_H 

extern void fileio_read_rte2dvis_input_file( 	const char * filename );
extern void fileio_read_array_from_file( 	const char * filename, double* array );
extern void fileio_read_nodes_file( 		const char * filename, double* nodes );
extern void fileio_read_triangles_file( 	const char * filename, double* triangles); 
extern void fileio_dump_msh_file(		const char * filename_msh,		\
						const char * filename_dat	);
#endif /*_FILE_IO_H*/
