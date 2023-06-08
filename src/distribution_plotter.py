import os
import csv
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

INITIAL_VALUES = "initial_values.csv"
SIMULATION_VALUES = "simulation_values.csv"

# Lists to store the positions and velocities
x_positions = []
y_positions = []
z_positions = []

initial_particle_ke = {}
final_particle_ke = {}

# Read data from initial CSV file
with open(os.path.join(os.path.dirname(__file__), INITIAL_VALUES), 'r') as file:
    reader = csv.reader(file)
    next(reader)  # Skip header row
    for row in reader:
        # Assuming x position is in the first column
        x_positions.append(float(row[0]))
        # Assuming y position is in the second column
        y_positions.append(float(row[1]))
        # Assuming z position is in the third column
        z_positions.append(float(row[2]))

        vx = float(row[3])
        vy = float(row[4])
        vz = float(row[5])
        mass = float(row[6])

        initial_particle_ke.setdefault(row[8], []).append(
            0.5 * mass * (vx ** 2 + vy ** 2 + vz ** 2))

# Read data from simulation CSV file
with open(os.path.join(os.path.dirname(__file__), SIMULATION_VALUES), 'r') as file:
    last_timestep = []

    for line in file:
        line = line.strip()
        if line:  # Skip empty lines
            last_timestep.append(line)
        else:  # Reset the list if an empty line is encountered
            last_timestep = []

    for row in last_timestep:
        row = row.split(",")
        vx = float(row[3])
        vy = float(row[4])
        vz = float(row[5])
        mass = float(row[6])

        final_particle_ke.setdefault(row[8], []).append(
            0.5 * mass * (vx ** 2 + vy ** 2 + vz ** 2))


# Plotting the histogram for x positions
plt.figure(figsize=(12, 4), num="Particle Property Distributions")
plt.subplot(331)
plt.hist(x_positions, bins=20)
plt.xlabel('x position')
plt.ylabel('Frequency')
plt.title('Distribution of x Positions')
plt.grid(True)

# Plotting the histogram for y positions
plt.subplot(332)
plt.hist(y_positions, bins=20)
plt.xlabel('y position')
plt.ylabel('Frequency')
plt.title('Distribution of y Positions')
plt.grid(True)

# Plotting the histogram for z positions
plt.subplot(333)
plt.hist(z_positions, bins=20)
plt.xlabel('z position')
plt.ylabel('Frequency')
plt.title('Distribution of z Positions')
plt.grid(True)

for i, (key, value) in enumerate(sorted(initial_particle_ke.items())):
    plt.subplot(334 + i)
    plt.hist(value, bins=20, label=key, color="red")
    plt.xlabel('KE')
    plt.ylabel('Frequency')
    plt.title(f'Distribution of initial KE of {key}s')
    plt.grid(True)
    plt.legend()

for i, (key, value) in enumerate(sorted(final_particle_ke.items())):
    plt.subplot(337 + i)
    plt.hist(value, bins=20, label=key, color="blue")
    plt.xlabel('KE')
    plt.ylabel('Frequency')
    plt.title(f'Distribution of final KE of {key}s')
    plt.grid(True)
    plt.legend()

plt.tight_layout()

# Plotting the 3D scatter plot
fig = plt.figure(num="Particle Positions")
ax = fig.add_subplot(111, projection='3d')
ax.scatter(x_positions, y_positions, z_positions)
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel('z')
ax.set_title('Particle Positions')
ax.set_box_aspect((np.ptp(x_positions), np.ptp(
    y_positions), np.ptp(z_positions)))

plt.subplots_adjust(hspace=0.5)

plt.show()
