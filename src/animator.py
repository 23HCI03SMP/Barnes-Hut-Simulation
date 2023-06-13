import glob
import cv2
import math
import os
import threading
import time
import concurrent.futures

import matplotlib
import matplotlib.pyplot as plt
import matplotlib.animation

SIMULATION_VALUES = "simulation_values.csv"
OUTPUT_POINTS_VIDEO = "output_points.mp4"
OUTPUT_KE_VIDEO = "output_ke.mp4"

FPS = 10

MIN = 0
MAX = 20

COLORS = {
    "Deutron": "red",
    "Electron": "blue"
}


def generate_points_frames(value, i):
    fig = plt.figure()
    ax = fig.add_subplot(projection="3d")
    ax.set_box_aspect((1, 1, 1))
    
    ax.clear()

    xlist = [x[0] for x in value]
    ylist = [y[1] for y in value]
    zlist = [z[2] for z in value]
    colorList = [c[3] for c in value]

    # Plot the scatter graph
    ax.scatter(xlist, ylist, zlist, c=colorList)
    ax.set_title(f"Time step {i}")
    ax.set_xlim(MIN, MAX)
    ax.set_ylim(MIN, MAX)
    ax.set_zlim(MIN, MAX)

    plt.savefig(os.path.join(os.path.dirname(__file__),
                f"point_frames/frame{str(i).zfill(4)}.png"))
    plt.close(fig)
    print(f"Saved particle frame {i}")


def generate_ke_frames(i, timestep: dict):
    num_plots = len(timestep)
    num_cols = 2  # Number of columns in the grid
    num_rows = math.ceil(num_plots / num_cols)  # Number of rows in the grid

    fig, axs = plt.subplots(num_rows, num_cols, figsize=(12, 8))
    axs = axs.flatten()  # Flatten the array of subplots

    for idx, (key, value) in enumerate(sorted(timestep.items())):
        ax = axs[idx]  # Get the current subplot

        ax.hist(value, bins=20, label=key, color=COLORS[key])
        ax.set_xlabel('KE')
        ax.set_ylabel('Frequency')
        ax.set_title(f'Distribution of initial KE of {key}s at timestep {i}')
        ax.grid(True)
        ax.legend()

    plt.tight_layout()
    plt.savefig(os.path.join(os.path.dirname(__file__),
                                f"ke_frames/frame{str(i).zfill(4)}.png"))
    plt.close(fig)
    print(f"Saved ke frame {i}")


# Initialize directory
# Create a ./frames directory if it doesn't exist
# Otherwise, delete all files in the ./frames directory
if not os.path.exists(os.path.join(os.path.dirname(__file__), "point_frames")):
    os.makedirs(os.path.join(os.path.dirname(__file__), "point_frames"))
elif not os.path.isdir(os.path.join(os.path.dirname(__file__), "ke_frames")):
    os.makedirs(os.path.join(os.path.dirname(__file__), "ke_frames"))
else:
    for file in os.listdir(os.path.join(os.path.dirname(__file__), "point_frames")):
        os.remove(os.path.join(os.path.dirname(
            __file__), "point_frames", file))

    for file in os.listdir(os.path.join(os.path.dirname(__file__), "ke_frames")):
        os.remove(os.path.join(os.path.dirname(
            __file__), "ke_frames", file))

process_timer_start = time.time()

# Open the CSV file and read its contents
with open(os.path.join(os.path.dirname(__file__), SIMULATION_VALUES)) as csv:
    csv.readline()  # Skip the header line

    point_groups = []
    point_values = []
    particle_groups = []
    particle_ke = {}
    i = 0

    # Read the CSV file line by line
    # If the line is empty, add the values to the groups list
    # Otherwise, add the values to the values list
    for line in csv:
        if line.isspace():
            point_groups.append(point_values)
            point_values = []

            particle_groups.append(particle_ke)
            particle_ke = {}
        else:
            line_values = line.split(",")

            particle_alias = line_values[8].strip()
            color = COLORS[particle_alias]

            x = float(line_values[0])
            y = float(line_values[1])
            z = float(line_values[2])
            vx = float(line_values[3])
            vy = float(line_values[4])
            vz = float(line_values[5])
            mass = float(line_values[6])
            alias = line_values[8].strip()

            point_values.append([x, y, z, color])
            particle_ke.setdefault(alias, []).append(
                0.5 * mass * (vx ** 2 + vy ** 2 + vz ** 2))

    lock = threading.Lock()
    threads = []

    frame_timer_start = time.time()
    
    for i, value in enumerate(point_groups):
        generate_points_frames(value, i)

    for i, timestep in enumerate(particle_groups):
        generate_ke_frames(i, timestep)

    print(
        f"Saved {len(point_groups)} frames in {time.time() - frame_timer_start}s")

    # Create the animation by combining all the images together
    point_output_video = os.path.join(os.path.dirname(__file__), OUTPUT_POINTS_VIDEO)
    point_images = glob.glob(os.path.join(
        os.path.dirname(__file__), "point_frames", "*.png"))
    
    ke_output_video = os.path.join(os.path.dirname(__file__), OUTPUT_KE_VIDEO)
    ke_images = glob.glob(os.path.join(
        os.path.dirname(__file__), "ke_frames", "*.png"))

    point_frames = []
    ke_frames = []

    for image in point_images:
        frame = cv2.imread(image)
        point_frames.append(frame)

    for image in ke_images:
        frame = cv2.imread(image)
        ke_frames.append(frame)

    # get the height and width of frames
    point_height, point_width, _ = point_frames[0].shape if len(point_frames) > 0 else (0, 0, 0)
    ke_height, ke_width, _ = ke_frames[0].shape if len(ke_frames) > 0 else (0, 0, 0)

    # create the video writer object
    fourcc = cv2.VideoWriter_fourcc(*"mp4v")
    point_video_writer = cv2.VideoWriter(point_output_video, fourcc, FPS, (point_width, point_height))
    ke_video_writer = cv2.VideoWriter(ke_output_video, fourcc, FPS, (ke_width, ke_height))

    # write frames to video
    for frame in point_frames:
        point_video_writer.write(frame)
    
    for frame in ke_frames:
        ke_video_writer.write(frame)

    # release the video writer object
    point_video_writer.release()
    ke_video_writer.release()
    print(f"Saved point video to {point_output_video}")
    print(f"Saved ke video to {ke_output_video}")
    print(f"Task completed in {time.time() - process_timer_start}s")