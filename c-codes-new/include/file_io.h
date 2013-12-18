#ifndef _FILE_IO_H_
#define _FILE_IO_H_
/*******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
int fileio_read_info_nodes_triangles_from_txt(const char *filename_base,	
		double **nodes, int *num_nodes, int **triangles, 
		int *num_triangles );

int fileio_msh_to_txt(const char *filename_msh, const char *filename_dat);
int fileio_msh_to_binary(const char *filename_msh, const char *filename_dat);
#ifdef __cplusplus
}
#endif
/*******************************************************************************/
#endif /* end of include guard _FILE_IO_H_ */
