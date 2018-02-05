#include "fixed_string.hpp"

#include <sstream>
#include <string>
#include <utility>

namespace opossum {

void FixedString::swap(const FixedString &other) const {
  std::swap_ranges(_mem, _mem + _string_length, other._mem);
}

}  // namespace opossum
