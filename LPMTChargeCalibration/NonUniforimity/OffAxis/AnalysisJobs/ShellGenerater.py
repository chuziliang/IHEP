LPMT_ID = (8950,8952,8957,8963,8969,8971)
num=0
for ID in LPMT_ID:
    for energy in range(0,5):
        filename = f"job_{num}.sh"
        with open(filename, "a") as file:
            for i in range(0,10):
                file.write("root -l -q '/junofs/users/chuziliang125/LPMTChargeCalibration/NonUniforimity/OffAxis/Analysis.cc({},{},{})'\n".format(str(i),str(energy),ID))
        num += 1
