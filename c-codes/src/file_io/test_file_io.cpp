#include <unistd.h>
#include "file_io.h" 
#include <stdio.h>
#include <string.h>
#include <mkl.h>

int main(int argc, char *argv[]);
int test01(int argc, char *argv[]);
int test02(int argc, char *argv[]);
int test03(int argc, char *argv[]);
int test04(int argc, char *argv[]);
int test05(int argc, char *argv[]);
/******************************************************************************/
int main(int argc, char *argv[]) {
	int err=0;
	printf("###############################################################################\n");
	printf("START TESTING\n");
	printf("	FILE I/O\n");
	printf("\n");
	err += test01(argc,argv);
	err += test02(argc,argv);
	err += test03(argc,argv);
	err += test04(argc,argv);
	err += test05(argc,argv);
	if (!err)
		printf("NORMAL END OF EXECUTION\n");
	else {
		printf("ERROR CODE = %d\n",err);
		printf("END OF EXECUTION\n");
	}
	printf("###############################################################################\n");
	printf("\n");
	return err;
}
/******************************************************************************/
static int test01(int argc, char *argv[]) {
	int err = 0;
	printf("TEST01\n");
	printf("	|Read the system-dependent constant\n");
	printf("	|	FILENAME_MAX = %d\n",FILENAME_MAX);
	printf("	|Read the FIRST augument and the\n");
	printf("	|current working directory (cwd).\n");
	printf("	|Join them into a full path filename\n");
	printf("\n");

	char *cwd;
	cwd = (char*) malloc ( FILENAME_MAX * sizeof(char) );
	getcwd(cwd,FILENAME_MAX);
	int n1=strlen(cwd);
	int n2=strlen(argv[1]);
	printf("	The lengths of cwd and argv[1] are:\n");
	printf("			%d	%d\n",n1,n2);
	printf("	The last letters of cwd and argv[1] are:\n");
	printf("			%c	%c\n",cwd[n1-1],argv[1][n2-1]);
	strcat(cwd,"/");
	strcat(cwd,argv[1]);
	printf("	The file indicated was:\n");
	printf("		%s\n",cwd);
	free(cwd);
	printf("END OF TEST01\n");
	printf("\n");
	return err;
}

static int test02(int argc, char *argv[]) {
	int err=0;
	printf("TEST02\n");
	printf("	|Read the SECOND argument as the\n");
	printf("	|filename. Create the file. Write\n");
	printf("	|\"Hello World\" into it. Read and\n");
	printf("	|print out the content.\n");
	printf("\n");

	char *cwd;
	cwd = (char*) malloc ( FILENAME_MAX * sizeof(char) );
	getcwd(cwd,FILENAME_MAX);
	strcat(cwd,"/");
	strcat(cwd,argv[2]);
	printf("	Open file for write:\n");
	printf("		%s\n",cwd); 
	FILE *fp;
	if (  !(fp=fopen(cwd,"w+"))  ) { 
		// Cannot open file
		printf("	Cannot open file for write:\n");
		printf("		%s\n",cwd);
		printf("	Abort this test...\n");
		err = 2;
	}	
	else {
		// write to file
		printf("	Write to the file.\n");
		fprintf(fp,"Hello World\n");
		// read and print content
		rewind(fp);
		printf("	Read out the content:\n");
		char a[10],b[10];
		fscanf(fp,"%s %s\n",a,b);
		printf("	The first word is:	%s\n",a);
		printf("	The second word is:	%s\n",b);
		char cmd[80];
		strcat(cmd,"cat ");
		strcat(cmd,argv[2]);
		printf("	Calling system command to read:\n");
		printf("		$%s\n",cmd);
		system(cmd);
		printf("	Close file.\n");
		fclose(fp); 
	}
	free(cwd);
	printf("END OF TEST02\n");
	printf("\n");
	return err;
}

