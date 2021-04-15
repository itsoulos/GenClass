# ifndef __POPULATION__H
# include <program.h>
#include <classprogram.h>
# include <omp.h> 

#define MAXTHREADS 32
/* The Population class holds the current population. */
/* The mutation, selection and crossover operators are defined here */
class Population
{
	private:
		int	**children;
		int	**trialx;
		int	**genome;
		int	*valid;
		double *fitness_array;
		double	mutation_rate,selection_rate;
		int	genome_count;
		int	genome_size;
		int	generation;
		Program	**program;

		double 	fitness(vector<int> &g);
		void	select();
		void	crossover();
		void	mutate();
		void	calcFitnessArray();
		int	elitism;
		int	tournament();
	public:
		Population(int gcount,int gsize,ClassProgram **p);
		void	setElitism(int s);
		int	getGeneration() const;
		int	getCount() const;
		int	getSize() const;
		void	nextGeneration();
		void	setMutationRate(double r);
		void	setSelectionRate(double r);
		double	getSelectionRate() const;
		double	getMutationRate() const;
		double	getBestFitness() const;
		double	evaluateBestFitness();
		vector<int> getBestGenome() const;
		void    localSearch(int pos);
		void	reset();
		~Population();
		
};
# define __POPULATION__H
# endif
