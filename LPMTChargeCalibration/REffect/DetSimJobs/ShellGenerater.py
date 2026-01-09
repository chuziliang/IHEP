position=(2142.1269,19314.2442,-227.1948)
EvtPosition=(-0.8,-0.6,-0.4,-0.2,0.0,0.2,0.4,0.6,0.8)
n=0
for i in EvtPosition:
    filename = f"job_{n}.sh"
    with open(filename, "w") as file:
        file.write("#!/bin/bash\n")
        file.write("\n")
        file.write("j=$1\n")
        file.write("k=$(expr $j + 2)\n")
        file.write("i=$(expr $k \* {})\n".format(str(n+1)))
        file.write("source /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/setup.sh\n")
        file.write("python $TUTORIALROOT/share/tut_detsim.py \
--evtmax 50 --seed $i --light-yield 11522000 --debug-disable-sticks \
--output $TEMP/REffect_{}_detsim_$j.root --user-output $TEMP/REffect_{}_detsim_${{j}}_user.root gun --particles e- --momentums 1 --momentums-interp KineticEnergy \
--position {} {} {}\n".format(str(i),str(i),str(i*position[0]),str(i*position[1]),str(i*position[2])))
        file.write("eos cp $TEMP/REffect_{}_detsim_$j.root /eos/juno/groups/Calibration/chuziliang125/REffect/NoBar/{}/root/detsim_$j.root\n".format(str(i),str(i)))
        file.write("eos cp $TEMP/REffect_{}_detsim_${{j}}_user.root /eos/juno/groups/Calibration/chuziliang125/REffect/NoBar/{}/user/detsim_${{j}}_user.root\n".format(str(i),str(i)))
        file.write("rm $TEMP/REffect_{}_detsim_$j.root\n".format(str(i)))
        file.write("rm $TEMP/REffect_{}_detsim_${{j}}_user.root".format(str(i)))
    n += 1