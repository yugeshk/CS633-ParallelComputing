#!/bin/bash

rm hostfile.txt > /dev/null 2>&1
touch hostfile.txt
hosts=()
count=0
echo "Generating hostfile"
for i in $(awk '{print $1; }' myhosts.txt)
do
    ping -c2 $i.cse.iitk.ac.in > /dev/null
    if [ $? -eq 0 ]
    then
        hosts+=("$i")
        echo "$i" >> hostfile.txt
        count=$((count+1))
        # echo $count
        if [ $count == 12 ]
        then
            break
        fi
    fi
done
echo "Hostfile generated."

# echo ${hosts[@]}



