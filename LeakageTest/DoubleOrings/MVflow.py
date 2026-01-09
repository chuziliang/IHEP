import math
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import fsolve


L = 1E-3 #漏孔的长度为1mm
T = 295 #温度为295K
R = 8.3143 #molar gas constant, 8.3143[J/(K\bulletmol)]
M_He = 4E-3
M_SF6 = 146.055E-3 #摩尔质量, Kg/mol
Mu_He = 20.0E-6 #He的粘度系数，Pa*s
Mu_SF6 = 15.0E-6 #He的粘度系数，Pa*s
Pi = math.pi

def LeakRate_transition (d,P_High,P_Low,Mu,M):
    Q = Pi*pow(d,4)*(pow(P_High,2)-pow(P_Low,2))/(256*Mu*L) + (1.0/6.0)*math.sqrt(2*Pi*R*T/M)*(pow(d,3)/L)*(1+math.sqrt(M/(R*T))*d*((P_High+P_Low)/2)/Mu)/(1+1.24*math.sqrt(M/(R*T))*d*((P_High+P_Low)/2)/Mu)*(P_High-P_Low)
    return Q

def LeakRate_viscous (d,P_High,P_Low,Mu):
    Q = Pi*pow(d,4)*(pow(P_High,2)-pow(P_Low,2))/(256*Mu*L)
    return Q

def LeakRate_molecular (d,P_High,P_Low,M):
    Q = (1.0/6.0)*math.sqrt(2*Pi*R*T/M)*(pow(d,3)/L)*(P_High-P_Low)
    return Q

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

# 根据LeakRate_All Q反解出d
def calculate_d(Q, P_High, P_Low, Mu, M):
    def equation(d):
        # 计算 LeakRate_All 函数在当前 d 值下的输出，并减去目标泄漏率 Q
        return LeakRate_All(d, P_High, P_Low, Mu, M) - Q
    # 为 fsolve 函数提供初始猜测值，即从该值开始搜索满足方程的解
    d_initial_guess = Q
    d_solution = fsolve(equation, d_initial_guess)
    return d_solution[0]

P_High_SF6 = 1.25E5
P_Low_SF6 = 1E5
V = 2e-7
PM0 = P_Low_SF6

Q_SingleOring = [1e-5,1e-6,1e-7,1e-8,1e-9]

D_SingleOring = []

for i in range(len(Q_SingleOring)):
    D_SingleOring.append(calculate_d(Q_SingleOring[i], P_High_SF6, P_Low_SF6, Mu_SF6, M_SF6))
    # 验算解是否正确
    Q_calculated = LeakRate_All(D_SingleOring[i], P_High_SF6, P_Low_SF6, Mu_SF6, M_SF6)

    print("Q=",Q_SingleOring[i],"; D=",D_SingleOring[i],"; Q_calculated=",Q_calculated)

TotalTime = 10000000
tbin = 1000000
dt = TotalTime/tbin
Time = np.linspace(0,TotalTime,tbin)

PM = np.full(len(Q_SingleOring), PM0)
TotalLeakRate = []
for t in range(tbin):

    LeakRate_step = []
    for i in range(len(Q_SingleOring)):
        Leak_in = LeakRate_All(D_SingleOring[i],P_High_SF6,PM[i],Mu_SF6,M_SF6)*dt
        Leak_out_rate = LeakRate_All(D_SingleOring[i],PM[i],P_Low_SF6,Mu_SF6,M_SF6)
        Leak_out = Leak_out_rate*dt
        PM[i] = PM[i] + (Leak_in - Leak_out)/V
        LeakRate_step.append(Leak_out_rate)
    TotalLeakRate.append(LeakRate_step)
    del LeakRate_step

print(PM)

# 保存数据
np.savez('DoubleOrings.npz', Time=Time, TotalLeakRate=TotalLeakRate)

# 画出每个Q_SingleOring的泄漏率随时间的变化
for i in range(len(Q_SingleOring)):
    plt.plot(Time, np.array(TotalLeakRate)[:, i], label=f'Q = {Q_SingleOring[i]} $Pa\\cdot m^{3}/s$')
plt.xlabel('Time/s',fontsize='x-large')
plt.ylabel('Double O-rings leak rate/$Pa\\cdot m^{3}/s$',fontsize='x-large')
plt.legend(loc="best",fontsize='x-large')
axs = plt.gca()
axs.set_yscale('log')
axs.set_xscale('log')
plt.show()