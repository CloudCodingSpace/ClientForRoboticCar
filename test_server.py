import socket

server = socket.socket()
server.bind(("0.0.0.0", 8080))
server.listen(1)

client, addr = server.accept()
print("Client connected!")

while True:
    data = client.recv(1024)
    decodedData = data.decode()

    if decodedData == '/q':
        print("Client disconnecting ... Shutting down server!")
        break
    print(decodedData)
