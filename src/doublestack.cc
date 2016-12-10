# include <doublestack.h>
# include <iostream>
# include <stdio.h>
# include <stdlib.h>
# include <math.h>
using namespace std;

DoubleStack::DoubleStack()
{
	data = (double*)malloc(512*sizeof(double));//NULL;
	counter =0;
}

void	DoubleStack::clear()
{
	counter = 0;
}

int	DoubleStack::size() const
{
	return counter;
}

void DoubleStack::push(double x)
{
	if(isnan(x) || isinf(x)) {x=1e+8; }
	if(counter>=512) 
	{
		data=(double*)realloc(data,(counter+1)*sizeof(double));
	}
	data[counter]=x;
	counter++;
}

double  DoubleStack::top() const
{
	return (counter!=0)?data[counter-1]:-1;
}

double  DoubleStack::pop()
{
	if(!counter) return -1;
	double t=data[counter-1];
	counter--;
	return t;
}

DoubleStack::~DoubleStack()
{
	free(data);
}
