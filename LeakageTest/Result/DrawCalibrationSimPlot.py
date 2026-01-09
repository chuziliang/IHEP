import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.colors as mcolors
from matplotlib.backends.backend_pdf import PdfPages
import sys

D = 0.9475e-5        #扩散系数，单位m2/s
TotalTime = 54000     #一共多少秒
tbin = 540000          #时间共多少bin
LeakageRate = 1.0E-9    #Pa*m3/s
LengthX = 0.35
LengthY = 0.35
LengthZ = 0.09
dr = 0.005

LeakPointR = 0.01    #计算开始的位置，漏孔的半径，这里为1mm
L = 1.3e-7          # SF6从积分盒内漏到外面的概率，m3/s
S = 2*(LengthX*LengthY + LengthX*LengthZ + LengthY*LengthZ)     # 积分盒的表面积

dx = dr
dy = dr
dz = dr
dt = TotalTime/tbin

x = np.arange(-LengthX/2, LengthX/2, dx)
y = np.arange(-LengthY/2, LengthY/2, dy)
z = np.arange(-LengthZ/2, LengthZ/2, dz)
X, Y = np.meshgrid(x, y)

# 读取保存的npz文件
data_1 = np.load('G:\LeakageTest\Calibration.npz')

# 提取array1和array2
Concentration = data_1['Concentration']

# with PdfPages('combine.pdf') as pdf:
norm = mcolors.LogNorm(vmin=max(Concentration.min(),1e-5) , vmax=Concentration.max())
# 输出最大和最小值，要求距离中心5cm以上
# 输出最大和最小值，要求距离中心5cm以上
# distance_from_center = np.sqrt(X**2 + Y**2)  # 计算每个点到中心的距离
# mask = distance_from_center > 0.05  # 创建距离筛选掩码（5cm = 0.05m）
# valid_values = Concentration[mask]  # 获取距离中心5cm以上的浓度值

# if valid_values.size > 0:
#     print(f"距离中心5cm以上区域的最大浓度值: {valid_values.max():.2e} PPM")
#     print(f"距离中心5cm以上区域的最小浓度值: {valid_values.min():.2e} PPM")
# else:
#     print("警告：没有找到距离中心5cm以上的有效数据点")

distance_from_center = np.sqrt(X**2 + Y**2)
mask = distance_from_center >= 0.05
valid_values = Concentration[mask]

if valid_values.size > 0:
    # 获取有效区域的坐标索引
    rows, cols = np.where(mask)
    max_idx = np.argmax(valid_values)
    min_idx = np.argmin(valid_values)
    
    # 获取最大值的坐标（转换为实际距离）
    x_max = x[cols[max_idx]]
    y_max = y[rows[max_idx]]
    # 获取最小值的坐标
    x_min = x[cols[min_idx]]
    y_min = y[rows[min_idx]]
    
    print(f"距离中心5cm以上区域的最大浓度值: {valid_values.max():.2e} PPM 位置: (x={x_max:.3f}m, y={y_max:.3f}m)")
    print(f"距离中心5cm以上区域的最小浓度值: {valid_values.min():.2e} PPM 位置: (x={x_min:.3f}m, y={y_min:.3f}m)")
    print(f"ratio: {(valid_values.max()-valid_values.min())/(0.5*(valid_values.max()+valid_values.min()))}")
else:
    print("警告：没有找到距离中心5cm以上的有效数据点")


plt.pcolor(x,y,Concentration, cmap='coolwarm' )
cbar = plt.colorbar()
cbar.set_label('Concentration [PPM]',fontsize='x-large')
plt.xlabel("x [m]",fontsize='x-large')
plt.ylabel("y [m]",fontsize='x-large')
plt.show()
# pdf.savefig()