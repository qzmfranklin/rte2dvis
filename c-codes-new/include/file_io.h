#ifndef _FILE_IO_H_
#define _FILE_IO_H_
/******************************************************************************/
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*******************************************************************************/
//#ifdef __cplusplus
//extern "C" {
//#endif

//TODO
struct st_mesh_info {
	int    format;	// source file format, e.g., 1=MSH
	int    status;	// st_mesh_info internal status
	int num_nodes;	// number of nodes
	int num_trigs;	// number of triangles
	double *nodes;
	int    *trigs;
	char   fbase[FILENAME_MAX];	// file base name
};

void print_mesh  (struct st_mesh_info *q);

void init_mesh   (struct st_mesh_info *q, const char *fbase, int format=1);
void read_info   (struct st_mesh_info *q);
void alloc_mesh  (struct st_mesh_info *q);
void load_mesh   (struct st_mesh_info *q);
void destroy_mesh(struct st_mesh_info *q);

/*
 * One monolithic function that reads all information.
 * Deprecated.
 */
int fileio_read_info_nodes_triangles_from_txt(const char *filename_base,	
		double **nodes, int *num_nodes, int **triangles, 
		int *num_triangles );

/*
 * One monolithic function that dumps msh files to ASCII files.
 */
int msh_to_txt(const char *filename_in, const char *filename_out);

/*
 * One monolithic function that dumps msh files to BINARY files.
 * TODO
 */
int msh_to_bin(const char *filename_in, const char *filename_out);
//#ifdef __cplusplus
//}
//#endif
/*******************************************************************************/
#endif /* end of include guard _FILE_IO_H_ */
