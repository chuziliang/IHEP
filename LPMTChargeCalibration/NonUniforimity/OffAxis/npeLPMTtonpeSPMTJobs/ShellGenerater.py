num = 0
for DegreeNum in range(0,6):
    for energy in range(0,5):
        filename = f"job{num}.sh"
        with open(filename, "w") as file: 
            file.write("root -l -q '/junofs/users/chuziliang125/LPMTChargeCalibration/NonUniforimity/OffAxis/npeLPMTtonpeSPMT.cc({},{})'\n".format(str(energy),str(DegreeNum)))
        num += 1
