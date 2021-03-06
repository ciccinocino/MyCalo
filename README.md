These are the instruction to compile and execute the program MyCalo.
First, create and navigate into a build directory:

    $ mkdir build/
    $ cd build/

Then, to check if the requested tools are available,
and to copy the files into the build directory, execute:

    $ cmake -DGeant4_DIR=/path_to_geant4 ..

Once done, compile with:

    $ make

To run the program, simply launch:

    $ ./MyCalo

This will show the calorimeter setup
(if the visualization libraries are installed)
and the GUI to interact with the simulation.
From there, it is possible to select different
primary particles, energies, and to initiate a simulation.
In the visualization neutrons are filtered
out to avoid overcrowding the screen.

Alternatively, it is possible to immediately
start the simulation with a predefined macro, with the command:

    $ ./MyCalo run.mac

Available macros are:
  - `run.mac`: sample macro for testing, only few events, saves to MyCalo.root;
  - `run1.mac`: macro for 100 GeV run, 3000 events, saves to MyCalo1.root;
  - `run2.mac`: macro for 115 GeV run, 3000 events, saves to MyCalo2.root;
  - `run3.mac`: macro for 130 GeV run, 3000 events, saves to MyCalo3.root;
  - `run4.mac`: macro for 145 GeV run, 3000 events, saves to MyCalo4.root;

The script `run_simulation.sh` runs all macros `run1.mac` to `run4.mac`
in sequence, saving the log with the execution time to `MyCalo{1,2,3,4}.log`
