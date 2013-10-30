# include "dunavant-rule.h"

int main ( );
void test01 ( );
void test02 ( );
void test03 ( );
void test04 ( );
void test06 ( );

//****************************************************************************80

int main ( )

//****************************************************************************80
//
//  Purpose:
//
//    MAIN is the main program for DUNAVANT_PRB.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Discussion:
//
//    DUNAVANT_PRB calls the DUNAVANT test routines.
//
//  Modified:
//
//    11 December 2006
//
//  Author:
//
//    John Burkardt
//
{
	printf("\n");

	timestamp ( );

	printf("\n");
	printf("DUNAVANT_PRB:\n");
	printf("  C version\n");
	printf("  Test the DUNAVANT library.\n");


	test01 ( );
	test02 ( );
	test03 ( );
	test04 ( );
	/*test05 ( );*/
	test06 ( );
//
//  Terminate.
//
	printf("\n");
	printf("DUNAVANT_PRB\n");
	printf("  Normal end of execution.\n");

	timestamp ( );

	return 0;
}
//****************************************************************************80

void test01 ( )

//****************************************************************************80
//
//  Purpose:
//
//    TEST01 tests DUNAVANT_RULE_NUM, DUNAVANT_DEGREE, DUNAVANT_ORDER_NUM.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    11 December 2006
//
//  Author:
//
//    John Burkardt
//
{
	int degree;
	int order_num;
	int rule;
	int rule_num;

	printf("\n");
	printf("TEST01\n");
	printf("  DUNAVANT_RULE_NUM returns the number of rules\n");
	printf("  DUNAVANT_DEGREE returns the degree of a rules\n");
	printf("  DUNAVANT_ORDER_NUM returns the order of a rule\n");

	rule_num = dunavant_rule_num ( );

	printf("\n");
	printf("  Number of available rules = %d \n",rule_num);
	printf("\n");
	printf("\tRule\tDegree\tOrder\n");
	printf("\n");

	for ( rule = 1; rule <= rule_num; rule++ )
	{
		order_num = dunavant_order_num ( rule );
		degree = dunavant_degree ( rule );
		printf("\t%d\t%d\t%d\n",rule,degree,order_num);
	}


	return;
}
//****************************************************************************80

void test02 ( )

//****************************************************************************80
//
//  Purpose:
//
//    TEST02 tests DUNAVANT_RULE.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    11 December 2006
//
//  Author:
//
//    John Burkardt
//
{
	int order;
	int order_num;
	int rule;
	int rule_num;
	double *wtab;
	double wtab_sum;
	double *xytab;

	printf("\n");
	printf("TEST02\n");
	printf("  DUNAVANT_RULE returns the points and weights\n");
	printf("  of a Dunavant rule for the triangle\n");
	printf("\n");
	printf("  In this test, we simply check that the weights\n");
	printf("  sum to 1.\n");

	rule_num = dunavant_rule_num ( );

	printf("\n");
	printf("  Number of available rules = %d\n",rule_num);
	printf("\n");
	printf("\tRule\tOrder\tSum of weights\n");
	printf("\n");

	for ( rule = 1; rule <= rule_num; rule++ )
	{
		order_num = dunavant_order_num ( rule );

		xytab = (double*)malloc(sizeof(double)*2*order_num);
		wtab = (double*)malloc(sizeof(double)*order_num);

		dunavant_rule ( rule, order_num, xytab, wtab );

		wtab_sum = 0.0;
		for ( order = 0; order < order_num; order++ )
			wtab_sum = wtab_sum + wtab[order];

		printf("\t%d\t%d\t%f\n",rule,order_num,wtab_sum);

		free(wtab);
		free(xytab);
	}
	return;
}
//****************************************************************************80

void test03 ( )

//****************************************************************************80
//
//  Purpose:
//
//    TEST03 tests DUNAVANT_RULE.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    12 December 2006
//
//  Author:
//
//    John Burkardt
//
{
	int rule;
	int rule_num;
	int suborder;
	int suborder_num;
	double *suborder_w;
	double *suborder_xyz;
	double xyz_sum;

	printf("\n");
	printf("TEST03\n");
	printf("  DUNAVANT_RULE returns the points and weights\n");
	printf("  of a Dunavant rule for the triangle.\n");
	printf("\n");
	printf("  In this test, we simply check that, for each\n");
	printf("  quadrature point, the barycentric coordinates\n");
	printf("  add up to 1.\n");

	rule_num = dunavant_rule_num ( );

	printf("\n");
	printf("\tRule\tSuborder\tSum of coordinates\n");
	printf("\n");

	for ( rule = 1; rule <= rule_num; rule++ )
	{
		suborder_num = dunavant_suborder_num ( rule );

		suborder_xyz = (double*)malloc(sizeof(double)*3*suborder_num);
		suborder_w = (double*)malloc(sizeof(double)*suborder_num);

		dunavant_subrule ( rule, suborder_num, suborder_xyz, suborder_w );

		printf("\n");
		printf("\t%d\t%d\n",rule,suborder_num);

		for ( suborder = 0; suborder < suborder_num; suborder++ )
		{
			xyz_sum = suborder_xyz[0+suborder*3]
			          + suborder_xyz[1+suborder*3]
			          + suborder_xyz[2+suborder*3];
			printf("\t\t\t%25.16f\n",xyz_sum);
		}

		free(suborder_w);
		free(suborder_xyz);
	}
	return;
}
//****************************************************************************80

void test04 ( )

