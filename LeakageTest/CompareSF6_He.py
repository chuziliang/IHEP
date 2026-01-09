#这个脚本用于对比He和SF6的漏率 （在混合流的情况下）

import math
import numpy as np
import matplotlib.pyplot as plt

L = 1E-3 #漏孔的长度为1mm
T = 295 #温度为295K
R = 8.3143 #molar gas constant, 8.3143[J/(K\bulletmol)]
M_He = 4E-3
M_SF6 = 146.055E-3 #摩尔质量, Kg/mol
Mu_He = 20.0E-6 #He的粘度系数，Pa*s
Mu_SF6 = 15.0E-6 #He的粘度系数，Pa*s
Pi = math.pi

def LeakRate (d,P_High,P_Low,Mu,M):
    Q = Pi*pow(d,4)*(pow(P_High,2)-pow(P_Low,2))/(256*Mu*L) + (1.0/6.0)*math.sqrt(2*Pi*R*T/M)*(pow(d,3)/L)*(1+math.sqrt(M/(R*T))*d*((P_High+P_Low)/2)/Mu)/(1+1.24*math.sqrt(M/(R*T))*d*((P_High+P_Low)/2)/Mu)*(P_High-P_Low)
    return  Q

def LeakRate_viscous (d,P_High,P_Low,Mu):
    Q = Pi*pow(d,4)*(pow(P_High,2)-pow(P_Low,2))/(256*Mu*L)
    return  Q

def LeakRate_molecular (d,P_High,P_Low,M):
    Q = (1.0/6.0)*math.sqrt(2*Pi*R*T/M)*(pow(d,3)/L)*(P_High-P_Low)
    return  Q

P_High_He = 1E5
P_Low_He = 0
P_High_SF6 = 1.25E5
P_Low_SF6 = 1E5

d_test = np.linspace(0, 2E-5, 2000)
Q_He_test = LeakRate(d_test,P_High_He,P_Low_He,Mu_He,M_He)
Q_SF6_test = LeakRate(d_test,P_High_SF6,P_Low_SF6,Mu_SF6,M_SF6)

Q_He_test_viscous = LeakRate_viscous (d_test,P_High_He,P_Low_He,Mu_He)
Q_He_test_molecular = LeakRate_molecular (d_test,P_High_He,P_Low_He,M_He)

Q_SF6_test_viscous = LeakRate_viscous (d_test,P_High_SF6,P_Low_SF6,Mu_SF6)
Q_SF6_test_molecular = LeakRate_molecular (d_test,P_High_SF6,P_Low_SF6,M_SF6)

plt.plot(d_test, Q_He_test, label='He transition leak rate',color='blue')
plt.plot(d_test, Q_He_test_viscous, label='He viscous leak rate',color='blue',linestyle='--')
plt.plot(d_test, Q_He_test_molecular, label='He molecular leak rate',color='blue',linestyle=':')

plt.plot(d_test, Q_SF6_test, label='$SF_6$ transition leak rate',color='orange')
plt.plot(d_test, Q_SF6_test_viscous, label='$SF_6$ viscous leak rate',color='orange',linestyle='--')
plt.plot(d_test, Q_SF6_test_molecular, label='$SF_6$ molecular leak rate',color='orange',linestyle=':')
plt.axvline(x=0.02*2.0/(P_High_He + P_Low_He),color='blue',linestyle=':')
plt.axvline(x=0.67*2.0/(P_High_He + P_Low_He),color='blue',linestyle='--')
plt.axvline(x=0.02*2.0/(P_High_SF6 + P_Low_SF6),color='orange',linestyle=':')
plt.axvline(x=0.67*2.0/(P_High_SF6 + P_Low_SF6),color='orange',linestyle='--')
plt.xlabel('Radius of tube/m')
plt.ylabel('Leak rate/$Pa{\cdot}m^{3}/s$')
plt.legend()  # 显示图例

ax = plt.gca()
ax.set_yscale('log')
ax.set_xscale('log')
plt.show()

plt.plot(Q_SF6_test, Q_He_test)
plt.xlabel('$SF_6$ leak rate/$Pa{\cdot}m^{3}/s$')
plt.ylabel('He leak rate/$Pa{\cdot}m^{3}/s$')
plt.show()
