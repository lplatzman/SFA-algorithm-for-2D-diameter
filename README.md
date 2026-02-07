# SFA-algorithm-for-2D-diameter

The diameter of a set is the greatest distance between two points in the set. 
The problem of computing the diameter of a given set of points in two dimensions 
is well known, and just about everything you might want to learn about it 
can be found on Wikipedia. 
The eﬀort required to exactly compute the diameter D of a set P of n points in 2D 
is O(n log n).

Here we introduce an algorithm to compute an approximation D($`\epsilon`$), that lies within a given
factor $`\epsilon`$>0 of the true diameter, D. It is loosely based on a 2019 algorithm of Jieying Hong,
Zhipeng Wang and Wei Niu, posted [here](https://pmc.ncbi.nlm.nih.gov/articles/PMC6368283/). 
Their paper is entitled *A simple approximation algorithm for the diameter of a set of points in an Euclidean plane*. Accordingly, 
we have named our version of their algorithm SFA for Simpler, Faster and Approximate.

This repository contains C, Java and Python implementations of the algorithm, 
along with test programs to replicate an example provided in the [analysis page](/Analysis.md).
