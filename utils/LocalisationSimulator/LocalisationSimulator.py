#!/usr/bin/env python3
import sys
import argparse
import numpy.matlib
import numpy as np
import math

state = np.matlib.empty((3, 1))
covariance = np.matlib.empty((3, 3))

INITIAL_UNCERTAINTY_X = 30.0  # mm
INITIAL_UNCERTAINTY_Y = 30.0  # mm
INITIAL_UNCERTAINTY_H = math.radians(10)  # rad

LIDAR_LEFT_X = 45.0  # mm
LIDAR_LEFT_Y = 45.0  # mm
LIDAR_LEFT_H = math.radians(90)  # rad

LIDAR_RIGHT_X = 45.0  # mm
LIDAR_RIGHT_Y = -45.0  # mm
LIDAR_RIGHT_H = math.radians(-90)  # rad

LIDAR_FRONT_X = 55.0  # mm
LIDAR_FRONT_Y = 0.0  # mm
LIDAR_FRONT_H = 0.0  # rad

LIDAR_UNCERTAINTY = 60  # mm

CELL_SIZE = 250.0  # mm


ODOMETRY_FORWARD_MULTIPLY_FACTOR = 2.0
# ODOMETRY_TURN_MULTIPLY_FACTOR = 20.0
ODOMETRY_TURN_PROCESS_VARIANCE = math.radians(5) * math.radians(5)

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
            predict(float(forward), float(turn))

            print("len: ", len(x_list))

            print("left")
            updateWithLidar(LIDAR_LEFT_X, LIDAR_LEFT_Y, LIDAR_LEFT_H, float(lidarLeft))

            print("front")
            updateWithLidar(LIDAR_FRONT_X, LIDAR_FRONT_Y, LIDAR_FRONT_H, float(lidarFront))

            print("right")
            updateWithLidar(LIDAR_RIGHT_X, LIDAR_RIGHT_Y, LIDAR_RIGHT_H, float(lidarRight))

            x_list.append(state[0])
            y_list.append(state[1])

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

    # covariance[2, 2] += turnChange * turnChange * ODOMETRY_TURN_MULTIPLY_FACTOR
    covariance[2, 2] += ODOMETRY_TURN_PROCESS_VARIANCE

def updateWithLidar(lidarX, lidarY, lidarH, measuredDistance):
    global state
    global covariance
    
    # If there is no wall, return
    if measuredDistance == 255:
        return

    # Precalculate sin and cos of heading because its used everywhere
    cosHeading = math.cos(state[2])
    sinHeading = math.sin(state[2])

    relativeX = lidarX + measuredDistance * math.cos(lidarH)
    relativeY = lidarY + measuredDistance * math.sin(lidarH)

    expectedX = state[0] + cosHeading * relativeX - sinHeading * relativeY
    expectedY = state[1] + sinHeading * relativeX + cosHeading * relativeY

    closestWallX = expectedX // CELL_SIZE * CELL_SIZE + CELL_SIZE / 2
    closestWallY = expectedY // CELL_SIZE * CELL_SIZE + CELL_SIZE / 2

    angleLidar = normaliseTheta(state[2] + lidarH)

    print(math.degrees(angleLidar))

    # input()

    sinHeadingPlusLidarH = math.sin(state[2, 0] + lidarH)
    cosHeadingPlusLidarH = math.cos(state[2, 0] + lidarH)

    print("sinHeadingPlusLidarH: ", sinHeadingPlusLidarH)


    if (abs(normaliseTheta(angleLidar - math.radians(0))) < math.radians(20) or 
            abs(normaliseTheta(angleLidar - math.radians(180))) < math.radians(20)):
        # do x update

        lidarGlobalX = state[0] + cosHeading * lidarX - sinHeading * lidarY  # y position of lidar in global coordinates
        expectedDistance = abs((closestWallX - lidarGlobalX) / cosHeadingPlusLidarH)
        innovationVector = np.matrix([measuredDistance - expectedDistance[0, 0]])

        jacobian = np.zeros((1, 3))
        jacobian[0, 0] = - 1.0 / cosHeadingPlusLidarH
        jacobian[0, 1] = 0
        jacobian[0, 2] = (lidarY * cosHeading + lidarX * sinHeading) / cosHeadingPlusLidarH + (sinHeadingPlusLidarH * (closestWallX - state[0] - lidarX * cosHeading + lidarY * sinHeading)) / (cosHeadingPlusLidarH * cosHeadingPlusLidarH)

        print("state before: \n", state)
        print("covariance before: \n", covariance)
        print("closestWallX: \n", closestWallX)
        print("lidarGlobalX: \n", lidarGlobalX)
        print("expectedDistance: \n", expectedDistance[0, 0])
        print("innovationVector: \n", innovationVector)
        print("jacobian: \n", jacobian)

    elif (abs(normaliseTheta(angleLidar - math.radians(90))) < math.radians(20) or 
            abs(normaliseTheta(angleLidar - math.radians(-90))) < math.radians(20)):
        # do y update

        lidarGlobalY = state[1] + sinHeading * lidarX + cosHeading * lidarY  # y position of lidar in global coordinates
        expectedDistance = abs((closestWallY - lidarGlobalY) / sinHeadingPlusLidarH)
        innovationVector = np.matrix([measuredDistance - expectedDistance[0, 0]])

        jacobian = np.zeros((1, 3))
        jacobian[0, 0] = 0
        jacobian[0, 1] = -1.0 / sinHeadingPlusLidarH
        jacobian[0, 2] = (cosHeadingPlusLidarH * (state[1] - closestWallY + lidarY * cosHeading + lidarX * sinHeading))/(sinHeadingPlusLidarH * sinHeadingPlusLidarH) - (lidarX * cosHeading - lidarY * sinHeading) / sinHeadingPlusLidarH

        print("state before: \n", state)
        print("covariance before: \n", covariance)
        print("closestWallY: \n", closestWallY)
        print("lidarGlobalY: \n", lidarGlobalY)
        print("expectedDistance: \n", expectedDistance[0, 0])
        print("innovationVector: \n", innovationVector)
        print("jacobian: \n", jacobian)

    else:
        return

    jacobianTranspose = jacobian.transpose()
    innovationCovariance = jacobian * covariance * jacobianTranspose + LIDAR_UNCERTAINTY * LIDAR_UNCERTAINTY
    innovationCovarianceInv = np.linalg.inv(innovationCovariance)
    kalmanGain = covariance * jacobianTranspose * innovationCovarianceInv
    state = state + kalmanGain * innovationVector
    state[2, 0] = normaliseTheta(state[2, 0])
    covariance = (np.eye(3) - kalmanGain * jacobian) * covariance

    print("innovationCovariance: \n", innovationCovariance)
    print("kalmanGain: \n", kalmanGain)

    print("state after: \n", state)
    print("covariance after: \n", covariance)
    print("------------------------------")

    # input()


main()