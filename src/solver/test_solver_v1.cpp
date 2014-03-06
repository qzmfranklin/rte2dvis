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

	struct st_mesh *q=mshio_create_mesh("msh/square518");
	//mshio_print_mesh(q,PRINT_INFO_VERBOSE);

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
	//const int ipar[128]={1,3,1, 1,1,5,3, 1};
	const int ipar[128]={1,20,1, 1,1,5,3, 8};
	/*
	 * dpar[0] = g factor
	 * dpar[1] = mua (absorption coefficient)
	 * dpar[2] = mus (scattering coefficient)
	 * dpar[3] = phis (planewave incident)
	 */
	const double dpar[128]={0.7,1.0,2.0,0.0};

	struct st_solver_v1 *s=sv1_create_solver(q,ipar,dpar);
	sv1_print_solver(s);

	for (int i = 0; i < 10; i++)
		printf("[%5d] %.5E\n",i,s->K[i]); 

	// test sv1_mul
	double _Complex v1[50000],v2[50000];
	for (int i = 0; i < s->Ng; i++)
		v1[i] = 1;
	sv1_mul(s,v1,v2);
	//for (int i = 0; i < 20; i++)
		//printf("[%5d] %.10f + %.10f*I\n",i,creal(v2[i]),cimag(v2[i])); 

	// destroy
	sv1_destroy_solver(s); 
	mshio_destroy_mesh(q);

        printf("END OF TEST01\n");
        printf("\n");
	return err;
}
