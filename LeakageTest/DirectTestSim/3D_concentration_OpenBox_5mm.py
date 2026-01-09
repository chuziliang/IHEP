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
TotalTime = 1000     #一共多少秒
tbin = 1000000          #时间共多少bin
LeakageRate = 1.0E-9    #Pa*m3/s
LengthX = 0.012      #大约是dr的20倍
LengthY = 0.012
LengthZ = 0.012
dr = 0.0005

LeakPointR = 0.001    #计算开始的位置，漏孔的半径，这里为0.5mm
# L = 1.3e-7          # SF6从积分盒内漏到外面的概率，m3/s
# L = float(sys.argv[1])
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
SF6ToTime_z=[]
SF6ToTime_1mm = []
SF6ToTime_2mm = []
SF6ToTime_5mm = []
SF6ToTime_1cm = []
SF6ToTime_2cm = []
SF6ToTime_5cm = []

# 时间迭代
for ti in range(tbin): 
    un = u.copy()
    un = un + u_add

    # 中心区域的差分格式
    InceraseX = D * dt * (np.roll(un, -1, axis=0) - 2 * un + np.roll(un, 1, axis=0)) / dx**2
    InceraseY = D * dt * (np.roll(un, -1, axis=1) - 2 * un + np.roll(un, 1, axis=1)) / dy**2
    InceraseZ = D * dt * (np.roll(un, -1, axis=2) - 2 * un + np.roll(un, 1, axis=2)) / dz**2

    # 边界区域的差分格式，下面是假设开放边界条件时的格式
    InceraseX[0,:,:] = 3*InceraseX[1,:,:] -3*InceraseX[2,:,:] + InceraseX[3,:,:]
    InceraseX[nx-1,:,:] = 3*InceraseX[nx-2,:,:] -3*InceraseX[nx-3,:,:] + InceraseX[nx-4,:,:]

    InceraseY[:,0,:] = 3*InceraseX[:,1,:] -3*InceraseX[:,2,:] + InceraseX[:,3,:]
    InceraseY[:,ny-1,:] = 3*InceraseX[:,ny-2,:] -3*InceraseX[:,ny-3,:] + InceraseX[:,ny-4,:]

    InceraseZ[:,:,0] = 3*InceraseX[:,:,1] -3*InceraseX[:,:,2] + InceraseX[:,:,3]
    InceraseZ[:,:,nz-1] = 3*InceraseX[:,:,nz-2] -3*InceraseX[:,:,nz-3] + InceraseX[:,:,nz-4]

    # 边界区域的差分格式，下面是假设封闭边界条件时的格式
    # InceraseX[0,:,:] = D * dt * ((un[1, :, :] - un[0, :, :])) / dx**2
    # InceraseX[nx-1,:,:] = D * dt * ((un[nx-2, :, :] - un[nx-1, :, :])) / dx**2

    # InceraseY[:,0,:] = D * dt * ((un[:, 1, :] - un[:, 0, :])) / dy**2
    # InceraseY[:,ny-1,:] = D * dt * ((un[:, ny-2, :] - un[:, ny-1, :])) / dy**2

    # InceraseZ[:,:,0] = D * dt * ((un[:, :, 1] - un[:, :, 0])) / dz**2
    # InceraseZ[:,:,nz-1] = D * dt * ((un[:, :, nz-2] - un[:, :, nz-1])) / dz**2

    # 假设有一个小孔漏气的情况
    # Alpha = -un[0,ny,nz]*L/(dy*dz*D)
    # InceraseX[0,ny,nz] = D * dt * ((un[1, ny,nz] - un[0, ny,nz] + Alpha*dx)) / dx**2

    # 边界区域的差分格式，下面是假设有一定的漏率时，边界条件的格式
    # Alpha = -un[0, :, :]*L/(S*D)
    # InceraseX[0,:,:] = D * dt * ((un[1, :, :] - un[0, :, :] + Alpha*dx)) / dx**2
    # Alpha = -un[nx-1,:,:]*L/(S*D)
    # InceraseX[nx-1,:,:] = D * dt * ((un[nx-2, :, :] - un[nx-1, :, :] + Alpha*dx)) / dx**2

    # Alpha = -un[:,0,:]*L/(S*D)
    # InceraseY[:,0,:] = D * dt * ((un[:, 1, :] - un[:, 0, :] + Alpha*dy)) / dy**2
    # Alpha = -un[:,ny-1,:]*L/(S*D)
    # InceraseY[:,ny-1,:] = D * dt * ((un[:, ny-2, :] - un[:, ny-1, :] + Alpha*dy)) / dy**2

    # Alpha = -un[:,:,0]*L/(S*D)
    # InceraseZ[:,:,0] = D * dt * ((un[:, :, 1] - un[:, :, 0] + Alpha*dz)) / dz**2
    # Alpha = -un[:,:,nz-1]*L/(S*D)
    # InceraseZ[:,:,nz-1] = D * dt * ((un[:, :, nz-2] - un[:, :, nz-1] + Alpha*dz)) / dz**2

    u = un + InceraseX + InceraseY + InceraseZ

    # print(ti)

    if (ti%1000 == 0):
        if (ti == 0):
            SF6IncreaseRate.append(0.1*(u.sum()*dx*dy*dz - 0)/dt)
        else:
            SF6IncreaseRate.append(0.0005*0.1*(u.sum()*dx*dy*dz - TotalSF6[-1])/dt)
        TotalSF6.append(u.sum()*dx*dy*dz)
        Time.append(ti*dt)
        SF6ToTime.append(u[:,ny//2,nz//2])
        # SF6ToTime_1mm.append(u[nx//2+int(0.001/dx),ny//2,nz//2])
        SF6ToTime_2mm.append(u[nx//2+int(0.002/dx),ny//2,nz//2])
        SF6ToTime_5mm.append(u[nx//2+int(0.005/dx),ny//2,nz//2])
        # SF6ToTime_1cm.append(u[nx//2+int(0.01/dx),ny//2,nz//2])
        # SF6ToTime_2cm.append(u[nx//2+int(0.02/dx),ny//2,nz//2])
        # SF6ToTime_5cm.append(u[nx//2+int(0.05/dx),ny//2,nz//2])
    if (ti % 100 == 0):
        print(ti)
        # TheoryLeakRate = ((un[0, :, :].sum() + un[nx-1,:,:].sum() + un[:,0,:].sum() + un[:,ny-1,:].sum() + un[:,:,0].sum() + un[:,:,nz-1].sum())/(2*nx*ny+2*nx*nz+2*ny*nz))*0.1*L
        # Tolerance  = (LeakageRate - SF6IncreaseRate[-1]) - TheoryLeakRate
        # TolerancePerent = Tolerance/TheoryLeakRate
        # print("Tolerence = ", Tolerance," ,",TolerancePerent*100,"%")

filename = 'Result/OpenBox_5mm.npz'
SF6Concentration = u[nx//2:nx-1,ny//2,nz//2]
R = x[nx//2:nx-1]
np.savez(filename,TotalSF6=TotalSF6,SF6IncreaseRate=SF6IncreaseRate,\
         SF6ToTime_1mm=SF6ToTime_1mm,SF6ToTime_2mm=SF6ToTime_2mm,SF6ToTime_5mm=SF6ToTime_5mm,SF6ToTime_1cm=SF6ToTime_1cm,SF6ToTime_2cm=SF6ToTime_2cm,SF6ToTime_5cm=SF6ToTime_5cm,\
         R=R,SF6Concentration=SF6Concentration,Time=Time)

filename = 'Result/OpenBox_5mm.pdf'
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

    # norm = mcolors.LogNorm(vmin=max(np.min(SF6ToTime_z),1e-5) , vmax=np.max(SF6ToTime_z))
    # plt.pcolor(Time,z,np.transpose(SF6ToTime_z), cmap='coolwarm')
    # cbar = plt.colorbar()
    # cbar.set_label('PPM')
    # plt.xlabel("Time/s")
    # plt.ylabel("R/m")
    # # fig = plt.figure()
    # # ax = fig.add_subplot(111, projection='3d')
    # # ax.plot_surface(X, Y, Z, u[:,:,int(nz/2)], cmap='viridis')
    # pdf.savefig()
    # plt.clf()

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

    # plt.plot(Time, SF6ToTime_1mm)
    # plt.title('$SF_{6}$ concentration at 1mm/ppm')
    # plt.xlabel('Time/s')
    # plt.ylabel('$SF_{6}$')
    # plt.grid()
    # pdf.savefig()
    # plt.clf()

    plt.plot(Time, SF6ToTime_2mm)
    plt.title('$SF_{6}$ concentration at 2mm/ppm')
    plt.xlabel('Time/s')
    plt.ylabel('$SF_{6}$')
    plt.grid()
    pdf.savefig()
    plt.clf()

    plt.plot(Time, SF6ToTime_5mm)
    plt.title('$SF_{6}$ concentration at 5mm/ppm')
    plt.xlabel('Time/s')
    plt.ylabel('$SF_{6}$')
    plt.grid()
    pdf.savefig()
    plt.clf()

    # plt.plot(Time, SF6ToTime_1cm)
    # plt.title('$SF_{6}$ concentration at 1cm/ppm')
    # plt.xlabel('Time/s')
    # plt.ylabel('$SF_{6}$')
    # plt.grid()
    # pdf.savefig()
    # plt.clf()

    # plt.plot(Time, SF6ToTime_2cm)
    # plt.title('$SF_{6}$ concentration at 2cm/ppm')
    # plt.xlabel('Time/s')
    # plt.ylabel('$SF_{6}$')
    # plt.grid()
    # pdf.savefig()
    # plt.clf()

    # plt.plot(Time, SF6ToTime_5cm)
    # plt.title('$SF_{6}$ concentration at 5cm/ppm')
    # plt.xlabel('Time/s')
    # plt.ylabel('$SF_{6}$')
    # plt.grid()
    # pdf.savefig()
    # plt.clf()