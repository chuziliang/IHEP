procid=$1
# posion=$(expr $procid % 227)
for i in {0..19}
do
    root -l -q  '/junofs/users/chuziliang125/LPMTChargeCalibration/NonUniforimity/OtherLPMTs/Analysis.cc('${procid}','${i}',7048)'
done