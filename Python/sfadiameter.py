# SFADiameter.py

"""
MIT License

Copyright (c) 2026 Loren Platzman

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
"""

import math

def compute(points, epsilon):
    return _Engine(points, epsilon).run()

def computeExact(points):
    r = SFAResult()
    d2best = 0

    for i in range(0, len(points)):
        for j in range(0, i):
            d2ij = _d2(points[i], points[j]);
            if d2ij > d2best:
                d2best = d2ij
                r.p1 = points[i]
                r.p2 = points[j]

    r.diameter = math.sqrt(d2best)
    return r

class _Bin:
    pass

class SFAResult:
    p1 = (42, 42)
    p2 = (42, 42)
    diameter = 42
    
    def print(self):
        print("p1 = ", self.p1)
        print("p2 = ", self.p2)
        print("diameter = ", self.diameter)
        print()

# Returns the square of the distance between points a and b.
# This avoids taking square roots when comparing distances.
def _d2(a, b):
    dx = a[0] - b[0]
    dy = a[1] - b[1]
    return dx * dx + dy * dy

# Returns the angle of the line segment from a to b.
# The angle ranges from 0 (right) to 2*pi (just before full circle).
# This makes it possible to compare angles in the algorithms
# without having to do modulo operations.
def _theta(a, b):
    atan = math.atan2(b[1] - a[1], b[0] - a[0])
    if atan >= 0:
        return atan
    else:
        return atan + 2 * math.pi

class _Engine:

    def __init__(self, points, epsilon):
        self.points = points
        self.epsilon = epsilon
        
    # Returns the index of the slice containing point p
    def binIndex(self, p):
        th = _theta(self.hub, p);
        return math.floor(self.k * th / math.pi)

    # Basic operation underlying the packer.
    # It is invoked from two places and so appears in its own function.

    def addToPacker(self, p):
        while self.m > 0:
            b = self.packer[self.m - 1];
            th1 = b.theta;
            th2 = _theta(b.point, p);
            if th2 > th1:
                break

            self.m -= 1

        b = _Bin()
        b.point = p;
        b.theta = th2
        self.packer[self.m] = b;
        self.m += 1

    # Accumulates pairs of points retaining the longest.
    def apply(self, i, j):
        a = self.packer[i].point
        b = self.packer[j].point
        d2ij = _d2(a, b)

        if d2ij > self.greatestDiameter2:
            self.r.p1 = a
            self.r.p2 = b
            self.greatestDiameter2 = d2ij
    
    def run(self):
        self.r = SFAResult()
        self.greatestDiameter2 = 0

        self.n = len(self.points)
        self.k = math.ceil(2 * math.pi / self.epsilon)

        #######################################################################
        # Compute hub

        self.hub = self.points[0];

        for p in self.points:
            if p[1] > self.hub[1]:
                continue;
            if p[1] < self.hub[1] or p[0] < self.hub[0]:
                self.hub = p

        #######################################################################
        # Sort (bin sort / each bin has a capacity of one)

        self.sorter = [None] * self.k

        for p in self.points:
            r2 = _d2(self.hub, p)
            if r2 == 0:
                continue

            i = self.binIndex(p)
            if self.sorter[i] == None:
                b = _Bin()
                b.point = p
                b.r2 = r2;
                self.sorter[i] = b
                continue
 
            b = self.sorter[i]
            if b.r2 >= r2:
                continue;

            b.point = p;
            b.r2 = r2

        #######################################################################
        # Pack (Graham scan)

        self.packer = [None] * (self.k + 3)

        b = _Bin()
        b.point = self.hub
        b.theta = -1E-15
        self.packer[0] = b

        self.m = 1

        for i in range(0, self.k):
            b = self.sorter[i]
            if b !=  None:
                self.addToPacker(b.point)
       
        match self.m:
            case 1:
                self.r.p1 = slef.r.p2 = self.hub
                self.r.diameter = 0
                return

            case 2:
                self.r.p1 = self.hub
                self.r.p2 = self.packer[1].point
                self.r.diameter = math.sqrt(d2(self.r.p1, self.r.p2))
                return

        self.addToPacker(self.hub)

        self.packer[0].theta = self.packer[self.m - 1].theta - 2 * math.pi

        b = _Bin()
        b.point = self.packer[1].point
        b.theta = self.packer[1].theta + 2 * math.pi
        self.packer[self.m] = b
        self.m = self.m + 1

        #######################################################################
        # Measure (rotating calipers)

        i = 0
        th = self.packer[1].theta + math.pi

        j = 2
        while self.packer[j + 1].theta < th:
            j = j + 1
      
        self.apply(i, j)

        while j < self.m - 1:
            if self.packer[i + 1].theta + math.pi < self.packer[j + 1].theta:
                i = i + 1
            else:
                j = j+1

            self.apply(i, j)
            
        #######################################################################

        self.r.diameter = math.sqrt(self.greatestDiameter2)

        return self.r
