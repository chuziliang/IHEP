# energy=(0.5,2,5,10)
numbers=1000
position=(2142.1269,19314.2442,-227.1948)
EvtPosition=(0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9)


j=10
i=0.5
for n in range(0,10):
    filename = f"job_35_{n}.sh"
    with open(filename, "w") as file:
        file.write("python $TUTORIALROOT/share/tut_detsim.py --evtmax 1000 --output /junofs/users/chuziliang125/LPMTChargeCalibration/NonUniforimity/root_file/detsim_{}.root --user-output /junofs/users/chuziliang125/LPMTChargeCalibration/NonUniforimity/root_file/detsim_{}_user.root gun --particles e- --momentums {} --position {} {} {}".format(str(j)+"MeV"+str(i)+"_"+str(n),str(j)+"MeV"+str(i)+"_"+str(n),str(j),str(i*position[0]),str(i*position[1]),str(i*position[2])))
