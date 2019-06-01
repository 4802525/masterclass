#!/bin/sh
n=16384
randseed=1
chunksize=1
initmode=0
schmode=1
n_thread=20
nsubbox=10
result=../result_saturn/natom
g++ ../ex.cpp -o ../ex -fopenmp
rm -rf $result
mkdir -p $result
for initmode in 0 1 2;do
  for nsubbox in 02 04 06 08 10;do
    for n in 10 20 50 100 200 500 1000 2000 5000 10000 20000 50000;do
        echo $n $nsubbox $n_thread $initmode $schmode $chunksize $randseed > inputfile
        for i in `seq 5`;do ../ex < inputfile ;done | awk '{sum+=$1} END {print sum/NR}'> out
        echo $n `cat out` >> $result/Strong_ex_init${initmode}_nsubbox${nsubbox}_natom.out
    done
  done
done
rm inputfile out
