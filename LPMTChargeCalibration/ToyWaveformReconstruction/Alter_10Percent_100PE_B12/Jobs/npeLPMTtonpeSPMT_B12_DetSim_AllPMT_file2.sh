#!/bin/bash

# if [ $1 -gt 3 ]; then
#     j=2
#     i=`expr $1 - 4`
# fi
# if [ $1 -le 3 ]; then
#     j=1
#     i=$1
# fi

j=`expr $1 / 4 + 1`
i=`expr $1 % 4`
root -l -q '/junofs/users/chuziliang125/LPMTChargeCalibration/ToyWaveformReconstruction/Alter_10Percent_100PE_B12/npeLPMTtonpeSPMT_B12_DetSim_AllPMT.cc('$i','$j',1)' 
# echo $i
# echo $j
