#pragma once
#include "bignum.hpp"

bool isValidInteger(const std::string& str);

Bignum twoArgOperation(std::string num1, std::string num2, std::string operation);

Bignum encryption(std::string num1, std::string num2, std::string num3);

int prev_main(int argc, char *argv[]);