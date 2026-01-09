BinFileName={"Pedestal_10kHz","Pedestal_20kHz","Pedestal_2kHz","Pedestal_3kHz","Pedestal_5kHz"}
BinFileNameList=list(BinFileName)
for n in range(0,5):
    filename = f"job_{n}.sh"
    with open(filename, "w") as file:
        file.write("/junofs/users/chuziliang125/SPMT_data_analysis/V22Analysis/bin2root /junofs/users/chuziliang125/SPMT_data_analysis/V22Analysis/20231205/FranceBin/{}.bin /junofs/users/chuziliang125/SPMT_data_analysis/V22Analysis/20231205/root/{}.root".format(BinFileNameList[n],BinFileNameList[n]))
    n += 1