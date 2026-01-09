for i in range(0,50):
    filename = f"job{i}.sh"
    with open(filename, "w") as file:
        file.write("root -l -q '/junofs/users/chuziliang125/LPMTChargeCalibration/Spallation_neutron/Analysis/Analysis.cc({})'\n".format(str(i)))
