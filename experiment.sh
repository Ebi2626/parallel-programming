#!/bin/bash

if [ -f ./results.txt ]; then 
  rm ./results.txt
fi

for i in {1..10}
do
  ./monte-carlo-seq $1 >> ./results.txt
done

for i in {1..10}
do
  mpirun -np 3 ./monte-carlo $1 >> ./results.txt
done