# ifndef __CLASSPROGRAM__H
# include <program.h>
# include <cprogram.h>
# include <vector>
# include <string>
using namespace std;

typedef vector<double> Data;
class ClassProgram	:public Program
{
	private:
		vector<double> vclass;
		vector<string> pstring;
		vector<int>    pgenome;
		vector<double*> trainx;
		vector<double>  trainy;
		vector<double*> testx;
		vector<double>  testy;
		vector<double>  outy;
		Cprogram *program;
		vector<double> mapper;
		int dimension,nclass;
	public:
		ClassProgram(char *filename);
		string	printF(vector<int> &genome);
		void printPython(vector<int> &genome, std::string outname = "classifier.py");
		void printC(vector<int> &genome, std::string outname = "classifier.h");
		int	findMapper(double x);
		virtual double 	fitness(vector<int> &genome);
		double	getClassError(vector<int> &genome,char *filename);
		void	getOutputs(vector<double> &real,vector<double> &est);
		int	getClass() const;
		void	getTrainData(vector<Data> &tx,Data &ty);
		void	setTrainData(vector<Data> &tx,Data &ty);
		double	getClassError(vector<int> &genome,vector<Data> &tx,Data &ty);
		~ClassProgram();
};
# define __CLASSPROGRAM__H
# endif
