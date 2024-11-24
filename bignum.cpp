#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>
#include<cmath>

#include "bignum.hpp"

// bool Bignum::operator<(const BigNum &other) const
// {
//     // Compare by length first, and if equal, then by nucleotide sequence.
//     return (this->getLength() < other.getLength()) ||
//            (this->getLength() == other.getLength() && this->getNucleotideSequence() < other.getNucleotideSequence());
// }

Bignum::Bignum(){
  this->data = {'0'};
}

/** Creates a bignum, assuming that the string input is already valid, as 
already checked in main. */
Bignum::Bignum(const std::string& data) {
  bool leadingZero = true;
  for (auto& digit : data) {
    if (leadingZero && digit == '0') {
        continue;
    }
    leadingZero = false;
    this->data.emplace_back(digit);
  }
  if (this->data.size() == 0) {
    this->data.emplace_back('0');
  }
}

Bignum Bignum::operator+(const Bignum& other) const {
  if (this->check() && other.check()) {
    bool carry = false;
    std::string result;
    auto rit1 = this->getNum().rbegin();
    auto rit2 = other.getNum().rbegin();


    for (; rit1 != this->getNum().rend() && rit2 != other.getNum().rend(); rit1++, rit2++) {
      int sum = (*rit1 - '0') + (*rit2 - '0') + (carry ? 1 : 0);
      result = std::string(1, (sum % 10) + '0') + result;
      carry = sum > 9;
    }
    // for (int i = this->getNum().size() - 1, j = other.getNum().size() - 1; i >= 0 && j >= 0; --i, --j) {
    //   int sum = (*rit1 - '0') + (*rit2 - '0') + extra;
    // }
    while (rit1 != this->getNum().rend()) {
        int sum = (*rit1 - '0') + (carry ? 1 : 0);
        result = std::string(1, (sum % 10) + '0') + result;
        carry = sum > 9;
        ++rit1;
    }

    while (rit2 != other.getNum().rend()) {
        int sum = (*rit2 - '0') + (carry ? 1 : 0);
        result = std::string(1, (sum % 10) + '0') + result;
        carry = sum > 9;
        ++rit2;
    }
    result = std::string(1, (carry ? 1 : 0) + '0') + result;

    return Bignum(result);
  } else {
    std::cout << "HELLPPPPPPPPP" << std::endl;
    return Bignum();
  }
  
};

Bignum Bignum::operator-(const Bignum& other) const {
  // std::vector<std::string> vec = this->data ? this->data.length() > other.length() : 

  // bool carry = false;
  // for (std::vector<std::string>::reverse_iterator riter = this->data.rbegin(); riter != this->data.rend(); riter++) { 
  //   // do stuff
    

  // } 
  if (this->check() && other.check()) {
    if (*this < other) {
      std::cout << "Unsupported: Negative number" << std::endl;
      return Bignum();
    } else {
      bool borrow = false;
      std::string result;
      auto rit1 = this->getNum().rbegin();
      auto rit2 = other.getNum().rbegin();

      for (; rit1 != this->getNum().rend() && rit2 != other.getNum().rend(); rit1++, rit2++) {
        int extra = 0;
        if ((*rit1 - '0') - (*rit2 - '0') - (borrow ? 1 : 0) < 0) {
          extra = 10;
        }
        // std::cout << (*rit1 - '0') << " - " << (*rit2 - '0') << std::endl;
        int difference = (*rit1 - '0' + extra) - (*rit2 - '0') - (borrow ? 1 : 0);
        // std::cout << "difference: " << difference << std::endl;
        // std::cout << result << std::endl;
        result = std::string(1, difference + '0') + result;
        borrow = (extra == 10);
      }
      // for (int i = this->getNum().size() - 1, j = other.getNum().size() - 1; i >= 0 && j >= 0; --i, --j) {
      //   int sum = (*rit1 - '0') + (*rit2 - '0') + extra; 
      // }
      while (rit1 != this->getNum().rend()) {
        int extra = 0;
        if ((*rit1 - '0')- (borrow ? 1 : 0) < 0) {
          extra = 10;
        }
        int difference = (*rit1 - '0' + extra)- (borrow ? 1 : 0);
        // std::cout << "difference" << difference << std::endl;
        result = std::string(1, difference + '0') + result;
        borrow = (extra == 10);
        // std::cout << "reslt" << result << std::endl;
        rit1++;
      }

      // while (rit2 != other.getNum().rend()) {
      //   int extra = 0;
      //   if ((*rit1 - '0') - (*rit2 - '0') < 0) {
      //     extra = 10;
      //   }
      //   int difference = (*rit1 - '0' + extra) - (*rit2 - '0') - (borrow ? 1 : 0);
      //   result = std::string(1, difference + '0') + result;
      //   borrow = (extra == 10);
      // }

      return Bignum(result);

    }
  } else {
    std::cout << "HELLPPPPPPPPP" << std::endl;
    return Bignum();
  }

};

