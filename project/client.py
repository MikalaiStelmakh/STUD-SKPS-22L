import socket

ADDRESS = '10.42.0.156'
PORT = 8080


def receive_measurement():
    data = str(soc.recv(1024))[2:-1].split(',')
    angle, distance = data
    print(angle, distance)


soc = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
soc.connect((ADDRESS, PORT))

soc.send(b"")

while True:
    receive_measurement()