#!/bin/sh
n=16384
nsubbox=10
randseed=1
chunksize=1
initmode=0
schmode=0
result=../result_edu/comp
g++ ../ex.cpp -o ../ex -fopenmp
rm -rf $result
mkdir -p $result
for n_thread in 1 2 3 4;do
  for randseed in `seq 5`;do
    echo $n $nsubbox $n_thread $initmode $schmode $chunksize $randseed > inputfile
    ../ex < inputfile 
  done | awk '{sum+=$1} END {print sum/NR}'> out 
  echo $n_thread `cat out` >> $result/Strong_ex_comp.out
done
rm inputfile out
