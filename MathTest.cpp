#include <gtest/gtest.h>

#include "LongNum.hpp"

using namespace LongMath;

TEST(LongNumTest, DefaultConstructor) {
    LongNum num;
    EXPECT_TRUE(num.isZero())
        << "Default constructor should initialize to zero.";
}

TEST(LongNumTest, FloatConstructor) {
    LongNum num(3.14);
    EXPECT_EQ(num.toString(2), "3.14") << "Float constructor failed.";
}

TEST(LongNumTest, StringConstructor) {
    LongNum num("42.195");
    EXPECT_EQ(num.toString(), "42.195") << "String constructor failed.";
}

TEST(LongNumTest, AdditionOperator) {
    LongNum a(
        "87651476164587162354786125347651768324517862354871625476152."
        "0000239847128354719823751823645123576172436571236571623745672136457167"
        "5618265");
    LongNum b(
        "1235419253478126345875123087561276348701235087123640871."
        "7256728645716571345617654198764578165987436587131345643");
    LongNum result = a + b;
    std::string ans =
        "87652711583840640481132000470739329600866563589958749117023."
        "7256968492844926065441406022409701742159873158367917266745672136457167"
        "5618265";
    EXPECT_EQ(result.toString(), ans) << "Addition operator failed.";
}

TEST(LongNumTest, SubtractionOperator) {
    LongNum a("0.000000000056");
    LongNum b("-124357542.7623546235");
    LongNum result = a - b;
    EXPECT_EQ(result.toString(8), "124357542.76235462")
        << "Subtraction operator failed.";
}

TEST(LongNumTest, MultiplicationOperator) {
    LongNum a("1.00000002100000304000002156400005300042");
    LongNum b("-0.00000000000000236452763546125461812547815247651287465127863");
    LongNum result = a * b;
    std::string ans =
        "-0."
        "0000000000000023645276851163421509758879239996495855639988374753577633"
        "654462593948692451209270246";
    EXPECT_EQ(result.toString(), ans);
}

TEST(LongNumTest, DivisionOperator) {
    LongNum a(
        "0."
        "0000000000000000000000000516987882845642296794630432543726783478632569"
        "31304931640625");
    LongNum b(-16.0);
    LongNum result = a / b;
    std::string ans =
        "-0."
        "0000000000000000000000000032311742677852643549664402033982923967414535"
        "582065582275390625";
    EXPECT_EQ(result.toString(), ans) << "Division operator failed.";
}

TEST(LongNumTest, EqualityOperator) {
    LongNum a(3.14);
    LongNum b(3.14);
    EXPECT_TRUE(a == b) << "Equality operator failed.";
}

TEST(LongNumTest, InequalityOperator) {
    LongNum a(3.14);
    LongNum b(2.71);
    EXPECT_TRUE(a != b) << "Inequality operator failed.";
}

TEST(LongNumTest, SquareRoot) {
    LongNum a(16.0);
    LongNum result = a.sqrt();
    EXPECT_EQ(result.toString(), "4") << "Square root function failed.";
}

TEST(LongNumTest, AbsoluteValue) {
    LongNum a(-3.14);
    LongNum result = a.abs();
    EXPECT_EQ(result.toString(2), "3.14") << "Absolute value function failed.";
}

TEST(LongNumTest, Inverse) {
    LongNum a(2.0);
    LongNum result = a.inverse();
    EXPECT_EQ(result.toString(), "0.5") << "Inverse function failed.";
}

TEST(LongNumTest, ToString) {
    LongNum a(123.456);
    EXPECT_EQ(a.toString(3), "123.456") << "toString() function failed.";
}

TEST(LongNumTest, IsZero) {
    LongNum a(0.0);
    EXPECT_TRUE(a.isZero()) << "isZero() function failed.";
}

TEST(LongNumTest, AssignmentOperator) {
    LongNum a(3.14);
    LongNum b = a;
    EXPECT_EQ(b.toString(2), "3.14") << "Assignment operator failed.";
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}