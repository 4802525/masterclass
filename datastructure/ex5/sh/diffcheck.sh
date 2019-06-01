#!/bin/sh
n=16384
randseed=1
initmode=0
n_thread=1
nsubbox=10
chunksize=1
schmode=1
g++ ../ex.cpp -o ../ex -fopenmp

echo check thread
n_thread=1
echo $n $nsubbox $n_thread $initmode $schmode $chunksize $randseed > inputfile
../ex < inputfile 2> err1.out
n_thread=4
echo $n $nsubbox $n_thread $initmode $schmode $chunksize $randseed > inputfile
../ex < inputfile 2> err2.out
diff err1.out err2.out
n_thread=20
echo done thread

echo check nsubbox
nsubbox=1
echo $n $nsubbox $n_thread $initmode $schmode $chunksize $randseed > inputfile
../ex < inputfile 2> err1.out
nsubbox=10
echo $n $nsubbox $n_thread $initmode $schmode $chunksize $randseed > inputfile
../ex < inputfile 2> err2.out
diff err1.out err2.out
echo done nsubbox

echo check chunk 
chunksize=1
echo $n $nsubbox $n_thread $initmode $schmode $chunksize $randseed > inputfile
../ex < inputfile 2> err1.out
chunksize=10
echo $n $nsubbox $n_thread $initmode $schmode $chunksize $randseed > inputfile
../ex < inputfile 2> err2.out
diff err1.out err2.out
chunksize=1
echo done chunk

echo check schmode
schmode=0
echo $n $nsubbox $n_thread $initmode $schmode $chunksize $randseed > inputfile
../ex < inputfile 2> err1.out
schmode=1
echo $n $nsubbox $n_thread $initmode $schmode $chunksize $randseed > inputfile
../ex < inputfile 2> err2.out
diff err1.out err2.out
echo done schmode

rm inputfile err1.out err2.out
