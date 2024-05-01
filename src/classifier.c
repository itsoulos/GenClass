#include <math.h>

int classifier(double *x){

	int CLASS = 0;

	if(x[3]<=(((-533.45)+(x[12]+(x[9]-x[1]*(-35.455))))-(x[12]/x[6]))&&x[11]<x[9]) CLASS=0;
	else if(!(!(!(x[10]<x[1]&&x[2]>x[11]*x[6]&&!(x[6]>x[9]||x[10]>=x[7]*x[9]&&x[12]>((x[7]+(-9.15))+((((-373.36)-x[0])-(-2.86))*x[1]))))))) CLASS=1;
	else CLASS=2;
	
	return CLASS;
}
