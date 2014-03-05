#ifndef _MESH_H_
#define _MESH_H_
/******************************************************************************/
#include <unistd.h>
/******************************************************************************/
struct st_mesh{
	int    status;	// internal status

	int    format;	// 1=ASCII, 2=BINARY
	int num_nodes;	// number of nodes
	int num_trigs;	// number of triangles
	double     *p;	// nodes' coord
	int        *t;	// trigs' num
	double    *pt;	// trigs' coord
	double     *a;	// trigs' area
	char   fbase[FILENAME_MAX];	// file base name
};
/******************************************************************************/ 
#endif /* end of include guard */ 
