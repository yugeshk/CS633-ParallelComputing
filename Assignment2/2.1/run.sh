#!/bin/bash

make all
bash pinger.sh

for i in {1..10}
do
    for j in {0..2}
    do
        rm ${j}_time_$i.txt 2> /dev/null
        touch ${j}_time_$i.txt
        mpiexec -f hostfile.txt -np 30 -ppn 1 ./src.x ${j} | tee ${j}_time_$i.txt
    done
done

make clean

rm -rf plots 2>/dev/null
mkdir plots
python3 plot.py
