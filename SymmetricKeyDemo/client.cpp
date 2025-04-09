#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <string>
#include "AESClass.h"

#pragma comment(lib, "ws2_32.lib")

extern std::string generateEncryptedPackageForClient(const std::string& idA, const std::string& idB, std::string& ticketCipherOut);

std::string simulateContactKDC(std::string& ticketDecryptedOut) {
    std::string clientKeyHex = "00112233445566778899AABBCCDDEEFF";
    std::string ticketEncrypted;
    std::string encryptedPackage = generateEncryptedPackageForClient("Client", "Server", ticketEncrypted);

    // Decrypt the package
    AESClass clientAES;
    clientAES.setKey(clientKeyHex);

    std::string decryptedPackage;
    if (!clientAES.decryptString(encryptedPackage, decryptedPackage)) {
        std::cerr << "[Client] Failed to decrypt package from KDC.\n";
        return "";
    }

    std::cout << "[Client] Package from KDC (decrypted): " << decryptedPackage << std::endl;

    // Parse: sessionKey, idB, ticket
    size_t p1 = decryptedPackage.find(",");
    size_t p2 = decryptedPackage.find(",", p1 + 1);
    if (p1 == std::string::npos || p2 == std::string::npos) {
        std::cerr << "[Client] Invalid package format.\n";
        return "";
    }

    std::string sessionKey = decryptedPackage.substr(0, p1);
    std::string idB = decryptedPackage.substr(p1 + 1, p2 - p1 - 1);
    std::string ticketEncryptedBase = decryptedPackage.substr(p2 + 1);

    // Decrypt the ticket to display its contents (for debugging)
    //AESClass debugAES;
    //debugAES.setKey("AABBCCDDEEFF00112233445566778899"); // Server's key
    //std::string ticketPlain;
    //if (!debugAES.decryptString(ticketEncryptedBase, ticketPlain)) {
    //    std::cerr << "[Client] Failed to decrypt ticket for inspection.\n";
    //} else {
    //    std::cout << "[Client] Ticket for Server (Decrypted for debug): " << ticketPlain << std::endl;
    //}

    // Set ticket to send (in encrypted form)
    ticketDecryptedOut = ticketEncryptedBase;
    return sessionKey;
}

int main() {
    std::string ticket;
    std::string sessionKey = simulateContactKDC(ticket);
    if (sessionKey.empty()) return 1;

    // Start socket
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server;

    WSAStartup(MAKEWORD(2, 2), &wsa);
    sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(8888);

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        std::cerr << "[Client] Connection failed.\n";
        return 1;
    }

    // Input message
    std::string plaintext;
    std::cout << "[Client] Enter your message: ";
    std::getline(std::cin, plaintext);

    // Encrypt with session key
    AESClass sessionAES;
    sessionAES.setKey(sessionKey);
    std::string ciphertext;
    sessionAES.encryptString(plaintext, ciphertext);

    std::cout << "[Client] Session Key: " << sessionKey << std::endl;
    std::cout << "[Client] Plaintext: " << plaintext << std::endl;
    std::cout << "[Client] Ciphertext: " << ciphertext << std::endl;

    std::string finalMessage = ticket + "|" + ciphertext;
    std::cout << "[Client] Final message to send: " << finalMessage << std::endl;

    send(sock, finalMessage.c_str(), finalMessage.size(), 0);
    std::cout << "[Client] Sent message to server.\n";

    closesocket(sock);
    WSACleanup();
    system("pause");
    return 0;
}
