# include <classprogram.h>
# include <math.h>
# define NAN_CLASS	1e+10

static double dmax(double a,double b) {return a>b?a:b;}

int	problem_dimension;

ClassProgram::ClassProgram(char *filename)
{
	FILE *fp=fopen(filename,"r");
	if(!fp) return;
	int valid_count;
	int d,c;
	fscanf(fp,"%d",&d);
	dimension = d;
	fscanf(fp,"%d",&c);

	trainy.resize(c);
	trainx.resize(c);
	vclass.resize(0);
	problem_dimension = d;
	for(int i=0;i<c;i++) trainx[i]=new double[d];
	for(int i=0;i<c;i++)
	{
		for(int j=0;j<d;j++)
		{
			fscanf(fp,"%lf",&trainx[i][j]);
		}
		fscanf(fp,"%lf",&trainy[i]);
		int flag=0;
		for(int j=0;j<vclass.size();j++)
		{
			if(fabs(vclass[j]-trainy[i])<1e-5)
			{
				flag=1;
				break;
			}
		}
		if(!flag)
		{
			int s=vclass.size();
			vclass.resize(s+1);
			vclass[s]=trainy[i];
		}
	}
	fclose(fp);
	//sort vclass
	for(int i=0;i<vclass.size();i++)
	{
		for(int j=0;j<vclass.size()-1;j++)
		{
			if(vclass[j+1]<vclass[j])
			//if(vclass[j+1]>vclass[j])
			{
				double d=vclass[j];
				vclass[j]=vclass[j+1];
				vclass[j+1]=d;
			}
		}
	}

	mapper.resize(vclass.size());
	for(int i=0;i<vclass.size();i++)
	{
		mapper[i]=vclass[i];
		vclass[i]=i;
	}

	program = new Cprogram(d,vclass.size()-1);
	setStartSymbol(program->getStartSymbol());
	pstring.resize(vclass.size());
	for(int i=0;i<pstring.size();i++) pstring[i]=" ";
	pgenome.resize(0);
	nclass = vclass.size();
	outy.resize(trainy.size());
}

string	ClassProgram::printF(vector<int> &genome)
{
	string ret="";
	if(pgenome.size()!=genome.size()/(nclass-1))
		pgenome.resize(genome.size()/(nclass-1));
	char str[100];
	extern int wrapping;
	for(int i=0;i<nclass-1;i++)
	{
		for(int j=0;j<genome.size()/(nclass-1);j++)
			pgenome[j]=genome[i*genome.size()/(nclass-1)+j];
		int redo=0;
		pstring[i]=printRandomProgram(pgenome,redo);
		if(redo>=wrapping) return "";
		ret+="if(";
		ret+=pstring[i];
		ret+=") CLASS=";
		sprintf(str,"%.2lf",vclass[i]);
		ret+=str;
		ret+="\nelse \n";
	}
	sprintf(str,"%.2lf",vclass[nclass-1]);
	ret+="CLASS=";
	ret+=str;
	ret+="\n";
	return ret;
}

int	ClassProgram::findMapper(double y)
{
	for(int i=0;i<vclass.size();i++)
		if(fabs(mapper[i]-y)<1e-7) return i;
	return 0;
}

double	ClassProgram::getClassError(vector<int> &genome,vector<Data> &tx,Data &ty)
{
	if(testx.size()!=tx.size()) 
	 for(int i=0;i<testx.size();i++)
		delete[] testx[i];
	testy.resize(tx.size());
	testx.resize(tx.size());
	extern int wrapping;
	int d=tx[0].size();
	int c=tx.size();
	for(int i=0;i<c;i++) testx[i]=new double[d];
	for(int i=0;i<c;i++)
	{
		for(int j=0;j<d;j++)
			testx[i][j]=tx[i][j];
		testy[i]=ty[i];
	}
	double value=0.0;
	if(pgenome.size()!=genome.size()/(nclass-1))
		pgenome.resize(genome.size()/(nclass-1));
	if(outy.size()!=testy.size()) outy.resize(testy.size());
	for(int i=0;i<outy.size();i++) outy[i]=NAN_CLASS;
	for(int i=0;i<nclass-1;i++)
	{
		for(int j=0;j<pgenome.size();j++)
			pgenome[j]=genome[i*genome.size()/(nclass-1)+j];
		int redo=0;
		string s = printRandomProgram(pgenome,redo);
		if(redo>=wrapping) return -1e+8;
		pstring[i]=s;
	}
	for(int j=0;j<nclass-1;j++)
	{
		program->Parse(pstring[j]);
		double dclass;
		for(int i=0;i<testy.size();i++)
		{
			if(fabs(outy[i]-NAN_CLASS)>1e-5) continue;
			double v=program->Eval(testx[i]);
			if(isnan(v) || isinf(v)) return -1e+8;
			if(fabs(v-1.0)<1e-5) outy[i]=vclass[j];
		}
	}
	
	for(int i=0;i<testy.size();i++)
	{
		if(fabs(outy[i]-NAN_CLASS)<1e-5) outy[i]=vclass[nclass-1];
		value=value+(fabs(findMapper(testy[i])-outy[i])>1e-5);
	}
	
	if(isnan(value) || isinf(value)) return -1e+8;
	return -value*100.0/testy.size();
}

