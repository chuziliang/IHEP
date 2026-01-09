num=0
for DegreeNum in range(0,6):
    filename = f"job_{num}.sh"
    with open(filename, "a") as file:
        for EnergyNum in range(0,21):
            file.write("root -l -q '/junofs/users/chuziliang125/LPMTChargeCalibration/NonUniforimity/OffAxis/TrueNonlinearity/CombineRootFile.cc({},{})'\n".format(str(EnergyNum),str(DegreeNum)))
    num += 1
