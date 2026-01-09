for energy in range(0,5):
    filename = f"job{energy}.sh"
    with open(filename, "a") as file: 
        file.write("root -l -q '/junofs/users/chuziliang125/LPMTChargeCalibration/NonUniforimity/npeLPMTtonpeSPMT.cc({})'\n".format(str(energy)))