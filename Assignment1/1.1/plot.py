from matplotlib import pyplot as plt

result_file = open("result.txt", "r")
line = result_file.readline()
times = []
while line:
    times.append(max(list(map(float, line.split(',')))))
    line = result_file.readline()

data_size = [128, 1024, 65536, 1048576, 4194304]
data_size = [d/(1024*1024) for d in data_size]
speed = [100*a/b for (a,b) in zip(data_size, times)]
plt.plot(data_size, speed)
plt.xlabel("Size in MB")
plt.ylabel("Speed of transfer in MBps")
plt.savefig("plot1.png")



