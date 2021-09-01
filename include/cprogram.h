# ifndef __CPROGRAM__H
# include <symbol.h>
# include <rule.h>
# include <fparser.hh>
# include <string>
# define SCALE	1
extern int mperror;
class Cprogram
{
	protected:
		string		vars;
		FunctionParser 	parser;
		int		dimension,pdimension;
		vector<Rule*> rule;
		Symbol		Start, Expr, Near,NotNear,nearfunction,Number,
					addN,subN,multN,divN,
					function, binaryop, terminal,
					XXlist,DigitList, Digit0, Digit1,
				MinMax,Sin, Cos, Exp, Log, Abs, Sqrt,Avg,
				Min, PI, Max,Delim,BooleanExpr,In,NotIn;
		Symbol		andor,Not,boolExpr,inexpr,infunction,
					boolop,Gt,Lt,Ge,Le,Eq,Neq,And,Or;
		Symbol		Plus, Minus, Mult, Div, Pow;
		Symbol		Lpar, Rpar, Dot, Comma;
		Symbol		Tan, Int, Log10,Kernel,xlist;
		Symbol *StartPtr;
		vector<Symbol>	Digit;
		vector<Symbol>	XX;
		vector<Symbol> CustomSymbols;
		int			newRule();
		void			makeTerminals();
		void			makeNonTerminals();
		void			makeRules();
		void 			createRule(string &head, string &body);
	public:
		Cprogram(int dim,int pdim);
		int	Parse(string expr);
		double	Eval(const double *X);
		int	EvalError();
		Symbol	*getStartSymbol();
		~Cprogram();
};
# define __CPROGRAM__H
# endif
