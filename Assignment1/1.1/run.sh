#!/bin/bash

make all

bash pinger.sh

rm 0.txt && touch 0.txt
rm 1.txt && touch 1.txt
rm 2.txt && touch 2.txt
rm 3.txt && touch 3.txt
rm 4.txt && touch 4.txt

mpiexec -f hostfile.txt -np 2 ./src.x 0 | tee tmp.txt
echo "$(grep -i sending0 tmp.txt | cut -d' ' -f4),$(grep -i receiving0 tmp.txt | cut -d' ' -f4)" >> 0.txt
echo "$(grep -i sending1 tmp.txt | cut -d' ' -f4),$(grep -i receiving1 tmp.txt | cut -d' ' -f4)" >> 0.txt
echo "$(grep -i sending2 tmp.txt | cut -d' ' -f4),$(grep -i receiving2 tmp.txt | cut -d' ' -f4)" >> 0.txt
echo "$(grep -i sending3 tmp.txt | cut -d' ' -f4),$(grep -i receiving3 tmp.txt | cut -d' ' -f4)" >> 0.txt
echo "$(grep -i sending4 tmp.txt | cut -d' ' -f4),$(grep -i receiving4 tmp.txt | cut -d' ' -f4)" >> 0.txt
# cat tmp.txt
# cat 0.txt
rm tmp.txt
mpiexec -f hostfile.txt -np 2 ./src.x 1 |tee tmp.txt
echo "$(grep -i sending0 tmp.txt | cut -d' ' -f4),$(grep -i receiving0 tmp.txt | cut -d' ' -f4)" >> 1.txt
echo "$(grep -i sending1 tmp.txt | cut -d' ' -f4),$(grep -i receiving1 tmp.txt | cut -d' ' -f4)" >> 1.txt
echo "$(grep -i sending2 tmp.txt | cut -d' ' -f4),$(grep -i receiving2 tmp.txt | cut -d' ' -f4)" >> 1.txt
echo "$(grep -i sending3 tmp.txt | cut -d' ' -f4),$(grep -i receiving3 tmp.txt | cut -d' ' -f4)" >> 1.txt
echo "$(grep -i sending4 tmp.txt | cut -d' ' -f4),$(grep -i receiving4 tmp.txt | cut -d' ' -f4)" >> 1.txt
# cat tmp.txt
# cat 0.txt
rm tmp.txt
mpiexec -f hostfile.txt -np 2 ./src.x 2 | tee tmp.txt
echo "$(grep -i sending0 tmp.txt | cut -d' ' -f4),$(grep -i receiving0 tmp.txt | cut -d' ' -f4)" >> 2.txt
echo "$(grep -i sending1 tmp.txt | cut -d' ' -f4),$(grep -i receiving1 tmp.txt | cut -d' ' -f4)" >> 2.txt
echo "$(grep -i sending2 tmp.txt | cut -d' ' -f4),$(grep -i receiving2 tmp.txt | cut -d' ' -f4)" >> 2.txt
echo "$(grep -i sending3 tmp.txt | cut -d' ' -f4),$(grep -i receiving3 tmp.txt | cut -d' ' -f4)" >> 2.txt
echo "$(grep -i sending4 tmp.txt | cut -d' ' -f4),$(grep -i receiving4 tmp.txt | cut -d' ' -f4)" >> 2.txt
# cat tmp.txt
# cat 0.txt
rm tmp.txt
mpiexec -f hostfile.txt -np 2 ./src.x 3 | tee tmp.txt
echo "$(grep -i sending0 tmp.txt | cut -d' ' -f4),$(grep -i receiving0 tmp.txt | cut -d' ' -f4)" >> 3.txt
echo "$(grep -i sending1 tmp.txt | cut -d' ' -f4),$(grep -i receiving1 tmp.txt | cut -d' ' -f4)" >> 3.txt
echo "$(grep -i sending2 tmp.txt | cut -d' ' -f4),$(grep -i receiving2 tmp.txt | cut -d' ' -f4)" >> 3.txt
echo "$(grep -i sending3 tmp.txt | cut -d' ' -f4),$(grep -i receiving3 tmp.txt | cut -d' ' -f4)" >> 3.txt
echo "$(grep -i sending4 tmp.txt | cut -d' ' -f4),$(grep -i receiving4 tmp.txt | cut -d' ' -f4)" >> 3.txt
# cat tmp.txt
# cat 3.txt
rm tmp.txt
mpiexec -f hostfile.txt -np 2 ./src.x 4 | tee tmp.txt
echo "$(grep -i sending0 tmp.txt | cut -d' ' -f4),$(grep -i receiving0 tmp.txt | cut -d' ' -f4)" >> 4.txt
echo "$(grep -i sending1 tmp.txt | cut -d' ' -f4),$(grep -i receiving1 tmp.txt | cut -d' ' -f4)" >> 4.txt
echo "$(grep -i sending2 tmp.txt | cut -d' ' -f4),$(grep -i receiving2 tmp.txt | cut -d' ' -f4)" >> 4.txt
echo "$(grep -i sending3 tmp.txt | cut -d' ' -f4),$(grep -i receiving3 tmp.txt | cut -d' ' -f4)" >> 4.txt
echo "$(grep -i sending4 tmp.txt | cut -d' ' -f4),$(grep -i receiving4 tmp.txt | cut -d' ' -f4)" >> 4.txt
# cat tmp.txt
# cat 4.txt
rm tmp.txt
# rm result.txt > /dev/null
# cat 0.txt >> result.txt
# cat 1.txt >> result.txt
# cat 2.txt >> result.txt
# cat 3.txt >> result.txt
# cat 4.txt >> result.txt
# rm 0.txt && rm 1.txt && rm 2.txt && rm 3.txt && rm 4.txt
# cat result.txt

make clean

python plot.py
