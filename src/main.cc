# include <population.h>
# include <get_options.h>
# include <classprogram.h>
# include <cmath>
#include <omp.h>


extern int maxthreads;
int ok_to_print=0;
int pattern_dimension;
extern vector<double> lmargin,rmargin;
ClassProgram **p;
Population *pop;
vector<int> genome;
string s;

void	printConfusionMatrix()
{
	int i,j;
	vector<double> T;
	vector<double> O;
	p[omp_get_thread_num()]->getOutputs(T,O);
	int N=T.size();
	int nclass=p[omp_get_thread_num()]->getClass();
	int **CM;
	printf("** CONFUSION MATRIX ** Number of classes: %d\n",nclass);
	CM=new int*[nclass];
	for(i=0;i<nclass;i++) CM[i]=new int[nclass];
	for(i=0;i<nclass;i++)
  		for(j=0;j<nclass;j++) CM[i][j] = 0;

	for(i=0;i<N;i++) CM[(int)T[i]][(int)O[i]]++;

	for(i=0;i<nclass;i++)
	{
		for(j=0;j<nclass;j++)
		{
			printf("%4d ",CM[i][j]);
		}
		printf("\n");
		delete[] CM[i];
	}
	delete[] CM;
}

void	initProgram(int argc,char **argv)
{
	parse_cmd_line(argc,argv);
	srand(random_seed);
	p = new ClassProgram*[maxthreads];
	for(unsigned i = 0; i < maxthreads;i++){
		p[i]=new ClassProgram(train_file);
	}
	pattern_dimension = p[omp_get_thread_num()]->getClass()-1;
	pop=new Population(pcount,length * pattern_dimension,p);
	pop->setSelectionRate(srate);
	pop->setMutationRate(mrate);
}

void	doneProgram()
{
	delete p;
	delete pop;
}

void	runGenetic()
{
	for(int i=1;i<generations;i++)
	{
		pop->nextGeneration();
		genome=pop->getBestGenome();
		s=p[omp_get_thread_num()]->printF(genome);
		if(!strcmp(output_method,FULL_METHOD))
			ok_to_print=1;
		pop->evaluateBestFitness();
		double bestf=pop->getBestFitness();
		ok_to_print=0;
		if(fabs(bestf)<1e-7) break;
		if(strcmp(output_method,CSV_METHOD)==0)
		{
			if(strlen(test_file)==0)
			printf("%5d,%10.4lg\n",
				i,-pop->getBestFitness());
			else
			{
				printf("%5d,%10.4lg,%10.4lg\n",
					i,-pop->getBestFitness(),-p[omp_get_thread_num()]->getClassError(genome,test_file));
			}
		}
		if(!strcmp(output_method,FULL_METHOD))
		{
			if(strlen(test_file)==0)
				printf("GENERATION=%5d FITNESS=%10.4lg%%\nPROGRAMS=\n%s",
					i,-pop->getBestFitness(),s.c_str());
			else
				printf("GENERATION=%5d FITNESS=%10.4lg%% TEST_ERROR=%10.4lg%%\nPROGRAMS=\n%s",
					i,-pop->getBestFitness(),-p[omp_get_thread_num()]->getClassError(genome,test_file),s.c_str());
		}
	}
}


void	foldValidation()
{
	int i,j;
	srand(random_seed);
	vector<Data> trainx,testx;
	vector<Data> totalx;
	Data  totaly;
	Data trainy,testy;
	vector<int> Index;
	p[omp_get_thread_num()]->getTrainData(totalx,totaly);
	Index.resize(totalx.size());
	for(i=0;i<Index.size();i++) Index[i]=i;
	for(i=0;i<Index.size();i++)
	{
		int randpos=rand() % Index.size();
		int t=Index[i];
		Index[i]=Index[randpos];
		Index[randpos]=t;
	}
	int testsize=Index.size() / foldcount;
	int trainsize=Index.size() - testsize;
	trainx.resize(trainsize);
	trainy.resize(trainsize);
	testx.resize(testsize);
	testy.resize(testsize);
	for(i=0;i<trainsize;i++) trainx[i].resize(totalx[0].size());
	for(i=0;i<testsize;i++) testx[i].resize(totalx[0].size());
	double avg_train_error=0.0;
	double avg_test_error=0.0;
	int nclass=p[omp_get_thread_num()]->getClass();
	vector<double> T;
	vector<double> O;
	int **CM;
	CM=new int*[nclass];
	for(i=0;i<nclass;i++) CM[i]=new int[nclass];
	for(i=0;i<nclass;i++)
  		for(j=0;j<nclass;j++) CM[i][j] = 0;
	for(i=0;i<foldcount;i++)
	{
		pop->reset();
		printf("FOLD COUNT: %d \n",i);
		int icount=0;
		for(j=i*testsize;j<(i+1)*testsize;j++)
		{
			testx[icount]=totalx[j];
			testy[icount]=totaly[j];
			icount++;
		}
		icount=0;
		for(j=0;j<totalx.size();j++)
		{
			if(icount>=trainsize) break;
			if(j>=i*testsize && j<(i+1)*testsize) continue;
			trainx[icount]=totalx[j];
			trainy[icount]=totaly[j];
			icount++;
		}
		p[omp_get_thread_num()]->setTrainData(trainx,trainy);
		runGenetic();
		p[omp_get_thread_num()]->fitness(genome);
		s=p[omp_get_thread_num()]->printF(genome);
		avg_train_error+=fabs(pop->getBestFitness());
		avg_test_error+=fabs(p[omp_get_thread_num()]->getClassError(genome,testx,testy));
		p[omp_get_thread_num()]->getOutputs(T,O);
		for(j=0;j<T.size();j++) CM[(int)T[j]][(int)O[j]]+=1;
		printf("EXPRESSION=\n%s",s.c_str());
	}
	printf("AVERAGE OUTPUT\n");
	printf("AVERAGE TRAIN ERROR=%.2lf%%\n",avg_train_error/foldcount);
	printf("AVERAGE TEST  ERROR=%.2lf%%\n",avg_test_error/foldcount);
	printf("** CONFUSION MATRIX ** Number of classes: %d\n",nclass);
	for(i=0;i<nclass;i++)
	{
		for(j=0;j<nclass;j++)
		{
			printf("%4d ",CM[i][j]);
		}
		printf("\n");
		delete[] CM[i];
	}
	delete[] CM;
}

void	runOneFold()
{
	runGenetic();
	p[omp_get_thread_num()]->fitness(genome);
	s=p[omp_get_thread_num()]->printF(genome);
	printf("FINAL OUTPUT\n");
	printf("EXPRESSION=\n%s",s.c_str());
	printf("TRAIN ERROR = %.2lf%%\n",fabs(pop->getBestFitness()));
	if(strlen(test_file)!=0)
	{
		printf("CLASS ERROR = %.2lf%%\n",-p[omp_get_thread_num()]->getClassError(genome,test_file));
		printConfusionMatrix();
	}
	p[omp_get_thread_num()]->printC(genome);
	p[omp_get_thread_num()]->printPython(genome);
}

int main(int argc,char **argv)
{
	initProgram(argc,argv);
	genome.resize(pattern_dimension * length);
	if(foldcount)
		foldValidation();
	else
	{
		runOneFold();
	}
	doneProgram();
	return 0;
}
