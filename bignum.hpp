#pragma once

#include <string>
#include <iostream>
#include <vector>

class Bignum
{
public:
  Bignum();
  Bignum(const std::string& data);

  // bool operator<(const BigNum &other) const;
  Bignum operator+(const Bignum& other) const;
  Bignum operator-(const Bignum& other) const;
  Bignum operator*(const Bignum& other) const;
  Bignum operator/(const Bignum& other) const;
  Bignum operator%(const Bignum& other) const;
  Bignum modexp(const Bignum& b, const Bignum& c) const;


  bool operator<(const Bignum& other) const;
  bool operator>(const Bignum& other) const;
  bool operator==(const Bignum& other) const;

  Bignum multiplyBySingleDigit(int digit) const;
  Bignum divide(const Bignum& other, std::string op) const;

  bool check() const;
  std::string to_string() const;
  const std::vector<char>& getNum() const;
    
private:

  std::vector<char> data;


};