#pragma once
#include "bignum.hpp"

bool isValidInteger(const std::string& str);

Bignum twoArgOperation(std::string num1, std::string num2, std::string operation);

Bignum encryption(std::string num1, std::string num2, std::string num3);
// std::string decryption(Bignum num1, Bignum num2, Bignum num3);
// std::string decryption(std::string num1, std::string num2, std::string num3);
// std::string decryption(std::string& line);
std::string decryption(std::string& line, std::string rsa_d, std::string rsa_n);

int prev_main(int argc, char *argv[]);