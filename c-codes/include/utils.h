#ifndef __dvutils25June2013__
#define __dvutils25June2013__
#include <cstdio>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <mkl.h>

/*
 * based on http://stackoverflow.com/questions/5252375/custom-c-assert-macro
 * works outside debug mode too
 */
#define assrt(cond) \
	do \
{ \
	if (!(cond)) \
	{ \
		std::cout<<"ASSRT FAILED:"<<#cond<<std::endl	\
		<<"file: "<<__FILE__<<std::endl	\
		<<"line: "<<__LINE__<<std::endl;	\
		__builtin_trap();				\
	} \
} while(0)  

namespace utils {
	const double PI = 3.1415926535897932384e+00;
	class Random{
		private:
			VSLStreamStatePtr stream;
			Random(const Random& );
		public:
			Random(unsigned int seed = 0){
				int status = vslNewStream(&stream, VSL_BRNG_MCG59, seed);
			}
			/* returns normally distributed random variable of variance sigma */
			double randn(double sigma = 1.0){
				double val;
				int status = vdRngGaussian(VSL_METHOD_DGAUSSIAN_ICDF,
						stream, 1, &val, 0, sigma);//mean=0
				return val;
			}
			/* returns variable uniformly distributed in [a,b] */
			double rand(double a = 0.0, double b = 1.0){
				double val;
				int status = vdRngUniform(VSL_METHOD_DUNIFORM_STD_ACCURATE,
						stream, 1, &val, a, b);
				return val;
			}
			~Random(){
				int status = vslDeleteStream(&stream);
			}
	};
	class Table{
		private:
			int state;
			int width;
			const char* (*rowlist);
			const char* (*collist);
			double *datarr;
			int m, n; //dim of data
		public:
			Table():state(0){
				width = 15;
			};
			/*
			 * change column width from default of 15
			 */
			void set_width(int w){
				assrt(state == 0);
				width = w;
			}
			void dim(int nrows, int ncols){
				assrt(state == 0);
				state = 1;
				m = nrows;
				n = ncols;
			}

			void rows(const char *rowsi[]){
				assrt(state == 1);
				state = 2;
				rowlist = rowsi;
			}

			void cols(const char* colsi[]){
				assrt(state == 2);
				state = 3;
				collist = colsi;
			}

			/*
			 * templates might be a good idea here
			 */
			void data(double *datai){
				assrt(state == 3);
				state = 4;
				datarr = datai;
			}

			void print(const char *banner="");
	};
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
	class StatVector; // Forward declaration
	class TimeStamp: public StatVector {
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
			double convert(unsigned int A1, unsigned int D1, unsigned int A2, unsigned int D2){
				//printf("TimeStamp::convert()\n");
				double val;
				val = D2-D1;
				val *= pow(2.0, 32.0);
				val = (val + A2) - A1;
				return val;
			}
	};

	/*
	 * v[i] = fabs(v[i]) for i=0...n-1
	 */
	void array_abs(double *v, int n);

	/*
	 * returns max of abs values of v[0..n-1]
	 */
	double array_max(double *v, int n);

	/*
	 * display n entries of v on stdout
	 */
	void array_show(double *v, int n, const char* mesg);

	/*
	 * v = v - w
	 */
	void array_diff(double *restrict v, double *restrict w, int n);

	/*
	 * copy w = v
	 */
	void array_copy(double *restrict v, double *restrict w, int n);

	/*
	 * v[]   = array to be output (column major)
	 * m     = num of rows
	 * n     = num of cols 
	 * fname = name of file for output
	 *         (uses stdout if fname == NULL)
	 */
	void array_out(double *v, int m, int n, const char *fname = NULL);

	/*
	 * same as above except for lda
	 * i,j th entry = v[i+j*lda]
	 * uses stdout if fname == NULL
	 */
	void array_out(double *v, int m, int n, int lda, const char *fname = NULL);

	/*
	 * counterpart of array_out()
	 * size = number of entries of v
	 */
	void array_in(double *v, int size,  const char* fname);

	/*
	 * A = 4 doubles in column major format
	 * rhs = 2 doubles
	 * x = soln of Ax=rhs
	 */
	void solve2x2(double *restrict A, double *restrict rhs, double *restrict x);

	/*
	 * verify if the directory exists
	 * if not create it
	 */
	void verify_dir(const char *dir);

	/*
	 * open file fname and link its stream to that of cout
	 */
	void link_cout(const char *fname);

	/*
	 * close file opened with link_cout and restore cout stream
	 */
	void unlink_cout();

	/*
	 * open file fname and link its stream to that of stdout
	 */
	void link_stdout(const char *fname);

	/*
	 * close file opened with link_stdout and restore stdout stream
	 */
	void unlink_stdout();

	/*
	 * prints the file to cout (omitting unprintable characters)
	 * the contents of the file are put inside a box
	 */
	void box_file(const char* fname, const char* mesg);

	/*
	 * getpid() and use it to copy /proc/pid/status to std::cout
	 */
	void print_proc_status(const char* mesg);

	/*
	 * initialize a vector randomly
	 */
	void init_vector(int n, double *v, int incx=1);

	/*
	 * initialize a matrix randomly
	 */
	void init_matrix(int m, int n, double *a, int lda=0);
	/*
	 * print the content of a vector
	 */
	void print_vector(const char* desc, int n, double* v, int incx=1);

	/*
	 * print the content of a matrix in col-major
	 */
	void print_matrix(const char* desc, int m, int n, double* a, int lda=0);
	


} // namespace Utils
#endif