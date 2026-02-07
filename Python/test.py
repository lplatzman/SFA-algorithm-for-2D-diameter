# test.py

import sfadiameter

points = (
    (0, 0),
    (0.5001, 0.8661),
    (0.5424, 0.8399),
    (-0.5001, 0.8661),
    (-0.5424, 0.8399)
)

epsilon = 0.1

fsa = sfadiameter.compute(points, epsilon)
fsa.print()

exact = sfadiameter.computeExact(points)
exact.print()

print("eps = ", epsilon)
print ("err = ", (1 - fsa.diameter/exact.diameter))

