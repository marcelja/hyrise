#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../lib/storage/value_vector.hpp"

namespace opossum {

void swap(const FixedString lha, const FixedString rhs) { lha.swap(rhs); }
}

using namespace opossum;


void clear_cache() {
  std::vector<int> clear = std::vector<int>();
  clear.resize(500 * 1000 * 1000, 42);
  for (uint i = 0; i < clear.size(); i++) {
    clear[i] += 1;
  }
  clear.resize(0);
}

void print_vector_memory(const ValueVector<FixedString>& vector, std::string compiler) {
  uint64_t string_length = vector[0].size();

  std::cout << "Memory consumption: \t"
            << (sizeof(ValueVector<FixedString>) + int(vector.size()) * string_length) / 1000 << " kilobytes"
            << std::endl;
}

void print_vector_memory(const ValueVector<std::string>& vector, std::string compiler) {
  uint64_t string_length = vector[0].size();
  uint64_t size = 0;

  if (compiler == "gcc") {
    if (string_length <= 16) {
      size = (sizeof(ValueVector<std::string>) + uint64_t(vector.size()) * 17) / 1000;
    } else {
      size =
          (sizeof(ValueVector<std::string>) + uint64_t(vector.size()) * (sizeof(std::string) + string_length)) / 1000;
    }
  } else if (compiler == "clang") {
    if (string_length <= 22) {
      size = (sizeof(ValueVector<std::string>) + uint64_t(vector.size()) * 23) / 1000;
    } else {
      size =
          (sizeof(ValueVector<std::string>) + uint64_t(vector.size()) * (sizeof(std::string) + string_length)) / 1000;
    }
  }
  std::cout << "Memory consumption: \t" << size << " kilobytes" << std::endl;
}

void benchmark_m() {
  ValueVector<std::string> a;
  ValueVector<FixedString> b(10);
  std::vector<std::string> c;

  std::string insert_me{"blablabla"};
  size_t inserts = 100000;
  {
    auto t1 = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < inserts; ++i) a.push_back(insert_me);
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "inserting " << inserts << " values into ValueVector<std::string>: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;
  }
  print_vector_memory(a, "clang");
  // std::cin.ignore();

  {
    auto t1 = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < inserts; ++i) b.push_back(FixedString(insert_me));
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "inserting " << inserts << " values into ValueVector<FixedString>: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;
  }
  print_vector_memory(b, "clang");
  // std::cin.ignore();

  {
    auto t1 = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < inserts; ++i) c.push_back(insert_me);
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "inserting " << inserts << " values into std::vector<std::string>:      "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;
  }
}

template <typename T>
void read_file(std::vector<ValueVector<T>>& value_vectors, std::vector<std::vector<std::string>>& search_values) {
  std::string line;
  size_t search_counter = 0;
  size_t searches = 100;

  std::ifstream string_table_file("strings_table/string_table.tbl");
  if (string_table_file.is_open()) {
    // Skip first 2 header lines
    std::getline(string_table_file, line);
    std::getline(string_table_file, line);

    while (std::getline(string_table_file, line)) {
      size_t pos = 0, found;
      int index = 0;
      while ((found = line.find_first_of('|', pos)) != std::string::npos) {
        if (search_counter < searches) search_values[index].push_back(line.substr(pos, found - pos));
        value_vectors[index].push_back(line.substr(pos, found - pos));
        pos = found + 1;
        index++;
      }
      value_vectors[index].push_back(line.substr(pos));
      if (search_counter < searches) search_values[index].push_back(line.substr(pos, found - pos));
      search_counter++;
    }
    string_table_file.close();
  } else {
    std::cout << "Unable to open file" << std::endl;
  }

  for (auto& vv : value_vectors) {
    print_vector_memory(vv, "clang");
  }
}

template <typename T>
void benchmark_search(std::vector<ValueVector<T>>& value_vectors, std::vector<std::vector<std::string>>& search_values) {
  for (size_t i = 0; i < value_vectors.size(); ++i) {
    clear_cache();
    auto t1 = std::chrono::high_resolution_clock::now();
    for (auto& sv : search_values[i]) {
      std::lower_bound(value_vectors[i].begin(), value_vectors[i].end(), T(sv));
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "lower_bound for index " << i << " (string_length is " << value_vectors[i][0].size() << ") : " << i << " "
              << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << "us" << std::endl;
  }
}

template <typename T>
void sort_value_vectors(std::vector<ValueVector<T>>& value_vectors) {
  for (auto vv : value_vectors) {
    std::sort(vv.begin(), vv.end());
    std::cout << "sorted: " << vv[0] << ", " << vv[vv.size() - 1 ] << std::endl;
  }
}

void value_vectors_from_file() {
  std::vector<ValueVector<FixedString>> value_vectors = {ValueVector<FixedString>(1),   ValueVector<FixedString>(3),
                                                         ValueVector<FixedString>(7),   ValueVector<FixedString>(15),
                                                         ValueVector<FixedString>(31),  ValueVector<FixedString>(63),
                                                         ValueVector<FixedString>(127), ValueVector<FixedString>(255)};
  std::vector<std::vector<std::string>> search_values;
  for (auto& v : value_vectors) {
    v.size();
    search_values.push_back(std::vector<std::string>());
  }
  read_file(value_vectors, search_values);
  sort_value_vectors(value_vectors);
  std::cout << "FixedString search" << std::endl;
  benchmark_search(value_vectors, search_values);

  std::vector<ValueVector<std::string>> value_vectors_std = {
      ValueVector<std::string>(), ValueVector<std::string>(), ValueVector<std::string>(), ValueVector<std::string>(),
      ValueVector<std::string>(), ValueVector<std::string>(), ValueVector<std::string>(), ValueVector<std::string>()};
  std::vector<std::vector<std::string>> search_values2;
  for (auto& v : value_vectors) {
    v.size();
    search_values2.push_back(std::vector<std::string>());
  }
  read_file(value_vectors_std, search_values2);
  sort_value_vectors(value_vectors_std);
  
  std::cout << "std::string search" << std::endl;
  benchmark_search(value_vectors_std, search_values);
}

template <typename T>
void print_vector(ValueVector<T>& vec) {
  for (auto v : vec) {
    std::cout << v << std::endl;
  }
  std::cout << std::endl;
}

void sort_swap() {
  ValueVector<FixedString> a(10);
  a.push_back(FixedString("aaaaaaaa"));
  a.push_back(FixedString("aaaaaaaa"));
  a.push_back(FixedString("aaaaaaaa"));
  a.push_back(FixedString("abcie"));
  a.push_back(FixedString("sigt"));
  a.push_back(FixedString("sigt"));
  a.push_back("moooeeeepp");
  a.push_back(FixedString("3295629"));

  std::cout << "Original:" << std::endl << std::endl;
  print_vector(a);

  std::sort(a.begin(), a.end());
  std::cout << "Sorted:" << std::endl << std::endl;
  print_vector(a);

  a.erase(std::unique(a.begin(), a.end()), a.end());
  std::cout << "Unique + erase:" << std::endl << std::endl;
  print_vector(a);
}

int main() {
  // value_vector_from_file();
  // value_vector_from_file_stdstr();
  sort_swap();
  benchmark_m();
  value_vectors_from_file();
}
