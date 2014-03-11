#include "utils.h"
#include "sparse_mat.h"
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
	int err=0; 
        printf("TEST01\n");
        printf("	|Test rsparse_mat\n");

        printf("END OF TEST01\n");
        printf("\n");
	return err;
}
