
j=`expr $1 / 4 + 1`
i=`expr $1 % 4`
root -l -q '/junofs/users/chuziliang125/LPMTChargeCalibration/ToyWaveformReconstruction/Spallation_neutron/DetSim/npeLPMTtonpeSPMT_DetSim_AllPMT.cc('$i','$j')' 
