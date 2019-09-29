# This folder contains the following files:
1. src.c - contains the source code
2. plot.py - makes the plot : needs seaborn
3. Makefile - compiles the program
4. run.sh - runs the program 10 times across all configurations (please modify
   the _seq_ statement in pinger.sh for selecting hosts as required.)
5. pinger.sh - takes 30 responding hosts starting from 1.
6. clean\_files.sh - cleans up the directory of the timestamp files.
7. hostfile.txt - a list of hosts

The output of run.sh are a bunch of x.txt files (for each data size) that contains old Bcast,new Bcast times as
a csv file. The plot is saved as plot-P.png

For viewing, data and plot from a particular run of the program is included. 

## Observations

For the plot of P=8, ppn=8 we notice that the plot has high variance and is skewed. That is because all communications happen locally for that configuration. Our implementation of Bcast performs slightly better than the generic implementation due of specificity, in the sense that it is curated to the current data transfer setup in terms of data types etc.
