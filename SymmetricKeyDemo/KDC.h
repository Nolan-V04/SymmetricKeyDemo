#pragma once
#include <string>

using namespace std;

string generateEncryptedPackageForClient(const std::string& idA, const std::string& idB, std::string& ticketCipherOut);
