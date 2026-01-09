for n in range(0,10):
    filename = f"job_{n}.sh"
    with open(filename, "w") as file:
        file.write("python $TUTORIALROOT/share/tut_detsim.py --evtmax 10000 --seed {} --output root://junoeos01.ihep.ac.cn//eos/juno/groups/Calibration/chuziliang125/B12/root/detsim_{}.root --user-output root://junoeos01.ihep.ac.cn//eos/juno/groups/Calibration/chuziliang125/B12/user/detsim_{}_user.root gun --particles e- --momentums 7.123 --momentums-mode Gaus --momentums-extra-params 2.87 --volume pTarget --material LS".format(str(n),str(n),str(n)))
    n += 1