# SFA-algorithm-for-2D-diameter

This repository contains C, Java and Python implementations of a new algorithm to approximate
the diameter of a given set of n points in the plane, to within a given factor $`\epsilon`$.

More precisely:
* The diameter of a set is the greatest distance between two points in the set.
* The algorithm returns an approximation D($`\epsilon`$) to the true diameter D, such that | D($`\epsilon`$) - D | $`\leq`$ $`\epsilon\cdot D`$.
* It also returns a pair of points p and q in the given set, such that || p-q || = D($`\epsilon`$).

The algorithm is loosely based on a 2019 algorithm of Jieying Hong,
Zhipeng Wang and Wei Niu, posted [here](https://pmc.ncbi.nlm.nih.gov/articles/PMC6368283/). 
Their paper is entitled *A simple approximation algorithm for the diameter of a set of points in an Euclidean plane*. Accordingly, 
we have named our version of their algorithm SFA, for Simpler, Faster and Approximate.

More details of the algorithm can be found on the [Analysis page](/Analysis.md).
