#!/bin/bash

energy=(1000 100 10 1500 150 1 200 20 2 300 30 3 4 500 50 5 6 700 7 8 9);
numbers=(100 50 50 200 50 50 50 10 50 100 20 50 50 100 50 50 50 100 50 50 50);
m=0;
for i in "${energy[@]}"
do
    for ((j=0;j<${numbers[m]};j++))
    do
    echo "root -l -q '/junofs/users/chuziliang125/LPMTChargeCalibration/ToyWaveformReconstruction/Alter_10Percent_100PE/GetChargeSpectrum.cc($i,$j)'" >> "job_$m.sh"
    done
    ((m++));
done
