import numpy as np
import matplotlib
matplotlib.use("Agg")
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import sys

data_size = [65536, 524288, 2097152]

for d_size in [0,1,2]:

    fig = plt.figure(figsize=(13,13))
    array2d_list = []
    for n in range(1,11):
        filename = "{}_time_{}.txt".format(d_size, n)
        with open(filename, "r") as file:
            array2d = [[float(digit) for digit in line.split()] for line in file]
            array2d_matrix = np.zeros((30, 30))
            for l1 in range(0,30):
                for l2 in range(0,30):
                    array2d_matrix[l1, l2] = array2d[l1][l2]

            array2d_list.append(array2d_matrix)

    tensor = np.stack(array2d_list, axis=2)
    time_array = np.median(tensor, axis=2)

    X, Y, Z = [], [], [];
    a=0
    for i in time_array:
        b=0
        for j in i:
            X.append(a)
            Y.append(b)
            bandwidth = data_size[d_size]/(j*1024*1024)
            Z.append(bandwidth)
            b = b+1
        a = a+1

    data = pd.DataFrame({'X': X, 'Y': Y, 'Z': Z})
    data_pivoted = data.pivot("Y", "X", "Z")
    ax = sns.heatmap(data_pivoted)
    if d_size == 0:
        plt.savefig("plot-64.png".format(d_size))
    elif d_size == 1:
        plt.savefig("plot-512.png".format(d_size))
    else:
        plt.savefig("plot-2.png".format(d_size))

    """
    Code for plotting zeroed out central line
    """

    # fig = plt.figure(figsize=(13,13))

    # X, Y, Z = [], [], [];
    # a=0
    # for i in time_array:
    #     b=0
    #     for j in i:
    #         X.append(a)
    #         Y.append(b)
    #         if a == b:
    #             Z.append(0)
    #         else:
    #             bandwidth = data_size[d_size]/(j*1024*1024)
    #             Z.append(bandwidth)
    #         b = b+1
    #     a = a+1

    # data = pd.DataFrame({'X': X, 'Y': Y, 'Z': Z})
    # data_pivoted = data.pivot("Y", "X", "Z")
    # ax = sns.heatmap(data_pivoted)
    # if d_size == 0:
    #     plt.savefig("plot-64_zeroed.png".format(d_size))
    # elif d_size == 1:
    #     plt.savefig("plot-512_zeroed.png".format(d_size))
    # else:
    #     plt.savefig("plot-2_zeroed.png".format(d_size))
