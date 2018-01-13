#include <memory>

#include "../base_test.hpp"
#include "gtest/gtest.h"

#include "../lib/fixed_string.hpp"


namespace opossum {

class FixedStringTest : public BaseTest {
 protected:
  void SetUp() override {}
};

TEST_F(FixedStringTest, InitFixedString) {
  char mem[] = {'f', 'o', 'o'};
  FixedString s(mem, 3);
  FixedString s2("bar");
  FixedString s3(mem, 2);
  FixedString s4(s2);

  EXPECT_EQ(1, 1);
}

TEST_F(FixedStringTest, ToString) {
  char mem[] = {'f', 'o', 'o'};
  FixedString s(mem, 3);
  FixedString s2("bar");

  EXPECT_EQ(s.string(), "foo");
  EXPECT_EQ(s2.string(), "bar");
}

TEST_F(FixedStringTest, SizeTest) {

  FixedString str1("askjjklasfkl");

  EXPECT_EQ(str1.size(), 12u);
}

TEST_F(FixedStringTest, LengthTest) {

  std::vector<char> test123 {'a', 'b', 'c', 'f'};

  FixedString str1(&test123[0], 3);

  EXPECT_EQ(str1.string(), "abc");
}

TEST_F(FixedStringTest, CompareStrings) {

  std::vector<char> test123 {'a', 'b', 'c', 'f'};
  std::vector<char> test456 {'b', 'b', 'c', 'f'};

  FixedString str1(&test123[0], 3);
  FixedString str2(&test456[0], 4);

  EXPECT_TRUE(str1 < str2);
  EXPECT_FALSE(str2 < str1);
}

TEST_F(FixedStringTest, EqualStrings) {
  FixedString str1("foo");
  FixedString str2("bar");
  std::vector<char> testString {'f', 'o', 'o'};
  FixedString str3(&testString[0], 3);

  EXPECT_FALSE(str1.string().compare(str2.string()) == 0);
  EXPECT_FALSE(str2.string().compare(str3.string()) == 0);
  EXPECT_TRUE(str1.string().compare(str3.string()) == 0);
}

}  // namespace opossum
