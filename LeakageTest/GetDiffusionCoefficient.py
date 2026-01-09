def DiffusionCoefficient(T,MA,MB,P,VA,VB):
    D=0.0101*(T**1.75)*((1.0/MA + 1.0/MB)**0.5)/(P*((VA**(1/3))+(VB**(1/3)))**2)
    return D

print(DiffusionCoefficient(295,146,28.959,1.01e5,20.1,69.7))
