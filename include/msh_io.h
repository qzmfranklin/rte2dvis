#ifndef _FILE_IO_H_
#define _FILE_IO_H_
/******************************************************************************/
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
/*******************************************************************************/
struct st_mesh_info {
	int    status;	// st_mesh_info internal status

	int    format;	// 1=ASCII, 2=BINARY
	int num_nodes;	// number of nodes
	int num_trigs;	// number of triangles
	double *nodes;
	double *trigs;
	char   fbase[FILENAME_MAX];	// file base name
};

void load_mesh   (struct st_mesh_info &q, const char *fbase);
void release_mesh(struct st_mesh_info &q);
/*
 * flag:
 * 	0	show less
 * 	1	show more
 */
void print_mesh  (struct st_mesh_info &q, int flag=0);
/******************************************************************************/
/*
 * The following routines are not supposed to be called by the user.
 */
void init_mesh   (struct st_mesh_info &q, const char *fbase);
void read_info   (struct st_mesh_info &q);
void alloc_mesh  (struct st_mesh_info &q);
void read_mesh   (struct st_mesh_info &q);

/*
 * Read MSH file, dump the nodes and trigs into ASCII/BINARY files.
 *   |output filenames are:
 *   |	filename_in.nodes
 *   |	filename_in.trigs
 *   |	filename_in.info
 *   |Inputs:
 *   |	const char * filename_in:	name of the MSH file to read
 *   |	const char * filename_out:	base name of files to write
 *   |					NULL to enable default behavior
 *   |	int format:			1=ASCII 2=BINARY
 *   |Outputs: 
 *   |	filename_out.nodes:		ASCII file of nodes coordinates
 *   |	filename_out.trigs:		ASCII file of triangles integer pairs
 *   |	filename_out.info:		ASCII file of the number of nodes and triangles
 *   ------------------------------
 */ 
int dump_msh(const char *filename_in, const char *filename_out, int format);
/*******************************************************************************/
#endif /* end of include guard _FILE_IO_H_ */
