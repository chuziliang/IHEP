for n in range(0,50):
    filename = f"job_{n}.sh"
    with open(filename, "w") as file:
        file.write("python $TUTORIALROOT/share/tut_det2calib.py --evtmax -1 --seed {} --input root://junoeos01.ihep.ac.cn//eos/juno/groups/Calibration/chuziliang125/Spallation_neutron/root/detsim_{}.root --output root://junoeos01.ihep.ac.cn//eos/juno/groups/Calibration/chuziliang125/Spallation_neutron/Calib/root/calib_{}.root".format(str(n+1),str(n),str(n)))
    n += 1
