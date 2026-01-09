for i in range(0,8):
    filename = f"job{i}.sh"
    with open(filename, "w") as file:
        file.write("root -l -q '/junofs/users/chuziliang125/LPMTChargeCalibration/B12/Analysis/CutDegree/npeLPMTtonpeSPMT.cc({})'\n".format(str(i*5.0+5.0)))
