EvtPosition=(-0.8,-0.6,-0.4,-0.2,0.0,0.2,0.4,0.6,0.8)
n=0
for i in EvtPosition:
    filename = f"job_{n}.sh"
    with open(filename, "w") as file:
        file.write("#!/bin/bash\n")
        file.write("\n")
        file.write("source /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/setup.sh\n")
        file.write("root -l -q '/junofs/users/chuziliang125/LPMTChargeCalibration/REffect/CombineRootFile.cc({})'\n".format(str(i)))
    n += 1