#ifndef _TIMESTAMP_H_
#define _TIMESTAMP_H_
/******************************************************************************/
#include "StatVector.h"
#include <math.h>
/******************************************************************************/
class StatVector; // Forward declaration
/*
 * Example of usage:
 *
 * 	#define N 100
 * 		TimeStamp clock(N);
 * 		for (int i = 0; i < N; i++) {
 * 			clock.tic();
 * 	     	// do some work here ... //
 * 	     	clock.toc();
 * 	     }
 * 	     printf("median # of cycles = %d\n",clock.median());
 * 	#undef N
 */
class TimeStamp: public StatVector {
	private:
		unsigned int a1, d1;
		unsigned int a2, d2;
	public:
		TimeStamp(int n): StatVector(n) {};
		void tic(){//does not use cpuid
			asm volatile("rdtsc" : "=a" (a1), "=d" (d1));
		}
		double toc(){//does not use cpuid
			asm volatile("rdtsc" : "=a" (a2), "=d" (d2));
			double t;
			t = convert(a1, d1, a2, d2);
			insert( t );
			return t;	// compatible with the old TimeStamp
		}
	private:
		double convert(unsigned int A1, unsigned int D1, 
				unsigned int A2, unsigned int D2){
			double val;
			val = D2-D1;
			val *= pow(2.0, 32.0);
			val = (val + A2) - A1;
			return val;
		}
};
/******************************************************************************/
#endif
