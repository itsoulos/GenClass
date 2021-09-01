#include <math.h>

int classifier(double *x){

	int CLASS = 0;

	if(x[24]>exp((sin(0.596)-cos(x[3])))||x[22]<=(-3.8)||x[26]<cos(log(x[4]))) CLASS=0;
	else CLASS=1;
	
	return CLASS;
}
