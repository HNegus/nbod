from constants import *
import sys
from os import makedirs
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from scipy.signal import find_peaks


def get_data(filename):

    with open(filename, 'r') as f:
        n = int(f.readline())
        names = f.readline().strip().split(" ")

        data = {name: {'x': [], 'y': [], 'z': [], 'vx': [], 'vy': [], 'vz': []} for name in names}
        T = []

        for line in f.readlines():
            line = [float(x) for x in line.strip().split(" ")]

            T.append(line.pop(0))
            for name in names:
                data[name]['x'].append(line.pop(0))
                data[name]['y'].append(line.pop(0))
                data[name]['z'].append(line.pop(0))

                data[name]['vx'].append(line.pop(0))
                data[name]['vy'].append(line.pop(0))
                data[name]['vz'].append(line.pop(0))
    return np.array(T), data


def print_orbital_periods(name, X, Y, Z, T):

    Xpeaks = find_peaks(X)[0]
    Ypeaks = find_peaks(Y)[0]
    Zpeaks = find_peaks(Z)[0]

    if len(Xpeaks) >= 2:
        Xpeaks = np.mean((T[Xpeaks[1:]] - T[Xpeaks[:-1]]))
    else:
        Xpeaks = 0

    if len(Ypeaks) >= 2:
        Ypeaks = np.mean((T[Ypeaks[1:]] - T[Ypeaks[:-1]]))
    else:
        Ypeaks = 0

    if len(Zpeaks) >= 2:
        Zpeaks = np.mean((T[Zpeaks[1:]] - T[Zpeaks[:-1]]))
    else:
        Zpeaks = 0

    Xpeaks /= 60
    Ypeaks /= 60
    Zpeaks /= 60

    print(f"""{name} estimated orbital period:
Minutes ---------
x:  {Xpeaks}
y:  {Ypeaks}
z:  {Zpeaks}
""")

    Xpeaks /= 60
    Ypeaks /= 60
    Zpeaks /= 60

    print(f"""
Hours   ---------
x:  {Xpeaks}
y:  {Ypeaks}
z:  {Zpeaks}
""")


    Xpeaks /= 24
    Ypeaks /= 24
    Zpeaks /= 24

    print(f"""
Days    ---------
x:  {Xpeaks}
y:  {Ypeaks}
z:  {Zpeaks}
""")

    Xpeaks /= 365
    Ypeaks /= 365
    Zpeaks /= 365

    print(f"""
Years   ---------
x:  {Xpeaks}
y:  {Ypeaks}
z:  {Zpeaks}
""")


def plot_orbits_3d(filename, show, save):
    T, data = get_data(filename)
    names = list(data.keys())

    for name in names:
        X = np.array(data[name]['x'])
        Y = np.array(data[name]['y'])
        Z = np.array(data[name]['z'])

        fig = plt.figure()
        ax = fig.gca(projection='3d')
        ax.plot(X, Y, Z)

        plt.title(f"Orbit over time for {name}")
        ax.set_xlabel('x (m)')
        ax.set_ylabel('y (m)')
        ax.set_zlabel('z (m)')


        if show:
            plt.show()
        plt.close()


        plt.title(f"{name}'s position over time")
        plt.plot(T, X, label='x position')
        plt.plot(T, Y, label='y position')
        plt.plot(T, Z, label='z position')

        print_orbital_periods(name, X, Y, Z, T)

        plt.xlabel('time (s)')
        plt.ylabel('position')

        plt.legend()
        if show:
            plt.show()

        if save:
            plt.savefig(f"{name}-orbit3d.png", dpi=DPI)
        plt.close()

def plot_orbits_2d_onefig(filename, show, save):
    T, data = get_data(filename)
    names = list(data.keys())
    x, y, index = (len(names) // 3) + 1, len(names)//3, 1

    for name in names:
        X = np.array(data[name]['x'])
        Y = np.array(data[name]['y'])

        plt.subplot(x, y, index)
        index += 1
        plt.title(f"{name}")
        plt.plot(X, Y)


    plt.tight_layout()
    if show:
        plt.show()

    if save:
        plt.savefig(f'data/orbits_flat.png', dpi=DPI)
    plt.close()


def main(filename, show, save):
    plot_orbits_3d(filename, show, save)
    plot_orbits_2d_onefig(filename, show, save)

if __name__ == '__main__':
    usage = f"""
usage: python3 {sys.argv[0]} <logfile> [show] [help]

Required parameters:
<logfile>: string - name of logfile to analyse

Optional parameters:
[help]: string - show help
[show]: int    - (1) DEFAULT: show graphs ;
                 (0) show nothing;
                 (2) show and save graphs;
                 (3) save graphs

Example: python3 {sys.argv[0]} example.log 0 1

Description:


"""

    for param in sys.argv:
        if param == 'help':
            print(usage)
            exit(0)

    if len(sys.argv) < 2:
        print(usage)
        exit(1)

    filename = sys.argv[1]
    show = 1
    if len(sys.argv) == 3:
        show = int(sys.argv[2])
    print(show)
    show_plot, save_plot = True, False

    if show == 0:
        show_plot = False
    if show == 2:
        save_plot = True
    elif show == 3:
        show_plot = False
        save_plot = True

    # exit(0)
    try:
        makedirs('data', exist_ok=True)
        main(filename, show_plot, save_plot)
    except IOError as e:
        print(e)
