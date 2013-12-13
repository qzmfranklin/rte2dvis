#ifndef _TIMESTAMP_H_
#define _TIMESTAMP_H_
/******************************************************************************/
#include "StatVector.h"
/******************************************************************************/
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
		TimeStamp(int n);
		void tic();
		double toc();
		double convert(unsigned int A1, unsigned int D1, 
				unsigned int A2, unsigned int D2);
};
/******************************************************************************/
#endif
