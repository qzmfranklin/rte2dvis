#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <mkl.h>
#include "msh_io.h" 
/******************************************************************************/ 
#define MALLOC_ALIGNMENT 64
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
/******************************************************************************/ 
static struct st_mesh* init_mesh   (const char *fbase);
static void read_info   (struct st_mesh *q);
static void alloc_mesh  (struct st_mesh *q);
static void read_mesh   (struct st_mesh *q);
/******************************************************************************/

void print_mesh(struct st_mesh *q, int flag)
{
	printf("FORMAT\t\t= %d (1=ASCII 2=BINARY)\n",	q->format);
	printf("STATUS\t\t= %d\n",	q->status);
	printf("NUM_NODES\t= %d\n",	q->num_nodes);
	printf("NUM_TRIGS\t= %d\n",	q->num_trigs);
	printf("NODES\t\t= %p\n",	q->nodes);
	printf("TRIGS\t\t= %p\n",	q->trigs);

	if (!flag) { printf("\n"); return; }

	assert(q->status==3); // after calling load_mesh()
	printf("nodes:\n");
	for (int i = 0; i < MIN(20,q->num_nodes); i++)
		printf("[%7d] (%8.3lf,%8.3lf)\n",
				i,
				q->nodes[2*i],q->nodes[2*i+1]);
	printf("trigs:\n");
	for (int i = 0; i < MIN(20,q->num_trigs); i++)
		printf("[%7d] (%8.3lf,%8.3lf) (%8.3lf,%8.3lf) (%8.3lf,%8.3lf)\n",
				i,
				q->trigs[6*i  ],q->trigs[6*i+1],
				q->trigs[6*i+2],q->trigs[6*i+3],
				q->trigs[6*i+4],q->trigs[6*i+5]);
	printf("\n");
}

struct st_mesh *load_mesh(const char *fbase)
{
	struct st_mesh *q;

	q=init_mesh(fbase);
	read_info(q);
	alloc_mesh(q);
	read_mesh(q);

	return q;
}

struct st_mesh *init_mesh(const char *fbase)
{
	struct st_mesh *q;
	q=(struct st_mesh*)mkl_malloc(sizeof(struct st_mesh),64);
	assert(q);

	strcpy(q->fbase,fbase);
	q->status=0;

	return q;
}

void read_info(struct st_mesh *q)
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

void alloc_mesh(struct st_mesh *q)
{
	//fprintf(stderr,"alloc_mesh(struct st_mesh &q)\n");
	assert(q->status==1);

	q->nodes=(double*)mkl_malloc(
			2 * (q->num_nodes) * sizeof(double),
			MALLOC_ALIGNMENT);
	q->trigs=(double*)mkl_malloc(
			6 * (q->num_trigs) * sizeof(double),
			MALLOC_ALIGNMENT);
	assert(q->nodes);
	assert(q->trigs);

	q->status=2;
}

void read_mesh(struct st_mesh *q)
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

	int *trigs;
	trigs=(int*)mkl_malloc(3*q->num_trigs*sizeof(int),MALLOC_ALIGNMENT);

	switch (q->format) {
	case 1: // 1=ASCII
		for (int i = 0; i < q->num_nodes; i++)
			fscanf(fin_nodes,"%lf %lf\n",
					q->nodes+2*i,q->nodes+2*i+1);
		for (int i = 0; i < q->num_trigs; i++)
			fscanf(fin_trigs,"%d %d %d\n",
					trigs+3*i,trigs+3*i+1,trigs+3*i+2);
		break;
	case 2: // 2=BINARY
		fread(q->nodes,sizeof(double),2*q->num_nodes,fin_nodes);
		fread(trigs   ,sizeof(int),3*q->num_trigs,fin_trigs);
		break;
	}
	fclose(fin_nodes);
	fclose(fin_trigs);

	// Assemble q->trigs
	// Note: MSH indices starts from 1, C/C++ indices starts from 0
	for (int i = 0; i < q->num_trigs; i++)  {
		q->trigs[6*i  ] = q->nodes[2*trigs[3*i  ]-2];  // x0
		q->trigs[6*i+1] = q->nodes[2*trigs[3*i  ]-1];  // y0
		q->trigs[6*i+2] = q->nodes[2*trigs[3*i+1]-2];  // x1
		q->trigs[6*i+3] = q->nodes[2*trigs[3*i+1]-1];  // y1
		q->trigs[6*i+4] = q->nodes[2*trigs[3*i+2]-2];  // x2
		q->trigs[6*i+5] = q->nodes[2*trigs[3*i+2]-1];  // y2
	} 
	mkl_free(trigs);


	q->status=3;
}

