#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
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
    if (string_length <= 15) {
      size = (sizeof(ValueVector<std::string>) + uint64_t(vector.size()) * sizeof(std::string)) / 1000;
    } else {
      size = (sizeof(ValueVector<std::string>) + uint64_t(vector.size()) * (sizeof(std::string) + string_length + 1)) /
             1000;
    }
  } else if (compiler == "clang") {
    if (string_length <= 22) {
      size = (sizeof(ValueVector<std::string>) + uint64_t(vector.size()) * sizeof(std::string)) / 1000;
    } else {
      size = (sizeof(ValueVector<std::string>) + uint64_t(vector.size()) * (sizeof(std::string) + 32)) / 1000;
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
      uint64_t index = 0;
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
void benchmark_search(std::vector<ValueVector<T>>& value_vectors,
                      std::vector<std::vector<std::string>>& search_values) {
  for (size_t i = 0; i < value_vectors.size(); ++i) {
    std::vector<uint64_t> times;
    for (auto& sv : search_values[i]) {
      // clear_cache();
      auto t1 = std::chrono::high_resolution_clock::now();
      std::lower_bound(value_vectors[i].begin(), value_vectors[i].end(), T(sv));

      auto t2 = std::chrono::high_resolution_clock::now();
      times.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count());
    }
    auto sum_of_elems = std::accumulate(times.begin(), times.end(), 0);
    std::cout << "lower_bound for index " << i << " (string_length is " << value_vectors[i][0].size()
              << ") : " << sum_of_elems / times.size() << " ns" << std::endl;
  }
}

template <typename T>
void sort_value_vectors(std::vector<ValueVector<T>>& value_vectors) {
  for (auto vv : value_vectors) {
    std::sort(vv.begin(), vv.end());
    std::cout << "sorted: " << vv[0] << ", " << vv[vv.size() - 1] << std::endl;
  }
}

void value_vectors_from_file_old() {
  // includes sorting of FixedString column
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

void value_vectors_from_file() {
  // std::vector<ValueVector<FixedString>> value_vectors = {ValueVector<FixedString>(1),   ValueVector<FixedString>(3),
  //                                                        ValueVector<FixedString>(7),   ValueVector<FixedString>(15),
  //                                                        ValueVector<FixedString>(31),  ValueVector<FixedString>(63),
  //                                                        ValueVector<FixedString>(127), ValueVector<FixedString>(255)};
  std::vector<ValueVector<FixedString>> value_vectors;

  std::vector<std::vector<std::string>> search_values;
  for (auto i = 0; i < 8; i++) {
    search_values.push_back(std::vector<std::string>());
  }
  // read_file(value_vectors, search_values);

  std::vector<ValueVector<std::string>> value_vectors_std = {
      ValueVector<std::string>(), ValueVector<std::string>(), ValueVector<std::string>(), ValueVector<std::string>(),
      ValueVector<std::string>(), ValueVector<std::string>(), ValueVector<std::string>(), ValueVector<std::string>()};
  std::vector<std::vector<std::string>> search_values2;
  for (auto& v : value_vectors) {
    v.size();
    search_values2.push_back(std::vector<std::string>());
  }
  read_file(value_vectors_std, search_values);

  sort_value_vectors(value_vectors_std);
  std::cout << "std::string search" << std::endl;
  benchmark_search(value_vectors_std, search_values);

  // sort_value_vectors(value_vectors);

  for (const auto& vvs : value_vectors_std) {
    value_vectors.push_back(ValueVector<FixedString>(vvs.begin(), vvs.end(), vvs[0].size()));
  }
  std::cout << "FixedString search" << std::endl;
  benchmark_search(value_vectors, search_values);
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

std::string gen_random(const int len) {
  static const char alphanum[] =
      "0123456789"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz";

  std::string str = "";

  for (int i = 0; i < len; ++i) {
    str += alphanum[rand() % (sizeof(alphanum) - 1)];
  }

  return str;
}

void memory_test() {
  std::cout << gen_random(10) << std::endl;
  std::cout << gen_random(10) << std::endl;
  std::cout << gen_random(10) << std::endl;
  std::cout << gen_random(10) << std::endl;
  // ValueVector<FixedString> vec(10);
  // int inserts = 100000000;
  // vec.reserve(inserts);
  // for (int i = 0; i < inserts; i++) {
  //   vec.push_back(gen_random(10));
  // }

  // print_vector_memory(vec, "clang");
  // print_vector_memory(vec, "gcc");

  ValueVector<std::string> vec;
  int inserts = 100000000;
  vec.reserve(inserts);
  for (int i = 0; i < inserts; i++) {
    vec.push_back(gen_random(30));
  }

  print_vector_memory(vec, "clang");
  print_vector_memory(vec, "gcc");
  std::cin.ignore();
  std::cout << vec.size() << std::endl;
}

void single_char() {
  auto elems = 2000000000;

  std::vector<char> chari;
  auto t1 = std::chrono::high_resolution_clock::now();

  for (auto i = 0; i < elems; i++) {
    chari.push_back('A');
  }
  auto t2 = std::chrono::high_resolution_clock::now();
  std::cout << "inserting in std::vector<char>: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;

  pmr_vector<char> pmr;
  auto t11 = std::chrono::high_resolution_clock::now();

  for (auto i = 0; i < elems; i++) {
    pmr.push_back('A');
  }
  auto t21 = std::chrono::high_resolution_clock::now();
  std::cout << "inserting pmr_vector<char>: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(t21 - t11).count() << "ms" << std::endl;

  pmr_concurrent_vector<char> pmrc;
  auto t111 = std::chrono::high_resolution_clock::now();

  for (auto i = 0; i < elems; i++) {
    pmrc.push_back('A');
  }
  auto t211 = std::chrono::high_resolution_clock::now();
  std::cout << "inserting pmr_concurrent_vector<char>: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(t211 - t111).count() << "ms" << std::endl;
}

void word_inserter() {
  auto elems = 200000000;

  std::vector<char> chari;
  auto t1 = std::chrono::high_resolution_clock::now();

  for (auto i = 0; i < elems; i++) {
    std::string str = "Hallo";
    std::copy(str.begin(), str.end(), std::back_inserter(chari));
  }
  auto t2 = std::chrono::high_resolution_clock::now();
  std::cout << "inserting hello in std::vector<char>: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;

  pmr_vector<char> pmr;
  auto t11 = std::chrono::high_resolution_clock::now();

  for (auto i = 0; i < elems; i++) {
    std::string str = "Hallo";
    std::copy(str.begin(), str.end(), std::back_inserter(pmr));
  }
  auto t21 = std::chrono::high_resolution_clock::now();
  std::cout << "inserting hello in pmr_vector<char>: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(t21 - t11).count() << "ms" << std::endl;

  pmr_concurrent_vector<char> pmrc;
  auto t111 = std::chrono::high_resolution_clock::now();

  for (auto i = 0; i < elems; i++) {
    std::string str = "Hallo";
    std::copy(str.begin(), str.end(), std::back_inserter(pmrc));
  }
  auto t211 = std::chrono::high_resolution_clock::now();
  std::cout << "inserting hello pmr_concurrent_vector<char>: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(t211 - t111).count() << "ms" << std::endl;
}

void copy_constructor_test() {
  std::cout << "Creating value vector" << std::endl;
  ValueVector<FixedString> a(10);
  a.push_back(FixedString("aaaaaaaa"));
  a.push_back(FixedString("bbbbbbbb"));

  std::cout << "Creating fixed_string1" << std::endl;
  auto fixed_string1 = FixedString("fs1");
  std::cout << "Creating fixed_string2" << std::endl;
  auto fixed_string2 = FixedString(fixed_string1);
  std::cout << "Creating fixed_string3" << std::endl;
  auto fixed_string3 = FixedString(*a.begin());
}

void long_words() {
  auto elems = 2000000;

  pmr_vector<char> pmr;
  auto t11 = std::chrono::high_resolution_clock::now();

  for (auto i = 0; i < elems; i++) {
    const std::string str =
        "HalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHallo"
        "HalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHallo"
        "HalloHalloHalloHallo";
    pmr.insert(pmr.cend(), str.begin(), str.end());

    // std::copy(str.begin(), str.end(), std::back_inserter(pmr));
  }

  // for (auto ele : pmr) {
  //   std::cout << ele;
  // }

  auto t21 = std::chrono::high_resolution_clock::now();
  std::cout << "inserting hello in pmr_vector<char>: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(t21 - t11).count() << "ms" << std::endl;

  pmr_vector<std::string> pmrs;
  auto t111 = std::chrono::high_resolution_clock::now();

  for (auto i = 0; i < elems; i++) {
    std::string str =
        "HalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHallo"
        "HalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHalloHallo"
        "HalloHalloHalloHallo";
    pmrs.push_back(str);
  }
  auto t211 = std::chrono::high_resolution_clock::now();
  std::cout << "inserting hello in pmr_vector<std::string>: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(t211 - t111).count() << "ms" << std::endl;
}

void iterator_test() {
  int assignments = 10000000;

  std::vector<std::string> a = {"abc", "def", "hifads"};
  ValueVector<FixedString> b{a.begin(), a.end(), 10};

  auto it = a.begin();
  auto t111 = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < assignments; i++) {
    it = a.end();
    it = a.begin();

  }
  auto t211 = std::chrono::high_resolution_clock::now();
  std::cout << "std vect iterator "
            << std::chrono::duration_cast<std::chrono::milliseconds>(t211 - t111).count() << "ms" << std::endl;


  auto it2 = b.begin();
  t111 = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < assignments; i++) {
    it2 = b.end();
    it2 = b.begin();

  }
  t211 = std::chrono::high_resolution_clock::now();

  std::cout << "value vector fixed string iterator "
            << std::chrono::duration_cast<std::chrono::milliseconds>(t211 - t111).count() << "ms" << std::endl;

}

int main() {
  // value_vector_from_file();
  // value_vector_from_file_stdstr();
  // sort_swap();
  // benchmark_m();
  // value_vectors_from_file();
  // memory_test();
  // long_words();
  copy_constructor_test();
  iterator_test();
  value_vectors_from_file_old();
}
