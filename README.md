# Traffic-Simulator
Simulation using Traffic Cellular Automata.

The cars obey the following basic rules:

1. Cars always move from West to East (i.e., left to right).
2. The top lane is the first lane, and the bottom lane is the last lane. Considering the
direction of moving cars, lane-changing to the right (in the real-world) is equivalent to
moving to the lane below in our simulation.
3. Each car can move at most one cell for each unit time interval tick. Essentially, carfollowing is moving forward by one cell, and lane-changing is moving to the side cell.
However, in our assignment, only one action can be taken at a time, because the cars
cannot move more than one cell at a time.

