#include "LongNum.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>

namespace LongMath {
void LongNum::normalize() {
    while (!mant.empty() && mant.back() == 0) {
        mant.pop_back();
    }

    while (mant.size() >= 2 && *mant.rbegin() && *(mant.rbegin() + 1)) {
        mant.pop_back();
    }

    if (mant.empty()) {
        exp = 0;
        sign = false;
        mant.push_back(0);
    }
}

LongNum::LongNum(const long double x) {
    sign = std::signbit(x);
    exp = static_cast<int>(std::ceil(std::log2(x)));

    long double frac = x / std::pow(2, exp);

    mant.clear();
    for (uint32_t i = 0; i < prec; ++i) {
        frac *= 2;
        bool bit = static_cast<int>(frac);
        mant.push_back(bit);
        frac -= bit;
        if (frac == 0.0) break;
    }

    normalize();
}

LongNum::LongNum(const std::string& s) {}

LongNum::LongNum(const LongNum& other)
    : sign(other.sign), mant(other.mant), exp(other.exp), prec(other.prec) {}

LongNum::LongNum(const LongNum&& other) {
    sign = std::move(other.sign);
    mant = std::move(other.mant);
    exp = std::move(other.exp);
    prec = std::move(other.prec);
}

LongNum& LongNum::operator=(const LongNum& other) {
    sign = other.sign;
    mant = other.mant;
    exp = other.exp;
    prec = other.prec;

    return *this;
}

LongNum& LongNum::operator=(const LongNum&& other) {
    sign = std::move(other.sign);
    mant = std::move(other.mant);
    exp = std::move(other.exp);
    prec = std::move(other.prec);

    return *this;
}

LongNum LongNum::abs() const {
    LongNum result = *this;
    result.sign = 0;

    return result;
}

LongNum LongNum::operator+(const LongNum& other) const {
    LongNum result;

    if (this->sign == other.sign) {
        result.sign = this->sign;

        result.exp = std::max(this->exp, other.exp);
        std::vector<bool> this_mant = this->mant;
        std::vector<bool> other_mant = other.mant;

        if (this->exp < result.exp) {
            size_t shift = result.exp - this->exp;
            this_mant.insert(this_mant.begin(), shift, false);
        } else if (other.exp < result.exp) {
            size_t shift = result.exp - other.exp;
            other_mant.insert(other_mant.begin(), shift, false);
        }

        bool carry = false;
        size_t max_length = std::max(this_mant.size(), other_mant.size());
        result.mant.resize(max_length);
        for (size_t i = 0; i < max_length; ++i) {
            size_t j = max_length - 1 - i;
            bool bit1 =
                (i >= max_length - this_mant.size()) ? this_mant[j] : false;
            bool bit2 =
                (i >= max_length - other_mant.size()) ? other_mant[j] : false;
            bool sum = bit1 ^ bit2 ^ carry;
            carry = (bit1 && bit2) || (bit1 && carry) || (bit2 && carry);
            result.mant[j] = sum;
        }
        if (carry) {
            result.mant.insert(result.mant.begin(), 1);
            result.exp += 1;
        }
    } else {
        LongNum larger, smaller;

        if (this->abs() > other.abs()) {
            larger = *this;
            smaller = other;
        } else {
            larger = other;
            smaller = *this;
        }

        result.sign = larger.sign;
        result.exp = larger.exp;

        std::vector<bool> larger_mant = larger.mant;
        std::vector<bool> smaller_mant = smaller.mant;

        if (smaller.exp < larger.exp) {
            size_t shift = larger.exp - smaller.exp;
            smaller_mant.insert(smaller_mant.begin(), shift, false);
        }

        bool borrow = false;
        size_t max_length = std::max(larger_mant.size(), smaller_mant.size());
        result.mant.resize(max_length);
        for (size_t i = 0; i < max_length; ++i) {
            size_t j = max_length - 1 - i;
            bool bit1 =
                (i >= max_length - larger_mant.size()) ? larger_mant[j] : false;
            bool bit2 = (i >= max_length - smaller_mant.size())
                            ? smaller_mant[j]
                            : false;
            bool diff = bit1 ^ bit2 ^ borrow;
            borrow = (!bit1 && bit2) || (!bit1 && borrow) || (bit2 && borrow);
            result.mant[j] = diff;
        }
        for (size_t i = 0; i < max_length; ++i) {
            if (result.mant[i]) {
                result.mant.erase(result.mant.begin(), result.mant.begin() + i);
                result.exp -= i;
                break;
            }
        }
    }

    result.normalize();

    return result;
}

LongNum LongNum::operator-(const LongNum& other) const {
    LongNum result;

    LongNum neg_other = other;
    neg_other.sign = !other.sign;
    result = *this + neg_other;

    return result;
}

LongNum LongNum::operator*(const LongNum& other) const {
    LongNum result;

    result.sign = this->sign ^ other.sign;
    result.exp = this->exp + other.exp;

    result.mant.resize(mant.size() + other.mant.size(), false);

    for (int i = mant.size() - 1; i >= 0; --i) {
        int carry = 0;
        for (int j = other.mant.size() - 1; j >= 0; --j) {
            int product =
                mant[i] * other.mant[j] + result.mant[i + j + 1] + carry;
            result.mant[i + j + 1] = product & 1;
            carry = product >> 1;
        }
        result.mant[i] = carry;
    }

    result.normalize();

    return result;
}

std::vector<bool> subtract(std::vector<bool>& a, const std::vector<bool>& b) {
    bool borrow = false;
    size_t max_length = std::max(a.size(), b.size());
    std::vector<bool> result(max_length);
    for (size_t i = 0; i < max_length; ++i) {
        size_t j = max_length - 1 - i;
        bool bit1 = a[j];
        bool bit2 = (b.size() - 1 - i >= 0) ? b[b.size() - 1 - i] : false;
        bool diff = bit1 ^ bit2 ^ borrow;
        borrow = (!bit1 && bit2) || (!bit1 && borrow) || (bit2 && borrow);
        result[j] = diff;
    }

    return result;
}

bool operator<=(const std::vector<bool>& lhs, const std::vector<bool>& rhs) {
    if (lhs.size() != rhs.size()) {
        return lhs.size() < rhs.size();
    }
    size_t max_size = std::max(lhs.size(), rhs.size());
    for (size_t i = 0; i < max_size; ++i) {
        if (lhs[i] < rhs[i]) return true;
        if (lhs[i] > rhs[i]) return false;
    }
    return true;
}

LongNum LongNum::operator/(const LongNum& other) const {
    LongNum result;

    result.sign = this->sign ^ other.sign;
    result.exp = this->exp - other.exp;

    std::cout << *this << std::endl << other << std::endl;

    std::vector<bool> dividend = this->mant;
    std::vector<bool> divisor = other.mant;
    std::vector<bool> quotient;

    std::reverse(dividend.begin(), dividend.end());

    std::vector<bool> remainder;
    remainder.push_back(dividend.back());
    dividend.pop_back();
    char flag = 0;
    while (quotient.size() < prec) {
        std::cout << std::endl;
        std::cout << "0.";
        for (int bit : remainder) {
            std::cout << bit;
        }
        std::cout << std::endl;
        std::cout << "0.";
        for (int bit : divisor) {
            std::cout << bit;
        }
        std::cout << std::endl;
        if (divisor <= remainder) {
            remainder = subtract(remainder, divisor);
            quotient.push_back(true);
            if (flag < 2) {
                ++result.exp;
                flag = 1;
            }
        } else {
            if (flag) {
                quotient.push_back(false);
            }
        }

        while (!remainder.empty() && !remainder.front()) {
            remainder.erase(remainder.begin());
        }

        std::cout << "0.";
        for (int bit : remainder) {
            std::cout << bit;
        }

        if (dividend.empty()) {
            flag = 2;
            remainder.push_back(0);
        } else {
            remainder.push_back(dividend.back());
            dividend.pop_back();
        }

        std::cout << std::find(remainder.begin(), remainder.end(), 1) -
                         remainder.begin()
                  << std::endl;
    }

    result.mant = quotient;

    result.normalize();

    return result;
}

LongNum& LongNum::operator+=(const LongNum& other) {
    return *this = std::move(*this + other);
}

LongNum& LongNum::operator-=(const LongNum& other) {
    return *this = std::move(*this - other);
}

LongNum& LongNum::operator*=(const LongNum& other) {
    return *this = std::move(*this * other);
}

LongNum& LongNum::operator/=(const LongNum& other) {
    return *this = std::move(*this / other);
}

bool LongNum::operator==(const LongNum& other) const {
    if (sign != other.sign || exp != other.exp ||
        mant.size() != other.mant.size()) {
        return false;
    }
    for (size_t i = 0; i < mant.size(); ++i) {
        if (mant[i] != other.mant[i]) {
            return false;
        }
    }
    return true;
}

bool LongNum::operator!=(const LongNum& other) const {
    return !(*this == other);
}

bool LongNum::operator<(const LongNum& other) const {
    if (sign && !other.sign) return true;
    if (!sign && other.sign) return false;

    if (sign) {
        if (exp > other.exp) return true;
        if (exp < other.exp) return false;

        for (size_t i = 0; i < std::min(mant.size(), other.mant.size()); ++i) {
            if (mant[i] > other.mant[i]) return true;
            if (mant[i] < other.mant[i]) return false;
        }
    } else {
        if (exp < other.exp) return true;
        if (exp > other.exp) return false;

        for (size_t i = 0; i < std::min(mant.size(), other.mant.size()); ++i) {
            if (mant[i] < other.mant[i]) return true;
            if (mant[i] > other.mant[i]) return false;
        }
    }

    return false;
}

bool LongNum::operator>(const LongNum& other) const {
    return !(*this < other) && (*this != other);
}

bool LongNum::operator>=(const LongNum& other) const {
    return !(*this < other);
}

bool LongNum::operator<=(const LongNum& other) const {
    return !(*this < other) || (*this == other);
}

uint32_t LongNum::getPrecision() const { return this->prec; }

void LongNum::setPrecision(uint32_t prec) { this->prec = prec; }

std::string LongNum::toString() const {
    std::ostringstream oss;
    if (sign) oss << "-";

    oss << "0.";
    for (int bit : mant) {
        oss << bit;
    }

    oss << " * 2^" << exp;
    return oss.str();
}

std::ostream& operator<<(std::ostream& os, const LongNum& f) {
    os << f.toString();
    return os;
}

LongNum operator""_longnum(long double number) { return LongNum(number); }
};