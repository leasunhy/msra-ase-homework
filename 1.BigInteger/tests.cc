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

    BigInteger one_trillion;
    BigInteger one_trillion_neg;
    BigInteger two_trillion;
    BigInteger one;
    BigInteger zero;
};

TEST_F(BigIntegerTest, Add1)
{
    EXPECT_EQ(two_trillion, one_trillion + one_trillion);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

