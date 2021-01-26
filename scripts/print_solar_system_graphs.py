#! /bin/python3
from constants import *
import sys
import numpy as np
from os import makedirs
import matplotlib.pyplot as plt
from scipy.interpolate import interp1d
from mpl_toolkits.mplot3d import Axes3D


def test():

    n = 0
    data = {}
    names, T = [], []

    earth_moon = "logs/default-1611592863.log"
    sun_earth = "logs/sun_earth-1611597946.log"

    with open(sun_earth, 'r') as f:



        data = {name: {'x': [], 'y': []} for name in names}
        T = []
        for line in f.readlines():
            line = [float(x) for x in line.strip().split(" ")]

            T.append(line.pop(0))
            for name in names:
                data[name]['x'].append(line.pop(0))
                data[name]['y'].append(line.pop(0))
                line.pop(0)
                line.pop(0)

    T = np.array(T)
    MoonY = np.array(data['moon']['y'])
    EarthY = np.array(data['earth']['y'])

    MoonY -= EarthY
    EarthY /= np.max(EarthY)
    MoonY /= np.max(MoonY)

    # plt.plot(T, np.sin(1/(60*60*24*356.25) * T * 2*np.pi), label='true earth')
    # plt.plot(T, EarthY, label='earth')
    # plt.legend()
    # plt.show()

    # plt.plot(T, np.sin(1/(60*60*24*27) * T * 2*np.pi), label='true moon')
    # plt.plot(T, MoonY, label='moon')
    # plt.legend()
    # plt.show()

    MoonX = np.array(data['moon']['x'])
    EarthX = np.array(data['earth']['x'])

    MoonX -= EarthX
    EarthX /= np.max(EarthX)
    MoonX /= np.max(MoonX)

    # plt.plot(T, MoonX, label='moon')
    plt.plot(T, EarthX, label='earth')
    plt.plot(T, np.cos(1 / (60 * 60 * 24 * 356.25)
                       * T * 2 * np.pi), label='true earth')

    plt.legend()
    plt.show()

    # plt.plot(T, np.sin(1/(60*60*24*356.25) * T * 2*np.pi), label='true earth')
    # plt.plot(T, EarthY, label='earth')
    # plt.legend()
    # plt.show()

    # plt.plot(T, np.sin(1/(60*60*24*27) * T * 2*np.pi), label='true moon')
    # plt.plot(T, MoonY, label='moon')
    # plt.legend()
    # plt.show()

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

def show_mse_positions(labels, x, y, z):

    X = np.arange(len(labels))
    x = np.array(x) / METER_PER_AU
    y = np.array(y) / METER_PER_AU
    z = np.array(z) / METER_PER_AU
    width = 0.3

    fig, ax = plt.subplots()
    rects1 = ax.bar(X - width, x, width, label='x')
    rects2 = ax.bar(X, y, width, label='y')
    rects3 = ax.bar(X + width, z, width, label='z')

    ax.set_xticks(X)
    ax.set_xticklabels(labels)
    ax.autoscale(tight=True)
    ax.legend()

    plt.show()


