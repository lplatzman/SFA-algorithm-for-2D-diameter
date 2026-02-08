# Analysis of the SFA algorithm

These notes are posted on [GitHub](https://github.com/lplatzman/SFA-algorithm-for-2D-diameter) along with implementations of the algorithm.

## The exact algorithm

Here is how the exact diameter of a set of n points, P, is computed:

Step 1.  Select a point in the convex closure of P that we will call the hub.  In principle, it doesn’t matter where the hub is located.  But traditionally, the hub is an extreme point of the given set, that minimizes y and then (among points that minimize y) minimizes x.

Step 2.  Sort the points according to their polar-coordinate angles, taking the hub as origin. 
  
Step 3.  Perform a [Graham scan]() of the sorted points to obtain their convex hull.  

Step 4.  Use the [method of rotating calipers]() to find the diameter of the convex hull.

___


Note:  Steps 1, 3 and 4 require O(n) operations.   Only Step 2 (sorting) requires O(n log n).

Note:  In this account, we have deliberately glossed over a difficulty that arises when computing the polar coordinates of the hub itself.  So does everyone else.  For details, see the code.

Note:  In times past, much was made of the calculation of polar coordinates, which requires trigonometric functions.   Strangely, there were no such qualms about the square root, which is also a transcendental function.   It hardly matters today, when these functions are evaluated by polynomial approximations, and the time to evaluate them is roughly the same as that of adding or multiplying.

# The approximate algorithm

In the new algorithm, Step 2 is replaced by a modified bin-sorting procedure that executes in O(n + 1/$`\epsilon`$) operations.  The other steps remain unchanged.  The diameter obtained in Step 4 is retained as D($`\epsilon`$).

The new Step 2 proceeds as follows:

a) Compute k = $`\left \lceil \frac{2 \pi}{\epsilon} \right \rceil `$.

b) Prepare k empty bins B[i], i = 0, … , k-1.

c)  For each given point p, compute a bin index i = 
$`\left \lfloor \frac{k \theta}{\pi} \right \rfloor`$, 
where 𝜃 is the angle component of the polar coordinate expression for p.  If B[i] is empty, place p in B[i].  If B[i] already contains a point, keep the point of greater radius r and discard the other. This requires O(n) operations.

d)  Construct a new list of points from the points in the bins traversed in sequence.  The new list will contain a subset of the original list, and will be sorted by 𝜃.  This requires O(1/$`\epsilon`$) operations.

This can be thought of as a partitioning algorithm (the bins induce a partition of the half-plane) or a modified bin-sorting algorithm.  Either way, the general idea is to use the bins to produce a reduced set of points, and compute its diameter exactly.   Because D($`\epsilon`$) is the diameter of a subset of the original points, D($`\epsilon`$) < D.  So D($`\epsilon`$) is not only an approximation; it is also a lower bound.
