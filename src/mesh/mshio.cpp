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
	q->a =(double*)mkl_malloc(sizeof(double)*1*(q->num_trigs),64);
	assert(q->p);
	assert(q->t);
	assert(q->pt);
	assert(q->a);

	q->status=2;
}

// return (a1-a2)x(b1-b2)
static double det2x2(const double *restrict a1, const double *restrict a2,
		const double *restrict b1, const double *restrict b2)
{
	const double a=a1[0]-a2[0];
	const double b=a1[1]-a2[1];
	const double c=b1[0]-b2[0];
	const double d=b1[1]-b2[1];
	return a*c-b*d;
}
// return area of triangle defined by p1,p2,p3
static double trigarea(const double *restrict p1, const double *restrict p2, const double *restrict p3)
{
	return det2x2(p3,p1,p2,p1);
}
static void swp(int *a, int *b)
{
	int c=*a;
	*a=*b;
	*b=c;
}
static void swp(double *a, double *b)
{
	double c=*a;
	*a=*b;
	*b=c;
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

	// Assemble q->pt, ensure right-handed, i.e., have positive area
	for (int i = 0; i < q->num_trigs; i++)  {
		q->pt[6*i  ] = q->p[2*q->t[3*i  ]  ];  // x0
		q->pt[6*i+1] = q->p[2*q->t[3*i  ]+1];  // y0
		q->pt[6*i+2] = q->p[2*q->t[3*i+1]  ];  // x1
		q->pt[6*i+3] = q->p[2*q->t[3*i+1]+1];  // y1
		q->pt[6*i+4] = q->p[2*q->t[3*i+2]  ];  // x2
		q->pt[6*i+5] = q->p[2*q->t[3*i+2]+1];  // y2
		// if left-handed, swap p1 and p2
		if ((q->a[i]=trigarea(q->pt+6*i,q->pt+6*i+2,q->pt+6*i+4)) < 0.0) {
			swp(q->t +3*i  ,q->t +3*i+1);
			swp(q->pt+6*i  ,q->pt+6*i+2);
			swp(q->pt+6*i+1,q->pt+6*i+3);
			q->a[i] = -q->a[i];
		}
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
	mkl_free(q->a);

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
	printf("TRIGS a    = %p\n", q->a);

	if (!flag) { printf("\n"); return; }

	assert(q->status==3); // after calling mshio_create_mesh()
	printf("nodes:\n");
	for (int i = 0; i < MIN(20,q->num_nodes); i++)
		printf("[%7d] (%8.3lf,%8.3lf)\n",
				i,
				q->p [2*i  ],q->p [2*i+1]);
	printf("trigs:\n");
	for (int i = 0; i < MIN(20,q->num_trigs); i++)
		printf("[%6d] %7.2E (%6d,%6d,%6d) (%7.2E,%7.2E) (%7.2E,%7.2E) (%7.2E,%7.2E)\n",
				i,
				q->a [i    ],
				q->t [3*i  ],q->t [3*i+1],q->t [3*i+2],
				q->pt[6*i  ],q->pt[6*i+1],
				q->pt[6*i+2],q->pt[6*i+3],
				q->pt[6*i+4],q->pt[6*i+5]);

	printf("\n");
}
