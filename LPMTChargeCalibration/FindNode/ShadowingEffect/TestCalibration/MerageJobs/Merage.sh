m=$1
CutLevel=`expr $m / 4 + 1`
ChosenType=`expr $m % 4`
root -l -q '/junofs/users/chuziliang125/LPMTChargeCalibration/FindNode/ShadowingEffect/TestCalibration/npeLPMTtonpeSPMT_DetSim_AllPMT_merage.cc('$ChosenType','$CutLevel')' 
