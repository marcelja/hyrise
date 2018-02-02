#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <storage/value_column.hpp>

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

void print_vector_memory(const ValueVector<FixedString> &vector, std::string compiler) {
  uint64_t string_length = vector[0].size();

  std::cout << "Memory consumption: \t"
            << (sizeof(ValueVector<FixedString>) + int(vector.size()) * string_length) / 1000 << " kilobytes"
            << std::endl;
}

void print_vector_memory(const ValueVector<std::string> &vector, std::string compiler) {
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

template<typename T>
void read_file(std::vector<ValueVector<T>> &value_vectors, std::vector<std::vector<std::string>> &search_values) {
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

//  for (auto &vv : value_vectors) {
//    print_vector_memory(vv, "clang");
//  }
}

template<typename T>
void read_file_std(std::vector<pmr_vector<T>> &value_vectors) {
  std::string line;

  std::ifstream string_table_file("strings_table/string_table.tbl");
  if (string_table_file.is_open()) {
    // Skip first 2 header lines
    std::getline(string_table_file, line);
    std::getline(string_table_file, line);

    while (std::getline(string_table_file, line)) {
      size_t pos = 0, found;
      int index = 0;
      while ((found = line.find_first_of('|', pos)) != std::string::npos) {
        value_vectors[index].push_back(line.substr(pos, found - pos));
        pos = found + 1;
        index++;
      }
      value_vectors[index].push_back(line.substr(pos));
    }
    string_table_file.close();
  } else {
    std::cout << "Unable to open file" << std::endl;
  }
}

template<typename T>
void sort_value_vectors(std::vector<ValueVector<T>> &value_vectors) {
  for (auto vv : value_vectors) {
    std::cout << "sort value vector column" << std::endl;
    std::sort(vv.begin(), vv.end());
  }
}

template<typename T>
void sort_std_vectors(std::vector<std::vector<T>> &value_vectors) {
  for (auto vv : value_vectors) {
    std::cout << "sort std vector column" << std::endl;
    std::sort(vv.begin(), vv.end());
  }
}

void value_vectors_from_file() {
  std::vector<ValueVector<FixedString>> value_vectors = {ValueVector<FixedString>(1, 100000), ValueVector<FixedString>(3, 100000),
                                                         ValueVector<FixedString>(7, 100000), ValueVector<FixedString>(15, 100000),
                                                         ValueVector<FixedString>(31, 100000), ValueVector<FixedString>(63, 100000),
                                                         ValueVector<FixedString>(127, 100000), ValueVector<FixedString>(255, 100000)};

  std::vector<pmr_vector<std::string>> std_vectors = {pmr_vector<std::string>(), pmr_vector<std::string>(),
                                                      pmr_vector<std::string>(), pmr_vector<std::string>(),
                                                      pmr_vector<std::string>(), pmr_vector<std::string>(),
                                                      pmr_vector<std::string>(), pmr_vector<std::string>()};


  // init search values
  std::vector<std::vector<std::string>> search_values;
  for (auto &v : value_vectors) {
    v.size();
    search_values.push_back(std::vector<std::string>());
  }

  // init value - and std vectors
//  clear_cache();
  auto t = std::chrono::high_resolution_clock::now();
  std::cout << "ValueVector read ";
  read_file(value_vectors, search_values);
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t).count();
  std::cout << "finished in: " << duration << " ms" << std::endl;

  auto t2 = std::chrono::high_resolution_clock::now();
  std::cout << "std::vector read ";
  read_file_std(std_vectors);
  auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t2).count();
  std::cout << "finished in: " << duration2 << " ms" << std::endl;

  std::cout << "valuevector+fixedstring is : " << (duration2 / duration) * 100 << "% faster" << std::endl;
//  clear_cache();



//  std::cout << "FixedString sort" << std::endl;
//  sort_value_vectors(value_vectors);
//  std::cout << "FixedString search" << std::endl;
//  benchmark_search(value_vectors, search_values);

//  std::vector<ValueVector<std::string>> value_vectors_std = {
//      ValueVector<std::string>(), ValueVector<std::string>(), ValueVector<std::string>(), ValueVector<std::string>(),
//      ValueVector<std::string>(), ValueVector<std::string>(), ValueVector<std::string>(), ValueVector<std::string>()};
//  std::vector<std::vector<std::string>> search_values2;
//  for (auto& v : value_vectors) {
//    v.size();
//    search_values2.push_back(std::vector<std::string>());
//  }
//  read_file(value_vectors_std, search_values2);
//  std::cout << "sort value vectors" << std::endl;
//  sort_value_vectors(value_vectors_std);
//
//  std::cout << "std::string search" << std::endl;
//  benchmark_search(value_vectors_std, search_values);
}

template<typename T>
void print_vector(ValueVector<T> &vec) {
  for (auto v : vec) {
    std::cout << v << std::endl;
  }
  std::cout << std::endl;
}

int main() {
  value_vectors_from_file();
  value_vectors_from_file();
  value_vectors_from_file();
  value_vectors_from_file();
  value_vectors_from_file();
}
