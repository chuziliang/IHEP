for i in range(0,10):
    filename = f"job{i}.sh"
    with open(filename, "w") as file:
        file.write("root -l -q '/junofs/users/chuziliang125/LPMTChargeCalibration/B12/Analysis/Analysis.cc({})'\n".format(str(i)))