Bignum Bignum::operator*(const Bignum& other) const {
  if (this->check() && other.check()) {
    std::string result;
    const Bignum& longer = this->getNum().size() >= other.getNum().size() ? *this: other;
    const std::vector<char>& shorter = this->getNum().size() >= other.getNum().size() ? other.getNum() : this->getNum();
    // std::cout << "longer is: " << longer.to_string() << std::endl;
    Bignum product = Bignum();
    int tens = 0;
    for (auto rit1 = shorter.rbegin(); rit1 != shorter.rend(); rit1++, tens++) {
      // std::cout << "tens: " << tens<< std::endl;
      int digit = (*rit1 - '0');
      if (digit == 0) {
        continue;
      } else {
        Bignum temp = longer.multiplyBySingleDigit(digit);
        // std::cout << "after first: " << temp.to_string() << std::endl;
        for (int i = 1; i <= tens; i++) {
          // std::cout << "tens loops: " << temp.to_string() << std::endl;
          temp = temp.multiplyBySingleDigit(10);
          // std::cout << "tens loops: " << temp.to_string() << std::endl;
        }
        product = product + temp;
      }
    }

    return product;
  
  } else {
    std::cout << "HELLPPPPPPPPP" << std::endl;
    return Bignum();
  }

};
 
Bignum Bignum::operator/(const Bignum& other) const {
  return divide(other, "/");

};
 
Bignum Bignum::operator%(const Bignum& other) const {
  return divide(other, "%");

};

std::string decimalToBinary(int decimal) { 
  if (decimal == 0) { 
    return "0";
  } 
  if (decimal == 1) { 
    return "1";
  }
  return decimalToBinary(decimal / 2) + std::to_string(decimal % 2);
}

// Bignum Bignum::modexp(const Bignum& b, const Bignum& c) const {
  
//   std::string binary = decimalToBinary(std::stoi(b.to_string()));
//   // std::cout << "binary: " << binary << std::endl;
//   Bignum result = Bignum("1");
//   // for (int i = binary.length()-1; i >= 0; i--) {
//   for (int i = 0; i < binary.length(); i++) {
//     int index = (binary.length()-1) - i;

//     if (binary[index] == '1') {
//       Bignum mod_term = Bignum("1");
//       for (int exp = 0; exp < std::pow(2, i); exp ++) {
//         std::cout << "exp: " << exp << std::endl;
//         mod_term = mod_term * *this;
//       }
//       result = result * (mod_term % c);

//     }

//   }

//   return (result % c);

// };
Bignum Bignum::modexp(const Bignum& b, const Bignum& c) const {
  Bignum base = *this % c;  
  Bignum exponent = b;
  Bignum result = Bignum("1");

  while (exponent > Bignum("0")) {
    if (exponent % Bignum("2") == Bignum("1")) { 
      result = (result * base) % c;
  }
    base = (base * base) % c;
    exponent = exponent / Bignum("2");
  }
  return result;
}


