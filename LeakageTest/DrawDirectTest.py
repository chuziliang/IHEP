import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.colors as mcolors
from matplotlib.backends.backend_pdf import PdfPages
import sys

Pi = 3.141592653589793
D = 9.48e-6
Q = 1.0e-9

Length = np.linspace(0.002, 0.006, 10000)
Concentration = 10*Q/(4*Pi*D*Length)
Sensitivity = (4*Pi*D*Length)/10

# with PdfPages('combine.pdf') as pdf:
plt.figure(figsize=(8, 6))
plt.plot(Length, Concentration)

# plt.legend(fontsize='large', loc='best')
plt.title('')
plt.xlabel('Length/m',fontsize='x-large')
plt.ylabel('Concentration/ppm',fontsize='x-large')
ax = plt.gca()
# ax.set_xscale('log')
# ax.set_yscale('log')
plt.grid()
plt.show()
# pdf.savefig()