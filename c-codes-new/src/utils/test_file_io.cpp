#include <stdio.h>
#include "utils.h"
/******************************************************************************/
int test01(void);
/******************************************************************************/
int main(int argc, char const* argv[])
{
	if (argc>1) link_stdout(argv[1]);
	if (argc>1) unlink_stdout();

	test01(); 

	return 0;
}

int test01(void)
{
	//TODO
	int err=0; 
        printf("TEST01\n");
        printf("	|Test work flow of read\n");


	char *fbase="example/mshtodat-example";


	struct st_mesh_info *q;
	q=(struct st_mesh_info*)malloc(sizeof(struct st_mesh_info)); 
	printf("sizeof(struct st_mesh_info=%lu\n",sizeof(struct st_mesh_info)); 

	init_mesh(q,fbase);
	print_mesh(q);

	read_info(q);
	print_mesh(q);

	alloc_mesh(q);
	print_mesh(q); 

	free(q);

        printf("END OF TEST01\n");
        printf("\n");
	return err;
}
