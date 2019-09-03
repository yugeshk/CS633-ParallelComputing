#!/bin/bash

make all

touch 4_blocking_1.txt
touch 4_unblocking_1.txt
printf "" > 4_blocking_1.txt
printf "" > 4_unblocking_1.txt
touch 4_blocking_2.txt
touch 4_unblocking_2.txt
printf "" > 4_blocking_2.txt
printf "" > 4_unblocking_2.txt
touch 4_blocking_3.txt
touch 4_unblocking_3.txt
printf "" > 4_blocking_3.txt
printf "" > 4_unblocking_3.txt
touch 4_blocking_4.txt
touch 4_unblocking_4.txt
printf "" > 4_blocking_4.txt
printf "" > 4_unblocking_4.txt

mpiexec -f hostfile.txt -ppn 4 -np 16 ./src.x 1 | tee tmp.txt
echo "$(grep -i call1 tmp.txt | cut -d' ' -f4)" >> 4_blocking_1.txt
echo "$(grep -i call2 tmp.txt | cut -d' ' -f4)" > 4_unblocking_1.txt
rm tmp.txt
mpiexec -f hostfile.txt -ppn 4 -np 16 ./src.x 2 | tee tmp.txt
echo "$(grep -i call1 tmp.txt | cut -d' ' -f4)" >> 4_blocking_2.txt
echo "$(grep -i call2 tmp.txt | cut -d' ' -f4)" >> 4_unblocking_2.txt
rm tmp.txt
mpiexec -f hostfile.txt -ppn 4 -np 16 ./src.x 3 | tee tmp.txt
echo "$(grep -i call1 tmp.txt | cut -d' ' -f4)" >> 4_blocking_3.txt
echo "$(grep -i call2 tmp.txt | cut -d' ' -f4)" >> 4_unblocking_3.txt
rm tmp.txt
mpiexec -f hostfile.txt -ppn 4 -np 16 ./src.x 4 | tee tmp.txt
echo "$(grep -i call1 tmp.txt | cut -d' ' -f4)" >> 4_blocking_4.txt
echo "$(grep -i call2 tmp.txt | cut -d' ' -f4)" >> 4_unblocking_4.txt
rm tmp.txt

make clean
