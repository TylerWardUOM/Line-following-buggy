import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import time

with open('Diagnostics\papersweep.txt', 'r') as data:
    rows = data.readlines()

# Extract labels from the first row
labels = rows[0].strip().split('\t')[1:]  # Skip "Time" and keep the series labels

# Convert the remaining rows to floats
columns = []
for row in rows[1:]:  # Skip the first row (labels)
    column = row.strip().split('\t')
    column = [float(value) for value in column]  # Convert all values to floats
    columns.append(column)


# Initialize variables for live plotting
time_values = []  # Store time (x-axis)
series_values = [[] for _ in range(len(labels))]  # Separate y-values for each series

# Set up the plot
fig, ax = plt.subplots()
lines = [
    ax.plot([], [], label=label)[0] for label in labels
]
ax.set_xlim(0, 10)
ax.set_ylim(0, 10)
ax.set_xlabel("Time")
ax.set_ylabel("Value")
ax.legend()
plt.title("Live columns Plot")

# Update function for animation
def update(frame):
    if frame < len(columns):
        row = columns[frame]
        time_values.append(row[0])  # Append time
        for i, value in enumerate(row[1:]):
            series_values[i].append(value)  # Append corresponding values

        # Update the line columns
        for i, line in enumerate(lines):
            line.set_data(time_values, series_values[i])

        # Adjust x and y limits dynamically
        ax.set_xlim(max(0, time_values[-1] - 10), time_values[-1] + 1)
                # Update y-limits to account for both positive and negative values
        all_values = [val for series in series_values for val in series]  # Flatten series_values
        y_min = min(all_values) - 1
        y_max = max(all_values) + 1
        ax.set_ylim(y_min, y_max)
# Animation setup
ani = FuncAnimation(fig, update, frames=len(columns), interval=1000)  # 1-second interval
plt.show()