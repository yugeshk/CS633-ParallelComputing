from matplotlib import pyplot as plt
import numpy as np

data_size = [128, 1024, 65536, 1048576, 4194304]
times = []
speeds = []
for d_size in [0,1,2,3,4]:
    filename = "{}.txt".format(d_size)
    data_file = open(filename, "r")
    line = data_file.readline()
    time1 = []
    speed1 = []
    while line:
        m_time = max(list(map(float, line.split(','))))
        time1.append(m_time)
        speed1.append(data_size[d_size]*100/(1024*1024*m_time))
        line = data_file.readline()

    times.append(time1)
    speeds.append(speed1)



print(speeds)
colors = ["red", "blue", "green", "cyan", "orange", "purple"]
plt.figure(figsize=(12,12), dpi=80)
plt.plot(range(1,6), np.array([np.median(j) for j in speeds]), color = colors[3])
plt.boxplot(speeds, labels=[round(i/(1024*1024), 4) for i in data_size], medianprops={'color': colors[0]})

plt.xlabel("Size in MB")
plt.ylabel("Speed of transfer in MBps")
plt.title("Plot for Assignment 1.1")
plt.savefig("plot1.png")
