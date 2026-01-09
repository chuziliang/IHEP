# 读取保存的npz文件，并画图
import numpy as np
import matplotlib.pyplot as plt
import math

Q_SingleOring = [1e-5,1e-6,1e-7,1e-8,1e-9]

data = np.load('DoubleOrings.npz')
# 提取array1和array2
TotalLeakRate = data['TotalLeakRate']
Time = data['Time']

# 绘制图形
# plt.figure(figsize=(8, 6))
plt.subplots_adjust(left=0.16, bottom=0.16)
for i in range(len(Q_SingleOring)):
    plt.plot(Time, np.array(TotalLeakRate)[:, i], label=f'Q = $10^{{{int(math.log10(Q_SingleOring[i]))}}}$ $Pa\\cdot m^{3}/s$')
plt.xlabel('Time [s]',fontsize='x-large')
plt.ylabel('Double O-rings Leak Rate [$Pa\\cdot m^{3}/s$]',fontsize='x-large')
plt.legend(loc="lower right",fontsize='large')
plt.grid()
axs = plt.gca()
axs.set_yscale('log')
axs.set_xscale('log')

# 设置x轴和y轴字体大小
plt.xticks(fontsize='large')
plt.yticks(fontsize='large')

plt.show()

# 输出TotalLeakRate达到2.4e-9的时间
for i in range(len(Q_SingleOring)):
    for j in range(len(TotalLeakRate)):
        if TotalLeakRate[j][i] >= 2.329570389294201e-09:
            print(f'Q = {Q_SingleOring[i]}时，TotalLeakRate达到2.3e-9的时间为{Time[j]}s, 即{Time[j]/3600}小时')
            break

for i in range(len(Q_SingleOring)):
    for j in range(len(TotalLeakRate)):
        if Time[j]/3600. >= 35:
            print(f'Time达到35小时，Q={Q_SingleOring[i]}时， TotalLeakRate为{TotalLeakRate[j][i]}')
            break

for i in range(len(Q_SingleOring)):
    for j in range(len(TotalLeakRate)):
        if Time[j]/3600. >= 41.66:
            print(f'Time达到46小时，Q={Q_SingleOring[i]}时， TotalLeakRate为{TotalLeakRate[j][i]}')
            break