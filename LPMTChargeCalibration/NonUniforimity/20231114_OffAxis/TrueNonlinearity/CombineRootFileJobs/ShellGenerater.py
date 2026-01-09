# num=0
# for DegreeNum in range(0,6):
filename = "job.sh"

with open(filename, "a") as file:
    for EnergyNum in range(0,21):
        file.write("root -l -q '/junofs/users/chuziliang125/LPMTChargeCalibration/NonUniforimity/20231114_OffAxis/TrueNonlinearity/CombineRootFile.cc({})'\n".format(str(EnergyNum)))
# num += 1
