#include <cassert>
#include <exception>
#include <string>

#include "BigInteger.hh"

// constructors
BigInteger::BigInteger(int num)
{
    this->assign(num);
}

BigInteger::BigInteger(const std::string& str)
{
    this->assign(str);
}


// unary operators
BigInteger BigInteger::operator-() const
{
    BigInteger result = *this;
    result.sign_ = -result.sign_;
    return result;
}


// binary operators
BigInteger BigInteger::operator+(const BigInteger& rhs) const
{
    const BigInteger& lhs = *this;
    BigInteger result;
    if (lhs.sign_ == 0)
        return rhs;
    if (rhs.sign_ == 0)
        return lhs;
    if (lhs.sign_ == rhs.sign_) {
        result = lhs.unsignedAdd_(rhs);
        result.sign_ = lhs.sign_;
    } else {
        if (lhs.unsignedLessThan_(rhs)) {
            result = rhs.unsignedSubtract_(lhs);
            result.sign_ = rhs.sign_;
        } else {
            result = lhs.unsignedSubtract_(rhs);
            if (result.data_.size() == 1 && result.data_[0] == 0)
                result.sign_ = 0;
            else
                result.sign_ = lhs.sign_;
        }
    }
    return result;
}

BigInteger BigInteger::operator-(const BigInteger& rhs) const
{
    const BigInteger& lhs = *this;
    BigInteger result;
    if (lhs.sign_ == 0)
        return -rhs;
    if (rhs.sign_ == 0)
        return lhs;
    if (lhs.sign_ != rhs.sign_) {
        result = lhs.unsignedAdd_(rhs);
        result.sign_ = lhs.sign_;
    } else {
        if (lhs.unsignedLessThan_(rhs)) {
            result = rhs.unsignedSubtract_(lhs);
            result.sign_ = -lhs.sign_;
        } else {
            result = lhs.unsignedSubtract_(rhs);
            if (result.data_.size() == 1 && result.data_[0] == 0)
                result.sign_ = 0;
            else
                result.sign_ = lhs.sign_;
        }
    }
    return result;
}

BigInteger BigInteger::operator*(const BigInteger& rhs) const
{
    const std::vector<char> *longer, *shorter;
    if (data_.size() < rhs.data_.size())
        longer = &rhs.data_, shorter = &this->data_;
    else
        longer = &this->data_, shorter = &rhs.data_;
    std::vector<char> prod(shorter->size() + longer->size(), 0);
    for (size_t i = 0; i < shorter->size(); ++i) {
        int carry = 0;
        for (size_t j = 0; j < longer->size(); ++j) {
            int p = (*longer)[j] * (*shorter)[i] + carry + prod[i + j];
            prod[i + j] = p % 10;
            carry = p / 10;
        }
        if (carry)
            prod[i + longer->size()] = carry;
    }
    int size = prod.size();
    for (auto it = prod.rbegin(); it != prod.rend() && *it == 0; ++it)
        size -= 1;
    if (size == 0)
        size = 1;
    prod.resize(size);
    BigInteger result;
    result.data_ = prod;
    result.sign_ = sign_ * rhs.sign_;
    return result;
}

BigInteger BigInteger::operator/(const BigInteger& rhs) const
{
    throw std::runtime_error("Not implemented");
}

BigInteger BigInteger::operator+(long long rhs) const
{
    return *this + BigInteger(rhs);
}

BigInteger BigInteger::operator-(long long rhs) const
{
    return *this - BigInteger(rhs);
}

BigInteger BigInteger::operator*(long long rhs) const
{
    return *this * BigInteger(rhs);
}

BigInteger BigInteger::operator/(long long rhs) const
{
    return *this / BigInteger(rhs);
}

bool BigInteger::operator<(const BigInteger& rhs) const
{
    return compareTo(rhs) == -1;
}

bool BigInteger::operator==(long long num) const
{
    return *this == BigInteger(num);
}

bool BigInteger::operator==(const BigInteger& rhs) const
{
    return sign_ == rhs.sign_ && data_ == rhs.data_;
}


// input/output operators
std::istream& operator>>(std::istream& is, BigInteger& me)
{
    // currently only supports decimal input
    std::string str;
    is >> str;
    me.assign(str);
    return is;
}

std::ostream& operator<<(std::ostream& os, const BigInteger& me)
{
    os << me.toString();
    return os;
}


// accessors
std::string BigInteger::toString() const
{
    std::string result;
    if (sign_ == -1)
        result.push_back('-');
    for (auto it = data_.rbegin(); it != data_.rend(); ++it)
        result.push_back(*it + '0');
    return result;
}

