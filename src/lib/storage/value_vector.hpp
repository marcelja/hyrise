#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "types.hpp"

namespace opossum {

template <typename T>
class ValueVector {
 public:
  explicit ValueVector();

  explicit ValueVector(uint8_t fixed_string_length);

  // void copy_values(pmr_concurrent_vector<T> values) {
  //   _values(std::move(values));
  // };

  void push_back(const T& value);

  void push_back(T&& value);

 protected:
  pmr_vector<T> _values;
  std::vector<char> _fixed_string_vector;
  uint8_t _fixed_string_length;
};

}  // namespace opossum
