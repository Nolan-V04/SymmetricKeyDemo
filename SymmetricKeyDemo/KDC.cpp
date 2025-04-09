#include <iostream>
#include <string>
#include <unordered_map>
#include "AESClass.h"

std::unordered_map<std::string, std::string> sharedKeys = {
    {"Client", "00112233445566778899AABBCCDDEEFF"},
    {"Server", "AABBCCDDEEFF00112233445566778899"}
};

std::string generateEncryptedPackageForClient(const std::string& idA, const std::string& idB, std::string& ticketCipherOut) {
    AESClass sessionAES;
    std::string sessionKeyHex = sessionAES.getKeyString();
    std::cout << "[KDC] Session Key: " << sessionKeyHex << std::endl;

    AESClass serverAES;
    serverAES.setKey(sharedKeys[idB]);
    std::string ticketPlain = sessionKeyHex + "," + idA;
    std::cout << "[KDC] Ticket Plain: " << ticketPlain << std::endl;
    serverAES.encryptString(ticketPlain, ticketCipherOut);
    std::cout << "[KDC] Encrypted Ticket: " << ticketCipherOut << std::endl;

    AESClass clientAES;
    clientAES.setKey(sharedKeys[idA]);
    std::string packagePlain = sessionKeyHex + "," + idB + "," + ticketCipherOut;
    std::string encryptedPackage;
    clientAES.encryptString(packagePlain, encryptedPackage);
    std::cout << "[KDC] Encrypted Package: " << encryptedPackage << std::endl;
    return encryptedPackage;
}

/* Client--->           KDC--->            Server
|                        |                    |
|----ID_A, ID_B----->    |                    |    
| < --Encrypted Package--|                    |
|                        |<-----Ticket------> |
|--------Ticket----------| --> Ticket         |
|<-- - Encrypted Msg-->  | <-- - Giao tiếp--> |
*/