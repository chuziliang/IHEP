for n in range(0,50):
    filename = f"job_{n}.sh"
    with open(filename, "w") as file:
        file.write("python $TUTORIALROOT/share/tut_det2elec.py --evtmax -1 --seed {} --input IBD:root://junoeos01.ihep.ac.cn//eos/juno/groups/Calibration/chuziliang125/Spallation_neutron/root/detsim_{}.root --rate IBD:1.0 --output root://junoeos01.ihep.ac.cn//eos/juno/groups/Calibration/chuziliang125/Spallation_neutron/elec/root/elec_{}.root --user-output root://junoeos01.ihep.ac.cn//eos/juno/groups/Calibration/chuziliang125/Spallation_neutron/elec/user/elec_user{}.root --enableSPMT".format(str(n+1),str(n),str(n),str(n)))
    n += 1