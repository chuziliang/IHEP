for energy in range(0,5):
    filename = f"job{energy}.sh"
    with open(filename, "a") as file:
        for i in range(0,10):
            file.write("root -l -q '/junofs/users/chuziliang125/LPMTChargeCalibration/NonUniforimity/Analysis.cc({},{})'\n".format(str(i),str(energy)))
