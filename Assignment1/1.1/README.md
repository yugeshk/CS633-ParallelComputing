# This folder contains the following files:
1. src.c - contains the source code
2. plot1.py - makes the plot : needs matplotlib
3. Makefile - compiles the program
4. run.sh - runs the program on two hosts : currently starts form 1 (can be
   altered to start from whereever by changing the starting ws from the _seq_
   bash command)
5. pinger.sh - takes 2 hosts starting from a particular host (currently set to 25) to create the hostfile

The output of run.sh are a bunch of x.txt files (for each data size) that contains sendtime,receivetime as
a csv file. The plot is saved as plot1.png

For viewing, data and plot from a particular run of the program is included. 

## Observations

The plot acheives almost the correct value of the bandwidth with is approx 125 MBps. Moreover, the variance is low suggesting that our experiment is resonably consistent for reproducibility.
