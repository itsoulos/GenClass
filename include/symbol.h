# ifndef __SYMBOL__H
# include <string>
# include <vector>
using namespace std;

/* Creates the symbols based on the grammar file and adds them to the rules */

class Rule;

class Symbol
{
	private:
		string name;
		vector<Rule*> rule;
		int	count_rules;
		int	is_terminal;
	public:
		Symbol();

		void	set(string s,int status);
		void	setName(string s);
		string	getName() const;
		
		void	setTerminalStatus(int status);
		int	getTerminalStatus() const;

		void	addRule(Rule *r);
		void	cut(int N);
		Rule	*getRule(int pos) const;
		int	getCountRules() const;
		~Symbol();
		
};

# define __SYMBOL__H
# endif
