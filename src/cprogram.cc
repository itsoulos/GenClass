# include <cprogram.h>
# include <stdio.h>
# include <math.h>

extern vector<double> lmargin;
extern vector<double> rmargin;


static double dmax(double a,double b)
{
	return a>b?a:b;
}

static double dmin(double a,double b)
{
	return a<b?a:b;
}

double between(const double *X)
{
	return X[0]>=dmin(X[1],X[2]) && X[0]<=dmax(X[1],X[2]);
}

double not_between(const double *X)
{
	return !(X[0]>=dmin(X[1],X[2]) && X[0]<=dmax(X[1],X[2]));
}

double near(const double *X)
{
	double d=dmax(X[1],X[2])-dmin(X[1],X[2]);
	return fabs(X[0]-dmin(X[1],X[2]))<d/10 
	  ||
	       fabs(X[0]-dmax(X[1],X[2])<d/10);
}
double notnear(const double *X)
{
	double d=dmax(X[1],X[2])-dmin(X[1],X[2]);
	return !(fabs(X[0]-dmin(X[1],X[2]))<d/10 
	  ||
	       fabs(X[0]-dmax(X[1],X[2])<d/10));
}

double near2(const double *X)
{
	return fabs(X[0]-dmin(X[1],X[2]))<X[3] 
	  ||
	       fabs(X[0]-dmax(X[1],X[2])<X[3]);
}

double notnear2(const double *X)
{
	return !(fabs(X[0]-dmin(X[1],X[2]))<X[3]
	  ||
	       fabs(X[0]-dmax(X[1],X[2])<X[3]));
}

double	kernel(const double *X)
{
	return exp((X[0]-X[1])/(2.0 * X[2]));
}

double	sig(const double *x)
{
	return 1.0/(1.0+exp(-x[0]));
}


Cprogram::Cprogram(int dim,int pdim)
{
	parser.AddFunction("in",between,3);
	parser.AddFunction("near",near,3);
	parser.AddFunction("notnear",notnear,3);

	parser.AddFunction("near2",near2,4);
	parser.AddFunction("notnear2",notnear2,4);
	parser.AddFunction("sig",sig,1);

	dimension = dim;
	pdimension = pdim;
	makeTerminals();
	makeNonTerminals();
	makeRules();
}

int	Cprogram::newRule()
{
	int r;
	r=rule.size();
	rule.resize(r+1);
	rule[r]=new Rule();
	return r;
}

void	Cprogram::makeTerminals()
{
	Number.set("Number",1);
	In.set("in",1);
	Delim.set("#",1);
	PI.set("pi",1);
	Plus.set("+",1);
	Minus.set("-",1);
	Mult.set("*",1);
	Div.set("/",1);
	Pow.set("^",1);
	Comma.set(",",1);
	Dot.set(".",1);
	Lpar.set("(",1);
	Rpar.set(")",1);

	Sin.set("sin",1);
	Cos.set("cos",1);
	Exp.set("exp",1);
	Log.set("log",1);
	Not.set("!",1);
	addN.set("Add",1);
	subN.set("Sub",1);
	multN.set("Mult",1);
	divN.set("Div",1);
	
	Abs.set("abs",1);
	Sqrt.set("sqrt",1);
	Tan.set("tan",1);
	Int.set("int",1);
	Log10.set("log10",1);
	Min.set("min",1);
	Max.set("max",1);
	Near.set("near",1);
	NotNear.set("notnear",1);
	Kernel.set("kernel",1);
	Gt.set(">",1);
	Ge.set(">=",1);
	Lt.set("<",1);
	Le.set("<=",1);
	Eq.set("=",1);
	Neq.set("!=",1);
	And.set("&",1);
	Or.set("|",1);
	boolExpr.set("jump",1);
	XX.resize(dimension);
	Avg.set("sig",1);
	vars="";
	for(int i=0;i<dimension;i++)
	{
		char str[100];
		sprintf(str,"x%d",i+1);
		XX[i].set(str,1);
		vars=vars+str;
		if(i<dimension-1) vars=vars+",";
	}
	Digit.resize(10);
	for(int i=0;i<10;i++)
	{
		char str[100];
		sprintf(str,"%d",i);
		Digit[i].set(str,1);
	}
}

