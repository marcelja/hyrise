#include <memory>

#include "../base_test.hpp"
#include "gtest/gtest.h"

#include "../lib/fixed_string.hpp"

namespace opossum {

class FixedStringTest : public BaseTest {
 public:
  void SetUp() override {}
    char char_array1[3] = {'f', 'o', 'o'};
    char char_array2[6] = {'b', 'a', 'r', 'b', 'a', 'z'};
    FixedString fixed_string1 = FixedString(char_array1, 3u); 
    FixedString fixed_string2 = FixedString(char_array2, 6u);

};

TEST_F(FixedStringTest, CharvectorToString) {
  char char_array[4] = {'a', 'b', 'c', 'f'};

  auto str1 = FixedString(char_array, 3);
  EXPECT_EQ(str1, "abc");
}

TEST_F(FixedStringTest, StringLength) {
  char char_array3[4] = {'f', 'o', 'o', '\0'};
  FixedString str2 = FixedString(char_array3, 4);

  EXPECT_EQ(fixed_string1.size(), 3u);
  EXPECT_EQ(fixed_string1.maximum_length(), 3u);
  EXPECT_EQ(str2.size(), 3u);
  EXPECT_EQ(str2.maximum_length(), 4u);
}

TEST_F(FixedStringTest, Constructors) {
  auto str1 = FixedString(fixed_string1);
  EXPECT_EQ(str1.string(), "foo");

  // fixed_string2 = fixed_string1;
  // EXPECT_EQ(fixed_string2.string(), "foo");
}

TEST_F(FixedStringTest, CompareStrings) {
  // EXPECT_TRUE(FixedString("abcd") < FixedString("bbcd"));
  // EXPECT_TRUE(FixedString("abcd") < FixedString("bcd"));
  // EXPECT_TRUE(FixedString("abc") < FixedString("abcd"));
  // EXPECT_TRUE(FixedString("abc\0") < FixedString("abcd"));
  // EXPECT_FALSE(FixedString("abcdd") < FixedString("abcd"));
  // EXPECT_FALSE(FixedString("abcdd") < FixedString("abcd\0"));
  // EXPECT_FALSE(FixedString("abcd") < FixedString("abcd"));

  // EXPECT_TRUE(FixedString("abcd") == FixedString("abcd"));
  // EXPECT_TRUE(FixedString("abcd\0") == FixedString("abcd"));
  // EXPECT_FALSE(FixedString("abcd") == FixedString("abc"));
  // EXPECT_FALSE(FixedString("abc") == FixedString("abcd"));
  // EXPECT_FALSE(FixedString("abc") == FixedString("bbcd"));
}

TEST_F(FixedStringTest, Swap) {
  std::swap(fixed_string1, fixed_string2);
  std::cout << "fs1(bar): " << fixed_string1 << std::endl;
  std::cout << "fs2(foo): " << fixed_string2 << std::endl;
  EXPECT_EQ(fixed_string1, "bar");
  EXPECT_EQ(fixed_string2, "foo");
}

TEST_F(FixedStringTest, Print) {
  std::stringstream sstream;
  sstream << fixed_string1;
  EXPECT_EQ(sstream.str().find("foo"), 0u);
}

TEST_F(FixedStringTest, ImplicitCast) {
  std::string std_string = fixed_string1;
  EXPECT_EQ(std_string, "foo");
}

}  // namespace opossum
