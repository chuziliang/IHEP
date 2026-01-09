import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.colors as mcolors
from matplotlib.backends.backend_pdf import PdfPages
import sys

# 读取保存的npz文件
data_1 = np.load('LeakBox1.0e-5.npz')
data_2 = np.load('LeakBox1.0e-6.npz')
data_6 = np.load('LeakBox6.0e-7.npz')
data_3 = np.load('LeakBox1.0e-7.npz')
data_4 = np.load('LeakBox1.0e-8.npz')
data_5 = np.load('LeakBox1.0e-9.npz')
# data_7 = np.load('LeakBox0.7e-7.npz')
# data_8 = np.load('LeakBox5.7e-7.npz')
# 提取array1和array2
Time_1 = data_1['Time']
SF6ToTime_Average_1 = data_1['SF6ToTime_Average']
Time_2 = data_2['Time']
SF6ToTime_Average_2 = data_2['SF6ToTime_Average']
Time_6 = data_6['Time']
SF6ToTime_Average_6 = data_6['SF6ToTime_Average']
Time_3 = data_3['Time']
SF6ToTime_Average_3 = data_3['SF6ToTime_Average']
Time_4 = data_3['Time']
SF6ToTime_Average_4 = data_4['SF6ToTime_Average']
Time_5 = data_5['Time']
SF6ToTime_Average_5 = data_5['SF6ToTime_Average']
# Time_7 = data_7['Time']
# SF6ToTime_Average_7 = data_7['SF6ToTime_Average']
# Time_8 = data_8['Time']
# SF6ToTime_Average_8 = data_8['SF6ToTime_Average']

# 计算1ppm时的漏率
def calculate_leak_rate(SF6ToTime_Average):
    LeakRate=(0.02/SF6ToTime_Average)*1e-9
    return LeakRate

LeakRate_1=calculate_leak_rate(SF6ToTime_Average_1)
LeakRate_2=calculate_leak_rate(SF6ToTime_Average_2)
LeakRate_6=calculate_leak_rate(SF6ToTime_Average_6)
LeakRate_3=calculate_leak_rate(SF6ToTime_Average_3)
LeakRate_4=calculate_leak_rate(SF6ToTime_Average_4)
LeakRate_5=calculate_leak_rate(SF6ToTime_Average_5)

# with PdfPages('combine.pdf') as pdf:
# plt.figure(figsize=(8, 6))
fig,axs = plt.subplots(2,1,sharex=True)
fig.subplots_adjust(hspace=0)
# plt.plot(Time_1, SF6ToTime_Average_1,label='$1.0\\times 10^{-5}m^{3}/s$')
# plt.plot(Time_2, SF6ToTime_Average_2,label='$1.0\\times 10^{-6}m^{3}/s$')
# plt.plot(Time_6, SF6ToTime_Average_6,label='$6.0\\times 10^{-7}m^{3}/s$')
# plt.plot(Time_3, SF6ToTime_Average_3,label='$1.0\\times 10^{-7}m^{3}/s$')
# plt.plot(Time_4, SF6ToTime_Average_4,label='$1.0\\times 10^{-8}m^{3}/s$')
# plt.plot(Time_5, SF6ToTime_Average_5,label='$1.0\\times 10^{-9}m^{3}/s$')
# plt.plot(Time_7, SF6ToTime_Average_7,label='$0.7\\times 10^{-7}m^{3}/s$')    
# plt.plot(Time_8, SF6ToTime_Average_8,label='$5.7\\times 10^{-7}m^{3}/s$')

axs[0].plot(Time_1, LeakRate_1,label='$L = 1.0\\times 10^{-5}$ $m^{3}/s$')
axs[0].plot(Time_2, LeakRate_2,label='$L = 1.0\\times 10^{-6}$ $m^{3}/s$')
axs[0].plot(Time_6, LeakRate_6,label='$L = 6.0\\times 10^{-7}$ $m^{3}/s$')
axs[0].plot(Time_3, LeakRate_3,label='$L = 1.0\\times 10^{-7}$ $m^{3}/s$')
axs[0].plot(Time_4, LeakRate_4,label='$L = 1.0\\times 10^{-8}$ $m^{3}/s$')
axs[0].plot(Time_5, LeakRate_5,label='$L = 1.0\\times 10^{-9}$ $m^{3}/s$')
axs[0].set_xlabel('')
axs[0].set_ylabel(r'Leak rate [Pa$\cdot m^{3}/s$]',fontsize='large')
axs[0].set_yscale('log')
# axs[0].set_xscale('log')
axs[0].set_xlim(100, 86000)
axs[0].set_ylim(8e-11, 1e-7)
axs[0].grid()


