# include <rule.h>
# include <iostream>
Rule::Rule()
{
	length =0;
}

/* Adds a symbol to the stack */
void	Rule::addSymbol(Symbol *s)
{
	data.push_back(s);
	length++;
}

/* Return the position in the stack of a particular symbol */
int	Rule::getSymbolPos(string s)
{
	for(int i=0;i<length;i++)
		if(data[i]->getName()==s) return i;
	return -1;
}

/* Return the symbol from the stack given its position */
Symbol	*Rule::getSymbol(int pos) const
{
	if(pos<0 || pos>=length) return NULL;
	return data[pos];
}

/* Sets a symbol in a given position in the stack */
void	Rule::setSymbol(int pos,Symbol *s)
{
	if(pos<0 || pos>=length) return; 
	data[pos]=s;
}

/* Returns the stack length */
int	Rule::getLength() const
{
	return length;
}

/* Prints the expression for this grammar given an array of integer (a chromosome) */
string	Rule::printRule(vector<int> genome,int &pos,int &redo)
{

	string str="";
	string str2="";
	Rule *r;
	int old_pos;
	extern int wrapping;
	for(int i=0;i<length;i++)
	{
		Symbol *s=data[i];
		if(s->getTerminalStatus())
		{
			str=str+s->getName();	
		}
		else
		{
			if(pos>=genome.size()) {redo++;pos=0;}
			r=s->getRule((genome[pos]%s->getCountRules()));
			pos++;
			if(pos>=genome.size()) {redo++;pos=0;}
			if(redo>=wrapping) return str;
			str2=r->printRule(genome,pos,redo);
			str=str+str2;
		}
	}
	return str;
}

/* Evaluates and returns the function result given an instance of the functions (as produced by the given chromosome), */
/* the given grammar and an input vector X */
double	Rule::getValue(vector<int> genome,int &pos,int &redo,DoubleStack &stack,double *X)
{
	extern int wrapping;
	for(int i=0;i<length;i++)
	{
		Symbol *s=data[i];
		if(s->getTerminalStatus())
		{
			string str=s->getName();
			if(str==string("+"))
			{
				double a,b;
				a=stack.pop();
				b=stack.pop();
				cout<<"a ="<<a<<"b="<<b<<endl;
				stack.push(b+a);
			}
			else
			if(str=="-")
			{
				double a,b;
				a=stack.pop();
				b=stack.pop();
				stack.push(b-a);
			}
			else
			if(str=="*")
			{
				double a,b;
				a=stack.pop();
				b=stack.pop();
				stack.push(b*a);
			}
			else
			if(str=="x")
			{
				stack.push(X[0]);
			}
		}
		else
		{
		if(pos>=genome.size()) {redo++;pos=0;}
		int k=genome[pos]%s->getCountRules();
		Rule *r;
		double rnd;
		r=s->getRule(genome[pos]%s->getCountRules());
		pos++;
		if(pos>=genome.size()) {redo++;pos=0;}
		if(redo>=wrapping) return 0;
		return (r->getValue(genome,pos,redo,stack,X));
		}
	}
	return stack.top();
}


Rule::~Rule()
{
}

