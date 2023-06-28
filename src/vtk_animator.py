import glob
import cv2
import os
import threading
import time
import numpy as np
import pyvista as pv

SIMULATION_VALUES = "simulation_values.csv"
OUTPUT_VIDEO = "output.mov"
COLORS = {
    "Deutron": "fuchsia",
    "Electron": "aqua"
}

FPS = 10

# Create/empty the vtk directory
if not os.path.exists(os.path.join(os.path.dirname(__file__), "vtk")):
    os.makedirs(os.path.join(os.path.dirname(__file__), "vtk"))
else:
    for file in os.listdir(os.path.join(os.path.dirname(__file__), "vtk")):
        os.remove(os.path.join(os.path.dirname(__file__), "vtk", file))


with open(os.path.join(os.path.dirname(__file__), SIMULATION_VALUES)) as csv:
    csv.readline()  # Skip the header line

    groups = []
    values = []
    i = 0

    # Read the CSV file line by line
    # If the line is empty, add the values to the groups list
    # Otherwise, add the values to the values list
    for line in csv:
        if line.isspace():
            groups.append(values)
            values = []
        else:
            line_values = line.split(",")

            particle_alias = line_values[8].strip()
            color = COLORS[particle_alias]

            values.append([float(line_values[0]), float(
                line_values[1]), float(line_values[2]), color])

i = 0
for values in groups:
    points = []
    colors = []

    for arr in values:
        points.append([arr[0], arr[1], arr[2]])
        colors.append(arr[3])

    # Creates a mesh of all points and sets each point's color
    mesh = pv.PolyData(points)
    mesh.point_data["Colors"] = colors

    # Create the frame
    plotter = pv.Plotter(off_screen=True)

    try:
        plotter.add_mesh(mesh, render_points_as_spheres=True, scalars="Colors")
        mesh.save("./vtk/test%s.vtk"%(str(i).zfill(4)), binary=False)
    except Exception as e:
        print(e)
    i += 1