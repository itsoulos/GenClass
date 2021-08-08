#include <math.h>

int classifier(double *x){

	int CLASS = 0;

	if(x[3]>=x[0]&&(-2.788)*x[2]>=x[0]*x[4]||x[1]>=(((-09.578)+(x[3]*((-349.0)*(-6.9)*x[0])))*x[0])) CLASS=0;
	else if(x[2]-x[0]*x[0]>=(-4.9)) CLASS=1;
	else if(!(x[2]*x[1]-x[1]<=(-2.0)||x[4]>=((x[0]/(((x[0]+x[3])+(-4.45))-((-58.577)*x[0]-(-5.306))))*x[1]*x[4]))) CLASS=2;
	else if(x[2]/x[1]-x[2]<=(-9.7)||x[2]>=(-90.5)||!(x[4]<=(-16.98)||x[1]<x[0]&&x[2]<x[0]||!(x[4]*x[3]<=x[3]-x[1]||x[2]<=x[2]))) CLASS=3;
	else if(!(!(x[3]<=(-47.911)))) CLASS=4;
	else if(x[4]<=x[0]) CLASS=5;
	else CLASS=6;
	
	return CLASS;
}
