num = 0
for position in range(0,227):
    filename = f"job{num}.sh"
    num += 1
    with open(filename, "w") as file:
        file.write("root -l -q '/junofs/users/chuziliang125/LPMTChargeCalibration/NonUniforimity/20231106_NewVersion_OffAxis/CombineRootFile.cc({})'\n".format(str(position)))
