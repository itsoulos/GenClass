# GenClass
GenClass: A portable tool for data classification based on Grammatical Evolution
# Introduction
GenClass is a software entirely written in ANSI C++ that constructs classification programs in a C – like programming language in order to classify the input data, producing simple if – else rules. The dataset should conform to the format outlined below: 

D

M

X11 x12 .. X1D y1

x21 x22 ... x2D y2

.... . ... .....

xM1 xM2 ... xMD yM

The integer value D determines the dimensionality of the problem and the value M determines the number of points in the file. Every subsequent line contains a pattern and the final column is the real output (category) for this pattern. The number of the classes is induced from the file. The software scans the file and identifies the number of problem’s classes. The classes should be integer numbers with number 0 assigned to the first class.
