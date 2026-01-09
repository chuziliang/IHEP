for n in range(0,10):
    filename = f"job_{n}.sh"
    with open(filename, "w") as file:
        file.write("root -l -q '/junofs/users/chuziliang125/SPMT_data_analysis/V22Analysis/20231201_ABC_FW2.3/CoarseTimeDistance.cc({})'".format(str(n)))
    n += 1