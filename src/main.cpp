#include <winsock2.h>
//#include <ws2tcpip.h>

#include <iostream>

int main() {
    std::string ip = "";
    int port = 0;

    std::cout << "Enter the ip address (don't include port) :- ";
    std::cin >> ip;
    std::cout << "Enter port :- ";
    std::cin >> port;

    WSADATA data{};
    WSAStartup(MAKEWORD(2, 2), &data);

    SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    sockaddr_in addr{};
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;

    if(connect(client, (sockaddr*)&addr, sizeof(addr)) != 0) {
        std::cerr << "Failed to connect to server!" << std::endl;
        return -1;
    }

    send(client, "Yooo!!", 6 * sizeof(char), 0);

    closesocket(client);
    WSACleanup();

    return 0;
}
