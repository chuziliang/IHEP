import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.colors as mcolors
from matplotlib.backends.backend_pdf import PdfPages
import sys

def merge_data_with_max_y(R1,R2,SF6_1,SF6_2,Rmin,Rstep):
    # 合并后的数据x的取值范围从0.1到5
    x_values = np.arange(Rmin, 0.05, Rstep)

    # 指针分别指向两组数据当前处理的位置
    index1 = 0
    index2 = 0

    for x in x_values:
        y1 = None
        y2 = None

        # 查找第一组数据中对应x的y值（如果存在）
        while index1 < len(R1) and R1[index1] < x:
            index1 += 1
        y1 = SF6_1[index1]

        # 查找第二组数据中对应x的y值（如果存在）
        while index2 < len(R2) and R2[index2] < x:
            index2 += 1
        y2 = SF6_2[index2]

        if (y2 >= y1) or (abs(y2 -y1)/y1 < 0.1):
            break

    return index1,index2

# 读取保存的npz文件
data_1 = np.load('OpenBox_2mm.npz')
data_2 = np.load('OpenBox_5mm.npz')
data_3 = np.load('OpenBox_1cm.npz')
data_4 = np.load('OpenBox_2cm.npz')
data_5 = np.load('OpenBox_5cm.npz')

# 提取array1和array2
R_1 = data_1['R']
SF6ToTime_Average_1 = data_1['SF6Concentration']
SF6ToTime_2mm = data_1['SF6ToTime_2mm']
Time_1 = data_1['Time']

R_2 = data_2['R']
SF6ToTime_Average_2 = data_2['SF6Concentration']
SF6ToTime_5mm = data_2['SF6ToTime_5mm']
Time_2 = data_2['Time']

R_3 = data_3['R']
SF6ToTime_Average_3 = data_3['SF6Concentration']
SF6ToTime_1cm = data_3['SF6ToTime_1cm']
Time_3 = data_3['Time']

R_4 = data_4['R']
SF6ToTime_Average_4 = data_4['SF6Concentration']
SF6ToTime_2cm = data_4['SF6ToTime_2cm']
Time_4 = data_4['Time']

R_5 = data_5['R']
SF6ToTime_Average_5 = data_5['SF6Concentration']
SF6ToTime_5cm = data_5['SF6ToTime_5cm']
Time_5 = data_5['Time']
# Time_6 = data_1['Time']
# SF6ToTime_Average_6 = data_1['SF6ToTime_1mm']

index1_Stop,index2_Start = merge_data_with_max_y(R_1,R_2,SF6ToTime_Average_1,SF6ToTime_Average_2,0.00025,0.00025)
print(index1_Stop,index2_Start)
index2_Stop,index3_Start = merge_data_with_max_y(R_2,R_3,SF6ToTime_Average_2,SF6ToTime_Average_3,R_2[index2_Start],0.0005)
index3_Stop,index4_Start = merge_data_with_max_y(R_3,R_4,SF6ToTime_Average_3,SF6ToTime_Average_4,R_3[index3_Start],0.00125)
index4_Stop,index5_Start = merge_data_with_max_y(R_4,R_5,SF6ToTime_Average_4,SF6ToTime_Average_5,R_4[index4_Start],0.0025)

combine_r = []
combine_SF6 = []
combine_r.extend(R_1[:index1_Stop+2])
combine_r.extend(R_2[index2_Start+1:index2_Stop])
combine_r.extend(R_3[index3_Start:index3_Stop])
combine_r.extend(R_4[index4_Start:index4_Stop])
combine_r.extend(R_5[index5_Start:])

combine_SF6.extend(SF6ToTime_Average_1[:index1_Stop+2])
combine_SF6.extend(SF6ToTime_Average_2[index2_Start+1:index2_Stop])
combine_SF6.extend(SF6ToTime_Average_3[index3_Start:index3_Stop])
combine_SF6.extend(SF6ToTime_Average_4[index4_Start:index4_Stop])
combine_SF6.extend(SF6ToTime_Average_5[index5_Start:])



# combine_r = np.append(R_1[:index1_Stop],R_2[index2_Start:index2_Stop] + R_3[index3_Start:index3_Stop] + R_4[index4_Start:index4_Stop] + R_5[index5_Start:]
# combine_SF6 = SF6ToTime_Average_1[:index1_Stop] + SF6ToTime_Average_2[index2_Start:index2_Stop] + SF6ToTime_Average_3[index3_Start:index3_Stop] + SF6ToTime_Average_4[index4_Start:index4_Stop] + SF6ToTime_Average_5[index5_Start:]

# with PdfPages('combine.pdf') as pdf:
plt.figure(figsize=(8, 6))
# plt.plot(R_1, SF6ToTime_Average_6,label='$SF_6$ at 1mm')
# plt.plot(R_1, SF6ToTime_Average_1,label='$SF_6$ at 2mm')
# plt.plot(R_2, SF6ToTime_Average_2,label='$SF_6$ at 5mm')
# plt.plot(R_3, SF6ToTime_Average_3,label='$SF_6$ at 1cm')
# plt.plot(R_4, SF6ToTime_Average_4,label='$SF_6$ at 2cm')
# plt.plot(R_5, SF6ToTime_Average_5,label='$SF_6$ at 5cm')
plt.plot(combine_r, combine_SF6,label='combine')


plt.legend(fontsize='large', loc='upper right')
plt.title('')
plt.xlabel('R/m')
plt.ylabel('Concentration/ppm')
plt.grid()
plt.xlim(5e-4, 0.05)
ax = plt.gca()
# ax.set_xscale('log')
# ax.set_yscale('log')
plt.show()
# pdf.savefig()

plt.plot(Time_1, SF6ToTime_2mm,label='$SF_6$ at 2 mm')
plt.plot(Time_2, SF6ToTime_5mm,label='$SF_6$ at 5 mm')
plt.plot(Time_3, SF6ToTime_1cm,label='$SF_6$ at 1 cm')
plt.plot(Time_4, SF6ToTime_2cm,label='$SF_6$ at 2 cm')
plt.plot(Time_5, SF6ToTime_5cm,label='$SF_6$ at 5 cm')
plt.legend(fontsize='large', loc='upper right')
plt.title('')
plt.xlabel('Time [s]',fontsize='x-large')
plt.ylabel('Concentration [PPM]',fontsize='x-large')
ax = plt.gca()
ax.set_xscale('log')
ax.set_yscale('log')
plt.xlim(0, 500)


plt.show()