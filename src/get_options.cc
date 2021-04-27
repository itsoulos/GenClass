# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <get_options.h>

int	pcount;
int	length;
int 	maxthreads=16;
double	srate;
double	mrate;
char	train_file[1024];
char	test_file[1024];
char	output_method[100];
int	generations;
int	random_seed;
int	wrapping;
int	foldcount;
void	parse_cmd_line(int argc,char **argv);

const char *short_options="c:l:s:m:p:t:g:w:r:f:o:d:";

void	print_usage()
{
	printf("\t-h	Print this help screen.\n"
			"\t-c	Specify population count.\n"
			"\t-f	Specify fold count for fold validation. Default value 0 (no folding).\n"
			"\t-g	Specify number of generations.\n"
			"\t-l	Specify	population length.\n"
			"\t-d	Specify maximum number of threads.\n"
			"\t-m	Specify mutation rate.\n"
			"\t-o	Specify output method.\n"
			"\t-p	Specify train file.\n"
			"\t-r	Specify random seed.\n"
			"\t-s	Specify selection rate.\n"
			"\t-t	Specify test file.\n"
			"\t-w	Specify maximum number of wrapping events.\n");
}
void	parse_cmd_line(int argc,char **argv)
{
	if(argc==1)
	{
		print_usage();
		exit(1);
	}
	int next_option;
	/*	DEFAULT VALUES
	 * */
	pcount=500;
	length=100;
	srate=0.1;mrate=0.05;
	strcpy(train_file,"");
	strcpy(test_file,"");
	strcpy(output_method,CSV_METHOD);
	generations=200;
	wrapping=1;
	random_seed=1;
	foldcount=0;
	do
	{
		next_option=getopt(argc,argv,short_options);
		switch(next_option)
		{
			case 'c':
				pcount=atoi(optarg);
				break;
			case 'd':
				maxthreads=atoi(optarg);
				break;
			case 'l':
				length=atoi(optarg);
				break;
			case 's':
				srate=atof(optarg);
				break;
			case 'm':
				mrate=atof(optarg);
				break;
			case 'g':
				generations=atoi(optarg);
				break;
			case 'p':
				strcpy(train_file,optarg);
				break;
			case 't':
				strcpy(test_file,optarg);
				break;
			case 'r':
				random_seed = atoi(optarg);
				break;
			case 'o':
				strcpy(output_method,optarg);
				break;
			case 'w':
				wrapping=atoi(optarg);
				break;
			case 'f':
				foldcount=atoi(optarg);
				break;
			case -1:
				break;
			case '?':
				print_usage();
				exit(1);
				break;
			default:
				print_usage();
				exit(1);
				break;
		}
	}while(next_option!=-1);
}
