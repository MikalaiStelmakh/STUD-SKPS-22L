from matplotlib import pyplot as plt
from pathlib import Path


for directory in Path('./results').iterdir():
    print(directory.name)

    task = int(directory.name[0])
    if task in [4, 5]:
        clients = []
        for client in sorted(directory.glob("cli_?.txt"), key=lambda f: f.name):
            with client.open(mode="r") as f:
                clients.append([int(line.split(" ")[3]) for line in f if line])

        range = (0, 400) if task == 5 else None
        plt.clf()
        plt.hist(clients, label=[
            f'Klient {num}' for num, _ in enumerate(clients)],  bins=20, range=range)

    else:
        servers = []
        for server in sorted(directory.glob("server_?.txt"), key=lambda f: f.name):
            with server.open(mode="r") as f:
                servers.append([int(line.split(' ')[2]) for line in f if line][1:])
        plt.clf()
        plt.hist(servers, label=['Bez zmian', 'Zmodyfikowane'], bins=20)

    plt.legend()
    plt.xlabel('Czas dostarczenia [us]')
    plt.ylabel('Liczba wystąpień')
    plt.savefig(directory / 'histogram.png')



