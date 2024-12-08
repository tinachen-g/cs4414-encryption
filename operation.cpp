#include <vector>

#include "operation.hpp"
#include "bignum.hpp"

bool isValidInteger(const std::string& str) {
    for (char ch : str) {
        if (!std::isdigit(ch)) return false;
    }
    return !str.empty();
}

Bignum twoArgOperation(std::string num1, std::string num2, std::string operation) {
  bool isValid = true;
  if (!isValidInteger(num1)) {
    std::cout << "Error: \"" << num1 << "\" is not an unsigned integer" << std::endl;
    isValid = false;
  }

  if (!isValidInteger(num2)) {
    std::cout << "Error: \"" << num2 << "\" is not an unsigned integer" << std::endl;
    isValid = false;
    
  }


  // if (!isValid) {
  //   return 1;
  // } else {
  std::cout << "bignum " << operation << " " << num1 << " " << num2;
  // std::cout << ( (operation == "^") ? (" " + num3) : "") << std::endl;

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
    
  } else {
    result = bigNum1 % bigNum2;
    std::cout << result.to_string() << std::endl;
    
  }
  return result;

  // }

}

Bignum encryption(std::string num1, std::string num2, std::string num3) { 
  Bignum bigNum1(num1);
  Bignum bigNum2(num2);
  Bignum result;
  result = bigNum1.modexp(bigNum2, Bignum(num3));
  // std::cout << result.to_string() << std::endl;
  return result;

}

// std::string decryption(Bignum num1, Bignum num2, Bignum num3) { 
//   std::string result = bigNum1.modexp(bigNum2, Bignum(num3));
//   // std::cout << result.to_string() << std::endl;
//   return result;

// }

// std::string decryption(std::string num1, std::string num2, std::string num3) { 
//   Bignum bigNum1(num1);
//   Bignum bigNum2(num2);
//   Bignum bigNum3(num3);
//   std::string result = bigNum1.modexp(bigNum2, Bignum(num3));
//   // std::cout << result.to_string() << std::endl;
//   return result;

// }

std::string decryption(std::string &line, std::string rsa_d, std::string rsa_n)
{
  std::cout << "operation.cpp line 90 " << std::endl;
  Bignum mod_line = Bignum(line).modexp(rsa_d, Bignum(rsa_n));
  std::string input = mod_line.to_string();
  std::cout << "operation.cpp line 93 " << std::endl;

  int padding_length = input.length() % 3;
  if (padding_length != 0)
  {
    int padding_needed = 3 - padding_length;
    input = std::string(padding_needed, '0') + input;
  }

  int i = 0;
  std::string result = "";
  while (i < input.length())
  {
    std::string triple = (input).substr(i, 3);

    int ascii_value = std::stoi(triple);
    // std::cout << "triple " << triple << std::endl;

    result += static_cast<char>(ascii_value);
    // std::cout << "curr " << result << std::endl;

    i += 3;
  }
  return result;
}



int prev_main(int argc, char *argv[])
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
