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
    "Deutron": "tan",
    "Electron": "blue"
}

FPS = 10

# Create/empty the frames directory
if not os.path.exists(os.path.join(os.path.dirname(__file__), "frames")):
    os.makedirs(os.path.join(os.path.dirname(__file__), "frames"))
else:
    for file in os.listdir(os.path.join(os.path.dirname(__file__), "frames")):
        os.remove(os.path.join(os.path.dirname(__file__), "frames", file))

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

    print("Rendering frame %i"%(i+1))

    # Create, view and save the frame
    plotter = pv.Plotter(off_screen=True)

    # Changes depending on the radius and other factors (change manually)
    plotter.camera.position = (13.562428421039028, 13.58590842103903, 13.63003842103903)

    try:
        #plotter.show_bounds(axes_range=[1,1,1,5,5,5])
        plotter.add_mesh(mesh, render_points_as_spheres=True, scalars="Colors")
        plotter.show(screenshot="./frames/test%s.png"%(str(i).zfill(4)))
        mesh.save("./vtk/test%s.vtk"%(str(i).zfill(4)), binary=False)
    except Exception as e:
        print(e)
    i += 1

# Animate the frames

output_video = os.path.join(os.path.dirname(__file__), OUTPUT_VIDEO)
images = glob.glob(os.path.join(os.path.dirname(__file__), "frames", "*.png"))

frames = []
for image in images:
    frame = cv2.imread(image)
    frames.append(frame)

# get the height and width of frames
height, width, _ = frames[0].shape

# create the video writer object
fourcc = cv2.VideoWriter_fourcc(*"mp4v")
video_writer = cv2.VideoWriter(output_video, fourcc, FPS, (width, height))

# write frames to video
for frame in frames:
    video_writer.write(frame)

# release the video writer object
video_writer.release()