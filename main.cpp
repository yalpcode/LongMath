#include <LongNum.hpp>
#include <iostream>

int main() {
    LongMath::LongNum f1(0.8), f2(2.);
    LongMath::LongNum f3(15.3), f4(3.2);
    LongMath::LongNum f5 = f1 / f2, f6 = f3 / f4;
    std::cout << f5 << std::endl << f6;
}