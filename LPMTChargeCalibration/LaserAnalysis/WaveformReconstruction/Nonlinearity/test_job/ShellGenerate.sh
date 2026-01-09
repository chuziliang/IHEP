#!/bin/bash

energy=(2);
numbers=(100 50 50 200 50 50 50 10 50 100 20 50 50 100 50 50 50 100 50 50 50);
m=0;
for i in "${energy[@]}"
do
    for ((j=0;j<${numbers[m]};j++))
    do
    echo "root -l -q '/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/GetTruePE_Amplitude.cc($i,$j)'" >> "job_$m.sh"
    done
    ((m++));
done
