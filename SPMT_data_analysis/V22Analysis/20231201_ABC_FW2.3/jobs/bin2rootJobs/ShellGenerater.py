BinFileName={"8001_231205-1412_2335_0","8001_231205-1426_2339_0","8001_231205-1540_2344_0","8001_231205-1418_2336_0","8001_231205-1535_2341_0","8001_231205-1542_2345_0","8001_231205-1422_2337_0","8001_231205-1537_2342_0","8001_231205-1424_2338_0","8001_231205-1538_2343_0"}
BinFileNameList=list(BinFileName)
for n in range(0,10):
    filename = f"job_{n}.sh"
    with open(filename, "w") as file:
        file.write("/junofs/users/chuziliang125/SPMT_data_analysis/V22Analysis/bin2root /junofs/users/chuziliang125/SPMT_data_analysis/V22Analysis/20231201_ABC_FW2.3/binary/{}.bin /junofs/users/chuziliang125/SPMT_data_analysis/V22Analysis/20231201_ABC_FW2.3/root/{}.root".format(BinFileNameList[n],BinFileNameList[n]))
    n += 1