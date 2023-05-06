import glob
import cv2
import os

import matplotlib.pyplot as plt
import matplotlib.animation

SIMULATION_VALUES = "simulation_values.csv"
OUTPUT_VIDEO = "output.mp4"

FPS = 10

MIN = 1
MAX = 10

# Initialize directory
# Create a ./frames directory if it doesn't exist
# Otherwise, delete all files in the ./frames directory
if not os.path.exists(os.path.join(os.path.dirname(__file__), "frames")):
    os.makedirs(os.path.join(os.path.dirname(__file__), "frames"))
else:
    for file in os.listdir(os.path.join(os.path.dirname(__file__), "frames")):
        os.remove(os.path.join(os.path.dirname(__file__), "frames", file))

fig = plt.figure()
ax = fig.add_subplot(projection="3d")
ax.set_box_aspect((1, 1, 1))

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
            values.append([float(value) for value in line.split(",")])

    for value in groups:
        ax.clear()

        # Extract the x, y, and z values and place them into one array
        xlist = [x[0] for x in value]
        ylist = [y[1] for y in value]
        zlist = [z[2] for z in value]

        # Plot the scatter graph
        ax.scatter(xlist, ylist, zlist)
        ax.set_title(f"Time step {i}")
        ax.set_xlim(MIN, MAX)
        ax.set_ylim(MIN, MAX)
        ax.set_zlim(MIN, MAX)

        plt.savefig(os.path.join(os.path.dirname(__file__), f"frames/frame{str(i).zfill(4)}.png"))
        print(f"Saved frame {i}")

        i += 1
    
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