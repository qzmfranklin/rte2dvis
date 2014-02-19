#include <stdio.h>
#include "utils.h"
#include "msh_io.h"
/******************************************************************************/
//int test01(void);
//int test02(void);
int test03(void);
/******************************************************************************/
int main(int argc, char const* argv[])
{
	if (argc>1) link_stdout(argv[1]);

	//test01(); 
	//test02(); 
	test03(); 

	if (argc>1) unlink_stdout(); 


	return 0;
}

/*
 *int test01(void)
 *{
 *        int err=0; 
 *        printf("TEST01\n");
 *        printf("	|Test work flow of read\n"); 
 *
 *        char *fbase="msh/square162"; 
 *
 *        struct st_mesh q;
 *        printf("sizeof(struct st_mesh=%lu\n",sizeof(struct st_mesh)); 
 *
 *        init_mesh(q,fbase);
 *        print_mesh(q);
 *
 *        read_info(q);
 *        print_mesh(q);
 *
 *        alloc_mesh(q);
 *        print_mesh(q); 
 *
 *        read_mesh(q);
 *        print_mesh(q,1);
 *
 *        release_mesh(q);
 *        print_mesh(q);
 *
 *        printf("END OF TEST01\n");
 *        printf("\n");
 *        return err;
 *}
 */

/*
 *int test02(void)
 *{
 *        int err=0; 
 *        printf("TEST02\n");
 *        printf("	|Test work flow of read\n"); 
 *
 *        char *fbase="msh/square162"; 
 *
 *        struct st_mesh q;
 *
 *        init_mesh(q,fbase);
 *        print_mesh(q);
 *
 *        read_info(q);
 *        print_mesh(q);
 *
 *        alloc_mesh(q);
 *        print_mesh(q); 
 *
 *        read_mesh(q);
 *        print_mesh(q,1); // verbose
 *
 *        release_mesh(q);
 *        print_mesh(q);
 *
 *        printf("END OF TEST02\n");
 *        printf("\n");
 *        return err;
 *}
 */

int test03(void)
{
	int err=0; 
        printf("TEST03\n");
        printf("	|Test load_mesh()\n");

	char *fbase="msh/square162"; 
	struct st_mesh *q;
	q=load_mesh(fbase);
	print_mesh(q,PRINT_INFO_VERBOSE);
	
	destroy_mesh(q);
        printf("END OF TEST03\n");
        printf("\n");
	return err;
}

