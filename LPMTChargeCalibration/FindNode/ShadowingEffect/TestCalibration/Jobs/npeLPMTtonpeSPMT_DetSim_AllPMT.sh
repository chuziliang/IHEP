FileId=`expr $1 / 12`
m=`expr $1 % 12`
CutLevel=`expr $m / 4 + 1`
ChosenType=`expr $m % 4`
root -l -q '/junofs/users/chuziliang125/LPMTChargeCalibration/FindNode/ShadowingEffect/TestCalibration/npeLPMTtonpeSPMT_DetSim_AllPMT.cc('$ChosenType','$CutLevel','$FileId')' 
