import matplotlib
import matplotlib.pyplot as plt
import os

SIMULATION_VALUES = "simulation_values.csv"

with open(os.path.join(os.path.dirname(__file__), SIMULATION_VALUES)) as csv:
    csv.readline() # Skip the header line

    values = []

    # Read the CSV file line by line
    # If the line is empty, add the values to the groups list
    # Otherwise, add the values to the values list
    for line in csv:
        if line.isspace():
            continue
        else:
            line_values = line.split(",")
            values.append([float(line_values[1]), float(line_values[2])])

    plt.scatter([value[0] for value in values], [value[1] for value in values], c="black")
    plt.xlabel("Y")
    plt.ylabel("Z")
    
    plt.axis("equal")
    plt.title("Trajectory of Deuteron")
    plt.show()