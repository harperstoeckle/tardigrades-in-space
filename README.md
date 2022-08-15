# Tardigrades in Space
This game uses an extremely basic terminal-based 3D software renderer. It was
originally created for the IB group 4 project in 2018.

## Building
Both curses and X11 must be installed on the system. While in the project
directory, issue the command
```
cmake -S . -B build && cmake --build build
```
which will create the executable `build/tardigrades`.
