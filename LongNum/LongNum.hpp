#include <cinttypes>
#include <string>
#include <vector>

namespace LongMath {
class LongNum {
    bool sign;
    std::vector<bool> mant;
    int32_t exp;
    uint32_t prec = 20;

    void normalize();

   public:
    LongNum();

    explicit LongNum(long double, uint32_t = 20);
    explicit LongNum(const std::string&, uint32_t = 20);

    LongNum(const LongNum&);
    LongNum(const LongNum&&);

    ~LongNum() = default;

    LongNum& operator=(const LongNum&);
    LongNum& operator=(const LongNum&&);

    LongNum abs() const;

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

    uint32_t getPrecision() const;
    void setPrecision(uint32_t perc);

    std::string toString() const;

    friend std::ostream& operator<<(std::ostream&, const LongNum&);
};
};  // namespace LongMath