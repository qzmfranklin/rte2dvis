#include "msh_ray_tracing.h"
#include "utils.h"
/******************************************************************************/
int test01(void);
/******************************************************************************/
int main(int argc, char const* argv[])
{ 
	if (argc>1) link_stdout(argv[1]); 
	test01();
	if (argc>1) unlink_stdout(); 

	return 0;
}

int test01(void)
{
	int err=0; 
        printf("TEST01\n");
        printf("	|Test /*something to test/*\n");
	
        printf("END OF TEST01\n");
        printf("\n");
	return err;
}
