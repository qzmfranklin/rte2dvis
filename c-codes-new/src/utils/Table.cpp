#include "Table.h"
#include <iomanip>
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
Table::Table():state(0){
	width = 15;
};

void Table::set_width(int w){
	assrt(state == 0);
	width = w;
}
void Table::dim(int nrows, int ncols){
	assrt(state == 0);
	state = 1;
	m = nrows;
	n = ncols;
}

void Table::rows(const char *rowsi[]){
	assrt(state == 1);
	state = 2;
	rowlist = rowsi;
}

void Table::cols(const char* colsi[]){
	assrt(state == 2);
	state = 3;
	collist = colsi;
}

/*
 * templates might be a good idea here
 */
void Table::data(double *datai){
	assrt(state == 3);
	state = 4;
	datarr = datai;
}

static void drawline(int width, int n){
	for(int i=0; i <= n; i++)
		for(int j=0; j < width; j++)
			cout<<"-";
	cout<<endl;
}

static void drawblankline(int width, int n){
	for(int i=0; i <= n; i++){
		cout<<"|";
		for(int j=1; j < width; j++)
			cout<<" ";
	}
	cout<<"|";
	cout<<endl;
}

static void drawtoprow(int width, const char* cols[], int n){
	cout<<"|";
	for(int j=1; j < width; j++)
		cout<<" ";
	for(int i=0; i < n; i++){
		cout<<"|"<<setw(width-1)<<cols[i];
	}
	cout<<"|";
	cout<<endl;
}

static void drawrow(int width, 
		const char *row, double *data, int n){
	cout<<"|";
	cout<<setw(width-1)<<row;
	for(int i=0; i < n; i++){
		cout<<"|"<<setw(width-1)<<data[i];
	}
	cout<<"|"<<endl;
} 


void Table::print(const char* banner){
	assrt(state == 4);
	state = 1;
	cout<<endl;
	cout<<endl;
	cout<<"Table: "<<banner<<endl<<endl;
	drawline(width, n);
	drawblankline(width, n);
	drawtoprow(width, collist, n);
	drawblankline(width, n);
	drawline(width, n);
	double *rdata = new double[n];
	for(int i=0; i < m; i++){
		drawblankline(width, n);
		for(int j=0; j < n; j++)
			rdata[j] = datarr[i+j*m];
		drawrow(width, rowlist[i], rdata, n);
		drawblankline(width, n);
		drawline(width, n);
	}
	delete[] rdata;
}


