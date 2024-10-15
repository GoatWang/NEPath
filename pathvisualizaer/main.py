import os
import numpy as np
import pandas as pd
from matplotlib import pyplot as plt

# Create the contour with the specified length
contour = {
    "length": 1000,  # number of waypoints
    "x": np.zeros(1000),  # x-coordinate of waypoints
    "y": np.zeros(1000)   # y-coordinate of waypoints
}

pi = np.arccos(-1.0)  # pi == 3.1415926...
for i in range(contour["length"]):
    theta = 2.0 * pi * i / contour["length"]
    r = 15.0 * (1.0 + 0.15 * np.cos(10.0 * theta))
    contour["x"][i] = r * np.cos(theta)
    contour["y"][i] = r * np.sin(theta)


paths_dir = os.path.join(os.path.dirname(__file__), "paths")
csv_fps = [os.path.join(paths_dir, fn) for fn in os.listdir(paths_dir) if fn.endswith("csv")]

plt.figure()
plt.plot(contour['x'], contour['y'], color='black')

for csv_fp in csv_fps:
    df = pd.read_csv(csv_fp)
    xs, ys = df['x'].values, df['y'].values
    plt.plot(xs, ys)
plt.show()



