#include <vector>

#include "bignum.hpp"

bool isValidInteger(const std::string& str) {
    for (char ch : str) {
        if (!std::isdigit(ch)) return false;
    }
    return !str.empty();
}

int main(int argc, char *argv[])
{
  bool isValid = true;
  std::string operation = argv[1];
  std::string num1 = argv[2];
  std::string num2 = argv[3];
  std::string num3 = "";

  if (operation == "+" || operation == "-" || operation == "*" || operation == "/" || operation == "%") {
    if (argc != 4) {
      std::cout << "Error: +-*/\% requires two numbers" << std::endl;
      return 1;
    }
    
  } else if (operation == "^") {
    if (argc != 5) {
      std::cout << "Error: Exponent requires three numbers" << std::endl;
      return 1;

    }

    num3 = argv[4];

  } else {
    std::cout << "Error: \"" << operation << "\" is not a supported operator" << std::endl;
    return 1;
  }

  if (!isValidInteger(num1)) {
    std::cout << "Error: \"" << num1 << "\" is not an unsigned integer" << std::endl;
    isValid = false;
  }

  if (!isValidInteger(num2)) {
    std::cout << "Error: \"" << num2 << "\" is not an unsigned integer" << std::endl;
    isValid = false;
    
  }
  if (operation == "^" && !isValidInteger(num3)) {
    std::cout << "Error: \"" << num3 << "\" is not an unsigned integer" << std::endl;
    isValid = false;
  }

  if (!isValid) {
    return 1;
  } else {
    std::cout << "bignum " << operation << " " << num1 << " " << num2;
    std::cout << ( (operation == "^") ? (" " + num3) : "") << std::endl;

    Bignum bigNum1(num1);
    Bignum bigNum2(num2);
    Bignum result;
    if (operation == "+") {
      result = bigNum1 + bigNum2;
      std::cout << result.to_string() << std::endl;
    }
    else if (operation == "-") {
      result = bigNum1 - bigNum2;
      std::cout << result.to_string() << std::endl;
    } else if (operation == "*") {
      result = bigNum1 * bigNum2;
      std::cout << result.to_string() << std::endl;

    } else if (operation == "/") {
      result = bigNum1 / bigNum2;
      std::cout << result.to_string() << std::endl;
      
    } else if (operation == "%") {
      result = bigNum1 % bigNum2;
      std::cout << result.to_string() << std::endl;
      
    } else {
      result = bigNum1.modexp(bigNum2, Bignum(num3));
      std::cout << result.to_string() << std::endl;

    }

  }
  return 0;

}