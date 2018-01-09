#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "types.hpp"


namespace opossum {

// ValueVector is a vector which can also store strings with a fixed length as a vector of chars.
template <typename T>
class ValueVector {
 public:
  explicit ValueVector(uint8_t fixed_string_length);

 protected:
  pmr_concurrent_vector<T> _values;
  std::vector<char> _fixed_string_vector;
  uint8_t _fixed_string_length;

};

}  // namespace opossum
