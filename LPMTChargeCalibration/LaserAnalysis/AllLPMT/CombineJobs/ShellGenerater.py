num = 0
for position in range(0,21):
    filename = f"job{num}.sh"
    num += 1
    with open(filename, "w") as file:
        file.write("root -l -q '/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/AllLPMT/CombineRootFile.cc({})'\n".format(str(position)))
