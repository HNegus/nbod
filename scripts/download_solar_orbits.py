#! /bin/python3
from constants import *
import sys
import locale
import numpy as np
import pandas as pd
from datetime import datetime
from astroquery.jplhorizons import Horizons



def vec_to_numpy(vectors):
    arr = vectors[:]['datetime_str', 'x', 'y', 'z', 'vx', 'vy', 'vz'].to_pandas().to_numpy()
    arr[:,1:4] *= METER_PER_AU
    arr[:,4:] = arr[:,4:] * METER_PER_AU / SECONDS_PER_DAY
    return arr

def process_array(vectors):
    FORMAT = "A.D. %Y-%b-%d %H:%M:%S.%f"
    start = datetime.strptime(vectors[0][0], FORMAT)
    start = start.timestamp()
    for i in range(vectors.shape[0]):
        date = vectors[i][0]
        date = datetime.strptime(date, FORMAT)
        vectors[i][0] = date.timestamp() - start
    return vectors


def row_to_string(row):
    return " ".join([str(x) for x in row])


def download_year():
    bodies = {'Sun':        {'id': 10},
              'Mercury':    {'id': 1},
              'Venus':      {'id': 2},
               'Earth':     {'id': 3},
               'Mars':      {'id': 4},
               'Jupiter':   {'id': 5},
               'Saturn':    {'id': 6},
               'Uranus':    {'id': 7},
               'Neptune':   {'id': 8},
               'Pluto':     {'id': 9}}

    for body in bodies:
        print("Fetching", body)
        epochs = {'start': '2021-01-01', 'stop': '2023-01-01', 'step': '30m'}
        b = Horizons(id=bodies[body]['id'], location=LOCATION_CENTER,
                     id_type='majorbody', epochs=epochs)

        vectors = b.vectors()
        vectors = vec_to_numpy(vectors)
        vectors = process_array(vectors)
        bodies[body]['data'] = vectors

    return bodies


def data_to_lines(data, names):

    lines = []
    lines_per_body = []
    for body in names:
        lines_per_body.append(data[body]['data'])

    n_bodies = len(names)
    n_lines = lines_per_body[0].shape[0]

    for i in range(n_lines):
        t = lines_per_body[0][i][0]
        line = [t]
        for j in range(n_bodies):
            line += list(lines_per_body[j][i][1:])
        lines.append(" ".join([str(x) for x in line]))
    return lines


def main():
    try:
        locale.setlocale(locale.LC_TIME, ('en_US', 'UTF-8'))
    except locale.Error as e:
        print(e)
        print("Error setting locale to en_US.\nExiting.")
        exit(1)

    data = download_year()
    n = len(data)
    names = list(data.keys())

    lines = data_to_lines(data, names)

    with open('solar_system_jpl.log', 'w') as f:
        print(n, file=f)
        print(" ".join(names), file=f)
        for line in lines:
            print(line, file=f)


if __name__  == '__main__':
    usage = f"""
usage: python3 {sys.argv[0]} [help]

Optional parameters:
[help]: string - show this message

Example: python3 {sys.argv[0]}

Description:
"""

    for param in sys.argv:
        if param == 'help':
            print(usage)
            exit(0)
    main()
