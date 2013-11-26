#ifndef _STATVECTOR_H_
#define _STATVECTOR_H_
/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
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
class StatVector{
	double *v;
	int size;
	int sortflag;
	int numitems;
	void dosort(){
		std::sort(v, v+numitems);
		sortflag = 1;
	}
	public:
	StatVector(int n){
		size = n;
		v = new double[n];
		sortflag=-1;
		numitems=0;
	}
	~StatVector(){
		delete[] v;
	}
	int getSize(){
		return size;
	}
	void insert(double x){
		assrt(numitems < size);
		v[numitems] = x;
		numitems++;
		sortflag = -1;
	}
	double median(){
		assrt(numitems > 0);
		if(sortflag != 1)
			dosort();
		return v[numitems/2];
	}
	double mean(){
		assrt(numitems > 0);
		if(sortflag != 1)
			dosort();
		double sum = 0;
		for(int i=0; i < numitems; i++)
			sum += v[i];
		return sum/numitems;
	}
	double max(){
		assrt(numitems > 0);
		if(sortflag != 1)
			dosort();
		return v[numitems-1];
	}
	double min(){
		assrt(numitems > 0);
		if(sortflag != 1)
			dosort();
		return v[0];
	}
	void flush(){
		sortflag = -1;
		numitems = 0; 
	}
	double operator()(int i) const{
		return v[i];
	}
	void print(const char* banner = ""){
		std::cout<<std::setw(50)<<banner<<std::endl;
		std::cout<<std::setw(10)<<"mean: "<<this->mean()<<std::endl;
		std::cout<<std::setw(10)<<"median: "<<this->median()<<std::endl;
		std::cout<<std::setw(10)<<"min: "<<this->min()<<std::endl;
		std::cout<<std::setw(10)<<"max: "<<this->max()<<std::endl;
	}
};
/******************************************************************************/
#endif
