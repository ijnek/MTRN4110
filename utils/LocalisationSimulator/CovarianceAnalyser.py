#!/usr/bin/env python3
import sys
import os
import matplotlib.pyplot as plt
import argparse
import tty
import termios
from matplotlib.patches import Ellipse
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

    drawEllipse(state[index], covariance[index])

def main():
    global state, covariance

    args = parseArgs()
    (state, covariance) = readFile(args.files[0])
    global max_index
    max_index = len(state) - 1

    # fig = plt.figure(0)
    # ax = fig.add_subplot(111, aspect='equal')

    global fig
    global ax

    fig, ax = plt.subplots()
    fig.canvas.mpl_connect('key_press_event', press)
    plt.show()

    while True:
        pass


    '''
    while True:
        get()
        # plot(state[index], covariance[index])
        drawEllipse(state[index], covariance[index])
        # drawSomeEllipses()
    '''


def parseArgs():
    parser = argparse.ArgumentParser()
    parser.add_argument("files", help="filepaths of files to plot",
                        nargs='+')

    return parser.parse_args()


def plot(state, covariance):
    plt.figure()
    ax = plt.gca()

    ellipse = Ellipse(xy=(state[0], state[1]), width=0.036, height=0.012,
                      edgecolor='r', fc='None', lw=2)
    ax.add_patch(ellipse)

    plt.legend(loc='upper right')
    plt.show()


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


def drawEllipse(state, covariance):
    # u = state[0]  # x-position of the center
    # v = state[1]  # y-position of the center

    # print(state)

    # eig_vals = LA.eigvals(covariance)
    # print(eig_vals)

    # a = 2.  # radius on the x-axis
    # b = 1.5  # radius on the y-axis
    # t_rot = pi/4  # rotation angle

    # t = np.linspace(0, 2*pi, 100)
    # Ell = np.array([a*np.cos(t), b*np.sin(t)])
    # # u,v removed to keep the same center location
    # R_rot = np.array([[cos(t_rot), -sin(t_rot)], [sin(t_rot), cos(t_rot)]])
    # # 2-D rotation matrix

    # Ell_rot = np.zeros((2, Ell.shape[1]))
    # for i in range(Ell.shape[1]):
    #     Ell_rot[:, i] = np.dot(R_rot, Ell[:, i])

    # eig_vals = LA.eigvals(covariance[0:2])
    eig_vals = LA.eigvals(covariance[0:2, 0:2])
    print(eig_vals)

    plt.cla()

    print(covariance)

    r_major_axis = math.sqrt(max(eig_vals[0], eig_vals[1]))
    r_minor_axis = math.sqrt(min(eig_vals[0], eig_vals[1]))
    theta = math.atan2(2 * covariance[0, 1], (covariance[0, 0] - covariance[1, 1])) / 2.0
    print(theta)

    ellipse = Ellipse(xy=(state[0], state[1]), width=r_major_axis, height=r_minor_axis, angle=math.degrees(theta))

    ax.add_artist(ellipse)
    ellipse.set_clip_box(ax.bbox) 

    ax.set_xlim(-100, 2250+100)
    ax.set_ylim(-100, 2250+100)

    fig.canvas.draw()

if __name__ == "__main__":
    main()
