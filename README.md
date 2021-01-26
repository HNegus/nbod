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
| C | Center the camera  |
| G | Show/hide GUI |
| T | Enable/disable tracking
| V | Enable/disable variable timesteps |
| L | Toggle between 60 and 60+ fps |

#### GUI
The top menu bar allows you to save/load worlds (see [loading and saving](#loading-and-saving)).

##### Body/camera control

This panel shows the bodies in the simulation. Here body parameters such as position or velocity can be tweaked and new bodies can be added or removed.

The button 'Fit camera' scales the camera to include all bodies.

##### Configuration

Here you can set the scene name (for saving).

Delta position/radius/etc. control the step size for these parameters in the body control panel.

The history sampling resolution controls how often a body location is saved to draw trajectories to the screen (increasing leads to better performance, but lower accuracy).

Auto-resizing the camera scales it to include all bodies on screen.

Variable timesteps can improve the accuracy of the simulation (timestep is lower if bodies are closer).

If 'start logging' is pressed, the simulation saves positions and velocities to file at every timestep.

##### Info

Displays data about the simulation, such as camera position.

## Loading and saving

In order to define initial conditions for the simulator you can create a directory with the name of the world (such as 'solar_system') in the directory 'saves' or 'scenes'. In this directory you must then add a file world.cfg. For example, the full path can become:
```
-scenes
    - solar_system
        - world.cfg
```

The \*.cfg file **must** be formatted as follows (or the program will hang):
```
[id]
[name]
[x position] [y position] [z position]
[x velocity] [y velocity] [z velocity]
[radius]
[mass]
[R] [G] [B] [A]
---
[id]
[name]
[x position] [y position] [z position]
[x velocity] [y velocity] [z velocity]
[radius]
[mass]
[R] [G] [B] [A]
---
```

ID's must be a unique integer and RGBA should be integers from the range 0-255
representing color values;
See the scenes directory for examples.

Worlds created by users in the simulator are always saved to the 'saves' directory.

#### Logs

Log files have a different structure:
```
[N]
[Name 1] [Name 2]
[t0] [x1] [y1] [z1] [vx1] [vy1] [vz1] [x2] [y2] [z2] [vx2] [vy2] [vz2]
[t1] [x1] [y1] [z1] [vx1] [vy1] [vz1] [x2] [y2] [z2] [vx2] [vy2] [vz2]
[t2] [x1] [y1] [z1] [vx1] [vy1] [vz1] [x2] [y2] [z2] [vx2] [vy2] [vz2]
```

Where `[N]` is the number of bodies, followed by their names.
The next lines log all body positions and velocities for each time step.

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

- Some code OpenGL code is taken from tutorials by [The Cherno](https://www.youtube.com/watch?v=W3gAzLwfIP0&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2)
- The actual n-body interactions are calculated using [code by Piet Hut](https://www.ias.edu/ids/~piet/act/comp/algorithms/starter)
- Initial conditions for the scene 'solar_system' and data used in the experiments are from
  [NASA's HORIZONS system](https://ssd.jpl.nasa.gov/?horizons)

## Notes
- The simulation is limited to 60fps per second. Uncheck the 'limit framerate'
option to speed up the simulation.
- Currently, the lines showing body trajectories are quite inefficient. Disable drawing
them, or increase the rate at which a location is sampled to speed up computation.


#### Known bugs
- File dialog staying open after saving
- Program hangs when loading incorrectly formatted configuration files is wrongly
