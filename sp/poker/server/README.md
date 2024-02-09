# Poker Server application
## Intro

This project is used as a server for a multiplayer Poker game. 
It is part of a coursework for the subject “Introduction to Computer Networks” at the University of West Bohemia.
## Specification
- Language: **C++ (version 21)**
- Supported build system: **CMake**
- Connection protocol: **TCP**

## Run
### Required SW

- **Linux** OS
- **gcc** compiler
- **CMake** (version 3.22.1+)
- **liblog4cxx-dev** (sudo apt-get install liblog4cxx-dev) - is used as logging library for C++

### Configuration adjustment

- In the "CMakeLists.txt" file adjust **LOG4CXX_INCLUDE_DIRS** and **LOG4CXX_LIBRARIES** properties