num = 0
for position in range(0,227):
    for i in range(0,20):
        filename = f"job{num}.sh"
        num += 1
        with open(filename, "w") as file:
            file.write("root -l -q '/junofs/users/chuziliang125/LPMTChargeCalibration/NonUniforimity/20231114_OffAxis/Analysis.cc({},{},6828)'\n".format(str(position),str(i)))
