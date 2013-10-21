#include "file-io.h"
#include <stdio.h><`2`>

int main(int argc, char *argv[])
{
	char *cwd;
	cwd = (char*)malloc(FILENAME_MAX*sizeof(char));
	getcwd(cwd,FILENAME_MAX);
	printf("%s/%s\n",cwd,argv[1]);

}
