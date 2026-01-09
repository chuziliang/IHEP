from scipy.integrate import quad
import numpy as np
import matplotlib.pyplot as plt

# 水的深度/m
def WaterDepth(t):
    if t < 44.0:
        depth = t*(43.0/44.0)
    else:
        depth = 43.0
    return depth

# 电子学盒没过的深度/m
def UWBDepth(t,d):
    if WaterDepth(t) < d:
        UWB_Depth = 0
    else:
        UWB_Depth = WaterDepth(t) - d
    return UWB_Depth

# 水的漏率
def LeakV(t,d):
    P_water = 1+UWBDepth(t,d)/10.34 # 水的压强/atm
    V = P_water*P_water - 1
    return V

d_s9 = 2.9686

# 以S9可能漏水的日期，计算漏水的阈值
Target, error = quad(LeakV, 0, 48,args=(d_s9,))
print(Target)

d = [40635.5,38050.3,36242.4,34544,32182.7,29531.27,26833.69,23790.04,19834.05,16832.55,13921.29,11356.6,8878.7,6801.2,5060.1]
layer = ["N-9", "N-7", "N-6", "N-5", "N-4", "N-3", "N-2", "N-1", "S-1", "S-2", "S-3", "S-4", "S-5", "S-6", "S-7"]


# t_test = np.linspace(0,50,50)
# depth_test = []
# for  i,k in enumerate(t_test):
#     depth_test.append(UWBDepth(k,40.6355))

# plt.plot(t_test,depth_test)
# plt.show()

for i,k in enumerate(d):
    d_m = k/1000.
    # print(layer[i])
    t = 48
    test = 0
    while test<Target:
        test, Test_error = quad(LeakV, 0, t,args=(d_m,))
        t += 1
        # print(test,t)
    print(layer[i],"2月4日后",t-49,"天")

