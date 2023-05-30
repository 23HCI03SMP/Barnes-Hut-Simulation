import glob
import cv2
import os
import threading
import time

import matplotlib.pyplot as plt
import matplotlib.animation

SIMULATION_VALUES = "simulation_values.csv"
OUTPUT_VIDEO = "output.mp4"

FPS = 30

MIN = 1
MAX = 10

COLORS = {
    "Deutron": "red",
    "Electron": "blue"
}

fig = plt.figure()
ax = fig.add_subplot(projection="3d")
ax.set_box_aspect((1, 1, 1))

def generate_frames(value, i):
    ax.clear()

    xlist = [x[0] for x in value]
    ylist = [y[1] for y in value]
    zlist = [z[2] for z in value]
    # colorList = [c[3] for c in value]

    # Plot the scatter graph
    # ax.scatter(xlist, ylist, zlist, c=colorList)
    ax.scatter(xlist, ylist, zlist)
    ax.set_title(f"Time step {i}")
    ax.set_xlim(MIN, MAX)
    ax.set_ylim(MIN, MAX)
    ax.set_zlim(MIN, MAX)

    try:
        plt.savefig(os.path.join(os.path.dirname(__file__), f"frames/frame{str(i).zfill(4)}.png"))
        print(f"Saved frame {i}")
    except:
        print(f"Error saving frame {i}. Trying again...")
        generate_frames(value, i)

# Initialize directory
# Create a ./frames directory if it doesn't exist
# Otherwise, delete all files in the ./frames directory
if not os.path.exists(os.path.join(os.path.dirname(__file__), "frames")):
    os.makedirs(os.path.join(os.path.dirname(__file__), "frames"))
else:
    for file in os.listdir(os.path.join(os.path.dirname(__file__), "frames")):
        os.remove(os.path.join(os.path.dirname(__file__), "frames", file))

process_timer_start = time.time()

# Open the CSV file and read its contents
with open(os.path.join(os.path.dirname(__file__), SIMULATION_VALUES)) as csv:
    csv.readline() # Skip the header line

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

            # values.append([float(line_values[0]), float(line_values[1]), float(line_values[2]), color])
            values.append([float(line_values[0]), float(line_values[1]), float(line_values[2])])

    lock = threading.Lock()
    threads = []

    frame_timer_start = time.time()
    for i, value in enumerate(groups):
        t = threading.Thread(target=generate_frames, args=(value, i))

        t.start()
        threads.append(t)
    
    for t in threads:
        t.join()

    print(f"Saved {len(groups)} frames in {time.time() - frame_timer_start}s")    
    
    # Create the animation by combining all the images together
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
    print(f"Saved video to {output_video}")
    print(f"Task completed in {time.time() - process_timer_start}s")