# ifndef __GETOPTIONS__H
# define __GETOPTIONS__H
extern int	pcount;
extern int	length;
extern double	srate;
extern double	mrate;
extern char	train_file[1024];
extern char	test_file[1024];
extern char	output_method[100];
extern int	generations;
extern int	random_seed;
extern	int	wrapping;
extern void	parse_cmd_line(int argc,char **argv);
extern	int	foldcount;

/** Output methods **/
# define CSV_METHOD	"csv"
# define SIMPLE_METHOD	"simple"
# define FULL_METHOD	"full"
# endif
