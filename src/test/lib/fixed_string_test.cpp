#include <memory>

#include "../../base_test.hpp"
#include "gtest/gtest.h"

// #include "import_export/binary.hpp"
// #include "operators/export_binary.hpp"
// #include "operators/table_scan.hpp"
// #include "operators/table_wrapper.hpp"
// #include "storage/storage_manager.hpp"
// #include "storage/value_vector.hpp"
// #include "types.hpp"
#include "../lib/storage/value_vector.hpp"
#include "../lib/fixed_string.hpp"


namespace opossum {

class ValueVectorTest : public BaseTest {
 protected:
  void SetUp() override {}
};

TEST_F(ValueVectorTest, FirstExample) {
  // uint8_t string_length = 10;
  // pmr_concurrent_vector<int> test111;
  // test111.push_back(123);
  // test111.push_back(14);
  // test111.push_back(6);
  // std::cout << test111[0];

  ValueVector<int> _values;
  _values.push_back(27);

  char mem[] = {'f', 'o', 'o'};
  FixedString s(mem, 3);
  FixedString s2("bar");
  FixedString s3(mem, 2);
  FixedString s4(s2);default
  std::cout << s << std::endl;
  std::cout << s2.string() << std::endl;
  std::cout << s2 << std::endl;
  std::cout << s3 << std::endl;
  std::cout << s3.string() << std::endl;
  std::cout << s4 << std::endl;

  EXPECT_EQ(1, 1);
}

TEST_F(ValueVectorTest, asdfasdfsadf) {

  auto str1 = FixedString(std::string("askjjklasfkl"));
  std::cout << str1.size();

  EXPECT_EQ(str1.size(), size_t(12));
}

TEST_F(ValueVectorTest, asdfasdfsalkasdfljkafsdjkldf) {

  std::vector<char> test123 {'a', 'b', 'c', 'f'};

  auto str1 = FixedString(&test123[0], 3);
  std::cout << str1.string();

  EXPECT_EQ(str1.string(), "abc");
}

TEST_F(ValueVectorTest, comparestrings) {

  std::vector<char> test123 {'a', 'b', 'c', 'f'};
  std::vector<char> test456 {'b', 'b', 'c', 'f'};

  auto str1 = FixedString(&test123[0], 3);
  auto str2 = FixedString(&test456[0], 4);

  EXPECT_TRUE(str1 < str2);
}

}  // namespace opossum
