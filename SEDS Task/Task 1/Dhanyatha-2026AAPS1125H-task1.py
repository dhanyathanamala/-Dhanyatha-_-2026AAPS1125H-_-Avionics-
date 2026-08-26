# Task 1: Finding the Sea Floor

import os
import pandas
import matplotlib.pyplot as pyplot
import matplotlib.animation as animation


# Find the folder containing this Python file
script_folder = os.path.dirname(os.path.abspath(__file__))
csv_file = os.path.join(script_folder, "Depth Data.csv")


# Read the sensor data
data = pandas.read_csv(csv_file)

print("\n--- Data Information ---")
print(data.head())
print("Shape:", data.shape)
data.info()


# Convert depth values to numbers
# Invalid values such as "#VALUE!" become NaN
depth = pandas.to_numeric(data["Depth (m)"], errors="coerce")

# Store invalid readings separately
invalid = data[depth.isna()]

print("\n--- Invalid Readings ---")
print("Number of invalid readings:", len(invalid))
print(invalid)


# Basic statistics
print("\n--- Basic Statistics ---")

print(
    "Min:",
    data.loc[depth.idxmin()].to_dict(),
    "| Max:",
    data.loc[depth.idxmax()].to_dict()
)

print("Mean depth:", depth.mean())
print("Standard deviation:", depth.std())


# Inspect suspicious readings
print("\n--- Neighbourhood of suspicious readings ---")
print(data.iloc[145:157])
print("\n", data.iloc[270:282])


# Mark known corrupted numerical readings as missing
# Point 151: -1271.1
# Point 276: 0
depth.loc[150] = pandas.NA
depth.loc[275] = pandas.NA


# Fill missing values using neighbouring readings
depth = depth.interpolate()

print("\n--- Corrected Values ---")
print(depth.iloc[94:99])
print(depth.iloc[148:153])
print(depth.iloc[273:278])


# Reduce random noise using a 5-point moving average
smooth_depth = depth.rolling(
    window=5,
    min_periods=1
).mean()


# Create the graph
figure, axis = pyplot.subplots()

line, = axis.plot([], [], label="Smoothed Depth")

axis.set_xlim(
    data["Point"].min(),
    data["Point"].max()
)

axis.set_ylim(
    smooth_depth.min() - 20,
    smooth_depth.max() + 20
)

axis.set_xlabel("Time (seconds)")
axis.set_ylabel("Depth (m)")
axis.set_title("Ship Depth vs Time - Live Sensor Data")

axis.grid()
axis.legend()


# Update the graph every second
def update(frame):

    line.set_data(
        data["Point"].iloc[:frame],
        smooth_depth.iloc[:frame]
    )

    current_time = data["Point"].iloc[frame - 1]
    current_depth = smooth_depth.iloc[frame - 1]

    axis.set_title(
        f"Ship Depth vs Time | "
        f"Time: {current_time}s | "
        f"Depth: {current_depth:.1f} m"
    )

    return line,


# Start the animation
ani = animation.FuncAnimation(
    figure,
    update,
    frames=range(1, len(data) + 1),
    interval=1000,
    repeat=False
)

pyplot.show()


# Summary
print("\n--- Data Cleaning Summary ---")
print("Total readings:", len(data))
print("Invalid readings found:", len(invalid))
print("Moving average window: 7")
print("Animation interval: 1 second")