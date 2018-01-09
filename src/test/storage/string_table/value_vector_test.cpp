#include <memory>

#include "../../base_test.hpp"
#include "gtest/gtest.h"

#include "import_export/binary.hpp"
#include "operators/export_binary.hpp"
#include "operators/table_scan.hpp"
#include "operators/table_wrapper.hpp"
#include "storage/storage_manager.hpp"
#include "fixed_string.hpp"

namespace opossum {

class ValueVectorTest : public BaseTest {
protected:
  void SetUp() override {

  }
};

TEST_F(ValueVectorTest, FirstExample) {
  // uint8_t string_length = 10;

  EXPECT_EQ(1, 1);
}


}  // namespace opossum
