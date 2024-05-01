#!/bin/sh
BASEPATH=~/TEST/GenClass/
PROGRAM=$BASEPATH/src/genclass
DATAPATH=~/Desktop/ERGASIES/FeatureConstruction2/datasets/tenfolding/
GENOMES=500
GENERATIONS=500
DATAFILE=$1
OUTFILE=$1.out
INSTANCES=10
for i in $(seq 1 $INSTANCES)
do
$PROGRAM -p $DATAPATH/$DATAFILE.train -c $GENOMES -g $GENERATIONS   -t $DATAPATH/$DATAFILE.test  -o full  -r $i >> $OUTFILE
echo "DATA: $1. Finish $i from $INSTANCES"
done

grep CLASS $1.out  | grep ERROR |awk '{print $4}' | awk '{ sum += $1 } END { if (NR > 0) print sum / NR }'

