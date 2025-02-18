#include "LongNum.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>

namespace LongMath {
void LongNum::setPrecision(uint32_t prec) { LongNum::prec = prec; }

void LongNum::removeZeroes() {
    size_t n = std::max(1, exp);

    while (mant.size() > n && !mant.back()) mant.pop_back();

    while (mant.size() > 1 && !mant.front()) {
        mant.erase(mant.begin());
        --exp;
    }

    while (mant.size() > 1 && !mant.back()) mant.pop_back();

    if (isZero()) {
        exp = 1;
        sign = 1;
    }

    normalize();
}

void LongNum::normalize() {
    mant.resize(std::min(size_t(prec * k_mant * 2), mant.size()));
}

LongNum::LongNum() : sign(0), mant(1), exp(1) {}

LongNum::LongNum(const long double x) {
    if (x == 0.) {
        sign = 0;
        mant.resize(1);
        exp = 1;
        return;
    }
    sign = std::signbit(x);
    exp = static_cast<int>(std::ceil(std::log2(std::abs(x))));
    if (exp >= 0) ++exp;

    long double frac = std::abs(x) / std::pow(2, exp);

    for (uint32_t i = 0; i < prec * k_mant; ++i) {
        frac *= 2;
        bool bit = static_cast<int>(frac);
        mant.push_back(bit);
        frac -= bit;
        if (frac == 0.0) break;
    }

    removeZeroes();
}

LongNum::LongNum(std::string s) {
    sign = 0;
    if (s.front() == '-') {
        sign = 1;
        s.erase(s.begin());
    }
    size_t idxPoint = std::find(s.begin(), s.end(), '.') - s.begin();
    size_t expCount = s.size() - idxPoint - 1;
    s.erase(s.begin() + idxPoint);
    do {
        mant.push_back((s.back() - '0') % 2);
        s = divideByTwo(s);

    } while (s != "0");

    std::reverse(mant.begin(), mant.end());

    exp = mant.size();
    for (size_t i = 0; i < expCount; ++i) {
        *this /= 10;
    }

    removeZeroes();
}

LongNum::LongNum(const LongNum& other)
    : sign(other.sign), mant(other.mant), exp(other.exp) {}

LongNum::LongNum(LongNum&& other)
    : sign(std::move(other.sign)),
      mant(std::move(other.mant)),
      exp(std::move(other.exp)) {}

LongNum& LongNum::operator=(const LongNum& other) {
    sign = other.sign;
    mant = other.mant;
    exp = other.exp;

    return *this;
}

LongNum& LongNum::operator=(LongNum&& other) {
    sign = std::move(other.sign);
    mant = std::move(other.mant);
    exp = std::move(other.exp);

    return *this;
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

bool LongNum::operator>(const LongNum& other) const {
    if (sign != other.sign) return sign < other.sign;

    if (exp != other.exp) return (exp > other.exp) ^ sign;

    const std::vector<bool>& m1 = mant;
    const std::vector<bool>& m2 = other.mant;
    size_t size = std::max(m1.size(), m2.size());

    for (size_t i = 0; i < size; ++i) {
        bool bit1 = (i < m1.size()) ? m1[i] : 0;
        bool bit2 = (i < m2.size()) ? m2[i] : 0;

        if (bit1 != bit2) return (bit1 > bit2) ^ sign;
    }

    return false;
}

bool LongNum::operator<(const LongNum& other) const {
    return !(*this > other) && (*this != other);
}

bool LongNum::operator>=(const LongNum& other) const {
    return (*this > other) || (*this == other);
}

bool LongNum::operator<=(const LongNum& other) const {
    return !(*this > other);
}

LongNum LongNum::operator-() const {
    LongNum res(*this);
    res.sign ^= 1;

    return res;
}

LongNum LongNum::operator+(const LongNum& other) const {
    if (sign == other.sign) {
        LongNum res;
        int32_t exp1 = exp;
        int32_t exp2 = other.exp;
        res.exp = std::max(exp1, exp2);

        const std::vector<bool>& m1 = mant;
        const std::vector<bool>& m2 = other.mant;

        size_t offset1 = res.exp - exp1;
        size_t offset2 = res.exp - exp2;

        size_t size1 = m1.size() + offset1;
        size_t size2 = m2.size() + offset2;
        size_t size = std::max(size1, size2);
        res.mant.resize(size + 1);

        bool carry = false;

        const size_t size1_o = m1.size() + (size - size1);
        const size_t size2_o = m2.size() + (size - size2);

        for (size_t i = 0; i < size; ++i) {
            bool bit1;
            if (i < size - size1) {
                bit1 = 0;
            } else if (i < size1_o) {
                bit1 = m1[size1_o - 1 - i];
            } else {
                bit1 = 0;
            }
            bool bit2;
            if (i < size - size2) {
                bit2 = 0;
            } else if (i < size2_o) {
                bit2 = m2[size2_o - 1 - i];
            } else {
                bit2 = 0;
            }

            bool sum = bit1 ^ bit2 ^ carry;
            carry = (bit1 && bit2) || (bit1 && carry) || (bit2 && carry);
            res.mant[size - i] = sum;
        }
        res.mant[0] = carry;
        ++res.exp;

        res.removeZeroes();

        res.sign = sign;
        return res;
    }

    if (sign) return other - (-(*this));
    return *this - (-other);
}

LongNum LongNum::operator-(const LongNum& other) const {
    if (!sign && !other.sign) {
        bool cmp = *this > other;

        LongNum res;

        int32_t exp1 = cmp ? exp : other.exp;
        int32_t exp2 = cmp ? other.exp : exp;
        res.exp = std::max(exp1, exp2);

        const std::vector<bool>& m1 = cmp ? mant : other.mant;
        const std::vector<bool>& m2 = cmp ? other.mant : mant;

        size_t offset1 = res.exp - exp1;
        size_t offset2 = res.exp - exp2;

        size_t size1 = m1.size() + offset1;
        size_t size2 = m2.size() + offset2;
        size_t size = std::max(size1, size2);
        res.mant.resize(size + 1);

        res.sign = !cmp;

        const size_t size1_o = m1.size() + (size - size1);
        const size_t size2_o = m2.size() + (size - size2);

        bool borrow = false;
        for (size_t i = 0; i < size; ++i) {
            bool bit1;
            if (i < size - size1) {
                bit1 = 0;
            } else if (i < size1_o) {
                bit1 = m1[size1_o - 1 - i];
            } else {
                bit1 = 0;
            }
            bool bit2;
            if (i < size - size2) {
                bit2 = 0;
            } else if (i < size2_o) {
                bit2 = m2[size2_o - 1 - i];
            } else {
                bit2 = 0;
            }

            bool diff = bit1 ^ bit2 ^ borrow;
            borrow = (!bit1 && bit2) || (!bit1 && borrow) || (bit2 && borrow);
            res.mant[size - i] = diff;
        }

        ++res.exp;

        res.removeZeroes();

        return res;
    }

    if (sign && other.sign) return (-other) - (-(*this));

    return *this + (-other);
}

LongNum LongNum::operator*(const LongNum& other) const {
    size_t len = mant.size() + other.mant.size();

    LongNum res;

    res.sign = sign ^ other.sign;
    res.mant.assign(len, 0);
    res.exp = exp + other.exp;

    for (int i = mant.size() - 1; i >= 0; --i) {
        int carry = 0;
        for (int j = other.mant.size() - 1; j >= 0; --j) {
            int product = mant[i] * other.mant[j] + res.mant[i + j + 1] + carry;
            res.mant[i + j + 1] = product & 1;
            carry = product >> 1;
        }
        res.mant[i] = carry;
    }

    res.removeZeroes();

    return res;
}

LongNum LongNum::operator/(const LongNum& other) const {
    LongNum res = *this * other.inverse();

    size_t intPart = std::max(0, exp);

    if (intPart > res.mant.size() - 1) return res;

    size_t i = res.mant.size() - 1 - intPart;
    size_t n = std::max(0, res.exp);

    if (i > n && res.mant[i]) {
        while (i > n && res.mant[i]) --i;

        if (res.mant[i]) {
            res.mant.erase(res.mant.begin() + n, res.mant.end());
            res = res + (res.sign ? -1 : 1);
        } else {
            res.mant.erase(res.mant.begin() + i + 1, res.mant.end());
            res.mant[i] = 1;
        }
    }

    return res;
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

LongNum LongNum::inverse() const {
    if (isZero())
        throw std::string("LongNum LongNum::inverse() - division by zero!");

    LongNum x(*this);
    x.sign = 0;

    LongNum d(1.);

    LongNum res;
    res.sign = sign;
    res.exp = 1;
    res.mant.resize(0);

    while (x < 1.) {
        ++x.exp;
        ++res.exp;
    }

    while (d < x) ++d.exp;

    res.exp -= d.exp - 1;

    size_t numbers = 0;
    size_t intPart = std::max(0, res.exp);
    size_t maxNumbers = prec * k_mant + intPart;
    do {
        bool div = 0;

        if (d >= x) {
            div = 1;
            d -= x;
        }

        ++d.exp;

        res.mant.push_back(div);
        ++numbers;
    } while (!d.isZero() && numbers < maxNumbers);

    return res;
}

LongNum LongNum::abs() const {
    LongNum res(*this);
    res.sign = 0;

    return res;
}

LongNum LongNum::sqrt() const {
    if (sign) throw std::string("LongNum LongNum::sqrt() - number is negative");

    if (isZero()) return 0;

    LongMath::LongNum x = *this;
    for (size_t i = 0; i < prec; ++i) {
        x = (x + (*this) / x) / 2;
    }
    return x;
}

bool LongNum::isZero() const { return mant.size() == 1 && !mant.front(); }

std::string LongNum::toString(uint32_t precision) const {
    std::string res = "0";
    for (size_t i = 0; i < mant.size(); ++i) {
        if (!mant[i]) {
            continue;
        }
        std::string t = "1";
        for (size_t j = 0; j < mant.size() - i; ++j) {
            t = multiplyByTwo(t);
        }
        res = addStrings(res, t);
    }

    res.append(prec * k_mant, '0');
    for (int32_t i = 0; i <= int32_t(mant.size() - exp); ++i) {
        res = divideByTwo(res);
    }
    if (res.size() <= prec * k_mant) {
        res.insert(res.begin(), prec * k_mant - res.size() + 1, '0');
    }
    res.insert(res.begin() + (res.size() - prec * k_mant), '.');

    if (sign) {
        res.insert(res.begin(), '-');
    }

    return roundStrings(res, precision);
}

std::ostream& operator<<(std::ostream& os, const LongNum& f) {
    os << f.toString();
    return os;
}

std::string divideByTwo(const std::string& number) {
    std::string result;
    int carry = 0;

    for (char digit : number) {
        int currentDigit = digit - '0';
        int value = currentDigit + carry * 10;
        int quotient = value / 2;
        carry = value % 2;

        if (!result.empty() || quotient != 0) {
            result.push_back(quotient + '0');
        }
    }

    if (result.empty()) {
        return "0";
    }

    return result;
}

std::string multiplyByTwo(const std::string& number) {
    std::string result;
    int carry = 0;

    for (int i = number.length() - 1; i >= 0; --i) {
        int currentDigit = number[i] - '0';
        int value = currentDigit * 2 + carry;
        carry = value / 10;
        result.push_back((value % 10) + '0');
    }

    if (carry != 0) {
        result.push_back(carry + '0');
    }

    std::reverse(result.begin(), result.end());

    return result;
}

std::string addStrings(const std::string& num1, const std::string& num2) {
    std::string result;
    int carry = 0;
    int i = num1.length() - 1;
    int j = num2.length() - 1;

    while (i >= 0 || j >= 0 || carry > 0) {
        int digit1 = (i >= 0) ? num1[i] - '0' : 0;
        int digit2 = (j >= 0) ? num2[j] - '0' : 0;
        int sum = digit1 + digit2 + carry;
        carry = sum / 10;
        result.push_back((sum % 10) + '0');

        if (i >= 0) --i;
        if (j >= 0) --j;
    }

    std::reverse(result.begin(), result.end());
    return result;
}

std::string roundStrings(const std::string& number, int n) {
    size_t decimalPointPos = number.find('.');

    if (decimalPointPos == std::string::npos) {
        return number;
    }

    size_t roundPos = decimalPointPos + n + 1;

    if (roundPos >= number.length()) {
        return number;
    }

    std::string roundedStr = number.substr(0, roundPos);

    char nextChar = number[roundPos];
    if (nextChar >= '5' && nextChar <= '9') {
        int i = roundedStr.length() - 1;
        while (i >= 0) {
            if (roundedStr[i] == '.') {
                --i;
                continue;
            }
            if (roundedStr[i] < '9') {
                ++roundedStr[i];
                break;
            } else {
                roundedStr[i] = '0';
                --i;
            }
        }

        if (i < 0) {
            roundedStr.insert(roundedStr.begin(), '1');
        }
    }

    size_t lastNonZero = roundedStr.find_last_not_of('0');
    if (lastNonZero != std::string::npos && roundedStr[lastNonZero] == '.') {
        --lastNonZero;
    }
    roundedStr = roundedStr.substr(0, lastNonZero + 1);

    return roundedStr;
}
};  // namespace LongMath

LongMath::LongNum operator""_longnum(long double number) {
    return LongMath::LongNum(number);
}
