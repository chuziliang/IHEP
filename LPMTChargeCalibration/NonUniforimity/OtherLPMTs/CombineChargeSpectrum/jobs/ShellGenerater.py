LPMTID = {6611,7048,7049,7268,7465}
num = 0
for thisID in LPMTID:
    for position in range(0,227):
        filename = f"job{num}.sh"
        num += 1
        with open(filename, "w") as file:
            file.write("root -l -q '/junofs/users/chuziliang125/LPMTChargeCalibration/NonUniforimity/OtherLPMTs/CombineRootFile.cc({},{})'\n".format(str(position),str(thisID)))
