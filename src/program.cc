# include <program.h>		

Program::Program()
{
	start_symbol = NULL;
}

void	Program::setStartSymbol(Symbol *s)
{
	start_symbol = s;
}

Symbol	*Program::getStartSymbol() const
{
	return start_symbol;
}


string	Program::printRandomProgram(vector<int> &genome,int &redo)
{
	string str="";
	int count=0;
	Rule *r;
	int pos=genome[count]%start_symbol->getCountRules();
	r=start_symbol->getRule(genome[count]%start_symbol->getCountRules());
	redo = 0;
	str=r->printRule(genome,count,redo);
	return str;
}

double	Program::fitness(vector<int> &genome)
{
	return 0.0;
}

Program::~Program()
{
}
