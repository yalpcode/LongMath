#include <LongNum.hpp>
#include <iostream>

int main() {
    LongMath::LongNum f1(0.8), f2(2.);
    // LongNum f1(15.3), f2(3.2);
    LongMath::LongNum f3 = f1 / f2;
    std::cout << f3;
}