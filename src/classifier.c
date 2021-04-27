#include <math.h>

int classifier(double *x){

	int CLASS = 0;

	if(x[3]*x[4]/x[6]<=x[2]*x[2]) CLASS=0;
	else if(x[7]<=(x[5]*x[3]-x[7]*x[3])) CLASS=1;
	else if(x[1]<((-378.0)+(-471.2))) CLASS=2;
	else if(x[0]+((x[5]*(-822.057))+x[6])*x[5]>(-2.27)&&x[0]<(-570.5)||x[6]<=x[3]+x[3]) CLASS=3;
	else if(x[4]<(-038.116)) CLASS=4;
	else CLASS=5;
	
	return CLASS;
}
