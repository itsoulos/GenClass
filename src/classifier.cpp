#include <math.h>

int classifier(double *x){

	int CLASS = 0;

	if(x[12]>((-378.7)/(-0.5))&&x[8]<((-1.764)+x[9])&&!(!(!(x[12]>=(((-738.2)+x[11])-x[12])&&x[6]<((-634.887)/(-599.8)))))) CLASS=0;
	else if(x[11]>x[1]&&x[7]<=x[6]&&x[3]>=(-23.98)||x[2]<x[11]||x[1]<(-368.7)&&x[4]>((x[4]+((-43.554)*x[8]))*x[3])) CLASS=1;
	else CLASS=2;
	
	return CLASS;
}
