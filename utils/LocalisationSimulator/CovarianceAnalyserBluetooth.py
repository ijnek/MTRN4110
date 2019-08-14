#!/usr/bin/env python
import matplotlib.pyplot as plt
from matplotlib.patches import Ellipse, Arrow
import numpy as np
from matplotlib import pyplot as plt
from math import pi, cos, sin
from numpy import linalg as LA
import matplotlib.pyplot as plt
import numpy.random as rnd
import math
import serial


index = 0
max_index = 0
state = None
covariance = None
fig = None
ax = None
bluetooth = None

def main():
    global state, covariance
    global bluetooth


    global fig
    global ax

    fig, ax = plt.subplots(figsize=(10, 20))
    
    ax.set_xlim(-100, 2250+100)
    ax.set_ylim(-100, 2250+100)
    ax.set_aspect('equal', adjustable='box')

    ax.set_xticks(np.arange(-125, 2250, 250))
    ax.set_yticks(np.arange(-2250, 2250, 250))
    plt.grid()
    
    plt.show(block=False)

    bluetooth = serial.Serial("/dev/rfcomm0",9600,timeout = 2)

    while True:
        # receive bluetooth here
        bluetooth.flush()

        try:

            buffer = bluetooth.read_until()
            print((buffer.decode('ascii')))
            buffer = [float(i) for i in buffer.decode('ascii').rstrip('\n').split(', ')]

            state = np.matrix(buffer[0:3]).reshape(3, 1)
            covariance = np.matrix(buffer[3:]).reshape(3, 3)

            print(state)
            print(covariance)

            plt.cla() 

            drawArrow(state)
            drawEllipse(state, covariance)

        except:
            bluetooth.close()
            bluetooth = serial.Serial("/dev/rfcomm0",9600,timeout = 2)
            print("could not find device. resetting connection")

    bluetooth.close()

def drawArrow(state):
    arrow_length = 250
    arrow = Arrow(state[0], state[1], arrow_length * math.cos(state[2]), arrow_length * math.sin(state[2]), width=50, edgecolor="black", facecolor=None)
    ax.add_artist(arrow)


def drawEllipse(state, covariance):
    eig_vals = LA.eigvals(covariance[0:2, 0:2])

    r_major_axis = math.sqrt(max(eig_vals[0], eig_vals[1]))
    r_minor_axis = math.sqrt(min(eig_vals[0], eig_vals[1]))
    theta = math.atan2(2 * covariance[0, 1], (covariance[0, 0] - covariance[1, 1])) / 2.0

    ellipse = Ellipse(xy=(state[0], state[1]), width=r_major_axis, height=r_minor_axis, angle=math.degrees(theta), edgecolor="red", facecolor=None)

    ax.add_artist(ellipse)
    ellipse.set_clip_box(ax.bbox)

    ax.set_xticks(np.arange(-125, 2250, 250))
    ax.set_yticks(np.arange(-2250, 2250, 250))
    plt.grid()

    fig.canvas.draw()

if __name__ == "__main__":
    main()