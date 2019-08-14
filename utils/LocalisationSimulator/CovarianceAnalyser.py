#!/usr/bin/env python3
import sys
import os
import matplotlib.pyplot as plt
import argparse
import tty
import termios
from matplotlib.patches import Ellipse, Arrow
import numpy as np
from matplotlib import pyplot as plt
from math import pi, cos, sin
from numpy import linalg as LA
import matplotlib.pyplot as plt
import numpy.random as rnd
import math


index = 0
max_index = 0
state = None
covariance = None
fig = None
ax = None

def press(event):
    global index
    print("press", event.key)

    if event.key == "up":
        index = 0
    elif event.key == "down":
        index = max_index
    elif event.key == "left":
        index = max(index - 1, 0)
    elif event.key == "right":
        index = min(index + 1, max_index)

    print("index: ", index)
    print("state: \n", state[index])

    plt.cla()        

    drawArrow(state[index])
    drawEllipse(state[index], covariance[index])

def main():
    global state, covariance

    args = parseArgs()
    (state, covariance) = readFile(args.files[0])
    global max_index
    max_index = len(state) - 1

    global fig
    global ax

    fig, ax = plt.subplots()
    fig.canvas.mpl_connect('key_press_event', press)
    
    ax.set_xlim(-100, 2250+100)
    ax.set_ylim(-100, 2250+100)
    ax.set_aspect('equal', adjustable='box')

    ax.set_xticks(np.arange(-125, 2250, 250))
    ax.set_yticks(np.arange(-2250, 2250, 250))
    plt.grid()
    
    plt.show()

    while True:
        pass


def parseArgs():
    parser = argparse.ArgumentParser()
    parser.add_argument("files", help="filepaths of files to plot",
                        nargs='+')

    return parser.parse_args()


def readFile(filepath):
    state_list = []
    covariance_list = []

    f = open(filepath, 'r')
    for line in f:
        x, y, h, *rest = (float(item)
                          for item in line.rstrip("\n").split(", "))
        # print(rest)
        covariance = np.matrix(rest).reshape(3, 3)
        # print(covariance)
        state = np.matrix([x, y, h]).reshape(3, 1)
        # print(state)

        state_list.append(state)
        covariance_list.append(covariance)

    return state_list, covariance_list


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
