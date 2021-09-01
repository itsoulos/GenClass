# include <cprogram.h>
# include <stdio.h>
# include <math.h>
# include <iostream>
# include <fstream>
# include <string>
#include <algorithm>

using namespace std;

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

string getNonTerminal(string target){

	string retVal;
	if(target[0]=='<'){
		//cout << "size: " << target.size() << endl;
		//*
		for(size_t i = 1; i < target.size();i++){
			if(target[i]=='>') break;
			else retVal.push_back(target[i]);
		}
		if (retVal == string("S"))
			return string("START");//*/
		else
			return retVal;
	}

	return "";
}

void	Cprogram::makeNonTerminals()
{
extern char grammar_file[1024];
	ifstream GFile(grammar_file);
  if (GFile.is_open()){
//		cout << "Opened Grammar.bnf" << endl;
    string tp;
    while(getline(GFile, tp)){
			string nt;
			nt = getNonTerminal(tp);
			if(nt.size()>0){
					Symbol Sym;
					Sym.set(nt.c_str(), 0);
					CustomSymbols.push_back(Sym);
			}

    }
    GFile.close();
  }
	else {
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
}

size_t get_index(string Name, const vector<Symbol> &SymbolsList){
	for(size_t i = 0; i < SymbolsList.size(); i++){
		if(Name == string(SymbolsList[i].getName())){
			return i;
		}
	}
	return -1;
}

void Cprogram::createRule(string &head, string &body){
	if(body.size()<1) return;
	size_t r = newRule();
	size_t sym_index = -1;
	// Find NT index
	for(size_t i = 0; i < CustomSymbols.size(); i++){
		if(string(CustomSymbols[i].getName())==head){
			sym_index=i;
			break;
		}
	}
	while(body.length()>0){
		cout << body << endl;
		string temp;
		size_t temp_index;
		if(body[0]=='<' && body.length()>2){
			temp = getNonTerminal(body);
			for(size_t i = 0; i < CustomSymbols.size(); i++){
				if(string(CustomSymbols[i].getName())==temp){
					temp_index=i;
					break;
				}
			}
			rule[r]->addSymbol(&CustomSymbols[temp_index]);
			if(body.length()>temp.length()+2){
				body = body.substr(temp.length()+2);
			}
			else{
				break;
			}
			//cout << temp_index << " : " << temp << " -- " << body << endl;

		}
		else if(body.substr(0,6)=="Number"){
			rule[r]->addSymbol(&Number);
			body = body.substr(6);
		}
		else if(body.substr(0,2)=="in"){
			rule[r]->addSymbol(&In);
			body = body.substr(2);
		}
		else if(body.substr(0,1)=="#"){
			rule[r]->addSymbol(&Delim);
			body = body.substr(1);
		}
		else if(body.substr(0,2)=="pi"){
			rule[r]->addSymbol(&PI);
			body = body.substr(2);
		}
		else if(body.substr(0,1)=="+"){
			rule[r]->addSymbol(&Plus);
			body = body.substr(1);
			//cout << "PLUS" << body << endl;
		}
		else if(body.substr(0,1)=="-"){
			rule[r]->addSymbol(&Minus);
			body = body.substr(1);
			//cout << "MINUS" << body << endl;
		}
		else if(body.substr(0,1)=="*"){
			rule[r]->addSymbol(&Mult);
			body = body.substr(1);
		}
		else if(body.substr(0,1)=="/"){
			rule[r]->addSymbol(&Div);
			body = body.substr(1);
		}
		else if(body.substr(0,1)=="^"){
			rule[r]->addSymbol(&Pow);
			body = body.substr(1);
		}
		else if(body.substr(0,1)=="."){
			rule[r]->addSymbol(&Dot);
			body = body.substr(1);
		}
		else if(body.substr(0,1)=="("){
			rule[r]->addSymbol(&Lpar);
			body = body.substr(1);
		}
		else if(body.substr(0,1)==")"){
			rule[r]->addSymbol(&Rpar);
			body = body.substr(1);
		}
		else if(body.substr(0,3)=="sin"){
			rule[r]->addSymbol(&Sin);
			body = body.substr(3);
		}
		else if(body.substr(0,3)=="cos"){
			rule[r]->addSymbol(&Cos);
			body = body.substr(3);
		}
		else if(body.substr(0,3)=="exp"){
			rule[r]->addSymbol(&Exp);
			body = body.substr(3);
		}
		else if(body.substr(0,3)=="log"){
			rule[r]->addSymbol(&Log);
			body = body.substr(3);
		}
		else if(body.substr(0,1)=="!"){
			rule[r]->addSymbol(&Not);
			body = body.substr(1);
		}
		else if(body.substr(0,3)=="Add"){
			rule[r]->addSymbol(&addN);
			body = body.substr(3);
		}
		else if(body.substr(0,3)=="Sub"){
			rule[r]->addSymbol(&subN);
			body = body.substr(3);
		}
		else if(body.substr(0,4)=="Mult"){
			rule[r]->addSymbol(&multN);
			body = body.substr(4);
		}
		else if(body.substr(0,3)=="Div"){
			rule[r]->addSymbol(&divN);
			body = body.substr(3);
		}
		else if(body.substr(0,3)=="abs"){
			rule[r]->addSymbol(&Abs);
			body = body.substr(3);
		}
		else if(body.substr(0,4)=="sqrt"){
			rule[r]->addSymbol(&Sqrt);
			body = body.substr(4);
		}
		else if(body.substr(0,3)=="tan"){
			rule[r]->addSymbol(&Tan);
			body = body.substr(3);
		}
		else if(body.substr(0,3)=="int"){
			rule[r]->addSymbol(&Int);
			body = body.substr(3);
		}
		else if(body.substr(0,5)=="log10"){
			rule[r]->addSymbol(&Log10);
			body = body.substr(5);
		}
		else if(body.substr(0,3)=="min"){
			rule[r]->addSymbol(&Min);
			body = body.substr(3);
		}
		else if(body.substr(0,3)=="max"){
			rule[r]->addSymbol(&Max);
			body = body.substr(3);
		}
		else if(body.substr(0,4)=="near"){
			rule[r]->addSymbol(&Near);
			body = body.substr(4);
		}
		else if(body.substr(0,7)=="notnear"){
			rule[r]->addSymbol(&NotNear);
			body = body.substr(7);
		}
		else if(body.substr(0,6)=="kernel"){
			rule[r]->addSymbol(&Kernel);
			body = body.substr(6);
		}
		else if(body.substr(0,2)==">="){
			rule[r]->addSymbol(&Ge);
			body = body.substr(2);
		}
		else if(body.substr(0,1)==">"){
			rule[r]->addSymbol(&Gt);
			body = body.substr(1);
		}
		else if(body.substr(0,2)=="<="){
			rule[r]->addSymbol(&Le);
			body = body.substr(2);
		}
		else if(body.substr(0,1)=="<"){
			rule[r]->addSymbol(&Lt);
			body = body.substr(1);
		}
		else if(body.substr(0,1)=="="){
			rule[r]->addSymbol(&Eq);
			body = body.substr(1);
		}
		else if(body.substr(0,2)=="!="){
			rule[r]->addSymbol(&Neq);
			body = body.substr(2);
		}
		else if(body.substr(0,1)=="&"){
			rule[r]->addSymbol(&And);
			body = body.substr(1);
		}
		else if(body.substr(0,1)=="|"){
			rule[r]->addSymbol(&Or);
			body = body.substr(1);
		}
		else if(body.substr(0,4)=="jump"){
			rule[r]->addSymbol(&boolExpr);
			body = body.substr(4);
		}
		else if(body.substr(0,3)=="sig"){
			rule[r]->addSymbol(&Avg);
			body = body.substr(1);
		}
		else if(body.substr(0,1)=="x"){
			int num = atoi(body.substr(1).c_str());
			int x = num;
			int tot = 0;
			rule[r]->addSymbol(&XX[num-1]);
			while(num>0)
   		{
      	tot++;
      	num = num/10;
   		}
			body = body.substr(1+tot);
		}
		else{
			rule[r]->addSymbol(&Digit[atoi(body.c_str())]);
			body = body.substr(1);
		}

	}
	CustomSymbols[sym_index].addRule(rule[r]);
}

void	Cprogram::makeRules()
{

extern char grammar_file[1024];
  ifstream GFile(grammar_file);
  if (GFile.is_open()){
		//cout << "Opened Grammar.bnf" << endl;
    string tp;
		size_t nt_num = 0;
		string last_terminal("");
    while(getline(GFile, tp)){
			tp.erase(remove_if(tp.begin(), tp.end(), ::isspace), tp.end());
			if(tp[0]=='<'){
				last_terminal = getNonTerminal(tp);
				tp = tp.substr(tp.find("::=")+3);
				/*
				cout << tp.find("::=") << endl;

				cout << last_terminal << endl;//*/
			}
			else{
				tp = tp.substr(tp.find("|")+1);
			}
			createRule(last_terminal, tp);
      cout << "Rule Done" << "\n";
    }
		cout << "Rules Done" << "\n";
    GFile.close();
		for(size_t i = 0; i < CustomSymbols.size(); i++){
			if(string(CustomSymbols[i].getName())=="START"){
				cout << "START: " << i << endl;
				StartPtr=&CustomSymbols[i];
			}
		}
  }
	else {

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
		//	Expr.addRule(rule[r]);

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
		rule[r]->addSymbol(&Digit0);
		//rule[r]->addSymbol(&DigitList);
		DigitList.addRule(rule[r]);

		r=newRule();
		rule[r]->addSymbol(&Digit0);
		rule[r]->addSymbol(&Digit0);
		rule[r]->addSymbol(&Digit0);
		DigitList.addRule(rule[r]);

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
		StartPtr=&Start;
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
	return StartPtr;
}

Cprogram::~Cprogram()
{
	for(int i=0;i<rule.size();i++)
		delete rule[i];
}
