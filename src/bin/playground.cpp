#include <chrono>
#include <iostream>
#include <fstream>
#include <string>

#include "../lib/storage/value_vector.hpp"


void benchmark() {
  opossum::ValueVector<std::string> a;
  opossum::ValueVector<opossum::FixedString> b(10);
  std::vector<std::string> c;

  std::string insert_me{"blablabla"};
  size_t inserts = 1000000;
  size_t searches = 10000;
  for (int run = 0; run < 2; ++run) {
    {
      auto t1 = std::chrono::high_resolution_clock::now();
      for (size_t i = 0; i < inserts; ++i) a.push_back(insert_me);
      auto t2 = std::chrono::high_resolution_clock::now();
      std::cout << "inserting " << inserts << " values into opossum::ValueVector<std::string>: "
                << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;
      std::cout << "sizeof: " << sizeof(std::string) * a.vector_size() << std::endl;
    }

    {
      auto t1 = std::chrono::high_resolution_clock::now();
      for (size_t i = 0; i < inserts; ++i) b.push_back(opossum::FixedString(insert_me));
      auto t2 = std::chrono::high_resolution_clock::now();
      std::cout << "inserting " << inserts << " values into opossum::ValueVector<opossum::FixedString>: "
                << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;
      std::cout << "sizeof: " << sizeof(char) * b.vector_size() << std::endl;
    }

    {
      auto t1 = std::chrono::high_resolution_clock::now();
      for (size_t i = 0; i < inserts; ++i) c.push_back(insert_me);
      auto t2 = std::chrono::high_resolution_clock::now();
      std::cout << "inserting " << inserts << " values into std::vector<std::string>:      "
                << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;
      std::cout << "sizeof: " << sizeof(std::string) * c.size() << std::endl;
    }
  }
  for(int run = 0; run < 2; ++run) {
    {
    auto t1 = std::chrono::high_resolution_clock::now();
    for(size_t i = 0; i < searches; ++i) std::lower_bound(a.begin(), a.end(), insert_me);
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "binary search for " << searches << " values in opossum::ValueVector<std::string>: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;
    }

    {
    auto t1 = std::chrono::high_resolution_clock::now();
    for(size_t i = 0; i < searches; ++i) std::lower_bound(b.begin(), b.end(), opossum::FixedString(insert_me));
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "binary search for " << searches << " values in opossum::ValueVector<opossum::FixedString>: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;
    }

    {
    auto t1 = std::chrono::high_resolution_clock::now();
    for(size_t i = 0; i < searches; ++i) std::lower_bound(c.begin(), c.end(), insert_me);
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "binary search for " << searches << " values in std::vector<std::string>:      " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;
    }
  }
}

void value_vector_from_file() {
  // std::string path = "../../strings_table";
  // for (auto & p : fs::directory_iterator(path))
  //   std::cout << p << std::endl;

  std::vector<opossum::ValueVector<opossum::FixedString>> value_vectors = {
    opossum::ValueVector<opossum::FixedString>(1),
    opossum::ValueVector<opossum::FixedString>(3),
    opossum::ValueVector<opossum::FixedString>(7),
    opossum::ValueVector<opossum::FixedString>(15),
    opossum::ValueVector<opossum::FixedString>(31),
    opossum::ValueVector<opossum::FixedString>(63),
    opossum::ValueVector<opossum::FixedString>(127),
    opossum::ValueVector<opossum::FixedString>(255)
  };

  std::string line;
  std::ifstream string_table_file("strings_table/string_table.tbl");
  if (string_table_file.is_open()) {
    // Skip first 2 header lines
    std::getline(string_table_file, line);
    std::getline(string_table_file, line);

    while (std::getline(string_table_file, line)) {
      size_t pos = 0, found;
      int index = 0;
      while((found = line.find_first_of('|', pos)) != std::string::npos) {
        value_vectors[index].push_back(opossum::FixedString(line.substr(pos, found - pos)));
        pos = found+1;
        index++;
      }
      value_vectors[index].push_back(opossum::FixedString(line.substr(pos)));
    }
    string_table_file.close();
  }
  else {
    std::cout << "Unable to open file" << std::endl;
  }
}

int main() {
  // benchmark();
  value_vector_from_file();
}
