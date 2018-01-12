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

class StringTableTest : public BaseTest {
protected:
  void SetUp() override {
    // StorageManager::get().add_table("string_table1",
    //                                 load_table("strings_table/string_table.tbl", Chunk::MAX_SIZE));

    // const auto table = StorageManager::get().get_table("string_table1");

    // auto table_wrapper = std::make_shared<TableWrapper>(std::move(table));
    // table_wrapper->execute();
    // auto ex = std::make_shared<opossum::ExportBinary>(table_wrapper, "string_table_out.bin");
    // ex->execute();
  }
};

// TEST_F(StringTableTest, StringAppend) {
//   uint8_t string_length = 10;

//   ValueColumn<std::string> vc(string_length);
//   vc.append("bla");
//   vc.append("blabla");

//   EXPECT_EQ(vc.get(1), "blabla");
// }

// TEST_F(StringTableTest, LongString) {
//   uint8_t string_length = 2;

//   ValueColumn<std::string> vc(string_length);
//   vc.append("bla");

//   EXPECT_EQ(vc.get(0), "bl");
// }

// TEST_F(StringTableTest, GetSize) {
//   uint8_t string_length = 4;

//   ValueColumn<std::string> vc(string_length);
//   vc.append("bla");
//   vc.append("bla");
//   EXPECT_EQ(vc.size(), 2u);
//   vc.append(std::string{});
//   EXPECT_EQ(vc.size(), 3u);
// }

// TEST_F(StringTableTest, SubscriptOperator) {
//   uint8_t string_length = 4;

//   ValueColumn<std::string> vc(string_length);
//   vc.append("bla");
//   EXPECT_EQ(type_cast<std::string>(vc[0]), "bla");
// }

}  // namespace opossum
