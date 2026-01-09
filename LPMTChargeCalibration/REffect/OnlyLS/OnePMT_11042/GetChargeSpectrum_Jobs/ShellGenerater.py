EvtPosition=(-0.8,-0.6,-0.4,-0.2,0.0,0.2,0.4,0.6,0.8)
n=0
for i in EvtPosition:
    filename = f"job_{n}.sh"
    with open(filename, "w") as file:
        file.write("#!/bin/bash\n")
        file.write("\n")
        file.write("source /junofs/users/chuziliang125/JUNO_offline/bashrc\n")
        file.write("root -l -q '/junofs/users/chuziliang125/LPMTChargeCalibration/REffect/OnlyLS/OnePMT_11042/GetChargeSpectrumDetSim_NoBar.cc('$1',{})'\n".format(str(i)))
    n += 1