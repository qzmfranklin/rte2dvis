#ifndef _MSH_IO_H_
#define _MSH_IO_H_
/******************************************************************************/
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
/*******************************************************************************/
struct st_mesh {
	int    status;	// st_mesh internal status

	int    format;	// 1=ASCII, 2=BINARY
	int num_nodes;	// number of nodes
	int num_trigs;	// number of triangles
	double *nodes;
	double *trigs;
	char   fbase[FILENAME_MAX];	// file base name
};

struct st_mesh *load_mesh(const char *fbase);
void destroy_mesh(struct st_mesh *q);

#define PRINT_INFO_BRIEF 0
#define PRINT_INFO_VERBOSE 1
void print_mesh  (struct st_mesh *q, int flag=0);
/******************************************************************************/
/*
 * Read MSH file, dump the nodes and trigs into ASCII/BINARY files.
 *   |output fnames are:
 *   |	fname_in.nodes
 *   |	fname_in.trigs
 *   |	fname_in.info
 *   |Inputs:
 *   |	const char * fname_in:		name of the MSH file to read
 *   |	const char * fname_out:		base name of files to write
 *   |					NULL to enable default behavior
 *   |	int format:			1=ASCII 2=BINARY
 *   |Outputs: 
 *   |	fname_out.nodes:		nodes coordinates
 *   |	fname_out.trigs:		triangles integer pairs
 *   |	fname_out.info:			the number of nodes and triangles
 *   ------------------------------
 */ 
int dump_msh(const char *fname_in, const char *fname_out, int format);
/*******************************************************************************/
#endif /* end of include guard _MSH_IO_H_ */
