#!/bin/bash -e
eos cp /eos/juno/user/wudr/DQM/root/8156_240809-1447_556_0.root $TMPDIR
root -l -q '/junofs/users/chuziliang125/UWB_PED_RMS_7thLightOff/GetPEDRMS.cc("$TMPDIR/8156_240809-1447_556_0.root")'
rm $TMPDIR/8156_240809-1447_556_0.root

