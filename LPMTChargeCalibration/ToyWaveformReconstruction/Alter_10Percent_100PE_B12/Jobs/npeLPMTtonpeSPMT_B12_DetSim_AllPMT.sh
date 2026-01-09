#!/bin/bash

j=`expr $1 / 4 + 1`
i=`expr $1 % 4`
root -l -q '/junofs/users/chuziliang125/LPMTChargeCalibration/ToyWaveformReconstruction/Alter_10Percent_100PE_B12/npeLPMTtonpeSPMT_B12_DetSim_AllPMT.cc('$i',3,0)' 
# echo $i
# echo $j
