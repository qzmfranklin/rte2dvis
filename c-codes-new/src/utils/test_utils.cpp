#include "utils.h"
#include <string.h>
#include <stdio.h>
#include <iostream>
/******************************************************************************/
using namespace std;

int test_Table();
void runstatv(int v);

int main(int argc, char const* argv[])
{
	char fname[FILENAME_MAX];
	if (!argc)
		sprintf(fname,"DEBUG/test_int_trig.txt");
	else
		strcpy(fname,argv[1]); 
	link_stdout(fname); 

	runstatv(100000);
	test_Table();

	unlink_stdout();

	return 0;
}

int test_Table(){
	const int m = 3;
	const int n = 4;
	const char* rows[3] = {"dog", "cat", "hat"};
	const char* cols[4] = {"Asia", "Europe", "Africa", "America"};
	double data[m*n];
	for(int i=0; i < m*n; i++)
		data[i] = rand()*1.0/RAND_MAX;

	Table table;
	table.dim(m, n);
	table.rows(rows);
	table.cols(cols);
	table.data(data);
	table.print("animals and continents");
	return 0;
}

//n=number of items to be inserted
//all numbers inserted are from 0 to 255
void runstatv(int n){
	long int pm = 32452843;
	StatVector v(n);
	for(int i=1; i <= n; i++){
		v.insert(i*pm%256);
	}
	printf("median\t=%f\n",v.median());
	printf("mean\t=%f\n",v.mean());
	printf("max\t=%f\n",v.max());
	printf("min\t=%f\n",v.min());
}
