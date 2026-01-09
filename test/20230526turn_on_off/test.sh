#!/bin/bash

i=0
GCU=()
ABC=()
LVchannel=()
times=0
while read line
do
    ((i++))
    if [[ $i -eq "1" ]]; then
    continue
    fi
    line_array=($line)
    GCU+=("${line_array[0]}")
    ABC+=("${line_array[1]}")
    LVchannel+=("${line_array[2]}")
    if [[ $i -eq "2" ]]; then
    times=${line_array[3]}
    echo "$times"
    fi
    echo "${GCU[$((i-2))]}"
    echo "${ABC[$((i-2))]}"
    echo "${LVchannel[$((i-2))]}"
done <  GCU.txt
for num in "${GCU[@]}"; do
    echo $num
done