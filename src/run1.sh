#!/bin/sh
BASEPATH=~/Desktop/ERGASIES/GenClass/
PROGRAM=$BASEPATH/src/genclass
DATAPATH=~/Desktop/ERGASIES/FeatureConstruction2/datasets/tenfolding/
#DATAPATH=~/Desktop/ERGASIES/NEWBETTER/
#DATAPATH=~/Downloads/GenClass/data/
GENOMES=500
GENERATIONS=50
DATAFILE=$1
SEED=$2
$PROGRAM -p $DATAPATH/$DATAFILE.train -c $GENOMES -g $GENERATIONS   -t $DATAPATH/$DATAFILE.test  -o full  -r $SEED
