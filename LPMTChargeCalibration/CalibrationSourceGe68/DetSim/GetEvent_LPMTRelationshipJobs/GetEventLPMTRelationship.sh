#!/bin/bash
current_time=$(date "+%Y-%m-%d %H:%M:%S")
echo "Current time: $current_time"
FileId=`expr $1 / 12`
m=`expr $1 % 12`
CutLevel=`expr $m / 4 + 1`
ChosenType=`expr $m % 4`
root -l -q '/junofs/users/chuziliang125/LPMTChargeCalibration/CalibrationSourceGe68/DetSim/npeLPMTtonpeSPMT_DetSim_AllPMT.cc('$ChosenType','$CutLevel','$FileId')'
current_time=$(date "+%Y-%m-%d %H:%M:%S")
echo "Current time: $current_time"