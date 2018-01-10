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

  EXPECT_EQ(1, 1);
}

}  // namespace opossum
