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

initial_particle_v = {}
final_particle_v = {}

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
        # mass = float(row[6])

        # initial_particle_v.setdefault(row[8], []).append(
        #     0.5 * mass * (vx ** 2 + vy ** 2 + vz ** 2))
        initial_particle_v.setdefault(row[8], []).append(
            np.sqrt(vx ** 2 + vy ** 2 + vz ** 2))

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

        final_particle_v.setdefault(row[8], []).append(
            0.5 * mass * (vx ** 2 + vy ** 2 + vz ** 2))


# Plotting the histogram for x positions
fig1 = plt.figure(figsize=(12, 4))
plt.hist(x_positions, bins=20)
plt.xlabel('x position')
plt.ylabel('Frequency')
plt.title('Distribution of x Positions')
plt.grid(True)
plt.show()

# Plotting the histogram for y positions
fig2 = plt.figure(figsize=(12, 4))
plt.hist(y_positions, bins=20)
plt.xlabel('y position')
plt.ylabel('Frequency')
plt.title('Distribution of y Positions')
plt.grid(True)
plt.show()

# Plotting the histogram for z positions
fig3 = plt.figure(figsize=(12, 4))
plt.hist(z_positions, bins=20)
plt.xlabel('z position')
plt.ylabel('Frequency')
plt.title('Distribution of z Positions')
plt.grid(True)
plt.show()

# initial particle velocity
for i, (key, value) in enumerate(sorted(initial_particle_v.items())):
    fig = plt.figure(figsize=(12, 4))
    plt.hist(value, bins=20, label=key, color="red")
    plt.xlabel('Velocity')
    plt.ylabel('Frequency')
    plt.title(f'Distribution of initial velocity of {key}s')
    plt.grid(True)
    plt.legend()
    plt.show()

# get average velocity of each particle type and print it
for key, value in initial_particle_v.items():
    print(f"Average velocity of {key}s: {np.mean(value)}")

# Plotting the 3D scatter plot
fig4 = plt.figure()
ax = fig4.add_subplot(111, projection='3d')
ax.scatter(x_positions, y_positions, z_positions)
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel('z')
ax.set_title('Particle Positions')
ax.set_box_aspect((np.ptp(x_positions), np.ptp(
    y_positions), np.ptp(z_positions)))
plt.show()
