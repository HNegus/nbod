# Experiments

The point of this project is to predict the trajectory of Earth as accurately
as possible for two years, starting from January 1st 2021.

In order to accomplish this, we simulate the Newtonian gravitational
interactions between all major bodies in the solar system (the sun and all
planets + Pluto).

Run `pip3 install -r requirements.txt` to install required packages.

**The programs only show plots by default, but command line arguments can be
provided to save figures to a folder called 'data'**

#### download_solar_orbits.py

This program downloads the positions and velocities for all bodies over the time
period we are interested in. The data is downloaded from NASA's HORIZONS system.
The positions and velocities are with respect to
the solar system barycenter and stored in the log format used by the simulator.
The log file is called `solar_system_jpl.log`

#### download_solar_system.py

This program retrieves the initial conditions from for each body in our experiment
and saves it to a file `world.cfg` file understood by the compiler.

#### print_solar_system_graphs.py

This program takes a log file and compares it to the reference
file `solar_system_jpl.log`.

The file `example.log` is generated by running the simulator with the initial
conditions generated by `download_solar_system.py`.

Running `python3 print_solar_system_graphs.py example.log` will show and/or
save plots showing:

- the absolute difference for in position between the selected log file
and the reference file for each body
- The difference in velocity magnitude between the log file and the reference
- A 3D plot comparing the predicted path in space with the actual path for each
  body
-

**NOTE:** The data from the HORIZONS system is generated in timesteps of
30 minutes, while our simulator provides more granular data. In comparing the two
sets of data, we linearly interpolate our data to get a value for the time step of
interest.

#### plot_log.py

This program takes an arbitrary log file created by the simulator and shows and/saves:

- A 3D plot for every body showing the path through space
- A 2D plot showing the position over time for each coordinate (x, y, z) for each body


**NOTE:** The program also estimates orbital periods for each body in the log file and outputs this to stdout.

Run `python3 plot_log.py earth_moon.log` for an example
