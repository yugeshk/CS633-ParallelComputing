#!/bin/bash

#PBS -N kmeans
#PBS -q courses
#PBS -l nodes=8:ppn=4
#PBS -j oe
export I_MPI_FABRICS=shm:tmi
export I_MPI_DEVICE=rdma:OpenIB-cma
cd $PBS_O_WORKDIR
source /opt/software/intel17_update4/initpaths intel64
sort $PBS_NODEFILE > hostfile.txt

make hpc

#Submission script for running on the hpc cluster.

echo "Running for dataset 1"
echo "Output files are saved under hpc/data1"

#Run 1 for dataset1

k=(17 17 18 18 18 19 19 19 20 20 21 22 22 23 23 22 22)
for P in $(seq 4 4 32)
do
    echo "Number of Processes: ${P}" > "output_${P}.txt"
    for f_no in $(seq 0 1 16)
    do
        cp ./data/data1/file${f_no} .
        num_pts=$(($(du -b "file${f_no}" | awk '{print $1}') / 32))
        mpiexec -np $P -ppn 4 -f hostfile.txt ./src.x $num_pts "file$f_no" ${k[f_no]} $((f_no+1)) "output_${P}.txt"
        rm "file${f_no}"
    done
    appt=$(awk '{ total += $1; count++ } END { print total/count }' pre_processing_time.txt)
    apt=$(awk '{ total += $1; count++ } END { print total/count }' processing_time.txt)
    rm pre_processing_time.txt && rm processing_time.txt
    echo "Average time to pre-process: $appt" >> "output_${P}.txt"
    echo "Average time to process: $apt" >> "output_${P}.txt"
    tt=$(echo $(echo "${appt}" | bc)+$(echo "${apt}" | bc) | bc)
    echo "Total time: ${tt}" >> "output_${P}.txt"
    echo "${P},Pre Processing Time,${appt}" >> "pre_processing_time_data.txt"
    echo "${P},Processing Time,${apt}" >> "processing_time_data.txt"
    echo "${P},Total Time,${tt}" >> "total_time_data.txt"
done

mv output_* ./hpc/data1/

## Additional runs for dataset 1
for tm in $(seq 2 1 5)
do
    for P in $(seq 4 4 32)
    do
        for f_no in $(seq 0 1 16)
        do
            cp data/data1/file${f_no} .
            num_pts=$(($(du -b "file${f_no}" | awk '{print $1}') / 32))
            mpiexec -np $P -ppn 4 -f hostfile.txt ./src.x $num_pts "file$f_no" ${k[f_no]} $((f_no+1)) "output_${P}_${tm}.txt"
            rm "output_${P}_${tm}.txt"
            rm "file${f_no}"
        done
        appt=$(awk '{ total += $1; count++ } END { print total/count }' pre_processing_time.txt)
        apt=$(awk '{ total += $1; count++ } END { print total/count }' processing_time.txt)
        rm pre_processing_time.txt && rm processing_time.txt
        tt=$(echo $(echo "${appt}" | bc)+$(echo "${apt}" | bc) | bc)
        echo "${P},Pre Processing Time,${appt}" >> "pre_processing_time_data.txt"
        echo "${P},Processing Time,${apt}" >> "processing_time_data.txt"
        echo "${P},Total Time,${tt}" >> "total_time_data.txt"
    done
done

cat "pre_processing_time_data.txt" >> "data1_time.txt"
cat "processing_time_data.txt" >> "data1_time.txt"
cat "total_time_data.txt" >> "data1_time.txt"

mv pre_processing_time_data.txt ./hpc/data1/pre_processing_time_data.txt
mv processing_time_data.txt ./hpc/data1/processing_time_data.txt
mv total_time_data.txt ./hpc/data1/total_time_data.txt
mv data1_time.txt ./hpc/data1/data1_time.txt

#Runs for dataset2

echo "Running for dataset 2"
echo "Output files are saved under hpc/data2"

#Run 1 for dataset2

k=(6 6 6 7 4 3 6 6 7 8 8 8 8 7 7 7)
for P in $(seq 4 4 32)
do
    echo "Number of Processes: ${P}" > "output_${P}.txt"
    for f_no in $(seq 0 1 15)
    do
        cp ./data/data2/file${f_no} .
        num_pts=$(($(du -b "file${f_no}" | awk '{print $1}') / 32))
        mpiexec -np $P -ppn 4 -f hostfile.txt ./src.x $num_pts "file$f_no" ${k[f_no]} $((f_no+1)) "output_${P}.txt"
        rm "file${f_no}"
    done
    appt=$(awk '{ total += $1; count++ } END { print total/count }' pre_processing_time.txt)
    apt=$(awk '{ total += $1; count++ } END { print total/count }' processing_time.txt)
    rm pre_processing_time.txt && rm processing_time.txt
    echo "Average time to pre-process: $appt" >> "output_${P}.txt"
    echo "Average time to process: $apt" >> "output_${P}.txt"
    tt=$(echo $(echo "${appt}" | bc)+$(echo "${apt}" | bc) | bc)
    echo "Total time: ${tt}" >> "output_${P}.txt"
    echo "${P},Pre Processing Time,${appt}" >> "pre_processing_time_data.txt"
    echo "${P},Processing Time,${apt}" >> "processing_time_data.txt"
    echo "${P},Total Time,${tt}" >> "total_time_data.txt"
done

mv output_* ./hpc/data2/

## Additional runs for dataset 2
for tm in $(seq 2 1 5)
do
    for P in $(seq 4 4 32)
    do
        for f_no in $(seq 0 1 15)
        do
            cp data/data2/file${f_no} .
            num_pts=$(($(du -b "file${f_no}" | awk '{print $1}') / 32))
            mpiexec -np $P -ppn 4 -f hostfile.txt ./src.x $num_pts "file$f_no" ${k[f_no]} $((f_no+1)) "output_${P}_${tm}.txt"
            rm "output_${P}_${tm}.txt"
            rm "file${f_no}"
        done
        appt=$(awk '{ total += $1; count++ } END { print total/count }' pre_processing_time.txt)
        apt=$(awk '{ total += $1; count++ } END { print total/count }' processing_time.txt)
        rm pre_processing_time.txt && rm processing_time.txt
        tt=$(echo $(echo "${appt}" | bc)+$(echo "${apt}" | bc) | bc)
        echo "${P},Pre Processing Time,${appt}" >> "pre_processing_time_data.txt"
        echo "${P},Processing Time,${apt}" >> "processing_time_data.txt"
        echo "${P},Total Time,${tt}" >> "total_time_data.txt"
    done
done

cat "pre_processing_time_data.txt" >> "data2_time.txt"
cat "processing_time_data.txt" >> "data2_time.txt"
cat "total_time_data.txt" >> "data2_time.txt"

mv pre_processing_time_data.txt ./hpc/data2/pre_processing_time_data.txt
mv processing_time_data.txt ./hpc/data2/processing_time_data.txt
mv total_time_data.txt ./hpc/data2/total_time_data.txt
mv data2_time.txt ./hpc/data2/data2_time.txt

echo "Please run plot.py for generating plots"
echo "***************************************"

make clean
