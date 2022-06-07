import socket
import matplotlib.pyplot as plt
import numpy as np
from math import radians


ADDRESS = '10.42.0.156'
PORT = 8080


def receive_measurement():
    data = str(soc.recv(1024))[2:-1].split(',')
    angle, distance = data
    print(angle, distance)
    return int(angle), int(distance)


def plotRadial():
    fig = plt.figure()
    fig.add_subplot(111, projection='polar' )
    plt.draw()
    plt.pause(0.00000001)
    return fig


def addPoint(angle, distance):
    plt.scatter([radians(angle)], [distance], marker='o', s=1, alpha=1, c='red')
    plt.draw()
    plt.pause(0.00000001)


if __name__ == "__main__":
    soc = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    soc.connect((ADDRESS, PORT))

    soc.send(b"")

    fig = plotRadial()

    while plt.fignum_exists(fig.number):
        angle, distance = receive_measurement()
        addPoint(angle, distance)
