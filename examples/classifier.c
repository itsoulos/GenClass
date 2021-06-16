#include <math.h>

int classifier(double *x){

	int CLASS = 0;

	if(((-038.99)-x[21])*x[4]<=((-9.0)/x[0])&&x[2]>x[13]*x[13]) CLASS=0;
	else CLASS=1;
	
	return CLASS;
}
