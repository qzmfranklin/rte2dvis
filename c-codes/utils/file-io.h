#ifndef _FILE_IO_H_
#define _FILE_IO_H_
void	fileio_read_rte2dvis_input_file( 	const char * filename ); 
int	fileio_read_info_nodes_triangles(	const char * filename_base,		
						double * nodes,		int num_nodes,	
						int * triangles,	int num_triangles ); 
int	fileio_dump_msh_file(			const char * filename_msh,		
						const char * filename_dat	); 
#endif /* end of include guard */
