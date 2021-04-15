# include <population.h>
# include <stdlib.h>
# include <string.h>
# include <math.h>
# include <iostream>
# include <omp.h>

# define MAX_RULE	1024
unsigned int seed = 666999666 ;  // random number seed

/* Population constructor */
/* Input: genome count , genome size, pointer to Program instance */
Population::Population(int gcount,int gsize,ClassProgram **p)
{
	elitism=1;
	selection_rate = 0.1;
	mutation_rate  = 0.1;
	genome_count   = gcount;
	genome_size    = gsize;
	generation     = 0;
	program = new Program*[MAXTHREADS];
	for(unsigned i = 0; i < MAXTHREADS;i++)
		program[i]        = p[i];

	/* Create the population and based on genome count and size */
	/* Initialize the genomes to random */
	double f;
	genome=new int*[genome_count];
	children=new int*[genome_count];
	for(int i=0;i<genome_count;i++)
	{
		genome[i]=new int[genome_size];
		children[i]=new int[genome_size];
			for(int j=0;j<genome_size;j++)
				genome[i][j]=rand()%MAX_RULE;
	}
	fitness_array=new double[genome_count];
}

/* Reinitialize the population to random */
void	Population::reset()
{
	generation = 0;
	for(int i=0;i<genome_count;i++)
		for(int j=0;j<genome_size;j++)
				genome[i][j]=rand()%MAX_RULE;
	for(int i=0;i<genome_count;i++)
			fitness_array[i]=-1e+8;
}

/* Return the fitness of a genome */
double 	Population::fitness(vector<int> &g)
{
	return program[omp_get_thread_num()]->fitness(g);
}

/* The selection of the chromosomes according to the fitness value is performed */
void	Population::select()
{
	int itemp[genome_size];
	for(int i=0;i<genome_count;i++)
	{
		for(int j=0;j<genome_count-1;j++)
		{
			if(fitness_array[j+1]>fitness_array[j])
			{
				double dtemp;
				dtemp=fitness_array[j];
				fitness_array[j]=fitness_array[j+1];
				fitness_array[j+1]=dtemp;

				memcpy(itemp,genome[j],genome_size*sizeof(int));
				memcpy(genome[j],genome[j+1],genome_size*sizeof(int));
				memcpy(genome[j+1],itemp,genome_size*sizeof(int));
			}
		}
	}
}

int	Population::tournament()
{
    double max_fitness=-1e+10;
    const int tournament_size =(genome_count<=100)?4:20;
    int    max_index=-1;
    int r;
// Select the best parents of  the candidates
    for(int j=0;j<tournament_size;j++)
    {
	r=rand() % (genome_count);
        if(j==0 || fitness_array[r]>max_fitness)
        {
            max_index=r;
            max_fitness=fitness_array[r];
         }
     }
	return max_index;
}

/* Crossover operation: based on tournament selection */
/* Select the tournament_size based on the genome count : */
/*     (if genome_count > 100 ) tournament_size = 10   else   tournament_size = 4 */
/* Select 2 chromosomes based on the tournament size and cross them over based on the crossover probability */
/* There is 1 crossover point and it is random */
void	Population::crossover()
{
        int parent[2];
        int nchildren=(int)((1.0 - selection_rate) * genome_count);
	if(!(nchildren%2==0)) nchildren++;
        int count_children=0;
        while(1)
        {
		// The two parents are selected here according to the tournament selection procedure
                for(int i=0;i<2;i++)
                {
                        parent[i]=tournament();
                }
		int pt1,pt2;
		// The one-point crossover is performed here (the point is pt1)
		pt1=rand() % genome_size;
		memcpy(children[count_children],
				genome[parent[0]],pt1 * sizeof(int));
		memcpy(&children[count_children][pt1],
			&genome[parent[1]][pt1],(genome_size-pt1)*sizeof(int));
		memcpy(children[count_children+1],
				genome[parent[1]],pt1 * sizeof(int));
		memcpy(&children[count_children+1][pt1],
			&genome[parent[0]][pt1],(genome_size-pt1)*sizeof(int));
		count_children+=2;
		if(count_children>=nchildren) break;
	}


	for(int i=0;i<nchildren;i++)
	{
		memcpy(genome[genome_count-i-1],
				children[i],genome_size * sizeof(int));
	}
}

