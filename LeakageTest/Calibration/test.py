import numpy as np

L_V = [6.146e-6, 1.433e-5, 1.631e-5, 3.645e-5, 5.162e-5]
L = []
for i,k in enumerate(L_V):
    L.append(k*(1.1e-2))

print(L)
print(np.mean(L))
print(np.std(L))