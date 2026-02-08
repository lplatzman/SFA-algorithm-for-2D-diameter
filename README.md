# SFA-algorithm-for-2D-diameter

The diameter of a set is the greatest distance between two points in the set. 
The problem of computing the diameter of a given set of points in two dimensions 
is well known, and just about everything you might want to learn about it 
can be found on Wikipedia. 
The eﬀort required to exactly compute the diameter D of a set P of n points in 2D 
is O(n log n).

This repository contains C, Java and Python implementations of a new algorithm to compute an approximation, 
D($`\epsilon`$), lying within a given
factor $`\epsilon`$>0 of the true diameter, D. The algorithm runs in O(n+1/$`\epsilon`$) time 
and is loosely based on a 2019 algorithm of Jieying Hong,
Zhipeng Wang and Wei Niu, posted [here](https://pmc.ncbi.nlm.nih.gov/articles/PMC6368283/). 
Their paper is entitled *A simple approximation algorithm for the diameter of a set of points in an Euclidean plane*. Accordingly, 
we have named our version of their algorithm SFA, for Simpler, Faster and Approximate.

More details of the algorithm can be found on the [Analysis page](/Analysis.md).
