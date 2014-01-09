#include "StatVector.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <cstdio>
#include <cstdlib>
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
void StatVector::dosort()
{
	std::sort(v, v+numitems);
	sortflag = 1;
}

StatVector::StatVector(int n)
{
	size = n;
	v = new double[n];
	sortflag=-1;
	numitems=0;
}

StatVector::~StatVector()
{
	delete[] v;
}

int StatVector::getSize()
{
	return size;
}

void StatVector::insert(double x)
{
	assrt(numitems < size);
	v[numitems] = x;
	numitems++;
	sortflag = -1;
}

double StatVector::median()
{
	assrt(numitems > 0);
	if(sortflag != 1)
		dosort();
	return v[numitems/2];
}

double StatVector::mean()
{
	assrt(numitems > 0);
	if(sortflag != 1)
		dosort();
	double sum = 0;
	for(int i=0; i < numitems; i++)
		sum += v[i];
	return sum/numitems;
}

double StatVector::max()
{
	assrt(numitems > 0);
	if(sortflag != 1)
		dosort();
	return v[numitems-1];
}

double StatVector::min()
{
	assrt(numitems > 0);
	if(sortflag != 1)
		dosort();
	return v[0];
}

void StatVector::flush()
{
	sortflag = -1;
	numitems = 0; 
}

double StatVector::operator()(int i) const
{
	return v[i];
}

void StatVector::print(const char* banner)
{
	std::cout<<std::setw(50)<<banner<<std::endl;
	std::cout<<std::setw(10)<<"mean: "<<this->mean()<<std::endl;
	std::cout<<std::setw(10)<<"median: "<<this->median()<<std::endl;
	std::cout<<std::setw(10)<<"min: "<<this->min()<<std::endl;
	std::cout<<std::setw(10)<<"max: "<<this->max()<<std::endl;
}

