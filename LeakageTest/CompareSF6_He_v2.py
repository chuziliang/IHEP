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
Mu_Water = 1e-3 #水的粘度系数，Pa*s
Pi = math.pi

def LeakRate_transition (d,P_High,P_Low,Mu,M):
    Q = Pi*pow(d,4)*(pow(P_High,2)-pow(P_Low,2))/(256*Mu*L) + (1.0/6.0)*math.sqrt(2*Pi*R*T/M)*(pow(d,3)/L)*(1+math.sqrt(M/(R*T))*d*((P_High+P_Low)/2)/Mu)/(1+1.24*math.sqrt(M/(R*T))*d*((P_High+P_Low)/2)/Mu)*(P_High-P_Low)
    return  Q

def LeakRate_viscous (d,P_High,P_Low,Mu):
    Q = Pi*pow(d,4)*(pow(P_High,2)-pow(P_Low,2))/(256*Mu*L)
    return  Q

def LeakRate_molecular (d,P_High,P_Low,M):
    Q = (1.0/6.0)*math.sqrt(2*Pi*R*T/M)*(pow(d,3)/L)*(P_High-P_Low)
    return  Q

def LeakRate_All(d,P_High,P_Low,Mu,M):

    conditions = [d < 0.02*2.0/(P_High + P_Low) , (d >= 0.02*2.0/(P_High + P_Low)) & (d <= 0.67*2.0/(P_High + P_Low)) , d > 0.67*2.0/(P_High + P_Low)]
    functions = [lambda d:LeakRate_molecular(d,P_High,P_Low,M),lambda d:LeakRate_transition(d,P_High,P_Low,Mu,M),lambda d:LeakRate_viscous(d,P_High,P_Low,Mu)]
    Q = np.piecewise(d, conditions, functions)
    # pd = d*(P_High + P_Low)/2.0
    # if pd < 0.02:
    #     Q =  LeakRate_molecular(d,P_High,P_Low,M)
    # elif pd > 0.67:
    #     Q =  LeakRate_viscous(d,P_High,P_Low,Mu)
    # else:
    #     Q = LeakRate_transition(d,P_High,P_Low,Mu,M)
    return Q

def LeakRate_Water(d,P_High,P_Low,Mu):
    Q = Pi*pow(d,4)*(P_High-P_Low)/(128*Mu*L)
    return  Q

P_High_He = 1E5
P_Low_He = 0
P_High_SF6 = 1.25E5
P_Low_SF6 = 1E5
P_High_water = 5E5
P_Low_water = 1E5

d_test = np.linspace(0, 2E-4, 200000)
d_SF6_molecular =  d_test < 0.02*2.0/(P_High_SF6 + P_Low_SF6)
d_SF6_transition = (d_test >= 0.02*2.0/(P_High_SF6 + P_Low_SF6)) & (d_test <= 0.67*2.0/(P_High_SF6 + P_Low_SF6))
d_SF6_viscous =    d_test > 0.67*2.0/(P_High_SF6 + P_Low_SF6)
d_He_molecular =   d_test < 0.02*2.0/(P_High_He + P_Low_He)
d_He_transition =  (d_test >= 0.02*2.0/(P_High_He + P_Low_He)) & (d_test <= 0.67*2.0/(P_High_He + P_Low_He))
d_He_viscous =     d_test > 0.67*2.0/(P_High_He + P_Low_He)

Q_He_test = LeakRate_All(d_test,P_High_He,P_Low_He,Mu_He,M_He)
Q_SF6_test = LeakRate_All(d_test,P_High_SF6,P_Low_SF6,Mu_SF6,M_SF6)

plt.plot(d_test[d_He_transition], Q_He_test[d_He_transition], label='He transition leak rate',color='blue')
plt.plot(d_test[d_He_viscous   ], Q_He_test[d_He_viscous   ], label='He viscous leak rate',color='blue',linestyle='--')
plt.plot(d_test[d_He_molecular ], Q_He_test[d_He_molecular], label='He molecular leak rate',color='blue',linestyle=':')