void destroy_mesh(struct st_mesh *q)
{
	//fprintf(stderr,"release_mesh(struct st_mesh &q)\n");
	assert(q->status>=2);

	mkl_free(q->nodes);
	mkl_free(q->trigs);

	q->status=-1;
}

/******************************************************************************/
int dump_msh(const char * fname_in, const char * fname_out, int format)
{
	int err=0;
	// Default fname_out
	if (  !fname_out  ) {
		printf("	fname_out == NULL, proceed with default fname_out:\n");
		printf("		%s\n",fname_in);
		fname_out = fname_in;
	}
	assert( fname_out);
	// Construct ouuput file names
	char *fname_out_nodes;
	char *fname_out_trigs;
	char *fname_out_info;
	fname_out_nodes 	= (char*) malloc( FILENAME_MAX * sizeof(char) );
	fname_out_trigs 	= (char*) malloc( FILENAME_MAX * sizeof(char) );
	fname_out_info 	= (char*) malloc( FILENAME_MAX * sizeof(char) );
	assert(fname_out_nodes);
	assert(fname_out_trigs);
	assert(fname_out_info );
	strcpy(fname_out_nodes,fname_out);
	strcpy(fname_out_trigs,fname_out);
	strcpy(fname_out_info, fname_out);
	strcat(fname_out_nodes,".nodes");
	strcat(fname_out_trigs,".trigs");
	strcat(fname_out_info, ".info" );

	// Open 4 files:
	// 	fin:		input MSH file
	// 	fout_nodes:	output ASCII nodes file
	// 	fout_trigs:	output ASCII trigs file
	// 	fout_info:	output ASCII info file
	/*printf("	------------------------------\n");*/
	FILE *fin;
	FILE *fout_nodes;
	FILE *fout_trigs;
	FILE *fout_info;
	// open fin for read
	//printf("	Opening file for read:\n");
	//printf("		%s\n",fname_in);
	if (  !(fin=fopen(fname_in,"r"))  ) {
		//printf("	Cannot open file for read:\n");
		//printf("	err = err + 1\n");
		err += 1;
	}	
	// open fout_nodes for write
	//printf("	Opening file for write:\n");
	//printf("		%s\n",fname_out_nodes);
	if (  !(fout_nodes=fopen(fname_out_nodes,"w"))  ) {
		//printf("	Cannot open file for write:\n");
		//printf("	err = err + 2\n");
		err += 2;
	}	
	// open fout_trigs for write
	//printf("	Opening file for write:\n");
	//printf("		%s\n",fname_out_trigs);
	if (  !(fout_trigs=fopen(fname_out_trigs,"w"))  ) {
		//printf("	Cannot open file for write:\n");
		//printf("	err = err + 4\n");
		err += 4;
	}	
	// open fout_info for write
	//printf("	Opening file for write:\n");
	//printf("		%s\n",fname_out_info);
	if (  !(fout_info=fopen(fname_out_info,"w"))  ) {
		//printf("	Cannot open file for write:\n");
		//printf("	err = err + 8\n");
		err += 8;
	}	
	// Abort if either occured
	if (  err  ) {
		//printf("	Abort with err = %2d\n",err);
		return err;
	} else {
		//printf("	Successfully opened all 4 files!\n");
	}
	//printf("	------------------------------\n");

	int nodes_num;
	int num_elements;
	int trigs_num;
	char str_Nodes[]="Nodes";
	char str_EndNodes[]="EndNodes";
	char str_Elements[]="Elements";
	char str_EndElements[]="EndElements";
	char buff[BUFSIZ];
	int i=0;
	// Find nodes
	//printf("	Search for \"%s\"...\n",str_Nodes);
	while (  (fgets(buff,BUFSIZ,fin))  ) {
		i++;
		if ( strstr(buff,str_Nodes)  ) {
			//printf("		Found \"%s\" at line %d\n",str_Nodes,i);
			break;
		}
	}
	// Scan nodes and write to fout_nodes
	fscanf(fin,"%d",&nodes_num);
	//printf("	Number of nodes is\n");
	//printf("		%d\n",nodes_num);
	//printf("	Start scanning nodes and writing to fname_out_nodes...\n");
	for (i = 0; i < nodes_num; i++) {
		int ip,z_zero;
		double x,y;
		fscanf(fin,"%d %lf %lf %d\n",&ip,&x,&y,&z_zero);
		//printf("\t\t\t%3d\t%12.8E\t%12.8E\n",ip,x,y);
		switch (format) {
		case 1:  // ASCII
			fprintf(fout_nodes,"%+25.19E %+25.19E\n",x,y);
			break;
		case 2:  // BINARY
			fwrite(&x,sizeof(double),1,fout_nodes);
			fwrite(&y,sizeof(double),1,fout_nodes);
			break;
		}
	}
	//printf("	Finished scanning nodes and writing.\n");
	//printf("	------------------------------\n");
	// Find trigs
	rewind(fin);
	i=0;
	//printf("	Search for \"%s\"...\n",str_Elements);
	while (  (fgets(buff,BUFSIZ,fin))  ) {
		i++;
		if ( strstr(buff,str_Elements)  ) {
			//printf("		Found \"%s\" at line %d\n.",str_Elements,i);
			break;
		}
	}
	// Scan trigs and write to fout_trigs
	fscanf(fin,"%d",&num_elements);
	i++;
	//printf("	Number of elements is\n");
	//printf("		%d\n",num_elements);
	trigs_num=0;
	//printf("	Start Scanning elements and writing trigs to fname_out_trigs...\n");
	int j;
	for ( j = 0; j < num_elements; j++) {
		int tri[5],n1,n2,n3;
		fscanf(fin,"%d %d %d %d %d",&tri[0],&tri[1],&tri[2],&tri[3],&tri[4]);
		fgets(buff,BUFSIZ,fin);
		i++;
		if (  (tri[1]==2) && (tri[4]==6)  ) {
			trigs_num++;
			if (  trigs_num == 1  ) {
				//printf("		Found first triangle\n");
				//printf("			at line %d\n",i);
				//printf("			as element #%d\n",j+1);
			}
			sscanf(buff," %d %d %d\n",&n1,&n2,&n3);
			/*printf("\t\t\t%3d\t%d\t%d\t%d\n",i+1,n1,n2,n3);*/
			switch (format) {
			case 1:  // ASCII
				//fprintf(fout_trigs,"%12d %12d %12d\n",n1,n2,n3);
				break;
			case 2:  // BINARY
				fwrite(&n1,sizeof(int),1,fout_trigs);
				fwrite(&n2,sizeof(int),1,fout_trigs);
				fwrite(&n3,sizeof(int),1,fout_trigs);
				break;
			}
		}
	}
	//printf("	Finished scanning elements and writing trigs.\n");
	//printf("	Found %d trigs.\n",trigs_num);
	// Write to fout_info
	//printf("	------------------------------\n"); 
	//printf("	Start writing into fname_out_info...\n");
	fprintf(fout_info,"%d\t\t# 1=ASCII 2=BINARY\n",format);
	fprintf(fout_info,"%d\t\t# number of nodes\n",nodes_num);
	fprintf(fout_info,"%d\t\t# number of trigs\n",trigs_num);
	//printf("	Finished writing into fname_out_info.\n");

	// Free memory
	//printf("	------------------------------\n");
	//printf("	Freeing memory...\n");
	fclose(fin);
	fclose(fout_nodes);
	fclose(fout_trigs);
	fclose(fout_info);
	free(fname_out_nodes);
	free(fname_out_trigs);
	free(fname_out_info);
	//printf("	------------------------------\n");

	//printf("END:	FILEIO_DUMP_MSH_FILE\n");
	//printf("--------------------------------------------------\n");

	return err;
}