axs[1].plot(Time_1, SF6ToTime_Average_1*5.0,label='$L = 1.0\\times 10^{-5}$ $m^{3}/s$')
axs[1].plot(Time_2, SF6ToTime_Average_2*5.0,label='$L = 1.0\\times 10^{-6}$ $m^{3}/s$')
axs[1].plot(Time_6, SF6ToTime_Average_6*5.0,label='$L = 6.0\\times 10^{-7}$ $m^{3}/s$')
axs[1].plot(Time_3, SF6ToTime_Average_3*5.0,label='$L = 1.0\\times 10^{-7}$ $m^{3}/s$')
axs[1].plot(Time_4, SF6ToTime_Average_4*5.0,label='$L = 1.0\\times 10^{-8}$ $m^{3}/s$')
axs[1].plot(Time_5, SF6ToTime_Average_5*5.0,label='$L = 1.0\\times 10^{-9}$ $m^{3}/s$')
# plt.plot(Time_7, SF6ToTime_Average_7,label='$0.7\\times 10^{-7}m^{3}/s$')
# plt.plot(Time_8, SF6ToTime_Average_8,label='$5.7\\times 10^{-7}m^{3}/s$')
axs[1].set_xlabel('Time [s]',fontsize='x-large')
axs[1].set_ylabel(r'Concentration [PPM]',fontsize='large')
axs[1].set_ylim(0,0.52)
axs[1].grid()

plt.legend(fontsize='large', loc='upper left',bbox_to_anchor=(-0.005, 1.33))
# plt.title('')
# plt.xlabel('Time/s',fontsize='x-large')
# plt.ylabel(r'Leak rate/$Pa\times m^{3}/s$',fontsize='x-large')
# plt.grid()
# ax = plt.gca()
# ax.set_xscale('log')
# ax.set_yscale('log')
# plt.xlim(100, 86000)
# plt.ylim(1e-10, 1e-6)
plt.show()
# pdf.savefig()

plt.plot(Time_1, LeakRate_1,label='$L = 1.0\\times 10^{-5}$ $m^{3}/s$')
plt.plot(Time_2, LeakRate_2,label='$L = 1.0\\times 10^{-6}$ $m^{3}/s$')
plt.plot(Time_6, LeakRate_6,label='$L = 6.0\\times 10^{-7}$ $m^{3}/s$')
plt.plot(Time_3, LeakRate_3,label='$L = 1.0\\times 10^{-7}$ $m^{3}/s$')
plt.plot(Time_4, LeakRate_4,label='$L = 1.0\\times 10^{-8}$ $m^{3}/s$')
plt.plot(Time_5, LeakRate_5,label='$L = 1.0\\times 10^{-9}$ $m^{3}/s$')
plt.xlabel('Time [s]',fontsize='large')
plt.ylabel(r'Minimum detectable Q$_{SF_6}$ [Pa$\cdot m^{3}/s$]',fontsize='large')
plt.yscale('log')
# axs[0].set_xscale('log')
plt.xlim(100, 86000)
plt.ylim(9e-11, 2e-6)
plt.grid()
plt.legend(fontsize='large', loc='upper right',bbox_to_anchor=(1, 1.05))
plt.show()


plt.plot(Time_1, SF6ToTime_Average_1*5.0,label='$L = 1.0\\times 10^{-5}$ $m^{3}/s$')
plt.plot(Time_2, SF6ToTime_Average_2*5.0,label='$L = 1.0\\times 10^{-6}$ $m^{3}/s$')
plt.plot(Time_6, SF6ToTime_Average_6*5.0,label='$L = 6.0\\times 10^{-7}$ $m^{3}/s$')
plt.plot(Time_3, SF6ToTime_Average_3*5.0,label='$L = 1.0\\times 10^{-7}$ $m^{3}/s$')
plt.plot(Time_4, SF6ToTime_Average_4*5.0,label='$L = 1.0\\times 10^{-8}$ $m^{3}/s$')
plt.plot(Time_5, SF6ToTime_Average_5*5.0,label='$L = 1.0\\times 10^{-9}$ $m^{3}/s$')
# plt.plot(Time_7, SF6ToTime_Average_7,label='$0.7\\times 10^{-7}m^{3}/s$')
# plt.plot(Time_8, SF6ToTime_Average_8,label='$5.7\\times 10^{-7}m^{3}/s$')
plt.xlabel('Time [s]',fontsize='large')
plt.ylabel(r'Concentration [PPM]',fontsize='large')
plt.xlim(100, 86000)
plt.ylim(0,0.41)
plt.grid()
plt.legend(fontsize='large', loc='best')
plt.show()

# 输出5.6小时时LeakRate_2的值
index_56 = np.where(Time_2 == 5.6 * 3600)[0][0]
print("LeakRate_2 at 5.6 hours:", LeakRate_2[index_56])
