#!/bin/bash
current_time=$(date "+%Y-%m-%d %H:%M:%S")
echo "Current time: $current_time"
# j=`expr $1 % 20`
# i=`expr $1 / 20`
# source /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J22.1.0-rc4/setup.sh
root -l -q '/junofs/users/chuziliang125/LPMTChargeCalibration/CalibrationSourceGe68/DetSim/CombineRootFile.cc('$1')'
current_time=$(date "+%Y-%m-%d %H:%M:%S")
echo "Current time: $current_time"