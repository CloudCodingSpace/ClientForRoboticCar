import socket

server = socket.socket()
server.bind(("0.0.0.0", 8080))
server.listen(1)

client, addr = server.accept()

while True:
    data = client.recv(1024)
    if not data:
        print("Client disconnected")
    else:
        decodedData = data.decode()

        if decodedData == '/q':
            break
        print(decodedData)
