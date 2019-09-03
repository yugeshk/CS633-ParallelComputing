# This folder contains the following files:
1. src.c - contains the source code
2. plot1.py - makes the plot : needs matplotlib
3. Makefile - compiles the program
4. run.sh - brings everything together
5. run\_2.sh - runs the scripts for 2 host requirement 
6. run\_4.sh - runs the scripts for 4 host requirement 
7. run\_8.sh - runs the scripts for 8 host requirement 
8. pinger.sh - takes 2 hosts starting from a particular host (currently set to 1) to create the hostfile

The output of run.sh are a bunch of x\_(un)blocking\_y.txt files (for each
node\_size _x_ and each data size _y_). Each file consists of 5 data times from
running the experiment 5 times. The plot is saved as plot1.png

For viewing, data and plot from a particular run of the program is included. 

## Observations

We notice that the plot goes beyond our expectations in terms of achieving
bandwidth - crosses 125. This suggests that there is some other factor that is
providing the speedup. On reflection we see that this is actually hapenning
because there is certain degree of intra-note interaction (and this is hapenning
locally, not over the network). That provides this artificial speedup. As the
number of processes in the experiment increases, it is expected that this should
come down (as is seen).
