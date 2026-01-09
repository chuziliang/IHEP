#!/bin/bash -e
eos cp /eos/juno/user/wudr/DQM/root/8187_240830-2131_667_0.root $TMPDIR
root -l -q '/junofs/users/chuziliang125/UWB_PED_RMS_7thLightOff/GetPEDRMS.cc("$TMPDIR/8187_240830-2131_667_0.root")'
rm $TMPDIR/8187_240830-2131_667_0.root

