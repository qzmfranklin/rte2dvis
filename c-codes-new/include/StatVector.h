#ifndef _STATVECTOR_H_
#define _STATVECTOR_H_
/******************************************************************************/
class StatVector{
	private:
		double *v;
		int size;
		int sortflag;
		int numitems;
		void dosort();
	public:
		StatVector(int n);		
		~StatVector();
		int getSize();
		void insert(double x);
		double median();
		double mean();
		double max();
		double min();
		void flush();
		double operator()(int i) const;
		void print(const char* banner = "");
};
/******************************************************************************/
#endif
