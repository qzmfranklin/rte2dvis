#ifndef _TABLE_H_
#define _TABLE_H_
/******************************************************************************/
using namespace std;
class Table{
	private:
		int state;
		int width;
		const char* (*rowlist);
		const char* (*collist);
		double *datarr;
		int m, n; //dim of data
	public:
		Table();
		void set_width(int w);
		void dim(int nrows, int ncols);
		void rows(const char *rowsi[]);
		void cols(const char* colsi[]);
		void data(double *datai);
		void print(const char *banner="");
}; 
/******************************************************************************/
#endif