double	ClassProgram::getClassError(vector<int> &genome,char *filename)
{
	FILE *fp=fopen(filename,"r");
	if(!fp) return -1.0;
	if(testx.size()) 
	 for(int i=0;i<testx.size();i++)
		delete[] testx[i];
	int d,c;
	fscanf(fp,"%d",&d);
	fscanf(fp,"%d",&c);
	testy.resize(c);
	testx.resize(c);
	extern int wrapping;
	for(int i=0;i<c;i++) testx[i]=new double[d];
	for(int i=0;i<c;i++)
	{
		for(int j=0;j<d;j++)
			fscanf(fp,"%lf",&testx[i][j]);
		fscanf(fp,"%lf",&testy[i]);
	}
	fclose(fp);
	double value=0.0;
	if(pgenome.size()!=genome.size()/(nclass-1))
		pgenome.resize(genome.size()/(nclass-1));
	if(outy.size()!=testy.size()) outy.resize(testy.size());
	for(int i=0;i<outy.size();i++) outy[i]=NAN_CLASS;
	for(int i=0;i<nclass-1;i++)
	{
		for(int j=0;j<pgenome.size();j++)
			pgenome[j]=genome[i*genome.size()/(nclass-1)+j];
		int redo=0;
		string s = printRandomProgram(pgenome,redo);
		if(redo>=wrapping) return -1e+8;
		pstring[i]=s;
	}
	for(int j=0;j<nclass-1;j++)
	{
		program->Parse(pstring[j]);
		double dclass;
		for(int i=0;i<testy.size();i++)
		{
			if(fabs(outy[i]-NAN_CLASS)>1e-5) continue;
			double v=program->Eval(testx[i]);
			if(isnan(v) || isinf(v)) return -1e+8;
			if(fabs(v-1.0)<1e-5) outy[i]=vclass[j];
		}
	}
	
	for(int i=0;i<testy.size();i++)
	{
		if(fabs(outy[i]-NAN_CLASS)<1e-5) outy[i]=vclass[nclass-1];
		value=value+(fabs(findMapper(testy[i])-outy[i])>1e-5);
	}
	
	if(isnan(value) || isinf(value)) return -1e+8;
	return -value*100.0/testy.size();
}

int	ClassProgram::getClass() const
{
	return nclass;
}

double 	ClassProgram::fitness(vector<int> &genome)
{
	double value=0.0;
	if(pgenome.size()!=genome.size()/(nclass-1))
		pgenome.resize(genome.size()/(nclass-1));
	if(outy.size()!=trainy.size()) outy.resize(trainy.size());
	for(int i=0;i<outy.size();i++) outy[i]=NAN_CLASS;
	extern int wrapping;
	for(int i=0;i<nclass-1;i++)
	{
		for(int j=0;j<pgenome.size();j++)
			pgenome[j]=genome[i*genome.size()/(nclass-1)+j];
		int redo=0;
		string s = printRandomProgram(pgenome,redo);
		if(redo>=wrapping) return -1e+8;
		pstring[i]=s;
	}
	
	for(int j=0;j<nclass-1;j++)
	{
		program->Parse(pstring[j]);
		double dclass;
		for(int i=0;i<trainy.size();i++)
		{
			if(fabs(outy[i]-NAN_CLASS)>1e-5) continue;
			double v=program->Eval(trainx[i]);
			if(isnan(v) || isinf(v) ) return -1e+8;
			if(fabs(v-1.0)<1e-5) outy[i]=vclass[j];
		}
	}
	
	vector<int> fail;
	vector<int> belong;
	fail.resize(nclass);
	belong.resize(nclass);
	for(int i=0;i<nclass;i++)
		fail[i]=belong[i]=0;
	for(int i=0;i<trainy.size();i++)
	{
		if(fabs(outy[i]-NAN_CLASS)<1e-5) 	outy[i]=vclass[nclass-1];
		int pos=findMapper(trainy[i]);
		value=value+((fabs(findMapper(trainy[i])-outy[i]))>1e-5);	
		belong[pos]++;
		if(fabs(findMapper(trainy[i])-outy[i])>1e-5)
		{
			fail[pos]++;
		}
	}
	extern int ok_to_print;
	if(ok_to_print)
	for(int i=0;i<nclass;i++)
	{
		printf("CLASS[%3d (%3d)] FAIL=%5.2lf%% \n",i,belong[i],fail[i]*100.0/belong[i]);
	}

	if(isnan(value) || isinf(value)) return -1e+8;
	return -value*100.0/trainy.size();
}


void	ClassProgram::getTrainData(vector<Data> &tx,Data &ty)
{
	tx.resize(trainx.size());
	ty.resize(trainy.size());	
	for(int i=0;i<trainx.size();i++)
	{
		tx[i].resize(dimension);
		for(int j=0;j<dimension;j++) tx[i][j]=trainx[i][j];
		ty[i]=trainy[i];
	}
}

void	ClassProgram::getOutputs(vector<double> &real,vector<double> &est)
{
	real.resize(testy.size());
	est.resize(testy.size());
	for(int i=0;i<testy.size();i++)
	{
		real[i]=findMapper(testy[i]);
		est[i]=outy[i];
	}
}

void	ClassProgram::setTrainData(vector<Data> &tx,Data &ty)
{
	if(trainx.size() != tx.size())
	{
		for(int i=0;i<trainy.size();i++) delete[] trainx[i];
		trainx.resize(tx.size());
		trainy.resize(ty.size());
		for(int i=0;i<tx.size();i++)
		{
			trainx[i]=new double[tx[0].size()];
			for(int j=0;j<tx[0].size();j++) trainx[i][j]=tx[i][j];
			trainy[i]=ty[i];
		}
	}
}

ClassProgram::~ClassProgram()
{
	for(int i=0;i<trainy.size();i++) delete[] trainx[i];
	for(int i=0;i<testy.size();i++)  delete[] testx[i];
	delete program;
}

