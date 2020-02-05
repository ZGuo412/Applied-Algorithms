# Algorithms
project related to Greedy Algorithm, Dynamic Programming, Graph and network flow


PA1 : I implement a program about inserting inverters into an RC tree with two constrains.(maximum Elmore delay at each stage is under a specified time constraint; all leaf nodes of the binary tree after inverter insertion are non-inverting)

PA3: I implement a program about finding the nth shortest path in a graph; calculate the minimum cycle mean of the input graph and store a minimum cycle path in files. To do this, based on the Karp, I implement Hartmann-Orlin algorithm to do this.

Karp uses dynamic programming to calculate the MCM(minimum cycle mean in O(VE) time complexity. However, it will terminate only when all Vith edge shortest paths are calculated. So I used Hartmann Orlin instead and check whether I got the mcm at the kth edge. K is a power of 2.
