#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <mkl.h>
#include "mshio.h" 
/******************************************************************************/ 
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
/******************************************************************************/ 
static struct st_mesh *init_mesh(const char *fbase)
{
	struct st_mesh *q;
	q=(struct st_mesh*)mkl_malloc(sizeof(struct st_mesh),64);
	assert(q);

	strcpy(q->fbase,fbase);
	q->status=0;

	return q;
}

static void read_info(struct st_mesh *q)
{
	//printf("read_info(stuct st_mesh &q)");
	assert(q->status==0); 

	char filename_info[FILENAME_MAX];
	strcpy(filename_info,q->fbase);
	strcat(filename_info,".info");

	// Open fin_info for read
	FILE *fin_info;
	fin_info = fopen(filename_info,"r");
	assert(fin_info);

	// Scan fin_info
	int format,num_nodes,num_trigs;
	fscanf(fin_info,"%d\t\t# 1=ASCII 2=BINARY",    &format);
	fscanf(fin_info,"%d\t\t# number of nodes",     &num_nodes);
	fscanf(fin_info,"%d\t\t# number of triangles", &num_trigs);
	fclose(fin_info);

	q->format   =format;
	q->num_nodes=num_nodes;
	q->num_trigs=num_trigs;


	q->status=1; 
}

static void alloc_mesh(struct st_mesh *q)
{
	//fprintf(stderr,"alloc_mesh(struct st_mesh &q)\n");
	assert(q->status==1);

	q->p =(double*)mkl_malloc(sizeof(double)*2*(q->num_nodes),64);
	q->t =(int*)   mkl_malloc(sizeof(int)   *3*(q->num_trigs),64);
	q->pt=(double*)mkl_malloc(sizeof(double)*6*(q->num_trigs),64);
	assert(q->p);
	assert(q->t);
	assert(q->pt);

	q->status=2;
}

static void read_mesh(struct st_mesh *q)
{
	//fprintf(stderr,"load_mesh(struct st_mesh &q)\n");
	assert(q->status==2);

	char filename_nodes[FILENAME_MAX];
	char filename_trigs[FILENAME_MAX];
	strcpy(filename_nodes,q->fbase);
	strcpy(filename_trigs,q->fbase);
	strcat(filename_nodes,".nodes");
	strcat(filename_trigs,".trigs");

	// Open files
	FILE *fin_nodes,*fin_trigs;
	fin_nodes=fopen(filename_nodes,"r");
	fin_trigs=fopen(filename_trigs,"r");
	assert(fin_nodes);
	assert(fin_trigs);

	switch (q->format) {
	case 1: // 1=ASCII
		for (int i = 0; i < q->num_nodes; i++)
			fscanf(fin_nodes,"%lf %lf\n",
					q->p+2*i,q->p+2*i+1);
		for (int i = 0; i < q->num_trigs; i++)
			fscanf(fin_trigs,"%d %d %d\n",
					q->t+3*i,q->t+3*i+1,q->t+3*i+2);
		break;
	case 2: // 2=BINARY
		fread(q->p,sizeof(double),2*q->num_nodes,fin_nodes);
		fread(q->t,sizeof(int)   ,3*q->num_trigs,fin_trigs);
		break;
	}
	fclose(fin_nodes);
	fclose(fin_trigs);

	// MSH indices starts from 1, C/C++ indices starts from 0
	for (int i = 0; i < 3*q->num_trigs; i++)
		q->t[i]--;

	// Assemble q->pt
	for (int i = 0; i < q->num_trigs; i++)  {
		//q->pt[6*i  ] = q->p[2*q->t[3*i  ]-2];  // x0
		//q->pt[6*i+1] = q->p[2*q->t[3*i  ]-1];  // y0
		//q->pt[6*i+2] = q->p[2*q->t[3*i+1]-2];  // x1
		//q->pt[6*i+3] = q->p[2*q->t[3*i+1]-1];  // y1
		//q->pt[6*i+4] = q->p[2*q->t[3*i+2]-2];  // x2
		//q->pt[6*i+5] = q->p[2*q->t[3*i+2]-1];  // y2
		q->pt[6*i  ] = q->p[2*q->t[3*i  ]  ];  // x0
		q->pt[6*i+1] = q->p[2*q->t[3*i  ]+1];  // y0
		q->pt[6*i+2] = q->p[2*q->t[3*i+1]  ];  // x1
		q->pt[6*i+3] = q->p[2*q->t[3*i+1]+1];  // y1
		q->pt[6*i+4] = q->p[2*q->t[3*i+2]  ];  // x2
		q->pt[6*i+5] = q->p[2*q->t[3*i+2]+1];  // y2
	} 

	q->status=3;
}

struct st_mesh *mshio_create_mesh(const char *fbase)
{
	struct st_mesh *q;

	q=init_mesh(fbase);
	read_info(q);
	alloc_mesh(q);
	read_mesh(q);

	return q;
}

void mshio_destroy_mesh(struct st_mesh *q)
{
	//fprintf(stderr,"release_mesh(struct st_mesh &q)\n");
	assert(q->status>=2);

	mkl_free(q->p);
	mkl_free(q->t);
	mkl_free(q->pt);

	q->status=-1;
}

void mshio_print_mesh(struct st_mesh *q, int flag)
{
	printf("FORMAT     = %d (1=ASCII 2=BINARY)\n", q->format);
	printf("STATUS     = %d\n", q->status);
	printf("NUM_NODES  = %d\n", q->num_nodes);
	printf("NUM_TRIGS  = %d\n", q->num_trigs);
	printf("NODES p    = %p\n", q->p);
	printf("TRIGS t    = %p\n", q->t);
	printf("TRIGS pt   = %p\n", q->pt);

	if (!flag) { printf("\n"); return; }

	assert(q->status==3); // after calling load_mesh()
	printf("nodes:\n");
	for (int i = 0; i < MIN(20,q->num_nodes); i++)
		printf("[%7d] (%8.3lf,%8.3lf)\n",
				i,
				q->p[2*i],q->p[2*i+1]);
	printf("trigs:\n");
	for (int i = 0; i < MIN(20,q->num_trigs); i++)
		printf("[%7d] (%8.3lf,%8.3lf) (%8.3lf,%8.3lf) (%8.3lf,%8.3lf)\n",
				i,
				q->pt[6*i  ],q->pt[6*i+1],
				q->pt[6*i+2],q->pt[6*i+3],
				q->pt[6*i+4],q->pt[6*i+5]);
	printf("\n");
}