static int test03(int argc, char *argv[]) { 
	int err=0;
	printf("TEST03\n");
	printf("	|Read the system-dependent constant\n");
	printf("	|	BUFSIZ = %d\n",BUFSIZ);
	printf("	|Read out nodes and triangles from\n");
	printf("	|	argv[3] = \"%s\"\n",argv[3]);
	printf("	|Dump nodes and triangles.\n");
	printf("	|This test also serves as the trial\n");
	printf("	|for fileio_dump_msh_file().\n");
	printf("\n");

	char *cwd;
	cwd = (char*) malloc ( FILENAME_MAX * sizeof(char) );
	getcwd(cwd,FILENAME_MAX);
	strcat(cwd,"/");
	strcat(cwd,argv[3]);
	printf("	Open file for scan:\n");
	printf("		%s\n",cwd); 


	int nodes_num;
	int triangles_num;
	char str_Nodes[]="Nodes";
	char str_EndNodes[]="EndNodes";
	char str_Elements[]="Elements"; 
	char str_EndElements[]="EndElements"; 
	char buff[BUFSIZ];
	FILE *fp;
	if (  !(fp=fopen(cwd,"r"))  ) { 
		// Cannot open file
		printf("	Cannot open file for read:\n");
		printf("		%s\n",cwd);
		printf("	Abort this test...\n");
		err = 4;
	}	
	else {
		int i=0;
		// Find nodes
		printf("	Search for \"%s\"...\n",str_Nodes);
		while (  (fgets(buff,BUFSIZ,fp))  ) {
			i++;
			if ( strstr(buff,str_Nodes)  ) {
				printf("		Found \"%s\" at line %d\n",str_Nodes,i); 
				break;
			}
		}
		// Read nodes
		fscanf(fp,"%d",&nodes_num);
		printf("	Number of nodes is\n");
		printf("		%d\n",nodes_num);
		for (i = 0; i < nodes_num; i++) { 
			int ip,z_zero;
			double x,y;
			fscanf(fp,"%d %lf %lf %d\n",&ip,&x,&y,&z_zero);
			printf("\t\t\t%3d\t%12.8E\t%12.8E\n",ip,x,y);
		} 
		// Find triangles
		rewind(fp);
		i=0;
		printf("	Search for \"%s\"...\n",str_Elements);
		while (  (fgets(buff,BUFSIZ,fp))  ) {
			i++;
			if ( strstr(buff,str_Elements)  ) {
				printf("		Found \"%s\" at line %d\n",str_Elements,i); 
				break;
			}
		}
		// Read triangles
		int num_elements;
		fscanf(fp,"%d",&num_elements);
		printf("	Number of elements is\n");
		printf("		%d\n",num_elements);
		int tri[5],n1,n2,n3;
		i=0;
		for (i = 0; i < num_elements; i++) { 
			fscanf(fp,"%d %d %d %d %d",&tri[0],&tri[1],&tri[2],&tri[3],&tri[4]);
			fgets(buff,BUFSIZ,fp);
			if (  (tri[1]==2) && (tri[4]==6)  ) {
				sscanf(buff," %d %d %d\n",&n1,&n2,&n3);
				printf("\t\t\t%3d\t%d\t%d\t%d\n",i+1,n1,n2,n3);
			}
		}
		fclose(fp); 
	}
	free(cwd);
	printf("END OF TEST03\n");
	printf("\n");
	return err;
}

static int test04(int argc, char *argv[]) {
	int err = 0;
	printf("TEST04\n");
	printf("	|Test FILEIO_DUMP_MSH_FILE()\n");
	printf("	|	filename_in = argv[3] = %s\n",argv[3]);
	printf("	|	filename_out= argv[4] = %s\n",argv[4]);

	char *file_in;
	char *file_out;
	file_in = (char*) malloc ( FILENAME_MAX * sizeof(char) );
	file_out= (char*) malloc ( FILENAME_MAX * sizeof(char) );
	getcwd(file_in, FILENAME_MAX);
	getcwd(file_out,FILENAME_MAX);
	strcat(file_in, "/");
	strcat(file_out,"/");
	strcat(file_in, argv[3]);
	strcat(file_out,argv[4]); 
	printf("\n"); 

	printf("	Calling FILEIO_DUMP_MSH_FILE(  file_in,	file_out  )...\n");
	printf("		file_in  = %s\n",file_in);
	printf("		file_out = %s\n",file_out);
	err += fileio_dump_msh_file(file_in,file_out);
	printf("	Finished running FILEIO_DUMP_MSH_FILE().\n"); 

	free(file_in);
	free(file_out);
	printf("END OF TEST04\n");
	printf("\n");
	return err;
}

static int test05(int argc, char *argv[]) {
	int err = 0;
	printf("TEST05\n");
	printf("	|Test FILEIO_READ_INFO_NODES_TRIANGLES()\n");
	printf("	|	filename_base = argv[4] = %s\n",argv[4]);

	char *filename_base;
	filename_base = (char*) malloc ( FILENAME_MAX * sizeof(char) );
	getcwd(filename_base, FILENAME_MAX);
	strcat(filename_base, "/");
	strcat(filename_base, argv[4]);
	printf("\n"); 

	printf("	Calling FILEIO_READ_INFO_NODES_TRIANGLES(  filename_base, file_out  )...\n");
	printf("		filename_base  = %s\n",filename_base);
	double *nodes;
	int nodes_num;
	int *triangles;
	int triangles_num;
	fileio_read_info_nodes_triangles( filename_base, 
			&nodes, &nodes_num, &triangles, &triangles_num );
	printf("	Finished running FILEIO_READ_INFO_NODES_TRIANGLES().\n"); 

	printf("	Free memory using MKL_FREE.\n");
	free(filename_base);
	if (nodes) { mkl_free(nodes); }
	if (triangles) { mkl_free(triangles); }
	printf("END OF TEST05\n");
	printf("\n");
	return err;
}
