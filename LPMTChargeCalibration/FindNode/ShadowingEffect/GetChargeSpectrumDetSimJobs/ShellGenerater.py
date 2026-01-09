for i in range(0,50):
    filename = f"job{i}.sh"
    with open(filename, "w") as file:
        file.write("root -l -q '/junofs/users/chuziliang125/LPMTChargeCalibration/FindNode/ShadowingEffect/GetChargeSpectrum_Neutron.cc({})'\n".format(str(i)))
