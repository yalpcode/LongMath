#include <cinttypes>
#include <string>
#include <vector>

namespace LongMath {
std::string divideByTwo(const std::string&);
std::string multiplyByTwo(const std::string&);
std::string addStrings(const std::string&, const std::string&);
std::string roundStrings(const std::string&, int);

class LongNum {
    bool sign;
    std::vector<bool> mant;
    int32_t exp;

    static inline uint32_t prec = 100;
    static const uint32_t k_mant = 6;

    void removeZeroes();
    void normalize();

   public:
    static uint32_t getPrecision();
    static void setPrecision(uint32_t perc);

    LongNum();

    LongNum(long double);
    explicit LongNum(std::string);

    LongNum(const LongNum&);
    LongNum(LongNum&&);

    ~LongNum() = default;

    LongNum& operator=(const LongNum&);
    LongNum& operator=(LongNum&&);

    LongNum operator-() const;

    LongNum operator+(const LongNum&) const;
    LongNum operator-(const LongNum&) const;
    LongNum operator*(const LongNum&) const;
    LongNum operator/(const LongNum&) const;

    LongNum& operator+=(const LongNum&);
    LongNum& operator-=(const LongNum&);
    LongNum& operator*=(const LongNum&);
    LongNum& operator/=(const LongNum&);

    bool operator==(const LongNum&) const;
    bool operator!=(const LongNum&) const;
    bool operator>(const LongNum&) const;
    bool operator<(const LongNum&) const;
    bool operator>=(const LongNum&) const;
    bool operator<=(const LongNum&) const;

    LongNum inverse() const;
    LongNum abs() const;
    LongNum sqrt() const;

    std::string toString(uint32_t precision = prec) const;

    bool isZero() const;

    friend std::ostream& operator<<(std::ostream&, const LongNum&);
};
};  // namespace LongMath

LongMath::LongNum operator""_longnum(long double number);