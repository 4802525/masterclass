#!/bin/sh
n=16384
randseed=1
chunksize=1
initmode=0
schmode=1
n_thread=20
nsubbox=10
result=../result_saturn/randseed
g++ ../ex.cpp -o ../ex -fopenmp
rm -rf $result
mkdir -p $result
for initmode in 0 1 2;do
  for n_thread in 5 10 20;do
    for randseed in `seq -w 100`;do
        echo $n $nsubbox $n_thread $initmode $schmode $chunksize $randseed > inputfile
        echo $randseed `../ex < inputfile` >> $result/Strong_ex_init${initmode}_thread${n_thread}_seed.out
    done
  done
done
rm inputfile
