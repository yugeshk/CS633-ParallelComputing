# This folder contains the following files:
1. src.c - contains the source code
2. plot.py - makes the plot : needs seaborn
3. Makefile - compiles the program
4. run.sh - runs the program 10 times across all configurations (please modify
   the _seq_ statement in pinger.sh for selecting hosts as required.)
5. pinger.sh - takes 30 responding hosts starting from 1. (this is commented out
   in run.sh because a set S of hosts was provided. To reset is, uncomment this
   line)
6. clean\_files.sh - cleans up the directory of the timestamp files.
7. hostfile.txt - a list of hosts given in assignment as S

The output of run.sh are a bunch of x.txt files (for each data size) that contains a 30x30 matrix for communication time between each pair of nodes as a csv file. The plot is saved as plot-P.png

For viewing, data and plot from a particular run of the program is included. 

## Observations

From the plots it is clear that self send/recv has way more bandwidth (which
disallows us to make any substansial observations). For this reason, I have also
plotted the central line zeroed out. It shows a few clear trends -

1. Traffic in the network/busy nodes : since the bandwidth we get is
   significantly less than the theoretical maximum, we infer that others were
   using the cluster significantly as well (probably for their assignments).

2. Symmetry - The zeroed plots are almost symmetric as they should be because
   comm between two nodes should be roughly same in both directions.

3. Topology - we notice that bandwidth decreases with increasing distance (as
   the nodes in the KD lab are not a completely conntected graph), we see that
   nodes further away report less bandwidth.

The bandwidth observed is not as good as it should theoretically be - this could
also be because of the order in which the pairs are generated. It may so be the
case that the particular algorithm adopted somehow created a conjestion between
the nodes and cases delays. There may be better hashing methods as alternate
approaches.
