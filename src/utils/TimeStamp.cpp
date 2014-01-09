#include "TimeStamp.h"
#include "StatVector.h"
#include <math.h>
/******************************************************************************/
TimeStamp::TimeStamp(int n): StatVector(n) {};

void TimeStamp::tic(){//does not use cpuid
	asm volatile("rdtsc" : "=a" (a1), "=d" (d1));
}
double TimeStamp::toc(){//does not use cpuid
	asm volatile("rdtsc" : "=a" (a2), "=d" (d2));
	double t;
	t = convert(a1, d1, a2, d2);
	insert( t );
	return t;	// compatible with the old TimeStamp
}
double TimeStamp::convert(unsigned int A1, unsigned int D1, 
		unsigned int A2, unsigned int D2){
	double val;
	val = D2-D1;
	val *= pow(2.0, 32.0);
	val = (val + A2) - A1;
	return val;
}
