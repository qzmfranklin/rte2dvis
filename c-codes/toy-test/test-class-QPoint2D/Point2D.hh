#ifndef _QMATH_POINT2D_HH_
#define _QMATH_POINT2D_HH_

#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <mkl.h>

namespace QMath {

	class Line2D; // forward declaration of friend class

	class Point2D {
		public:
			friend class Line2D; // Line2D can access Point2D

		public:
			double fx;
			double fy;
			Point2D( double x, double y ) {
				fx = x; 
				fy = y; 
			}
			~Point2D(); 

			double Arg() {
				atan2( fy, fx );
			}
			double Norm() {
				return sqrt(fx*fx+fy*fy);
			}
			Point2D operator + ( const &p1, const &p2 ) {
				Point2D _tmp_p( p1.fx+p2.fx, p1.fy+p2.fy );
				return _tmp_p;
			}
			Point2D operator - ( const &p1, const &p2 ) {
				Point2D _tmp_p( p1.fx-p2.fx, p1.fy-p2.fy );
				return _tmp_p;
			}

				
	}; // class Point2D
	
} // namespace QMath

#endif /* end of protective macro _QMATH_POINT2D_HH_*/