bool Bignum::operator<(const Bignum& other) const {
  const std::vector<char>& num1 = this->getNum();
  const std::vector<char>& num2 = other.getNum();
  if (num1.size() != num2.size()) {
    // std::cout << "this is shorter than other: " <<( num1.size() < num2.size()) << std::endl;
    return num1.size() < num2.size();
  } else {
    // for (i = num1.length()-1; i >= 0; i--) { 

    // }
    for (int i = 0; i < num1.size(); i++) {
      // this workks bc a charcater of a number is the number + 48 (the ascii respresentation of '0')
      if ((num1[i] - '0') < (num2[i] - '0')) {
        return true;
      } else if ((num1[i] - '0') > (num2[i] - '0')) {
        return false;
      }
    }
    return false;
  }

};

bool Bignum::operator>(const Bignum& other) const {
  return !(*this == other) && !(*this < other);

};
bool Bignum::operator==(const Bignum& other) const {
  std::vector<char> num1 = this->getNum();
  std::vector<char> num2 = other.getNum();
  return num1 == num2;

};

Bignum Bignum::multiplyBySingleDigit(int digit) const{
  if (this->check()) {
    int carry = 0;
    std::string result;

    for (auto rit1 = this->getNum().rbegin(); rit1 != this->getNum().rend(); rit1++) {
      int product = ((*rit1 - '0') * digit ) + (carry);
      // can divide bt 10 and dont need to mod 100 since 1 digit x 1digit is max 81
      result = std::string(1, (product % 10) + '0') + result;
      carry = ((product / 10) % 10);
    }
    if (carry > 0) {
      result = std::string(1, carry + '0') + result;
    }
    // std::cout << "result is: " << result << std::endl;
    // if (digit == 10) {
    //   std::cout << "result is before adding 0: " << result << std::endl;
    //   result += '0';
    // }

    return Bignum(result);
  
  } else {
    std::cout << "HELLPPPPPPPPP" << std::endl;
    return Bignum();
  }

};

Bignum Bignum::divide(const Bignum& other, std::string op) const {
  if (this->check() && other.check()) {
    Bignum mod = Bignum();
    Bignum div = Bignum();

    if (other.to_string() == "0") {
      std::cout << "Error: Divide by zero" << std::endl;
      return Bignum();
    } else {
      
      std::string result;
      std::string current_remainder;  
      // Bignum remainder;
      std::string divisor = other.to_string();

      // interate through the dividend
      for (auto rit1 = this->getNum().begin(); rit1 != this->getNum().end(); rit1++) {
        current_remainder += *rit1;
        // std::cout << "remainder: " << current_remainder << std::endl;
        // int remainder = std::stoi(current_remainder);
        Bignum remainder = Bignum(current_remainder);
        Bignum divisor_b = Bignum(divisor);
        // std::cout << "265 remainder: " << remainder << std::endl;
        int digit = 0;

        // if prefix of dividend is larger than the divisor
        if (remainder > divisor_b || remainder == divisor_b) {
          // keep subtracting until the difference is smaller than the divisor
      
          while (remainder > divisor_b || remainder == divisor_b) {
            remainder = remainder - divisor_b;
            // std::cout << "remainder: " << remainder << std::endl;
            digit += 1;
          }
          current_remainder = remainder.to_string();
          // std::cout << "277 remainder: " << current_remainder << std::endl;
          // std::cout << "digit: " << digit << std::endl;
          result += std::to_string(digit);
          // std::cout << "result: " << result << std::endl;
        } else {
          result += std::to_string(digit);
          // shift right
          continue;
        }
      }
      mod = Bignum(current_remainder); 
      div = Bignum(result);
      if (op == "/") {
        return div;
      } else if (op == "%"){
        return mod;
      } else {
        std::cout << "Some error occurred. Op is not / or %" << std::endl;
        return Bignum();
      }
    }
  
  } else {
    std::cout << "HELLPPPPPPPPP" << std::endl;
    return Bignum();
  }

}

/** Checks whther a bignum is a valid number (digits are numbers 0-9)
 */
bool Bignum::check() const {
  for (const char& digit: this->data ) {
    if (!std::isdigit(digit)) return false;
  }
  return true;
}

std::string Bignum::to_string() const {
  std::string result = "";
  for (const char& digit : this->data) {
    if (result == "0") {
      result = digit;
    } else {
      result += digit;
    }
  }
  return result;
}

const std::vector<char>& Bignum::getNum() const {
  return this->data;
};