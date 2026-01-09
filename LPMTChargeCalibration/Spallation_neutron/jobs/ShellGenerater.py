for n in range(0,50):
    filename = f"job_{n}.sh"
    with open(filename, "w") as file:
        file.write("python $TUTORIALROOT/share/tut_detsim.py --evtmax 10000 --seed {} --output root://junoeos01.ihep.ac.cn//eos/juno/groups/Calibration/chuziliang125/Spallation_neutron/root/detsim_{}.root --user-output root://junoeos01.ihep.ac.cn//eos/juno/groups/Calibration/chuziliang125/Spallation_neutron/user/detsim_{}_user.root gun --particles gamma --momentums 2.2 --volume pTarget --material LS".format(str(n),str(n),str(n)))
    n += 1