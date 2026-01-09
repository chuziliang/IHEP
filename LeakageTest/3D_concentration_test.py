import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.colors as mcolors
from matplotlib.backends.backend_pdf import PdfPages
import sys


def InLeakPoint(i,j,k):
    if ((abs(i*dx) < LeakPointR) and (abs(j*dx) < LeakPointR) and (abs(k*dx) < LeakPointR)):
        return True
    else:
        return False

# 定义扩散方程的参数
D = 0.9475e-5        #扩散系数，单位m2/s
TotalTime = 54000     #一共多少秒
tbin = 540000          #时间共多少bin
LeakageRate = 1.0E-9    #Pa*m3/s
LengthX = 0.35
LengthY = 0.35
LengthZ = 0.09
dr = 0.005

LeakPointR = 0.01    #计算开始的位置，漏孔的半径，这里为1mm
# L = 1.3e-7          # SF6从积分盒内漏到外面的概率，m3/s
L = float(sys.argv[1])
S = 2*(LengthX*LengthY + LengthX*LengthZ + LengthY*LengthZ)     # 积分盒的表面积

dx = dr
dy = dr
dz = dr
dt = TotalTime/tbin

print("rd = ",D*dt/dx**2) # 似乎0.1以下比较稳定
# 定义空间网格
nx = int(LengthX/dx)
ny = int(LengthY/dy)
nz = int(LengthZ/dz)
x = np.arange(-LengthX/2, LengthX/2, dx)
y = np.arange(-LengthY/2, LengthY/2, dy)
z = np.arange(-LengthZ/2, LengthZ/2, dz)
X, Y, Z = np.meshgrid(x, y, z)

# 初始化浓度场
# u = np.zeros((2*nx, 2*ny, 2*nz))
u = np.full((nx, ny, nz),0)

