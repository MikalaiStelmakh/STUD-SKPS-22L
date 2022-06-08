import socket
import matplotlib.pyplot as plt
import numpy as np
from math import radians


ADDRESS = '10.42.0.156'
PORT = 8080


def receive_measurement():
    data = str(soc.recv(1024))[2:-1].split(',')
    angle, distance = data
    print(f"angle={angle}, distance={distance}")
    return int(angle), int(distance)


def plotRadial():
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='polar' )
    ax.set_rmax(500)
    ax.set_title("Lidar", va='bottom')
    plt.draw()
    plt.pause(0.00000001)
    return fig, ax


def addPoint(angle, distance):
    plt.scatter([radians(angle)], [distance], marker='o', s=5, alpha=1, c='red')
    plt.draw()
    plt.pause(0.00000001)


if __name__ == "__main__":
    soc = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    soc.connect((ADDRESS, PORT))

    soc.send(b"")

    fig, ax = plotRadial()

    while plt.fignum_exists(fig.number):
        angle, distance = receive_measurement()
        if distance > 500:
            distance = 500
        if angle <= 0 or angle >= 180:
            ax.clear()
        addPoint(angle, distance)
