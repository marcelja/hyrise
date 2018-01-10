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
  using iterator = typename std::vector<T>::iterator;

  explicit ValueVector();

  explicit ValueVector(uint8_t fixed_string_length);

  // void copy_values(pmr_concurrent_vector<T> values) {
  //   _values(std::move(values));
  // };

  void push_back(const T& value);

  void push_back(T&& value);

  iterator begin() noexcept;

  iterator end() noexcept;

T& operator[] (size_t n);

const T& operator[] (size_t n) const;


 protected:
  pmr_vector<T> _values;
  std::vector<char> _fixed_string_vector;
  uint8_t _fixed_string_length;
};

}  // namespace opossum
