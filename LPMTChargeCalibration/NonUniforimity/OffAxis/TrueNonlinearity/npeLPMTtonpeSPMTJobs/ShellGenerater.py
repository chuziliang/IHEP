num=0
for DegreeNum in range(0,6):
    filename = f"job_{num}.sh"
    with open(filename, "w") as file:
        file.write("root -l -q '/junofs/users/chuziliang125/LPMTChargeCalibration/NonUniforimity/OffAxis/TrueNonlinearity/npeLPMTtonpeSPMT.cc({})'\n".format(str(DegreeNum)))
    num += 1