def show_positional_error(filename, show=True, save=False):

    T_pred, data_pred = get_data(filename)
    T_act, data_act = get_data("solar_system_jpl.log")
    names = list(data_pred.keys())
    x, y, index = (len(names) // 3) + 1, len(names)//3, 1
    plt.subplot()
    for name in names:

        X_pred = np.array(data_pred[name]['x'])
        Y_pred = np.array(data_pred[name]['y'])
        Z_pred = np.array(data_pred[name]['z'])


        X_act = np.array(data_act[name]['x'])
        Y_act = np.array(data_act[name]['y'])
        Z_act = np.array(data_act[name]['z'])

        X = interp1d(T_pred, X_pred, kind='linear')
        Y = interp1d(T_pred, Y_pred, kind='linear')
        Z = interp1d(T_pred, Z_pred, kind='linear')

        X_err = np.abs(X_act - X(T_act))
        Y_err = np.abs(Y_act - Y(T_act))
        Z_err = np.abs(Z_act - Z(T_act))
        Err = ((X(T_act) - X_act)**2 + (Y(T_act) - Y_act)**2 + (Z(T_act) - Z_act) ** 2) ** 0.5
        Err /= METER_PER_AU
        # Max_distance = np.max((X_act ** 2 + Y_act ** 2 + Z_act ** 2)**0.5)
        # Err /= Max_distance

        plt.subplot(x, y, index)
        index += 1
        plt.title(name)
        plt.plot(T_act, Err)
        plt.ylim([0, 1])
        plt.yticks([0, 0.5, 1.0])
        plt.xlabel('Time (s)')
        plt.ylabel('Distance (AU)')

        # plt.plot(T_act, Y_act, label=name + ' actual')
    # plt.legend()
    plt.suptitle("Absolute distance between actual position and prediction")
    plt.tight_layout()
    if save:
        plt.savefig('data/distances.png', dpi=DPI)
    if show:
        plt.show()
    plt.close()


def show_velocity_error(filename, show=True, save=False):

    T_pred, data_pred = get_data(filename)
    T_act, data_act = get_data("solar_system_jpl.log")
    names = list(data_pred.keys())
    x, y, index = (len(names) // 3) + 1, len(names)//3, 1
    plt.subplot()
    for name in names:

        X_pred = np.array(data_pred[name]['vx'])
        Y_pred = np.array(data_pred[name]['vy'])
        Z_pred = np.array(data_pred[name]['vz'])


        X_act = np.array(data_act[name]['vx'])
        Y_act = np.array(data_act[name]['vy'])
        Z_act = np.array(data_act[name]['vz'])

        X = interp1d(T_pred, X_pred, kind='linear')
        Y = interp1d(T_pred, Y_pred, kind='linear')
        Z = interp1d(T_pred, Z_pred, kind='linear')


        X_err = np.abs(X_act - X(T_act))
        Y_err = np.abs(Y_act - Y(T_act))
        Z_err = np.abs(Z_act - Z(T_act))
        Err = ((X(T_act) - X_act)**2 + (Y(T_act) - Y_act)**2 + (Z(T_act) - Z_act) ** 2) ** 0.5

        plt.subplot(x, y, index)
        index += 1
        plt.title(name)
        plt.plot(T_act, Err)
        # plt.ylim([0, 0.4e-7])
        plt.ylim([0, 4e3])
        plt.yticks([0, 2e3, 4e3])
        plt.xlabel('Time (s)')
        plt.ylabel('Δv (m/s)')

    plt.suptitle("Difference in magnitude between actual and predicted velocity")
    plt.tight_layout()
    if save:
        plt.savefig('data/velocities.png', dpi=DPI)
    if show:
        plt.show()
    plt.close()


def show_orbits(filename, show=True, save=False):

    T_pred, data_pred = get_data(filename)
    T_act, data_act = get_data("solar_system_jpl.log")
    names = list(data_pred.keys())

    for name in names:

        X_pred = np.array(data_pred[name]['x'])
        Y_pred = np.array(data_pred[name]['y'])
        Z_pred = np.array(data_pred[name]['z'])

        X_act = np.array(data_act[name]['x'])
        Y_act = np.array(data_act[name]['y'])
        Z_act = np.array(data_act[name]['z'])

        # X = interp1d(T_pred, X_pred, kind='linear')
        # Y = interp1d(T_pred, Y_pred, kind='linear')

        # plt.subplot(x, y, index)
        fig = plt.figure()
        ax = fig.gca(projection='3d')
        plt.title(f"{name}'s orbit over time")
        ax.plot(X_pred, Y_pred, Z_pred, label='prediction')
        ax.plot(X_act, Y_act, Z_act, label='actual')
        ax.set_xlabel('x (m)')
        ax.set_ylabel('y (m)')
        ax.set_zlabel('z (m)')
        plt.legend()
        if save:
            plt.savefig(f"data/{name}-orbit.png", dpi=DPI)
        if show:
            plt.show()
        plt.close()
        # if index > 3:
            # exit(0)

        # plt.ylim([0, 0.4e-7])
        # plt.ylim([0, 4e3])
        # plt.yticks([0, 2e3, 4e3])
        # plt.xlabel('Time (s)')
        # plt.ylabel('Δv (m/s)')

    # plt.suptitle("Difference in magnitude between actual and predicted velocity")
    # plt.tight_layout()
    # plt.show()

def main(filename, show, save):

    show_positional_error(filename, show, save)
    show_velocity_error(filename, show, save)
    show_orbits(filename, show, save)



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

Example: python3 {sys.argv[0]} example.log 1

Description:

NOTE: This program requires the existence of a file called solar_system_jpl.log
Make this file by running 'download_solar_orbits.py'
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
