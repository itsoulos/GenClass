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

    - CXX: It is the most important parameter. It specifies the name of the C++ compiler. In most systems running the GNU C++ compiler this parameter must be set to g++.

    - CFLAGS: Optimization parameters for the c++ compiler.
    - ROOTDIR: Is the location of the GenClass directory. 

## Contents ##
The software has the following subdirectories

    - bin: After compilation of the package, it will contain the executable genclass
    - doc: This directory contains the documentation of the package
    - examples: A directory that contains some test functions.
    - include: A directory which contains the header files for all the classes of the package.
    - src: A directory containing the source files of the package.
    - Makefile: The input file to the make utility in order to build the tool. Usually, the user does not need to change this file.
    - Makefile.inc: The file that contains some configuration parameters, such as the name of the C++ compiler etc. The user must edit and change this file before installation.
