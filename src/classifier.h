#include <math.h>

int classifier(double *x){

	int CLASS = 0;

	if(!(!(x[12]>=(x[9]+((-21.48)*(((-57.758)/x[11])-x[3])))))) CLASS=0;
	else if(!(!(x[9]<((x[6]*x[0])-x[3]/x[11]+x[11])))) CLASS=1;
	else CLASS=2;
	
	return CLASS;
}
