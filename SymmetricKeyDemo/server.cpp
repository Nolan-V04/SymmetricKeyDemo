#include <iostream>
#include <winsock2.h>
#include "AESClass.h"

#pragma comment(lib, "ws2_32.lib")

int main() {
    std::string serverKeyHex = "AABBCCDDEEFF00112233445566778899";

    WSADATA wsa;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in server, client;
    int c;

    WSAStartup(MAKEWORD(2, 2), &wsa);
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    bind(serverSocket, (struct sockaddr*)&server, sizeof(server));
    listen(serverSocket, 3);

    std::cout << "[Server] Waiting for client...\n";
    c = sizeof(struct sockaddr_in);
    clientSocket = accept(serverSocket, (struct sockaddr*)&client, &c);
    std::cout << "[Server] Client connected.\n";

    char buffer[4096] = { 0 };
    int recv_size = recv(clientSocket, buffer, sizeof(buffer), 0);
    std::string received(buffer, recv_size);

    size_t split = received.find("|");
    if (split == std::string::npos) {
        std::cerr << "[Server] Invalid message format.\n";
        return 1;
    }

    std::string ticketCipher = received.substr(0, split);
    std::string encMessage = received.substr(split + 1);

    AESClass serverAES;
    serverAES.setKey(serverKeyHex);
    std::string ticketPlain;
    if (!serverAES.decryptString(ticketCipher, ticketPlain)) {
        std::cerr << "[Server] Failed to decrypt ticket.\n";
        return 1;
    }

    std::cout << "[Server] Ticket Decrypted: " << ticketPlain << std::endl;

    size_t sep = ticketPlain.find(",");
    if (sep == std::string::npos) {
        std::cerr << "[Server] Invalid ticket content.\n";
        return 1;
    }

    std::string sessionKeyHex = ticketPlain.substr(0, sep);
    std::string idA = ticketPlain.substr(sep + 1);
    std::cout << "[Server] Session Key: " << sessionKeyHex << std::endl;
    std::cout << "[Server] ID_A: " << idA << std::endl;

    AESClass sessionAES;
    sessionAES.setKey(sessionKeyHex);

    std::string decrypted;
    if (!sessionAES.decryptString(encMessage, decrypted)) {
        std::cerr << "[Server] Failed to decrypt message.\n";
        return 1;
    }

    std::cout << "[Server] Received from " << idA << ": " << decrypted << "\n";

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}