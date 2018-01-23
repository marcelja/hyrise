#include "fixed_string.hpp"

#include <sstream>
#include <string>
#include <utility>

namespace opossum {

void FixedString::swap(const FixedString& other) const {
  const char* temp = string().c_str();
  std::memcpy(fixed._mem, other.fixed._mem, fixed._string_length);
  std::memcpy(other.fixed._mem, temp, fixed._string_length);
}

}  // namespace opossum
