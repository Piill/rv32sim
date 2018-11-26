# RV32sim

This is a emulator for the RISC-V architecture, for the course 02155 (Computer Architecture and Engineering) at DTU

It implements the RV32I, excluding `ebreak`, `fence` and `fence.i`

## Building & Usage

Build the program by running:
````
make
````

This will place the executable as `./build/rv`

To run the program, use:
````
./rv -f /path/to/binary/program
````
The program also has a number of optional flags:

* `-i` runs the program in interactive mode. This prints the status of the simulation as it runs
* `-o /path/to/file ` saves the output of the simulation in the file given.
* `-s` overrides the stackpointer at the end of execution. This is to make later comparision independent of the stack/memory size.

## Testing

In the test-folder, 4 folders with tests are found.
Running `make tests` from the project-root will run the tests.

