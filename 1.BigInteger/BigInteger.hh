#pragma once

#include <iostream>
#include <vector>
#include <string>

class BigInteger
{
  public:
    // constructors
    BigInteger() = default;
    BigInteger(const BigInteger& other) = default;
    BigInteger(int num);
    BigInteger(const std::string& str);

    // unary operators
    BigInteger operator-() const;

    // binary operators
    BigInteger& operator=(const BigInteger& other) = default;

    BigInteger operator+(const BigInteger& rhs) const;
    BigInteger operator-(const BigInteger& rhs) const;
    BigInteger operator*(const BigInteger& rhs) const;
    BigInteger operator/(const BigInteger& rhs) const;

    bool operator<(const BigInteger& rhs) const;

    // equality operators
    bool operator==(long long num) const;
    bool operator==(const BigInteger& rhs) const;

    // input/output operators
    friend std::istream& operator>>(std::istream& is, BigInteger& me);
    friend std::ostream& operator<<(std::ostream& os, const BigInteger& me);

    // accessors
    std::string toString() const;
    BigInteger abs() const;
    int unsignedCompareTo(const BigInteger& rhs) const;
    int compareTo(const BigInteger& rhs) const;

    // modifiers
    void assign(long long num);
    void assign(const BigInteger& other);
    void assign(const std::string& str);

  private:
    // underlying data
    std::vector<char> data_;
    int sign_;

    // helper functions
    BigInteger unsignedAdd_(const BigInteger& rhs) const;
    BigInteger unsignedSubtract_(const BigInteger& rhs) const;
    bool unsignedLessThan_(const BigInteger& rhs) const;
};