plt.plot(d_test[d_SF6_transition], Q_SF6_test[d_SF6_transition], label='$SF_6$ transition leak rate',color='orange')
plt.plot(d_test[d_SF6_viscous   ], Q_SF6_test[d_SF6_viscous   ], label='$SF_6$ viscous leak rate',color='orange',linestyle='--')
plt.plot(d_test[d_SF6_molecular ], Q_SF6_test[d_SF6_molecular ], label='$SF_6$ molecular leak rate',color='orange',linestyle=':')

plt.xlabel('Diameter of tube [m]',fontsize='x-large')
plt.ylabel('Leak rate [$Pa{\cdot}m^{3}/s$]',fontsize='x-large')
plt.legend(fontsize='large')  # 显示图例

ax = plt.gca()
ax.set_yscale('log')
ax.set_xscale('log')
plt.grid()
plt.xlim(9e-8, 1.1e-4)
plt.ylim(1e-12, 1)
plt.rcParams['axes.titlesize'] = 35
plt.rcParams['axes.labelsize'] = 30
plt.show()

fig,axs = plt.subplots(2,1,sharex=True)
fig.subplots_adjust(hspace=0)
axs[0].plot(Q_SF6_test, Q_He_test)
axs[0].set_xlabel('')
axs[0].set_ylabel('Q$_{He}$ [$Pa{\cdot}m^{3}/s$]',fontsize='x-large')
# ax = plt.gca()
axs[0].set_yscale('log')
# axs[0].set_xscale('log')
# ax.set_xticklabels([])
axs[0].set_xlim(1e-10, 1e-7)
axs[0].set_ylim(1e-9, 1e-6)
axs[0].grid()
# plt.show()

ratio = Q_He_test/Q_SF6_test

print(ratio[10])

axs[1].plot(Q_SF6_test, ratio)
axs[1].set_xlabel('Q$_{SF_6}$ [$Pa{\cdot}m^{3}/s$]',fontsize='x-large')
axs[1].set_ylabel('Q$_{He}$ / Q$_{SF_6}$',fontsize='x-large')
ax = plt.gca()
# ax.set_yscale('log')
ax.set_xscale('log')
axs[1].set_ylim(3, 15)
axs[1].grid()
plt.rcParams['axes.titlesize'] = 35
plt.rcParams['axes.labelsize'] = 30
plt.show()

CheckPoint=[1.6e-9,3.3e-9,0.5e-9,0.2e-9,1.1e-9,2.9e-9,6e-9,1.3e-9,10.6e-9,13.3e-9,39e-9,4.1e-9,33.5e-9,6.9e-9,88.1e-9,120e-9,26e-9,212e-9,2.4e-9]
for m,check in enumerate(CheckPoint):
    for i,k in enumerate(Q_SF6_test):
        if k>check:
            print("SF6=",Q_SF6_test[i]*1e9,"; He=",Q_He_test[i]*1e9)
            break


d_checkPoint = 5.0e-7

# 水的漏率为4.4时，漏孔的直径
for i in range(len(d_test)):
    if LeakRate_Water(d_test[i],P_High_water,P_Low_water,Mu_Water) >= 4.4e-13:
        print(d_test[i])
        d_checkPoint = d_test[i]
        break

for i in range(len(d_test)):
    if Q_SF6_test[i] >= 2.329570389294201e-09:
        print("SF6=",Q_SF6_test[i]*1e9,"; He=",Q_He_test[i]*1e9)
        break



print(LeakRate_All(d_checkPoint,P_High_SF6,P_Low_SF6,Mu_SF6,M_SF6))
print(LeakRate_All(d_checkPoint,P_High_He,P_Low_He,Mu_He,M_He))
# print(LeakRate_viscous(d_checkPoint,P_High_He,P_Low_He,Mu_SF6))
# print(LeakRate_transition(6.6e-7,P_High_He,P_Low_He,Mu_SF6,M_SF6))
# print(LeakRate_viscous(6.6e-7,P_High_He,P_Low_He,Mu_SF6))