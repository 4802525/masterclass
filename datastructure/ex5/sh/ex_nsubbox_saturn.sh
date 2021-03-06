#!/bin/sh
n=16384
randseed=1
chunksize=1
schmode=1
result=../result_saturn/nsubbox
g++ ../ex.cpp -o ../ex -fopenmp
rm -rf $result
mkdir -p $result
for nsubbox in `seq -w 10`;do
  for initmode in 0 1 2;do
    for n_thread in 1 2 3 5 7 10 15 20;do
      for randseed in `seq 5`;do
        echo $n $nsubbox $n_thread $initmode $schmode $chunksize $randseed > inputfile
        ../ex < inputfile 
      done | awk '{sum+=$1} END {print sum/NR}'> out 
      echo $n_thread `cat out` >> $result/Strong_ex_init${initmode}_ndiv${nsubbox}.out
    done
  done
done
rm inputfile out
