#include <memory>

#include "../base_test.hpp"
#include "gtest/gtest.h"

#include "../lib/storage/value_vector.hpp"
#include "../lib/fixed_string.hpp"


namespace opossum {

class ValueVectorTest : public BaseTest {
 protected:
  void SetUp() override {}
};

TEST_F(ValueVectorTest, SubscriptOperator) {
  ValueVector<std::string> valuevector;
  valuevector.push_back("abc");

  EXPECT_EQ(valuevector[0], "abc");
}

TEST_F(ValueVectorTest, SubscriptOperatorConst) {
  ValueVector<std::string> valuevector;
  const auto str1 = "const";
  valuevector.push_back(str1);

  EXPECT_EQ(valuevector[0], "const");
}

TEST_F(ValueVectorTest, Iterator) {
  ValueVector<std::string> valuevector;
  valuevector.push_back("str1");
  valuevector.push_back("str1");

  for (auto it = valuevector.begin(); it != valuevector.end(); ++it) {
    *it = "abc";
  }

  EXPECT_EQ(valuevector[0], "abc");
}

}  // namespace opossum
