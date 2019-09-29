#!/bin/bash

make all
bash pinger.sh

for i in 8 16 32
do
    for j in 2 4 8
    do
        for t in {1..10}
        do
            rm ${i}_${j}_time_${t}.txt 2> /dev/null
            touch ${i}_${j}_time_${t}.txt
            echo "Running for np ${i} ppn ${j}"
            mpiexec -f hostfile.txt -np ${i} -ppn ${j} ./src.x | head -n 5 | tee ${i}_${j}_time_${t}.txt
        done
    done
done

make clean

python3 plot.py
