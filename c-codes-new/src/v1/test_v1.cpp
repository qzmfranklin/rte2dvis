#include <complex.h>
#include <stdio.h>
#include <mkl.h>
#include "utils.h"
#include "file_io.h"
#include "QuadratureRules.h"
#include "toeplitz.h"
#include "v1.h"
/******************************************************************************/
int main(int argc, char const* argv[])
{
	if (argc>1) link_stdout(argv[1]);

	char fbase[]="example/square162.binary";
	struct st_mesh_info mesh;
	load_mesh(mesh,fbase); 
	print_mesh(mesh);


	//precalc();

	//solve();

	//dump();


	release_mesh(mesh);

	if (argc>1) unlink_stdout();

	return 0;
}