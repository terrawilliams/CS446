# Project 3
```

## Running
To run this project, the configuration file and metadata file should be stored with all of the other files. The name of the configuration file should be entered as a command-line argument.

For example, if the configuration file were called "simConfig.conf", then the following command will run the project.

./sim3 simConfig.conf


### Makefile Instructions
The building of the project is done using CMake, installation with apt-get or brew may be necessary. Enter the following commands to run the project:

make
./sim3 simConfig.conf

###PCB
The PCB class was used to keep track of what state the program was in at all times.  When the simulator begins, the state of the PCB is "New." When a process is being run, the PCB is in the "Running" state. When no process is running, the PCB is in the "Ready" state. When the program is waiting for an I/O device, the PCB is in the "Waiting" state. Finally, at the end of the simulation, the PCB is set to the "Terminated" state.  The PCB was created in main.cpp on line 23.

###Threads
The pthreads were used to simulate the operating system's handling of I/O devices. The pthreads were created in main.cpp on lines 217 and 238.
