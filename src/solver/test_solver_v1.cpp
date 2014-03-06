#include <stdio.h>
#include "mshio.h"
#include "solver_v1.h"
#include "utils.h"
/*****************************************************************************/
int main(int argc, char const* argv[]);
int test01(void);
/*****************************************************************************/
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
        printf("	|Test solver_v1 workflow\n");

	struct st_mesh *q=mshio_create_mesh("msh/square162");
	mshio_print_mesh(q,PRINT_INFO_VERBOSE);

	/*
	 * ipar[0] = M
	 * ipar[1] = Nd
	 * ipar[2] = pad
	 * ipar[3] = rule1
	 * ipar[4] = rule2
	 * ipar[5] = nu
	 * ipar[6] = nv
	 * ipar[7] = num_threads in omp
	 */
	//const int ipar[128]={1,3,1, 1,1,5,3,1};
	const int ipar[128]={1,50,32, 4,9,70,3,6};
	/*
	 * dpar[0] = g
	 * dpar[1] = phis (planewave incident)
	 * dpar[2] = mua (absorption coefficient)
	 * dpar[3] = mus (scattering coefficient)
	 */
	const double dpar[128]={0.8,0.0,1.0,2.0};

	struct st_solver_v1 *s=sv1_create_solver(q,ipar,dpar);
	sv1_print_solver(s);

	//for (int i = 0; i < s->Ns; i++)
		//printf("[%5d] %.3E\n",i,s->E[i]);


	sv1_destroy_solver(s); 
	mshio_destroy_mesh(q);

        printf("END OF TEST01\n");
        printf("\n");
	return err;
}