void	Cprogram::makeNonTerminals()
{
	Start.set("START",0);
	xlist.set("XLIST",0);
	DigitList.set("DIGITLIST",0);
	Digit0.set("DIGIT0",0);
	Digit1.set("DIGIT1",0);
	XXlist.set("XXLIST",0);
	Expr.set("EXPR",0);
	function.set("FUNCTION",0);
	binaryop.set("BINARYOP",0);
	terminal.set("TERMINAL",0);
	MinMax.set("MINMAX",0);
	BooleanExpr.set("BOOLEXPR",0);
	boolop.set("BOOLOP",0);
	inexpr.set("INEXPR",0);
	infunction.set("INFUNCTION",0);
	andor.set("ANDOR",0);
	nearfunction.set("NEARFUNCTION",0);
}

void	Cprogram::makeRules()
{
	int r;
        r=newRule();
        rule[r]->addSymbol(&BooleanExpr);
        Start.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&XXlist);
	rule[r]->addSymbol(&boolop);
	rule[r]->addSymbol(&Expr);
	BooleanExpr.addRule(rule[r]);
	
	r=newRule();
	rule[r]->addSymbol(&Not);
	rule[r]->addSymbol(&Lpar);
	rule[r]->addSymbol(&BooleanExpr);
	rule[r]->addSymbol(&Rpar);
	BooleanExpr.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&XXlist);
	rule[r]->addSymbol(&boolop);
	rule[r]->addSymbol(&Expr);
	rule[r]->addSymbol(&And);
	rule[r]->addSymbol(&BooleanExpr);
	BooleanExpr.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&XXlist);
	rule[r]->addSymbol(&boolop);
	rule[r]->addSymbol(&Expr);
	rule[r]->addSymbol(&Or);
	rule[r]->addSymbol(&BooleanExpr);
	BooleanExpr.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Gt);
	boolop.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Ge);
	boolop.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Lt);
	boolop.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Le);
	boolop.addRule(rule[r]);
	
	r=newRule();
	rule[r]->addSymbol(&Lpar);
	rule[r]->addSymbol(&Expr);
	rule[r]->addSymbol(&binaryop);
	rule[r]->addSymbol(&Expr);
	rule[r]->addSymbol(&Rpar);
	Expr.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&function);
        rule[r]->addSymbol(&Lpar);
	rule[r]->addSymbol(&Expr);
	rule[r]->addSymbol(&Rpar);
	Expr.addRule(rule[r]);
		
	r=newRule();
	rule[r]->addSymbol(&Log);
	function.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Exp);
	function.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Sin);
	function.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Cos);
	function.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Plus);
	binaryop.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Minus);
	binaryop.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Mult);
	binaryop.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Div);
	binaryop.addRule(rule[r]);
	
	r=newRule();
	rule[r]->addSymbol(&Digit0);
	DigitList.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Digit0);
	rule[r]->addSymbol(&DigitList);
	DigitList.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Digit0);
	rule[r]->addSymbol(&Digit0);
	rule[r]->addSymbol(&Digit0);
	//DigitList.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&DigitList);
	rule[r]->addSymbol(&Dot);
	rule[r]->addSymbol(&DigitList);
	terminal.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&Lpar);	
	rule[r]->addSymbol(&Minus);
	rule[r]->addSymbol(&DigitList);
	rule[r]->addSymbol(&Dot);
	rule[r]->addSymbol(&DigitList);
	rule[r]->addSymbol(&Rpar);
	terminal.addRule(rule[r]);
	Expr.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&XXlist);
	Expr.addRule(rule[r]);

	for(int i=0;i<dimension;i++)
	{
		r=newRule();
		rule[r]->addSymbol(&XX[i]);
		XXlist.addRule(rule[r]);
	}

	r=newRule();
	rule[r]->addSymbol(&Expr);
	rule[r]->addSymbol(&Mult);
	rule[r]->addSymbol(&XXlist);
	XXlist.addRule(rule[r]);

	r=newRule();
	rule[r]->addSymbol(&XXlist);
	rule[r]->addSymbol(&binaryop);
	rule[r]->addSymbol(&XXlist);
	XXlist.addRule(rule[r]);

	for(int i=0;i<10;i++)
	{
		r=newRule();
		rule[r]->addSymbol(&Digit[i]);
		Digit0.addRule(rule[r]);
	}

}

int		Cprogram::Parse(string expr)
{
	return (parser.Parse(expr,vars)==-1);
}

int		Cprogram::EvalError()
{
	return	parser.EvalError();
}

double	Cprogram::Eval( const double *X)
{
		return parser.Eval(X);
}

Symbol	*Cprogram::getStartSymbol()
{
	return &Start;
}

Cprogram::~Cprogram()
{
	for(int i=0;i<rule.size();i++)
		delete rule[i];
}
