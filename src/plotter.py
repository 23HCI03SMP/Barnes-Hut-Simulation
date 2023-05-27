import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import pandas as pd
import os

# Load the CSV file into a DataFrame
df = pd.read_csv(os.path.join(os.path.dirname(__file__), 'simulation_values.csv'))

# Extract the x, y, and z values
x = df['x']
y = df['y']
z = df['z']

# Create a 3D plot
fig = plt.figure()
ax = fig.add_subplot(projection='3d')
ax.scatter(x, y, z)

# Set the axis labels
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
ax.set_box_aspect((np.ptp(x), np.ptp(y), np.ptp(z)))

# Show the plot
plt.show()