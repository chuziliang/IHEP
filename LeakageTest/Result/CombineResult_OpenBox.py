import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.colors as mcolors
from matplotlib.backends.backend_pdf import PdfPages
import sys

# 读取保存的npz文件
data_1 = np.load('OpenBox_2mm.npz')
data_2 = np.load('OpenBox_5mm.npz')
data_3 = np.load('OpenBox_1cm.npz')
data_4 = np.load('OpenBox_2cm.npz')
data_5 = np.load('OpenBox_5cm.npz')

# 提取array1和array2
Time_1 = data_1['Time']
SF6ToTime_Average_1 = data_1['SF6ToTime_2mm']
Time_2 = data_2['Time']
SF6ToTime_Average_2 = data_2['SF6ToTime_5mm']
Time_3 = data_3['Time']
SF6ToTime_Average_3 = data_3['SF6ToTime_1cm']
Time_4 = data_3['Time']
SF6ToTime_Average_4 = data_4['SF6ToTime_2cm']
Time_5 = data_5['Time']
SF6ToTime_Average_5 = data_5['SF6ToTime_5cm']
# Time_6 = data_1['Time']
SF6ToTime_Average_6 = data_1['SF6ToTime_1mm']


# with PdfPages('combine.pdf') as pdf:
plt.figure(figsize=(8, 6))
plt.plot(Time_1, SF6ToTime_Average_6,label='$SF_6$ at 1mm')
plt.plot(Time_1, SF6ToTime_Average_1,label='$SF_6$ at 2mm')
plt.plot(Time_2, SF6ToTime_Average_2,label='$SF_6$ at 5mm')
plt.plot(Time_3, SF6ToTime_Average_3,label='$SF_6$ at 1cm')
plt.plot(Time_4, SF6ToTime_Average_4,label='$SF_6$ at 2cm')
plt.plot(Time_5, SF6ToTime_Average_5,label='$SF_6$ at 5cm')


plt.legend(fontsize='large', loc='upper right')
plt.title('')
plt.xlabel('Time/s')
plt.ylabel('Concentration/ppm')
plt.grid()
plt.xlim(0.0001, 1000)
ax = plt.gca()
# ax.set_xscale('log')
plt.show()
# pdf.savefig()