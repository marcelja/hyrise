#include <memory>

#include "../base_test.hpp"
#include "gtest/gtest.h"

#include "../lib/fixed_string.hpp"
#include "../lib/storage/value_vector.hpp"

namespace opossum {

class ValueVectorTest : public BaseTest {
 protected:
  void SetUp() override {}
};

TEST_F(ValueVectorTest, SubscriptOperator) {
  auto valuevector = ValueVector<std::string>();
  valuevector.push_back("abc");

  EXPECT_EQ(valuevector[0], "abc");
}

TEST_F(ValueVectorTest, SubscriptOperatorConst) {
  auto valuevector = ValueVector<std::string>();
  valuevector.push_back("const");
  const auto& const_value_vector = valuevector;

  EXPECT_EQ(const_value_vector[0], "const");
}

TEST_F(ValueVectorTest, PushBack) {
  auto valuevector = ValueVector<int>();
  valuevector.push_back(0);
  auto number = 1;
  valuevector.push_back(number);

  EXPECT_EQ(valuevector[0], 0);
  EXPECT_EQ(valuevector[1], 1);
}

TEST_F(ValueVectorTest, Iterator) {
  auto valuevector = ValueVector<std::string>();
  valuevector.push_back("str1");
  valuevector.push_back("str1");

  for (auto it = valuevector.begin(); it != valuevector.end(); ++it) {
    *it = "abc";
  }

  EXPECT_EQ(valuevector[0], "abc");
}

TEST_F(ValueVectorTest, SubscriptOperatorFixedString) {
  auto valuevector = ValueVector<FixedString>(3u);
  valuevector.push_back(FixedString("abc"));

  EXPECT_EQ(valuevector[0].string(), "abc");
}

TEST_F(ValueVectorTest, IteratorFixedString) {
  auto valuevector = ValueVector<FixedString>(5u);
  valuevector.push_back(FixedString("str1"));
  valuevector.push_back(FixedString("str1"));

  for (auto it = valuevector.begin(); it != valuevector.end(); ++it) {
    *it = FixedString("abcde");
  }

  EXPECT_EQ(valuevector[0].string(), "abcde");
}

}  // namespace opossum
