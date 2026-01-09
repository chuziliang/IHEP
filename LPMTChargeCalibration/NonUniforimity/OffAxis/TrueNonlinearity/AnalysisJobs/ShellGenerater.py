energy=(1000,100,10,1500,150,1,200,20,2,300,30,3,4,500,50,5,6,700,7,8,9)
numbers=(100,50,50,200,50,50,50,10,50,100,20,50,50,100,50,50,50,100,50,50,50)
LPMT_ID = (8950,8952,8957,8963,8969,8971)
num=0
for E in energy:
    filename = f"job_{num}.sh"
    with open(filename, "a") as file:
        for ID in LPMT_ID:
            for i in range(0,numbers[num]):
                file.write("root -l -q '/junofs/users/chuziliang125/LPMTChargeCalibration/NonUniforimity/OffAxis/TrueNonlinearity/Analysis.cc({},{},{})'\n".format(str(E),str(i),str(ID)))
    num += 1