BigInteger BigInteger::abs() const
{
    BigInteger result = *this;
    if (sign_ != 0)
        result.sign_ = 1;
    return result;
}

int BigInteger::unsignedCompareTo(const BigInteger& rhs) const
{
    const std::vector<char> &l = this->data_, &r = rhs.data_;
    assert(l.size() != 0 && r.size() != 0);
    if (r.size() > l.size())
        return -1;
    if (l.size() > r.size())
        return 1;
    int len = l.size();
    for (int i = len - 1; i >= 0; --i) {
        if (l[i] < r[i])
            return -1;
        else if (l[i] > r[i])
            return 1;
    }
    return 0;  // equal
}

int BigInteger::compareTo(const BigInteger& rhs) const
{
    const BigInteger& lhs = *this;
    if (lhs.sign_ == rhs.sign_)
        return lhs.unsignedCompareTo(rhs);
    else if (lhs.sign_ < rhs.sign_)
        return -1;
    else if (lhs.sign_ > rhs.sign_)
        return 1;
    else
        return 0;
}


// modifiers
void BigInteger::assign(long long num)
{
    if (num == 0) {
        sign_ = 0;
        data_ = std::vector<char>{ 0 };
        return;
    }
    sign_ = 1;
    if (num < 0) {
        num = -num;
        sign_ = -1;
    }
    while (num) {
        data_.push_back(num % 10);
        num /= 10;
    }
}

void BigInteger::assign(const BigInteger& other)
{
    data_ = other.data_;
    sign_ = other.sign_;
}

void BigInteger::assign(const std::string& str)
{
    BigInteger& me = *this;
    if (str.empty())
        throw std::runtime_error("str can't be empty.");
    me.sign_ = 0;
    int i = 0;
    if (str[i] == '-') {
        me.sign_ = -1;
        ++i;
    } else if (str[i] == '+') {
        me.sign_ = 1;
        ++i;
    }
    // ignoring preceding zeros
    while (str[i] && str[i] == '0')
        ++i;
    if (!str[i]) {
        me.data_ = std::vector<char>{ 0 };
    } else {
        if (me.sign_ != -1)
            me.sign_ = 1;
        for (int j = str.length() - 1; j >= i; --j)
            me.data_.push_back(str[j] - '0');
    }
}


// helper functions
BigInteger BigInteger::unsignedAdd_(const BigInteger& rhs) const
{
    const std::vector<char> *longer, *shorter;
    if (data_.size() < rhs.data_.size())
        longer = &rhs.data_, shorter = &this->data_;
    else
        longer = &this->data_, shorter = &rhs.data_;
    std::vector<char> sum;
    int carry = 0;
    for (size_t i = 0; i < shorter->size(); ++i) {
        int s = carry + (*shorter)[i] + (*longer)[i];
        if (s >= 10)
            sum.push_back(s - 10), carry = 1;
        else
            sum.push_back(s), carry = 0;
    }
    for (size_t i = shorter->size(); i < longer->size(); ++i) {
        int s = carry + (*longer)[i];
        if (s >= 10)
            sum.push_back(s - 10), carry = 1;
        else
            sum.push_back(s), carry = 0;
    }
    if (carry)
        sum.push_back(carry);
    BigInteger result;
    result.data_ = sum;
    return result;
}

BigInteger BigInteger::unsignedSubtract_(const BigInteger& rhs) const
{
    // we already know that the lhs is longer
    const std::vector<char> &l = this->data_, &r = rhs.data_;
    std::vector<char> diff;
    int carry = 0;
    for (size_t i = 0; i < r.size(); ++i) {
        int s = carry + (l[i] - r[i]);
        if (s < 0)
            diff.push_back(s + 10), carry = -1;
        else
            diff.push_back(s), carry = 0;
    }
    for (size_t i = r.size(); i < l.size(); ++i) {
        int s = carry + l[i];
        if (s < 0)
            diff.push_back(s + 10), carry = -1;
        else
            diff.push_back(s), carry = 0;
    }
    int size = diff.size();
    for (auto it = diff.rbegin(); it != diff.rend() && *it == 0; ++it)
        size -= 1;
    if (size == 0)
        size = 1;
    diff.resize(size);
    BigInteger result;
    result.data_ = diff;
    return result;
}

bool BigInteger::unsignedLessThan_(const BigInteger& rhs) const
{
    return this->unsignedCompareTo(rhs) == -1;
}

