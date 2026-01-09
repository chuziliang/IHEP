#!/bin/bash

for i in {0..53}
do
    echo "/junofs/users/chuziliang125/PED_analysis20230816/Analysis/Binary_Analysis_AR_2 -i $i" > "LightOffAnalysis_$i.sh"
done
