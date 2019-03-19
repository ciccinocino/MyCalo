These are the instruction to compile and
execute the program `MyCalo`.

First, create and navigate into a build directory:
```
$ mkdir build/
$ cd build/
```
Then, to check if the requested tools are available,
and to copy the files into the directory, execute:
```
$ cmake -DGeant4_DIR=/path/to/geant4 ..
```
Once done, compile with:
```
$ make [-jN]  
```
To run the program, simply launch:
```
$ ./MyCalo
```
This will show the calorimeter setup
(if the visualization libraries are installed)
and the GUI to interact with the simulation.
From there, it is possible to select different
primary particles, energies, and to initiate a simulation.

Alternatively, it is possible to immediately
start the simulation with a predefined macro, with the command:
```
$ ./MyCalo run1.mac
```
