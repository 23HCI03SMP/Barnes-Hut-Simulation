import os
import csv
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

INITIAL_VALUES = "initial_values.csv"

# Lists to store the positions and velocities
x_positions = []
y_positions = []
z_positions = []
vx_velocities = []
vy_velocities = []
vz_velocities = []

# Read data from CSV file
with open(os.path.join(os.path.dirname(__file__), INITIAL_VALUES), 'r') as file:
    reader = csv.reader(file)
    next(reader)  # Skip header row
    for row in reader:
        x_positions.append(float(row[0]))  # Assuming x position is in the first column
        y_positions.append(float(row[1]))  # Assuming y position is in the second column
        z_positions.append(float(row[2]))  # Assuming z position is in the third column
        vx_velocities.append(float(row[3]))  # Assuming vx velocity is in the fourth column
        vy_velocities.append(float(row[4]))  # Assuming vy velocity is in the fifth column
        vz_velocities.append(float(row[5]))  # Assuming vz velocity is in the sixth column

# Plotting the histogram for x positions
plt.figure(figsize=(12, 4))
plt.subplot(231)
plt.hist(x_positions, bins=20)
plt.xlabel('x position')
plt.ylabel('Frequency')
plt.title('Distribution of x Positions')
plt.grid(True)

# Plotting the histogram for y positions
plt.subplot(232)
plt.hist(y_positions, bins=20)
plt.xlabel('y position')
plt.ylabel('Frequency')
plt.title('Distribution of y Positions')
plt.grid(True)

# Plotting the histogram for z positions
plt.subplot(233)
plt.hist(z_positions, bins=20)
plt.xlabel('z position')
plt.ylabel('Frequency')
plt.title('Distribution of z Positions')
plt.grid(True)

# Plotting the histogram for vx velocities
plt.subplot(234)
plt.hist(vx_velocities, bins=20, color='red')
plt.xlabel('vx velocity')
plt.ylabel('Frequency')
plt.title('Distribution of vx Velocities')
plt.grid(True)

# Plotting the histogram for vy velocities
plt.subplot(235)
plt.hist(vy_velocities, bins=20, color='red')
plt.xlabel('vy velocity')
plt.ylabel('Frequency')
plt.title('Distribution of vy Velocities')
plt.grid(True)

# Plotting the histogram for vz velocities
plt.subplot(236)
plt.hist(vz_velocities, bins=20, color='red')
plt.xlabel('vz velocity')
plt.ylabel('Frequency')
plt.title('Distribution of vz Velocities')
plt.grid(True)

plt.tight_layout()
plt.show()

# Plotting the 3D scatter plot
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.scatter(x_positions, y_positions, z_positions)
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel('z')
ax.set_title('Particle Positions')
ax.set_box_aspect((np.ptp(x_positions), np.ptp(y_positions), np.ptp(z_positions)))
plt.show()