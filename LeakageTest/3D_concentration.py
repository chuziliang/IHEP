import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

def InLeakPoint(i,j,k):
    if ((abs(i*dx) < LeakPointR) and (abs(j*dx) < LeakPointR) and (abs(k*dx) < LeakPointR)):
        return True
    else:
        return False

# 定义扩散方程的参数
D = 0.9475e-5        #扩散系数，单位m3/s
TotalTime = 86400     #一共多少秒
tbin = 864000          #时间共多少bin
LeakageRate = 1.0    #Pa*m3/s
Length = 0.1         #一共多少m，+1mm
rbin = 100           #长度共多少bin
LeakPointR = 0.01    #计算开始的位置，漏孔的半径，这里为1mm

dx = Length/rbin
dy = Length/rbin
dz = Length/rbin
dt = TotalTime/tbin

# 定义空间网格
nx = rbin
ny = rbin
nz = rbin
x = np.arange(-Length, Length, 2*dx)
y = np.arange(-Length, Length, 2*dy)
z = np.arange(-Length, Length, 2*dz)
X, Y, Z = np.meshgrid(x, y, z)

# 初始化浓度场
u = np.zeros((2*nx, 2*ny, 2*nz))
# u[nx // 2, ny // 2, nz // 2] = 1.0

# 时间迭代
for ti in range(tbin): 
    un = u.copy()
    for i in range(nx-int(LeakPointR/dx), nx+int(LeakPointR/dx)):
        for j in range(ny-int(LeakPointR/dy), ny+int(LeakPointR/dy)):
            for k in range(nz-int(LeakPointR/dz), nz+int(LeakPointR/dz)):
                un[i,j,k] += LeakageRate*dt*0.1/LeakPointR**3
    for i in range(0, 2*nx):
        for j in range(0, 2*ny):
            for k in range(0, 2*nz):
                # if (InLeakPoint(i,j,k)): #如果在漏点内，不需要考虑
                #     continue
                
                # if (i==-nx or InLeakPoint(i-1,j,k)): # 判断x是否向前差分
                #     InceraseX = D * dt * ((-un[i + 3, j, k]+4.0*un[i + 2, j, k]-5.0*un[i + 1, j, k]+2.0*un[i, j, k])) / dx**2
                # elif (i==nx or InLeakPoint(i+1,j,k)): #判断x是否向后差分
                #     InceraseX = D * dt * ((-un[i - 3, j, k]+4.0*un[i - 2, j, k]-5.0*un[i - 1, j, k]+2.0*un[i, j, k])) / dx**2
                # else:
                #     InceraseX = D * dt * (un[i + 1, j, k] - 2 * un[i, j, k] + un[i - 1, j, k]) / dx**2
                
                # if (j==-ny or InLeakPoint(i,j-1,k)): # 判断y是否向前差分
                #     InceraseY = D * dt * ((-un[i, j + 3, k]+4.0*un[i, j + 2, k]-5.0*un[i, j + 1, k]+2.0*un[i, j, k])) / dy**2
                # elif (j==ny or InLeakPoint(i,j+1,k)): #判断y是否向后差分
                #     InceraseY = D * dt * ((-un[i, j - 3, k]+4.0*un[i, j - 2, k]-5.0*un[i, j - 1, k]+2.0*un[i, j, k])) / dy**2
                # else:
                #     InceraseY = D * dt * (un[i, j + 1, k] - 2 * un[i, j, k] + un[i, j - 1, k]) / dy**2
                
                # if (k==-nz or InLeakPoint(i,j,k-1)): # 判断z是否向前差分
                #     InceraseZ = D * dt * ((-un[i, j, k + 3]+4.0*un[i, j, k + 2]-5.0*un[i, j, k + 1]+2.0*un[i, j, k])) / dz**2
                # elif (i==nz or InLeakPoint(i,j,k+1)): #判断z是否向后差分
                #     InceraseZ = D * dt * ((-un[i, j, k - 3]+4.0*un[i, j, k - 2]-5.0*un[i, j, k - 1]+2.0*un[i, j, k])) / dz**2
                # else:
                #     InceraseZ = D * dt * (un[i, j, k + 1] - 2 * un[i, j, k] + un[i, j, k - 1]) / dz**2

                if (i==0): # 判断x是否向前差分
                    InceraseX = D * dt * ((-un[i + 3, j, k]+4.0*un[i + 2, j, k]-5.0*un[i + 1, j, k]+2.0*un[i, j, k])) / dx**2
                elif (i==2*nx-1): #判断x是否向后差分
                    InceraseX = D * dt * ((-un[i - 3, j, k]+4.0*un[i - 2, j, k]-5.0*un[i - 1, j, k]+2.0*un[i, j, k])) / dx**2
                else:
                    InceraseX = D * dt * (un[i + 1, j, k] - 2 * un[i, j, k] + un[i - 1, j, k]) / dx**2
                
                if (j==0): # 判断y是否向前差分
                    InceraseY = D * dt * ((-un[i, j + 3, k]+4.0*un[i, j + 2, k]-5.0*un[i, j + 1, k]+2.0*un[i, j, k])) / dy**2
                elif (j==2*ny-1): #判断y是否向后差分
                    InceraseY = D * dt * ((-un[i, j - 3, k]+4.0*un[i, j - 2, k]-5.0*un[i, j - 1, k]+2.0*un[i, j, k])) / dy**2
                else:
                    InceraseY = D * dt * (un[i, j + 1, k] - 2 * un[i, j, k] + un[i, j - 1, k]) / dy**2
                
                if (k==0): # 判断z是否向前差分
                    InceraseZ = D * dt * ((-un[i, j, k + 3]+4.0*un[i, j, k + 2]-5.0*un[i, j, k + 1]+2.0*un[i, j, k])) / dz**2
                elif (k==2*nz-1): #判断z是否向后差分
                    InceraseZ = D * dt * ((-un[i, j, k - 3]+4.0*un[i, j, k - 2]-5.0*un[i, j, k - 1]+2.0*un[i, j, k])) / dz**2
                else:
                    InceraseZ = D * dt * (un[i, j, k + 1] - 2 * un[i, j, k] + un[i, j, k - 1]) / dz**2
                
                u[i, j, k] = un[i, j, k] + InceraseX + InceraseY + InceraseZ
    print(ti)

# 绘制三维浓度场
plt.pcolor(u[:,:,nz], cmap='coolwarm')
# fig = plt.figure()
# ax = fig.add_subplot(111, projection='3d')
# ax.plot_surface(X, Y, Z, u[:,:,int(nz/2)], cmap='viridis')
plt.show()
print(u[nx,ny,nz])