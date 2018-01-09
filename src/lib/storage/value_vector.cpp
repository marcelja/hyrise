#include "value_vector.hpp"

#include <limits>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

// #include "column_visitable.hpp"
// #include "type_cast.hpp"
// #include "utils/assert.hpp"
// #include "utils/performance_warning.hpp"

namespace opossum {

template <typename T>
ValueVector<T>::ValueVector(uint8_t fixed_string_length) {
  _fixed_string_length = 77;
  // _fixed_string = true;
}

}  // namespace opossum
