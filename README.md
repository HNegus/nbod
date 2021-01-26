# Introduction

This repository includes a n-body gravitational simulation framework.

It was built for the course Project Computational Science of the University
of Amsterdam.

See the README in the 'scripts' directory for instructions on how to reproduce
the experiments.

# Project structure

```
- src
  - vendor
- scripts
- scenes
- saves
- logs
- shaders
```

- The 'src' directory contains all source files needed to build the executable, third
  party code is included in 'vendor'.
- The 'scripts' directory contains python files
- The 'scenes' directory contains pre-configured worlds that can be run with the
  simulator. See [loading and saving](#loading-and-saving) for an explanation of the format.
- Worlds built using the simulator are saved to the 'saves' directory. See [loading and saving](#loading-and-saving) for an explanation of the format.
- If logging is started, the data from the simulation is saved to the 'logs'
  directory. See [loading and saving](#loading-and-saving) for an explanation of the format.


# Usage

First [install the necessary dependencies](#dependencies-and-requirements)

The Makefile specifies the use of Clang++, but any C++ compiler should work.

After building, run with:

` ./nbod`


The program can also be run in headless mode. To do this, run

`./nbod <scene name>`

with a valid scene name. This will output results to a file in the log directory.


## Simulation control
#### Shortcuts
| Key | Effect |
|-----|----- |
| Arrow keys | Move left/right/up/down |
| Mouse wheel | Zoom in/out |
| Spacebar | Run/pause simulation
| H | Show/hide history
| G | Show/hide GUI |
| T | Enable/disable tracking

#### GUI
The top menu bar allows you to save/load configurations (see [loading and saving](#loading-and-saving)).

##### Body/camera control

##### Configuration

##### Info


## Loading and saving

## Dependencies and requirements

- Linux (tested on ubuntu 20.4)
- OpenGL 4.*
- GLFW
- GLEW

Run
`apt-get install libgl-dev libglu-mesa-dev libglfw3-dev libglew-dev`
to install the necessary libraries.

Install `mesa-utils` and run `glxinfo` to check which OpenGL version your
GPU supports.

Included in source directory and built with the project:
- Dear ImGui
- GLM
- stb

Change the Makefile to compile with anything other than clang++

# References

The Cherno
Piet Hut
NASA HORIZONS


# Contact

#### Known bugs
- File dialog staying open after saving
- Program hangs when loading incorrectly formatted configuration files is wrongly
