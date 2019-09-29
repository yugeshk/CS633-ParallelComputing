import matplotlib
matplotlib.use("Agg")
from matplotlib import pyplot as plt
import numpy as np
import seaborn as sns
import sys

fig = plt.figure(figsize=(13,13))
data_size = [65536, 262144, 524288, 2097152, 4194304]
for np in [8,16,32]:
    df = pd.DataFrame({"ppn": [], "data_size": [], "algo": [], "bandwidth": []})
    for ppn in [2,4,8]:
        for i in range(1,11):
            for d_size in data_size:
                filename = "{}_{}_time_{}.txt".format(np, ppn, i)
                with open(filename, "r") as data_file:
                    times_array = [[float(digit) for digit in line.split()] for line in data_file]
                    for t in times_array:
                        df = df.append({"ppn": ppn, "data_size": np, "algo": "bcast", "bandwidth": d_size*(np-1)/(t[0]*1024*1024)})
                        df = df.append({"ppn": ppn, "data_size": np, "algo": "new_bcast", "bandwidth": d_size*(np-1)/(t[1]*1024*1024)})


    catplot(kind="box", x="data_size", y="bandwidth", hue="algo", col="ppn", data=df)

    plt.savefig("plot-{}.png".format(np))
