#!/bin/bash

make all
bash pinger.sh

k=(17 17 18 18 18 19 19 19 20 20 21 22 22 23 23 22 22)
for P in $(seq 4 4 32)
do
    for f_no in $(seq 0 1 16)
    do
        num_pts=$(($(du -b "file${f_no}" | awk '{print $1}') / 32))
        mpiexec -np $P -ppn 4 -f hostfile.txt ./src.x $num_pts "file$f_no" ${k[f_no]} $((f_no+1)) "output_${P}.txt"
    done
    appt=$(awk '{ total += $1; count++ } END { print total/count }' pre_processing_time.txt)
    apt=$(awk '{ total += $1; count++ } END { print total/count }' processing_time.txt)
    rm pre_processing_time.txt && rm processing_time.txt
    echo "Average time to pre-process: $appt" >> "output_${P}.txt"
    echo "Average time to process: $apt" >> "output_${P}.txt"
    echo "Total time: $(awk '{print $1+$2' <<< '($appt + $apt)')" >> "output_${P}.txt"
done

make clean
