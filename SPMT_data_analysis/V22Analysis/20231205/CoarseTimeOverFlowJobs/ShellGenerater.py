BinFileName={"Pedestal_10kHz","Pedestal_20kHz","Pedestal_2kHz","Pedestal_3kHz","Pedestal_5kHz","8001_231205-1535_2341_0","8001_231205-1538_2343_0","8001_231205-1542_2345_0","8001_231205-1537_2342_0","8001_231205-1540_2344_0"}
BinFileNameList=list(BinFileName)
for n in range(0,10):
    filename = f"job_{n}.sh"
    with open(filename, "w") as file:
        file.write("root -l -q '/junofs/users/chuziliang125/SPMT_data_analysis/V22Analysis/CoarseTimeOverFlowCheckPlot.cc(\"/junofs/users/chuziliang125/SPMT_data_analysis/V22Analysis/20231205/root/{}.root\",\"/junofs/users/chuziliang125/SPMT_data_analysis/V22Analysis/20231205/CoarseTimeOverFlowResults/{}.root\")' ".format(BinFileNameList[n],BinFileNameList[n]))
    n += 1