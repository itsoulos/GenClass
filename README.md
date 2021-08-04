# GenClass - Producing classification rules using Grammatical Evolution #
GenClass is a software entirely written in ANSI C++ that constructs classification programs in a C – like programming language in order to classify the input data, producing simple if – else rules. The software can be executed in parallel exploiting the modern Multi Core environments, using the OpenMP library.   GenClass is a Free Software covered by the [GPLv3+](https://www.gnu.org/licenses/gpl-3.0.en.html) license.

## Building instructions ##
### Requirements ###

 - A C++ compiler (GNU GCC is recommended).
 - The GNU make utility
 - The OpenMP programming library 

### Installation ##
The following steps are required in order to build the tool:

    - Uncompress the tool 
    - cd GenClass
    - Edit the file Makefile.inc and change (if needed) the configuration parameters.
    - Type make.

The parameters in Makefile.inc are the following:

    - CXX: It is the most important parameter. It specifies the name of the C++ compiler. 
           In most systems running the GNU C++ compiler this parameter must be set to g++.

    - CFLAGS: Optimization parameters for the c++ compiler.
    - ROOTDIR: Is the location of the GenClass directory. 

After the compilation the executable genclass will be placed under bin subdirectory.
## Contents ##
The software has the following subdirectories

    - bin: After compilation of the package, it will contain the executable genclass
    - doc: This directory contains the documentation of the package
    - examples: A directory that contains some test functions.
    - include: A directory which contains the header files for all the classes of the package.
    - src: A directory containing the source files of the package.
    - Makefile: The input file to the make utility in order to build the tool. 
                Usually, the user does not need to change this file.
    - Makefile.inc: The file that contains some configuration parameters, such as the name of the C++ compiler etc. 
            The user must edit and change this file before installation.

## Data format ##
The dataset should conform to the format outlined below:

D

M

X11 x12 .. X1D y1

x21 x22 ... x2D y2

.... . ... .....

xM1 xM2 ... xMD yM

The integer value D determines the dimensionality of the problem and the value M determines the number of points in the file. Every subsequent line contains a pattern and the final column is the real output (category) for this pattern. The number of the classes is induced from the file. The software scans the file and identifies the number of problem’s classes. The classes should be integer numbers with number 0 assigned to the first class. As an example, consider the ionosphere.train file under examples subdirectory of the distribution.


## The executable genclass ## 
The executable has the following command line parameters:

1. -h:The program prints a help screen.

2. -c count: The parameter count determines the number of chromosomes with default value 500.

3. -f count. Specify fold count for fold validation. Default value 0 (no folding). 

4. -g gens: The parameter gens determines the maximum number of generations with default value is 200.

5. -d count. The parameter d determines the maximum number of threads used by the OpenMp library. The default value is 16.

6. -s srate: The parameter srate specifies the selection rate with default value 0.10 (10%).

7. -m mrate: The parameter mrate specifies the mutation rate with default value 0.05 (5%).

8. -l size: The parameter size determines the size of every chromosome with default value 100.

9. -p train_file: The string parameter train_file specifies the file containing the points that will be used as train data for the algorithm.  

10. -t test_file: The string parameter test_file specifies the file containing the test data for the particular problem. 

11. -o method: The string parameter method specifies the output method for the executable. The available options are

	(a) simple. The program prints output only on termination. 

	(b) csv. In every generation the program prints: number of generations, train error and test error. 

	(c) full. The program prints in every generation detailed information about the optimization procedure.

12. -r seed: The integer parameter seed specifies the seed for the random number generator. It can assume any integer value.
## A typical run ##
Consider the Ionosphere dataset available from the Machine Learning Repository in the following URL: http://www.ics.uci.edu/~mlearn/MLRepository.html. The ionosphere dataset contains data from the Johns Hopkins Ionosphere database. The two-class dataset contains 351 examples of 34 features each. The datasets has been divided into two files, ionosphere.train and ionosphere.test under directory examples. A typical run for the GenClass will be

~~~~~~~~
../bin/genclass -p ionosphere.train -t ionosphere.test -g 10 -o csv
~~~~~~~~

The output of this command is:

1, 15.43, 19.32

2, 15.43, 19.32

3, 15.43, 19.32

4, 13.71, 17.05

5, 12.57, 15.34

6, 12.57, 15.34

7, 12.57, 15.34

8, 12, 13.64

9, 12, 13.64

FINAL OUTPUT EXPRESSION= if(!(x7<log(cos(cos(((-788.787)+

((sin(x28)/sin(cos(((-7.17)/x34))))+(-83.6))))))|x6>x13&x7<log(x5))) CLASS=0.00

else CLASS=1.00

TRAIN ERROR = 12.00%

CLASS ERROR = 13.64%

** CONFUSION MATRIX ** Number of classes: 2

102 3

21 50

