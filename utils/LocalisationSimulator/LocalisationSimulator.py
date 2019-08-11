#!/usr/bin/env python3
import sys
import argparse
import numpy.matlib
import numpy as np
import math

state = np.matlib.empty((3, 1))
covariance = np.matlib.empty((3, 3))

INITIAL_UNCERTAINTY_X = 30  # mm
INITIAL_UNCERTAINTY_Y = 30  # mm
INITIAL_UNCERTAINTY_H = math.radians(15)  # rad


ODOMETRY_FORWARD_MULTIPLY_FACTOR = 2

def normaliseTheta(theta):
   r = (theta - 3.142) % (2 * 3.142)
   return r - 3.142 if r > 0 else r + 3.142


def main():

    args = parseArgs()
    
    for filepath in args.files:
        f = open(filepath)
        f_out = open(filepath.rstrip(".txt") + "_out.txt", "w")
        global state
        global covariance
        state = np.matrix([0.0, 0.0, 0.0]).reshape(3, 1)
        covariance = np.matrix([0.0 for _ in range(9)]).reshape(3, 3)
        covariance[0, 0] = INITIAL_UNCERTAINTY_X * INITIAL_UNCERTAINTY_X
        covariance[1, 1] = INITIAL_UNCERTAINTY_Y * INITIAL_UNCERTAINTY_Y
        covariance[2, 2] = INITIAL_UNCERTAINTY_H * INITIAL_UNCERTAINTY_H
        x_list = []
        y_list = []
        for line in f:
            (dt, forward, turn, lidarFront, lidarLeft, lidarRight, turnFromIMU) = line.split(", ")
            print("forward" , forward)
            print("turn: ", turn)
            predict(float(forward), float(turn))
            x_list += state[0]
            y_list += state[1]

            for i in range(3):
                f_out.write(str(state[i, 0]))
                f_out.write(", ")

            for i in range(3):
                for j in range(3):
                    f_out.write(str(covariance[i, j]))
                    if i != 2 or j != 2:
                        f_out.write(", ")
            f_out.write("\n")
        f.close()
        f_out.close()

def parseArgs():
    parser = argparse.ArgumentParser()
    parser.add_argument("files", help="filepaths of files to plot",
                        nargs='+')

    return parser.parse_args()


def predict(forwardChange, turnChange):
    global state
    global covariance

    deltaX = forwardChange * math.cos(state[2])
    deltaY = forwardChange * math.sin(state[2])

    state[0] += deltaX
    state[1] += deltaY
    state[2] = normaliseTheta(state[2] + turnChange)

    forwardCovariance = forwardChange * forwardChange * ODOMETRY_FORWARD_MULTIPLY_FACTOR

    rotationMatrix = np.matrix([math.cos(state[2]), math.sin(state[2])]).reshape(2, 1)
    rotationMatrixTranspose = np.transpose(rotationMatrix)

    covariance[0:2, 0:2] += rotationMatrix * forwardCovariance * rotationMatrixTranspose

    propagationJacobian = np.eye(3)
    propagationJacobian[0, 2] = -forwardChange * math.sin(state[2])
    propagationJacobian[1, 2] = forwardChange * math.cos(state[2])

    propagationJacobianTranspose = np.transpose(propagationJacobian)

    covariance = propagationJacobian * covariance * propagationJacobianTranspose



main()