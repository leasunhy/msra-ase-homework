#include <gtest/gtest.h>

#include "BigInteger.hh"

class BigIntegerTest : public ::testing::Test
{
  protected:
    BigIntegerTest()
        : one_trillion("1000000000000"), one_trillion_neg("-1000000000000"),
          two_trillion("2000000000000"),
          one(1), zero(0)
    {
        // blank
    }
    virtual ~BigIntegerTest() { }

    virtual void SetUp() { }
    virtual void TearDown() { }

    BigInteger one_trillion;
    BigInteger one_trillion_neg;
    BigInteger two_trillion;
    BigInteger one;
    BigInteger zero;
};

TEST_F(BigIntegerTest, Add1) {
    EXPECT_EQ(two_trillion, one_trillion + one_trillion);
}

TEST_F(BigIntegerTest, Add2) {
    EXPECT_EQ(zero, one_trillion + one_trillion_neg);
}

TEST_F(BigIntegerTest, Add3) {
    EXPECT_EQ(one_trillion, one_trillion_neg + two_trillion);
}

TEST_F(BigIntegerTest, AddCommutativity) {
    auto res1 = two_trillion + one_trillion;
    auto res2 = one_trillion + two_trillion;
    EXPECT_EQ(res1, res2);
}

TEST_F(BigIntegerTest, Sub1) {
    EXPECT_EQ(zero, one_trillion - one_trillion);
}

TEST_F(BigIntegerTest, Sub2) {
    EXPECT_EQ(one_trillion, two_trillion - one_trillion);
}

TEST_F(BigIntegerTest, Negate) {
    auto res1 = two_trillion - one_trillion;
    auto res2 = one_trillion - two_trillion;
    EXPECT_EQ(res1, -res2);
}

TEST_F(BigIntegerTest, Abs) {
    EXPECT_EQ(one_trillion, one_trillion_neg.abs());
    EXPECT_EQ(one_trillion, one_trillion.abs());
    EXPECT_EQ(zero, zero.abs());
}

TEST_F(BigIntegerTest, LessThan) {
    EXPECT_LT(one_trillion, two_trillion);
    EXPECT_LT(one_trillion_neg, one_trillion);
    EXPECT_LT(zero, one);
    EXPECT_LT(one_trillion_neg, zero);
}

TEST_F(BigIntegerTest, EqualityLongLong) {
    EXPECT_EQ(zero, 0);
    EXPECT_EQ(one, 1);
}

TEST_F(BigIntegerTest, ToString) {
    EXPECT_EQ(zero.toString(), "0");
    EXPECT_EQ(one.toString(), "1");
    EXPECT_EQ(one_trillion_neg.toString(), "-1000000000000");
}

TEST_F(BigIntegerTest, CompareTo) {
    EXPECT_EQ(zero.compareTo(one), -1);
    EXPECT_EQ(zero.compareTo(zero), 0);
    EXPECT_EQ(one_trillion_neg.compareTo(one_trillion), -1);
    EXPECT_EQ(one_trillion.compareTo(one_trillion_neg), 1);
}

TEST_F(BigIntegerTest, UnsignedCompareTo) {
    EXPECT_EQ(zero.unsignedCompareTo(one), -1);
    EXPECT_EQ(zero.unsignedCompareTo(zero), 0);
    EXPECT_EQ(one_trillion_neg.unsignedCompareTo(one_trillion), 0);
    EXPECT_EQ(one_trillion.unsignedCompareTo(one_trillion_neg), 0);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

