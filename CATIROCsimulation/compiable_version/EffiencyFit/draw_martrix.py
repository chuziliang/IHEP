import numpy as np
import matplotlib
import matplotlib as mpl
import matplotlib.pyplot as plt

filename1 = "/junofs/users/chuziliang125/CATIROCsimulation/compiable_version/EffiencyFit/SSHspectrum_eff.txt"
TrueEff = range(20)
with open(filename1, 'r') as f1:
    f1.readlines()
    # 到这了


neutrino_energies = [6.25, 6.75, 7.25, 7.75, 8.25, 8.75, 9.25, 9.75, 10.25, 10.75, 11.5] 
prompt_energies = [6.125, 6.375, 6.625, 6.875, 7.25, 7.75, 8.25, 8.75, 9.25, 9.75, 10.5]

filename2 = "/junofs/users/chuziliang125/CATIROCsimulation/compiable_version/EffiencyFit/response_matrix.txt"
response_matrix = []
with open(filename2, 'r') as f:#1
    lines1 = f.readlines()[7:]
    for line1 in lines1:
        value1 = [float(s) for s in line1.split()]
        response_matrix.append(value1)
response_matrix = np.array(response_matrix)

# harvest = np.array([[0.8, 2.4, 2.5, 3.9, 0.0, 4.0, 0.0],
#                     [2.4, 0.0, 4.0, 1.0, 2.7, 0.0, 0.0],
#                     [1.1, 2.4, 0.8, 4.3, 1.9, 4.4, 0.0],
#                     [0.6, 0.0, 0.3, 0.0, 3.1, 0.0, 0.0],
#                     [0.7, 1.7, 0.6, 2.6, 2.2, 6.2, 0.0],
#                     [1.3, 1.2, 0.0, 0.0, 0.0, 3.2, 5.1],
#                     [0.1, 2.0, 0.0, 1.4, 0.0, 1.9, 6.3]])


fig, ax = plt.subplots()
im = ax.imshow(response_matrix)
# Show all ticks and label them with the respective list entries
ax.set_xticks(np.arange(len(prompt_energies)), labels=prompt_energies)
ax.set_yticks(np.arange(len(neutrino_energies)), labels=neutrino_energies)
fig.colorbar(im)
# Rotate the tick labels and set their alignment.
# plt.setp(ax.get_xticklabels(), rotation=45, ha="right",
        #  rotation_mode="anchor")

# Loop over data dimensions and create text annotations.
for i in range(len(prompt_energies)):
    for j in range(len(neutrino_energies)):
        text = ax.text(j, i, round(response_matrix[i, j],3),
                       ha="center", va="center", color="w")

# ax.set_title("Harvest of local farmers (in tons/year)")
fig.tight_layout()


plt.show()