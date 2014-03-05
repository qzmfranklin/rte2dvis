#ifndef _MESH_H_
#define _MESH_H_
/******************************************************************************/
#include <unistd.h>
/******************************************************************************/
struct st_mesh{
	int    status;	// internal status

	int    format;	// 1=ASCII, 2=BINARY
	int num_verts;	// number of verts
	int num_trigs;	// number of trigs
	int num_edges;	// number of edges
	double     *v;	// verts' coord
	int        *t;	// trigs' num
	int        *e;	// edges' num
	double     *p;	// trigs' coord
	double     *a;	// trigs' area
	char   fbase[FILENAME_MAX];	// file base name
};
/******************************************************************************/ 
#endif /* end of include guard */ 
