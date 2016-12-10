# GenClass
GenClass: A portable tool for data classification based on Grammatical Evolution
# 1. Introduction
GenClass is a software entirely written in ANSI C++ that constructs classification programs in a C – like programming language in order to classify the input data, producing simple if – else rules. The dataset should conform to the format outlined below: 

D

M

X11 x12 .. X1D y1

x21 x22 ... x2D y2

.... . ... .....

xM1 xM2 ... xMD yM

The integer value D determines the dimensionality of the problem and the value M determines the number of points in the file. Every subsequent line contains a pattern and the final column is the real output (category) for this pattern. The number of the classes is induced from the file. The software scans the file and identifies the number of problem’s classes. The classes should be integer numbers with number 0 assigned to the first class.
# 2. Distribution 

The package is distributed in a zip file from Github https://github.com/itsoulos/GenClass named GenClass-master.zip and under UNIX systems the user must execute the command: unzip GenClass-master. This command creates a directory named GenClass with the following contents:

1. bin: A directory which is initially empty. After compilation of the package, it will contain the executable genclass 

2. doc: This directory contains the documentation of the package (this file) in different formats: A LyX file, A LaTeX file and a PostScript file.

3. examples: A directory that contains some test functions.

4. include: A directory which contains the header files for all the classes of the package.

5. src: A directory containing the source files of the package.

6. Makefile: The input file to the make utility in order to build the tool. Usually, the user does not need to change this file.

7. Makefile.inc: The file that contains some configuration parameters, such as the name of the C++ compiler etc. The user must edit and change this file before installation.

# 3.Installation 

The following steps are required in order to build the tool:

1. Uncompress the tool as described in the previous section.

2. cd GenClass 

3. Edit the file Makefile.inc and change (if needed) the configuration parameters.

4. Type make.

The parameters in Makefile.inc are the following:

1. CXX: It is the most important parameter. It specifies the name of the C++ compiler. In most systems running the GNU C++ compiler this parameter must be set to g++.

2. ROOTDIR: Is the location of the GenClass directory. 
