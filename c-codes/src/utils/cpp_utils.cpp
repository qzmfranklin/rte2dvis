#include "cpp_utils.h"
#include <iostream>
#include <fstream>
/******************************************************************************/
/*
 * based on http://stackoverflow.com/questions/5252375/custom-c-assert-macro
 * works outside debug mode too
 */
#define assrt(cond) \
	do \
{ \
	if (!(cond)) \
	{ \
		fprintf(stderr,"ASSRT FAILED\n");		\
		fprintf(stderr,"file: %s\n",__FILE__);		\
		fprintf(stderr,"line: %d\n",__LINE__);		\
		__builtin_trap();				\
	} \
} while(0)  
/******************************************************************************/
using namespace std;
/******************************************************************************/ 
static std::streambuf *sbuf_backup;
static std::ofstream ofile;
static int linkcout_state = 0;

void link_cout(const char *fname){
	assrt(linkcout_state == 0);
	linkcout_state = 1;

	ofile.open(fname);
	assrt(ofile.good());
	sbuf_backup = cout.rdbuf();
	std::cout.rdbuf(ofile.rdbuf());
	assrt(cout.good());
}

void unlink_cout(){
	assrt(linkcout_state == 1);
	linkcout_state = 0;

	std::cout.rdbuf(sbuf_backup);
	ofile.close();
}
/******************************************************************************/
