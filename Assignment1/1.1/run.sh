#!/bin/bash

make all

mpiexec -hosts csews43,csews45 -np 2 ./src.x 0 > tmp.txt
echo "$(grep -i sending tmp.txt | cut -d' ' -f4),$(grep -i receiving tmp.txt | cut -d' ' -f4)" > 0.txt
# cat tmp.txt
# cat 0.txt
rm tmp.txt
mpiexec -hosts csews43,csews45 -np 2 ./src.x 1 > tmp.txt
echo "$(grep -i sending tmp.txt | cut -d' ' -f4),$(grep -i receiving tmp.txt | cut -d' ' -f4)" > 1.txt
# cat tmp.txt
# cat 0.txt
rm tmp.txt
mpiexec -hosts csews43,csews45 -np 2 ./src.x 2 > tmp.txt
echo "$(grep -i sending tmp.txt | cut -d' ' -f4),$(grep -i receiving tmp.txt | cut -d' ' -f4)" > 2.txt
# cat tmp.txt
# cat 0.txt
rm tmp.txt
mpiexec -hosts csews43,csews45 -np 2 ./src.x 3 > tmp.txt
echo "$(grep -i sending tmp.txt | cut -d' ' -f4),$(grep -i receiving tmp.txt | cut -d' ' -f4)" > 3.txt
# cat tmp.txt
# cat 3.txt
rm tmp.txt
mpiexec -hosts csews43,csews45 -np 2 ./src.x 4 > tmp.txt
echo "$(grep -i sending tmp.txt | cut -d' ' -f4),$(grep -i receiving tmp.txt | cut -d' ' -f4)" > 4.txt
# cat tmp.txt
# cat 4.txt
rm tmp.txt
rm result.txt > /dev/null
cat 0.txt >> result.txt
cat 1.txt >> result.txt
cat 2.txt >> result.txt
cat 3.txt >> result.txt
cat 4.txt >> result.txt
rm 0.txt && rm 1.txt && rm 2.txt && rm 3.txt && rm 4.txt
cat result.txt

make clean

python plot.py
