#!/usr/bin/env python3
import sys
import os
import matplotlib.pyplot as plt
import argparse


def main():
    args = parseArgs()
    (x_lists, y_lists, theta_lists) = readXYFiles(args.files)
    plot(x_lists, y_lists, args.files)


def parseArgs():
    parser = argparse.ArgumentParser()
    parser.add_argument("files", help="filepaths of files to plot",
                        nargs='+')

    return parser.parse_args()


# plots X, Y coordinates
def plot(x_lists, y_lists, filenames):
    fig, ax = plt.subplots()

    for i in range(len(x_lists)):
        plt.plot(x_lists[i], y_lists[i], label=filenames[i])

    plt.legend(loc='upper right')
    plt.show()


# reads x, y, theta and time from list of filepaths
def readXYFiles(filepaths):

    x_lists, y_lists, theta_lists = ([] for i in range(3))

    for filepath in filepaths:

        x_list, y_list, theta_list = readXYs(filepath)

        x_lists.append(x_list)
        y_lists.append(y_list)
        theta_lists.append(theta_list)

    return x_lists, y_lists, theta_lists


# extracts x, y, theta and time from file
def readXYs(filepath):

    if not os.path.isfile(filepath):
        print("File path {} does not exist. Exiting...".format(filepath))
        sys.exit()

    x_list, y_list, theta_list, = ([] for i in range(3))

    f = open(filepath, 'r')

    for line in f:
        x, y, theta = line.split(', ')
        x_list.append(float(x))
        y_list.append(float(y))
        theta_list.append(float(theta))

    f.close()

    return x_list, y_list, theta_list


if __name__ == "__main__":
    main()
