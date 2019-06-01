#!/bin/sh
n=16384
nsubbox=10
randseed=1
chunksize=1
result=../result_edu/chunk
g++ ../ex.cpp -o ../ex -fopenmp
rm -rf $result
mkdir -p $result
for chunksize in 1 10 100 200 500 1000;do
  for initmode in 0 1;do
    for schmode in 0 1;do
      for n_thread in `seq -w 4`;do
        echo $n $nsubbox $n_thread $initmode $schmode $chunksize $randseed > inputfile
        echo $n_thread `../ex < inputfile` >> $result/Strong_ex_init${initmode}_sch${schmode}_chunk${chunksize}.out
      done
    done
  done
done
rm inputfile
