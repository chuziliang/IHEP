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
data_7 = np.load('LeakBox0.7e-7.npz')
data_8 = np.load('LeakBox5.7e-7.npz')
data_9 = np.load('LeakBox5.3e-7.npz')
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
Time_7 = data_7['Time']
SF6ToTime_Average_7 = data_7['SF6ToTime_Average']
Time_8 = data_8['Time']
SF6ToTime_Average_8 = data_8['SF6ToTime_Average']
Time_9 = data_9['Time']
SF6ToTime_Average_9 = data_9['SF6ToTime_Average']

# with PdfPages('combine.pdf') as pdf:
plt.figure(figsize=(8, 6))
# plt.plot(Time_1, SF6ToTime_Average_1,label='$1.0\\times 10^{-5}m^{3}/s$')
plt.plot(Time_2, SF6ToTime_Average_2,label='$1.0\\times 10^{-6}m^{3}/s$')
# plt.plot(Time_6, SF6ToTime_Average_6,label='$6.0\\times 10^{-7}m^{3}/s$')
# plt.plot(Time_3, SF6ToTime_Average_3,label='$1.0\\times 10^{-7}m^{3}/s$')
# plt.plot(Time_4, SF6ToTime_Average_4,label='$1.0\\times 10^{-8}m^{3}/s$')
# plt.plot(Time_5, SF6ToTime_Average_5,label='$1.0\\times 10^{-9}m^{3}/s$')
plt.plot(Time_7, SF6ToTime_Average_7,label='$0.7\\times 10^{-7}m^{3}/s$')    
# plt.plot(Time_8, SF6ToTime_Average_8,label='$5.7\\times 10^{-7}m^{3}/s$')
plt.plot(Time_9, SF6ToTime_Average_9,label='$5.3\\times 10^{-7}m^{3}/s$')

plt.legend(fontsize='large', loc='best')
plt.title('')
plt.xlabel('Time/s',fontsize='x-large')
plt.ylabel('Concentration/ppm',fontsize='x-large')
plt.grid()
plt.show()
# pdf.savefig()