#!/bin/bash

for i in {0..284}
do
    echo "/junofs/users/chuziliang125/LightOffAnalysis20230619/AnalysisABCF2.0/Binary_Analysis_AR_2 -i $i" > "LightOffAnalysis_$i.sh"
done
