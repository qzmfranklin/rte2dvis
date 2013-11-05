#ifndef _FILE_IO_H_
#define _FILE_IO_H_
#ifndef EXTERN_C 
	#ifdef __cplusplus
		#define EXTERN_C extern "C"
	#else
		#define EXTERN_C extern
	#endif
#endif
EXTERN_C void	fileio_read_rte2dvis_input_file( const char * filename ); 
EXTERN_C int	fileio_read_info_nodes_triangles(const char * filename_base,		
						double **nodes,		int *num_nodes,
						int **triangles,	int *num_triangles );
EXTERN_C int	fileio_dump_msh_file(		const char * filename_msh,		
						const char * filename_dat	); 
#endif /* end of include guard */
