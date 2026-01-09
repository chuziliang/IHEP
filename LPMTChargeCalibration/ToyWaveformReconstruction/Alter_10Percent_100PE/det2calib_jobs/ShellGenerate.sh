#!/bin/bash

energy=(1000 100 10 1500 150 1 200 20 2 300 30 3 4 500 50 5 6 700 7 8 9);
numbers=(100 50 50 200 50 50 50 10 50 100 20 50 50 100 50 50 50 100 50 50 50);
m=0;
n=0;
for i in "${energy[@]}"
do
    for ((j=0;j<${numbers[m]};j++))
    do
    echo "python \$TUTORIALROOT/share/tut_det2calib.py --evtmax -1 --seed $n --input root://junoeos01.ihep.ac.cn//eos/juno/dirac/juno/production/calibration/centos7_amd64_gcc830/Pre-Release/J22.1.0-rc0/Laser/MultiElectron/${i}MeV/detsim/root/detsim-$j.root --output root://junoeos01.ihep.ac.cn//eos/juno/groups/Calibration/chuziliang125/calib/ToyWaveRec/Alter_10PercentAt100PE/root/${i}MeV/calib-$j.root" >> "job_$n.sh"
    ((n++));
    done
    ((m++));
done
