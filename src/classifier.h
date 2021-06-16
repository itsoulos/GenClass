#include <math.h>

int classifier(double *x){

	int CLASS = 0;

	if(!(x[82]>=((-02.04)*(-2.1))&&x[527]<x[427]||x[393]<((-2.4)-(((-0.22)*x[207])-x[834]))&&!(!(!(x[48]<=x[746]&&x[702]>=((-17.1)-(-87.72))))))) CLASS=0;
	else CLASS=1;
	
	return CLASS;
}
