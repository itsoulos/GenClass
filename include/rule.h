# ifndef __RULE__H
# include <symbol.h>
# include <doublestack.h>
# include <vector>
using namespace std;

/* Creates the rules based on the grammar file and adds the symbols */

class Rule
{
	private:
		vector<Symbol*> data;	
		int	length;
	public:
		Rule();
		void	addSymbol(Symbol *s);
		int	getSymbolPos(string s);
		Symbol	*getSymbol(int pos) const;
		void	setSymbol(int pos,Symbol *s);
		int	getLength() const;
		string	printRule(vector<int> genome,int &pos,int &redo);
		double	getValue(vector<int> genome,int &pos,int &redo,DoubleStack &stack,double *X);
		~Rule();
};
# define __RULE__H
# endif
