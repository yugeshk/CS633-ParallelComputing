import matplotlib
matplotlib.use("Agg")
from matplotlib import pyplot as plt
import pandas as pd
import numpy as np
import seaborn as sns
import sys

#cse data1 plot
df=pd.read_csv("./cse/data1/data1_time.txt", names=["No. of Processes", "Timing", "Time in Seconds"])
sns.catplot(x="No. of Processes", y="Time in Seconds", row="Timing", data=df, kind="box", aspect=5)

plt.savefig("./cse/data1/plot.png")

#cse data2 plot
df=pd.read_csv("./cse/data2/data2_time.txt", names=["No. of Processes", "Timing", "Time in Seconds"])
sns.catplot(x="No. of Processes", y="Time in Seconds", row="Timing", data=df, kind="box", aspect=5)

plt.savefig("./cse/data2/plot.png")

#hpc plot 1
df=pd.read_csv("./hpc/data1/data1_time.txt", names=["No. of Processes", "Timing", "Time in Seconds"])
sns.catplot(x="No. of Processes", y="Time in Seconds", row="Timing", data=df, kind="box", aspect=5)

plt.savefig("./hpc/data1/plot.png")

#hpc plot 2
df=pd.read_csv("./hpc/data2/data2_time.txt", names=["No. of Processes", "Timing", "Time in Seconds"])
sns.catplot(x="No. of Processes", y="Time in Seconds", row="Timing", data=df, kind="box", aspect=5)

plt.savefig("./hpc/data2/plot.png")