void	Population::setElitism(int s)
{
	elitism = s;
}

/* Mutate the current population */
/* Standard mutation algorithm: mutate all chromosomes in the population based on the mutation probability */
void	Population::mutate()
{
	int start = 1;
	for(int i=start;i<genome_count;i++)
	{
		for(int j=0;j<genome_size;j++)
		{
			double r=rand()*1.0/RAND_MAX;
			if(r<mutation_rate)
				genome[i][j]=rand() %MAX_RULE;
		}
	}
}

/* Evaluate the fitness for all chromosomes in the current population */
void	Population::calcFitnessArray()
{
	vector<int> g[MAXTHREADS];
	for(unsigned i = 0; i < MAXTHREADS; i++)
		g[i].resize(genome_size);
		printf("%d\n",MAXTHREADS);
#pragma omp parallel for num_threads(MAXTHREADS) schedule(dynamic)
	for(int i=0;i<genome_count;i++)
	{
		for(int j=0;j<genome_size;j++) g[omp_get_thread_num()][j]=genome[i][j];
		fitness_array[i]=fitness(g[omp_get_thread_num()]);
	}
}

/* Return the current generation */
int	Population::getGeneration() const
{
	return generation;
}

/* Return the genome count */
int	Population::getCount() const
{
	return genome_count;
}

/* Return the size of the population */
int	Population::getSize() const
{
	return genome_size;
}

/* Evolve the next generation */
void	Population::nextGeneration()
{
	if(generation)
	mutate();
	calcFitnessArray();

//*
	  if(generation && generation%20==0)
#pragma omp threadprivate(seed)
#pragma omp parallel for num_threads(MAXTHREADS) schedule(dynamic)
                for(int i=0;i<50;i++)
                                localSearch(rand() % genome_count);
//*/
	select();
	crossover();
	++generation;
}

/* Set the mutation rate */
void	Population::setMutationRate(double r)
{
	if(r>=0 && r<=1) mutation_rate = r;
}

/* Set the selection rate */
void	Population::setSelectionRate(double r)
{
	if(r>=0 && r<=1) selection_rate = r;
}

/* Return the selection rate */
double	Population::getSelectionRate() const
{
	return selection_rate;
}

/* Return the mutation rate */
double	Population::getMutationRate() const
{
	return mutation_rate;
}

/* Return the best fitness for this population */
double	Population::getBestFitness() const
{
	return  fitness_array[0];
}

/* Return the best chromosome */
vector<int> Population::getBestGenome() const
{
	vector<int> g;g.resize(genome_size);
	for(int i=0;i<genome_size;i++) g[i]=genome[0][i];
	return g;
}

/* Evaluate and return the best fitness for all chromosomes in the population */
double	Population::evaluateBestFitness()
{
	vector<int> g;g.resize(genome_size);
	for(int i=0;i<genome_size;i++) g[i]=genome[0][i];
	return fitness(g);
}

unsigned int randUint( void )
{
#pragma omp threadprivate(seed)
	seed = seed * 1103515245 + 12345;
return seed ;
}

void    Population::localSearch(int pos)
{
        vector<int> g;
        g.resize(genome_size);
        for(int i=0;i<genome_size;i++) g[i]=genome[pos][i];
        for(int i=0;i<genome_size;i++)
        {
        //      double r=drand48();
        //      if(r<mutation_rate)
                {
                        int ik=0;
                        double f;
                        do
                        {
                                //g[i]=rand() % MAX_RULE;
								g[i]=randUint()% MAX_RULE;
                                ik++;
                                if(ik==10) break;
                                f=fitness(g);
                        }while(f<=fitness_array[pos]);
                        if(ik!=10)
                        {
                                fitness_array[pos]=f;
                                genome[pos][i]=g[i];
                        }
                        else g[i]=genome[pos][i];
                }
        }
}



/* Destructor */
Population::~Population()
{
	for(int i=0;i<genome_count;i++)
	{
		delete[] children[i];
		delete[] genome[i];
	}
	delete[] genome;
	delete[] children;
	delete[] fitness_array;
}
