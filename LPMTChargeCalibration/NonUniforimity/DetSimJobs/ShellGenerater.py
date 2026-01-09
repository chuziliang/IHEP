energy=(0.5,2,5,10)
numbers=10000
position=(2142.1269,19314.2442,-227.1948)
EvtPosition=(0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9)
n=0
for j in energy:
    for i in EvtPosition:
        filename = f"job_{n}.sh"
        with open(filename, "w") as file:
            file.write("python $TUTORIALROOT/share/tut_detsim.py --evtmax 10000 --output /junofs/users/chuziliang125/LPMTChargeCalibration/NonUniforimity/detsim_{}.root --user-output /junofs/users/chuziliang125/LPMTChargeCalibration/NonUniforimity/detsim_{}_user.root gun --particles e- --momentums {} --position {} {} {}".format(str(energy[j])+"MeV"+str(EvtPosition[i]),str(energy[j])+"MeV"+str(EvtPosition[i]),str(energy[j]),str(i*position[0]),str(i*position[1]),str(i*position[2])))
        n += 1