u_add = np.zeros((nx, ny, nz))
for i in range(nx//2-int(LeakPointR/dx), nx//2+int(LeakPointR/dx)):
    for j in range(ny//2-int(LeakPointR/dy), ny//2+int(LeakPointR/dy)):
        for k in range(nz//2-int(LeakPointR/dz), nz//2+int(LeakPointR/dz)):
            u_add[i,j,k] = LeakageRate*dt*10.0/(2*LeakPointR)**3
# u[nx // 2, ny // 2, nz // 2] = 1.0

TotalSF6=[]
SF6IncreaseRate=[]
Time=[]
SF6ToTime =[]
SF6ToTime_Center = []
SF6ToTime_Corner = []
SF6ToTime_Quarter = []
SF6ToTime_Average = []

# 时间迭代
ti=0
while ti < tbin: 
    un = u.copy()
    un = un + u_add

    # 中心区域的差分格式
    InceraseX = D * dt * (np.roll(un, -1, axis=0) - 2 * un + np.roll(un, 1, axis=0)) / dx**2
    InceraseY = D * dt * (np.roll(un, -1, axis=1) - 2 * un + np.roll(un, 1, axis=1)) / dy**2
    InceraseZ = D * dt * (np.roll(un, -1, axis=2) - 2 * un + np.roll(un, 1, axis=2)) / dz**2

    # 边界区域的差分格式，下面是假设开放边界条件时的格式
    # InceraseX[0,:,:] = D * dt * ((-un[3, :, :]+4.0*un[2, :, :]-5.0*un[1, :, :]+2.0*un[0, :, :])) / dx**2
    # InceraseX[2*nx-1,:,:] = D * dt * ((-un[2*nx-4, :, :]+4.0*un[2*nx-3, :, :]-5.0*un[2*nx-2, :, :]+2.0*un[2*nx-1, :, :])) / dx**2

    # InceraseY[:,0,:] = D * dt * ((-un[:, 3, :]+4.0*un[:, 2, :]-5.0*un[:, 1, :]+2.0*un[:, 0, :])) / dy**2
    # InceraseY[:,2*ny-1,:] = D * dt * ((-un[:, 2*ny-4, :]+4.0*un[:, 2*ny-3, :]-5.0*un[:, 2*ny-2, :]+2.0*un[:, 2*ny-1, :])) / dy**2

    # InceraseZ[:,:,0] = D * dt * ((-un[:, :, 3]+4.0*un[:, :, 2]-5.0*un[:, :, 1]+2.0*un[:, :, 0])) / dz**2
    # InceraseZ[:,:,2*nz-1] = D * dt * ((-un[:, :, 2*ny-4]+4.0*un[:, :, 2*ny-3]-5.0*un[:, :, 2*ny-2]+2.0*un[:, :, 2*ny-1])) / dz**2

    # 边界区域的差分格式，下面是假设封闭边界条件时的格式
    # InceraseX[0,:,:] = D * dt * ((un[1, :, :] - un[0, :, :])) / dx**2
    # InceraseX[nx-1,:,:] = D * dt * ((un[nx-2, :, :] - un[nx-1, :, :])) / dx**2

    # InceraseY[:,0,:] = D * dt * ((un[:, 1, :] - un[:, 0, :])) / dy**2
    # InceraseY[:,ny-1,:] = D * dt * ((un[:, ny-2, :] - un[:, ny-1, :])) / dy**2

    # InceraseZ[:,:,0] = D * dt * ((un[:, :, 1] - un[:, :, 0])) / dz**2
    # InceraseZ[:,:,nz-1] = D * dt * ((un[:, :, nz-2] - un[:, :, nz-1])) / dz**2

    # 假设有一个小孔漏气的情况
    # Alpha = -un[nx*3//4,ny*3//4,nz-1]*L/(dy*dx*D)
    # InceraseZ[nx*3//4,ny*3//4,nz-1] = D * dt * ((un[nx*3//4,ny*3//4,nz-2] - un[nx*3//4,ny*3//4,nz-1] + Alpha*dz)) / dz**2

    # 边界区域的差分格式，下面是假设有一定的漏率时，边界条件的格式
    Alpha = -un[0, :, :]*L/(S*D)
    InceraseX[0,:,:] = D * dt * ((un[1, :, :] - un[0, :, :] + Alpha*dx)) / dx**2
    Alpha = -un[nx-1,:,:]*L/(S*D)
    InceraseX[nx-1,:,:] = D * dt * ((un[nx-2, :, :] - un[nx-1, :, :] + Alpha*dx)) / dx**2

    Alpha = -un[:,0,:]*L/(S*D)
    InceraseY[:,0,:] = D * dt * ((un[:, 1, :] - un[:, 0, :] + Alpha*dy)) / dy**2
    Alpha = -un[:,ny-1,:]*L/(S*D)
    InceraseY[:,ny-1,:] = D * dt * ((un[:, ny-2, :] - un[:, ny-1, :] + Alpha*dy)) / dy**2

    Alpha = -un[:,:,0]*L/(S*D)
    InceraseZ[:,:,0] = D * dt * ((un[:, :, 1] - un[:, :, 0] + Alpha*dz)) / dz**2
    Alpha = -un[:,:,nz-1]*L/(S*D)
    InceraseZ[:,:,nz-1] = D * dt * ((un[:, :, nz-2] - un[:, :, nz-1] + Alpha*dz)) / dz**2

    u = un + InceraseX + InceraseY + InceraseZ

    if (ti%100 == 0):
        if (ti == 0):
            SF6IncreaseRate.append(0.1*(u.sum()*dx*dy*dz - 0)/dt)
        else:
            SF6IncreaseRate.append(0.01*0.1*(u.sum()*dx*dy*dz - TotalSF6[-1])/dt)
        TotalSF6.append(u.sum()*dx*dy*dz)
        Time.append(ti*dt)
        SF6ToTime.append(u[:,ny//2,nz//2])
        SF6ToTime_Center.append(u[nx//2,ny//2,nz//2])
        SF6ToTime_Corner.append(u[0,ny//2,nz//2])
        SF6ToTime_Quarter.append(u[nx//4,ny//2,nz//2])
        SF6ToTime_Average.append(np.mean(u))
    if (ti % 100 == 0):
        print(ti)
        # TheoryLeakRate = ((un[0, :, :].sum() + un[nx-1,:,:].sum() + un[:,0,:].sum() + un[:,ny-1,:].sum() + un[:,:,0].sum() + un[:,:,nz-1].sum())/(2*nx*ny+2*nx*nz+2*ny*nz))*0.1*L
        # Tolerance  = (LeakageRate - SF6IncreaseRate[-1]) - TheoryLeakRate
        # TolerancePerent = Tolerance/TheoryLeakRate
        # print("Tolerence = ", Tolerance," ,",TolerancePerent*100,"%")

    ti += 1

filename = './Result/LeakBox_15h'+sys.argv[1]+'.npz'
np.savez(filename,Concentration=u[:,:,nz//2],TotalSF6=TotalSF6,SF6IncreaseRate=SF6IncreaseRate,SF6ToTime_Center=SF6ToTime_Center,SF6ToTime_Corner=SF6ToTime_Corner,SF6ToTime_Quarter=SF6ToTime_Quarter,SF6ToTime_Average=SF6ToTime_Average,Time=Time)

filename = './Result/LeakBox_15h'+sys.argv[1]+'.pdf'
with PdfPages(filename) as pdf:
    # 绘制三维浓度场
    norm = mcolors.LogNorm(vmin=max(u[:,:,nz//2].min(),1e-5) , vmax=u[:,:,nz//2].max())
    plt.pcolor(x,y,u[:,:,nz//2], cmap='coolwarm' )
    cbar = plt.colorbar()
    cbar.set_label('PPM')
    plt.xlabel("x/m")
    plt.ylabel("y/m")
    # fig = plt.figure()
    # ax = fig.add_subplot(111, projection='3d')
    # ax.plot_surface(X, Y, Z, u[:,:,int(nz/2)], cmap='viridis')
    pdf.savefig()
    plt.clf()

    norm = mcolors.LogNorm(vmin=max(np.min(SF6ToTime),1e-5) , vmax=np.max(SF6ToTime))
    plt.pcolor(Time,x,np.transpose(SF6ToTime), cmap='coolwarm')
    cbar = plt.colorbar()
    cbar.set_label('PPM')
    plt.xlabel("Time/s")
    plt.ylabel("R/m")
    # fig = plt.figure()
    # ax = fig.add_subplot(111, projection='3d')
    # ax.plot_surface(X, Y, Z, u[:,:,int(nz/2)], cmap='viridis')
    pdf.savefig()
    plt.clf()

    plt.plot(Time, TotalSF6)
    # plt.title('多折线图示例')
    plt.xlabel('Time/s')
    plt.ylabel('$SF_{6}$')
    pdf.savefig()
    plt.clf()

    plt.plot(Time, SF6IncreaseRate)
    # plt.title('多折线图示例')
    plt.xlabel('Time/s')
    plt.ylabel('$SF_{6}$ Increase rate')
    plt.ylim([0, 2e-9])
    pdf.savefig()
    plt.clf()
    print(u[nx//2,ny//2,nz//2])

    plt.plot(Time, SF6ToTime_Center)
    plt.title('$SF_{6}$ concentration at center/ppm')
    plt.xlabel('Time/s')
    plt.ylabel('$SF_{6}$')
    plt.grid()
    pdf.savefig()
    plt.clf()

    plt.plot(Time, SF6ToTime_Corner)
    plt.title('$SF_{6}$ concentration at corner/ppm')
    plt.xlabel('Time/s')
    plt.ylabel('$SF_{6}$')
    plt.grid()
    pdf.savefig()
    plt.clf()

    plt.plot(Time, SF6ToTime_Quarter)
    plt.title('$SF_{6}$ concentration at quarter/ppm')
    plt.xlabel('Time/s')
    plt.ylabel('$SF_{6}$')
    plt.grid()
    pdf.savefig()
    plt.clf()

    plt.plot(Time, SF6ToTime_Average)
    plt.title('$Average SF_{6}$ concentration/ppm')
    plt.xlabel('Time/s')
    plt.ylabel('$SF_{6}$')
    plt.grid()
    pdf.savefig()
    plt.clf()