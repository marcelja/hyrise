#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "../lib/storage/value_vector.hpp"

namespace opossum {

void swap(const FixedString lha, const FixedString rhs) {
  lha.swap(rhs);
}

void print_vector_memory(const ValueVector<FixedString>& vector, std::string compiler) {
  uint64_t string_length = vector[0].size();

  std::cout << "Memory consumption: \t\t" << (sizeof(ValueVector<FixedString>) + int(vector.capacity())) / 1000 << " kilobytes" << std::endl;
  std::cout << "Memory consumption(size): \t" << (sizeof(ValueVector<FixedString>) + int(vector.size()) * string_length) / 1000 << " kilobytes" << std::endl;
}

void print_vector_memory(const ValueVector<std::string>& vector, std::string compiler) {
  uint64_t string_length = vector[0].size();
  uint64_t size = 0;
  uint64_t size_size = 0;

  if (compiler == "gcc") {
    if (string_length <= 16) {
      size = (sizeof(ValueVector<std::string>) + uint64_t(vector.capacity()) * 17) / 1000;
      size_size = (sizeof(ValueVector<std::string>) + uint64_t(vector.size()) * 17) / 1000;
    } else {
      size = (sizeof(ValueVector<std::string>) + uint64_t(vector.capacity()) * (sizeof(std::string) + string_length)) / 1000;
      size_size = (sizeof(ValueVector<std::string>) + uint64_t(vector.size()) * (sizeof(std::string) + string_length)) / 1000;
    }

  } else if (compiler == "clang") {
    if (string_length <= 22) {
      size = (sizeof(ValueVector<std::string>) + uint64_t(vector.capacity()) * 23) / 1000;
      size_size = (sizeof(ValueVector<std::string>) + uint64_t(vector.size()) * 23) / 1000;
    } else {
      size = (sizeof(ValueVector<std::string>) + uint64_t(vector.capacity()) * (sizeof(std::string) + string_length)) / 1000;
      size_size = (sizeof(ValueVector<std::string>) + uint64_t(vector.size()) * (sizeof(std::string) + string_length)) / 1000;
    }
  }
  std::cout << "Memory consumption: \t\t" << size << " kilobytes" << std::endl;
  std::cout << "Memory consumption(size): \t" << size_size << " kilobytes" << std::endl;
}

void benchmark() {
  ValueVector<std::string> a;
  ValueVector<FixedString> b(10);
  std::vector<std::string> c;

  std::string insert_me{"blablabla"};
  size_t inserts = 1000000;
  size_t searches = 10000;
  for (int run = 0; run < 2; ++run) {
    {
      auto t1 = std::chrono::high_resolution_clock::now();
      for (size_t i = 0; i < inserts; ++i) a.push_back(insert_me);
      auto t2 = std::chrono::high_resolution_clock::now();
      std::cout << "inserting " << inserts << " values into ValueVector<std::string>: "
                << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;
      std::cout << "sizeof: " << sizeof(std::string) * a.size() << std::endl;
    }
    print_vector_memory(a, "clang");
    // std::cin.ignore();

    {
      auto t1 = std::chrono::high_resolution_clock::now();
      for (size_t i = 0; i < inserts; ++i) b.push_back(FixedString(insert_me));
      auto t2 = std::chrono::high_resolution_clock::now();
      std::cout << "inserting " << inserts << " values into ValueVector<FixedString>: "
                << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;
      std::cout << "sizeof: " << sizeof(char) * b.size() << std::endl;
    }
    print_vector_memory(b, "clang");
    // std::cin.ignore();

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
    std::cout << "binary search for " << searches << " values in ValueVector<std::string>: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;
    }

    {
    auto t1 = std::chrono::high_resolution_clock::now();
    for(size_t i = 0; i < searches; ++i) std::lower_bound(b.begin(), b.end(), FixedString(insert_me));
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "binary search for " << searches << " values in ValueVector<FixedString>: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;
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

  std::vector<ValueVector<FixedString>> value_vectors = {
    ValueVector<FixedString>(1),
    ValueVector<FixedString>(3),
    ValueVector<FixedString>(7),
    ValueVector<FixedString>(15),
    ValueVector<FixedString>(31),
    ValueVector<FixedString>(63),
    ValueVector<FixedString>(127),
    ValueVector<FixedString>(255)
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
        value_vectors[index].push_back(FixedString(line.substr(pos, found - pos)));
        pos = found+1;
        index++;
      }
      value_vectors[index].push_back(FixedString(line.substr(pos)));
    }
    string_table_file.close();
  }
  else {
    std::cout << "Unable to open file" << std::endl;
  }

  for (auto& vv : value_vectors) {
    print_vector_memory(vv, "clang");
  }
}

void value_vector_from_file_stdstr() {
  // std::string path = "../../strings_table";
  // for (auto & p : fs::directory_iterator(path))
  //   std::cout << p << std::endl;

  std::vector<ValueVector<std::string>> value_vectors = {
    ValueVector<std::string>(),
    ValueVector<std::string>(),
    ValueVector<std::string>(),
    ValueVector<std::string>(),
    ValueVector<std::string>(),
    ValueVector<std::string>(),
    ValueVector<std::string>(),
    ValueVector<std::string>()
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
        value_vectors[index].push_back(line.substr(pos, found - pos));
        pos = found+1;
        index++;
      }
      value_vectors[index].push_back(line.substr(pos));
    }
    string_table_file.close();
  }
  else {
    std::cout << "Unable to open file" << std::endl;
  }

  for (auto& vv : value_vectors) {
    print_vector_memory(vv, "clang");
  }
}

void sort_swap() {
  ValueVector<FixedString> a(10);
  a.push_back(FixedString("abcie"));
  a.push_back(FixedString("aaaaaaaa"));
  a.push_back(FixedString("sigt"));
  a.push_back(FixedString("3295629"));

  auto it1 = a.begin();
  auto it2 = a.begin();
  ++it2;
  std::iter_swap(it1, it2);

  for (auto i = a.begin(); i != a.end(); ++i)
  {
    std::cout << *i << std::endl;
  }
  std::cout << std::endl;

  std::sort(a.begin(), a.end());
  for (auto i = a.begin(); i != a.end(); ++i)
  {
    std::cout << *i << std::endl;
  }

  std::cout << std::endl;
  std::cout << "rev" << std::endl;
  for (auto i = a.rbegin(); i != a.rend(); ++i)
  {
    std::cout << *i << std::endl;
  }
}

}
int main() {
  // benchmark();
  // value_vector_from_file();
  // value_vector_from_file_stdstr();
  opossum::sort_swap();
}