//****************************************************************************80
//
//  Purpose:
//
//    TEST04 tests DUNAVANT_RULE.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    11 December 2006
//
//  Author:
//
//    John Burkardt
//
{
	int a;
	double area = 0.5;
	int b;
	double coef;
	double err;
	double exact;
	int i;
	int order;
	int order_num;
	double quad;
	int rule;
	int rule_max = 10;
	double value;
	double *wtab;
	double x;
	double *xytab;
	double y;

	printf("\n");
	printf("TEST04\n");
	printf("  DUNAVANT_RULE returns the points and weights of\n");
	printf("  a Dunavant rule for the unit triangle.\n");
	printf("\n");
	printf("  This routine uses those rules to estimate the\n");
	printf("  integral of monomomials in the unit triangle.\n");

	for ( a = 0; a <= 10; a++ )
	{
		for ( b = 0; b <= 10 - a; b++ )
		{
//
//  Multiplying X**A * Y**B by COEF will give us an integrand
//  whose integral is exactly 1.  This makes the error calculations easy.
//
			coef = ( double ) ( a + b + 2 ) * ( double ) ( a + b + 1 );
			for ( i = 1; i <= b; i++ )
			{
				coef = coef * ( double ) ( a + i ) / ( double ) ( i );
			}

			/*cout << "\n";*/
			/*cout << "  Integrate " << coef*/
			/*<< " * X^" << a*/
			/*<< " * Y^" << b << "\n";*/
			/*cout << "\n";*/
			/*cout << "      Rule       QUAD           ERROR\n";*/
			/*cout << "\n";*/

			for ( rule = 1; rule <= rule_max; rule++ )
			{
				order_num = dunavant_order_num ( rule );

				xytab = (double*) malloc(sizeof(double)*2*order_num);
				wtab = (double*) malloc(sizeof(double)*order_num);

				dunavant_rule ( rule, order_num, xytab, wtab );

				quad = 0.0;

				for ( order = 0; order < order_num; order++ )
				{
					x = xytab[0+order*2];
					y = xytab[1+order*2];

					if ( a == 0 && b == 0 )
					{
						value = coef;
					}
					else if ( a == 0 && b != 0 )
					{
						value = coef * pow ( y, b );
					}
					else if ( a != 0 && b == 0 )
					{
						value = coef * pow ( x, a );
					}
					else if ( a != 0 && b != 0 )
					{
						value = coef * pow ( x, a ) * pow ( y, b );
					}
					quad = quad + wtab[order] * value;
				}
				quad = area * quad;

				exact = 1.0;
				err = fabs ( exact - quad );

				/*cout << "  " << setw(8)  << rule*/
				/*<< "  " << setw(14) << quad*/
				/*<< "  " << setw(14) << err << "\n";*/

				free(wtab);
				free(xytab);
			}
		}
	}
	return;
}
//****************************************************************************80

void test06 ( )
{
# define NODE_NUM 3

	double area;
	double area2;
	int i;
	int node;
	double node_xy[2*NODE_NUM] = {
		0.0, 0.0,
		1.0, 0.0,
		0.0, 1.0
	};
	double node_xy2[2*NODE_NUM] = {
		1.0, 2.0,
		1.0, 1.0,
		3.0, 2.0
	};
	int order;
	int order_num;
	int point_show = 2;
	int rule;
	double *w;
	double *xy;
	double *xy2;

	printf("\n");
	printf("TEST06\n");
	printf("  REFERENCE_TO_PHYSICAL_T3 transforms a rule\n");
	printf("  on the unit (reference) triangle to a rule on\n");
	printf("  an arbitrary (physical) triangle.\n");

	rule = 2;

	order_num = dunavant_order_num ( rule );

	xy = (double*)malloc(sizeof(double)*2*order_num);
	xy2 = (double*)malloc(sizeof(double)*2*order_num);
	w = (double*)malloc(sizeof(double)*order_num);

	dunavant_rule ( rule, order_num, xy, w );
//  Here is the reference triangle, and its rule.
	printf("\n");
	printf("  The reference triangle:\n");
	printf("\n");
	printf("\tpoint\t\t\tX\t\t\tY\n");
	for ( node = 0; node < NODE_NUM; node++ )
		printf("\t\t%d\t%17.8f\t%17.8f\n",node+1,node_xy[0+node*2],node_xy[1+node*2]);
	printf("\n");
	printf("  Rule for the reference triangle:\n");
	printf("\n");
	printf("\tX\t\t\tY\t\t\tW\n");
	for (order = 0; order < order_num; order++) 
		printf("%17.8f\t%17.8f\t%17.8f\n",xy[0+order*2],xy[1+order*2],w[order]);

	area = triangle_area ( node_xy );
//  Transform the rule.
	reference_to_physical_t3 ( node_xy2, order_num, xy, xy2 );
//  Here is the physical triangle, and its transformed rule.
	printf("\n");
	printf("  The physical triangle:\n");
	printf("\n");
	printf("\tpoint\t\t\tX\t\t\tY\n");
	for ( node = 0; node < NODE_NUM; node++ )
		printf("\t\t%d\t%17.8f\t%17.8f\n",node+1,node_xy2[0+node*2],node_xy2[1+node*2]);
	printf("\n");
	printf("  Rule for the physical triangle:\n");
	printf("\n");
	printf("\tX\t\t\tY\t\t\tW\n");
	for (order = 0; order < order_num; order++) 
		printf("%17.8f\t%17.8f\t%17.8f\n",xy2[0+order*2],xy2[1+order*2],w[order]);

	area2 = triangle_area ( node_xy2 );

	free(w);
	free(xy);
	free(xy2);

	return;
# undef NODE_NUM
}
