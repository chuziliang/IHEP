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
X, Y, Z = np.meshgrid(x, y, z)

# 读取保存的npz文件
data_1 = np.load('Calibration_SmallPoint.npz')

# 提取array1和array2
Concentration = data_1['Concentration']

# with PdfPages('combine.pdf') as pdf:
norm = mcolors.LogNorm(vmin=max(Concentration.min(),1e-5) , vmax=Concentration.max())
plt.pcolor(x,y,Concentration, cmap='coolwarm' )
cbar = plt.colorbar()
cbar.set_label('PPM',fontsize='x-large')
plt.xlabel("x/m",fontsize='x-large')
plt.ylabel("y/m",fontsize='x-large')
plt.show()
# pdf.savefig()