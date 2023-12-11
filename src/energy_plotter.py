# read from energy.txt and plot the energy as a line graph
# make three graphs - ke, pe, and total energy

import matplotlib.pyplot as plt
import numpy as np
import os

ENERGY_VALUES = "energy.txt"

# read in data from energy.txt (data is formatted as ke,pe,total)
data = np.loadtxt(os.path.join(os.path.dirname(__file__), ENERGY_VALUES), delimiter=",")

# plot the data
plt.plot(data[:, 0], label="Kinetic Energy")
# plt.plot(data[:, 1], label="Potential Energy")
# total energy by adding pe and ke data fields
# plt.plot(data[:, 0] + data[:, 1], label="Total Energy")

# add labels and legend
plt.xlabel("Time")
plt.ylabel("Energy")
plt.legend()

# show the plot
plt.show()