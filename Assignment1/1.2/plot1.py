from matplotlib import pyplot as plt
import numpy as np

data_size = [1024, 65536, 262144, 1048576]
times = []
speeds = []
for n_nodes in [2,4,8]:
    times1 = []
    speeds1 = []
    for d_size in [1,2,3,4]:
        filename = "{}_blocking_{}.txt".format(n_nodes, d_size);
        data_file = open(filename, "r")
        line = data_file.readline()
        time = []
        speed = []
        while line:
            time.append(float(line))
            speed.append(5*data_size[d_size-1]*100/(1024*1024*float(line)))
            line = data_file.readline()

        times1.append(time)
        speeds1.append(speed)

    times.append(times1)
    speeds.append(speeds1)

    times1 = []
    speeds1 = []
    for d_size in [1,2,3,4]:
        filename = "{}_unblocking_{}.txt".format(n_nodes, d_size);
        data_file = open(filename, "r")
        line = data_file.readline()
        time = []
        speed = []
        while line:
            time.append(float(line))
            speed.append(5*data_size[d_size-1]*100/(1024*1024*float(line)))
            line = data_file.readline()

        times1.append(time)
        speeds1.append(speed)

    times.append(times1)
    speeds.append(speeds1)

print(speeds)
colors = ["red", "blue", "green", "cyan", "orange", "purple"]
plt.figure(figsize=(12,12), dpi=80)
for i in range (0,6):
    plt.plot(range(1,5), np.array([np.median(j) for j in speeds[i]]), color = colors[i])
    plt.boxplot(speeds[i], labels=[round(i/(1024*1024), 4) for i in data_size], medianprops={'color': colors[i]})

plt.xlabel("Size in MB")
plt.ylabel("Speed of transfer in MBps")
plt.title("Plot for Assignment 1.2")
plt.savefig("plot1.png")
