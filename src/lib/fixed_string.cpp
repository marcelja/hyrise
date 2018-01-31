#include "fixed_string.hpp"

#include <sstream>
#include <string>
#include <utility>

namespace opossum {

void FixedString::swap(const FixedString &other) const {
  // const char *temp = string().c_str();
  const std::vector<char> temp = char_vector();
  // std::vector<char>
  std::memcpy(_mem, other._mem, _string_length);
  std::memcpy(other._mem, &temp[0], _string_length);
}

}  // namespace opossum
