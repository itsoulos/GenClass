# include <symbol.h>
Symbol::Symbol()
{
	name = "";
	count_rules = 0;
	is_terminal = 1;
}

/* Set a symbol (name,terminal status) */
void	Symbol::set(string s,int status)
{
	setName(s);
	setTerminalStatus(status);
}

/* Set the name of the symbol */
void	Symbol::setName(string s)
{
	name = s;
}

/* Get the name of the symbol */
string	Symbol::getName() const
{
	return name;
}

/* Set the terminal status of a symbol */
void	Symbol::setTerminalStatus(int status)
{
	is_terminal=status %  2;
}

/* Get the terminal status of a symbol */
int	Symbol::getTerminalStatus() const
{
	return is_terminal;
}

/* Add a new rule to the current rule stack */
void	Symbol::addRule(Rule *r)
{
	rule.push_back(r);
	count_rules++;
}

/* Get a rule for this symbol by position: return NULL if out of bounds */
Rule	*Symbol::getRule(int pos) const
{
	if(pos<0 || pos>=count_rules) return NULL;
	return rule[pos];
}

/* Count the rules for this symbol */
int	Symbol::getCountRules() const
{
	return count_rules;
}

/* Cut the rules to N */
void	Symbol::cut(int N)
{
	count_rules=N;
	rule.resize(N);
}

Symbol::~Symbol()
{
}
