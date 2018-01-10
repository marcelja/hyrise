#include "value_vector.hpp"

#include <limits>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "column_visitable.hpp"
#include "type_cast.hpp"
#include "utils/assert.hpp"
#include "utils/performance_warning.hpp"

namespace opossum {

// template <typename T>
// using iterator = typename std::vector<T>::iterator;

template <typename T>
ValueVector<T>::ValueVector() {
  _fixed_string_length = 0;
}

template <typename T>
ValueVector<T>::ValueVector(uint8_t fixed_string_length) {
  _fixed_string_length = 77;
  // _fixed_string = true;
}

// template <typename T>
// void ValueVector<T>::copy_valuess(pmr_concurrent_vector<T> values) {
//   // _values(std::move(values));
// }

template <typename T>
void ValueVector<T>::push_back(const T& value) {
  std::cout<<"const push back";
  _values.push_back(std::forward<const T>(value));
}

template <typename T>
void ValueVector<T>::push_back(T&& value) {
  std::cout<<"lkjsadflkjasfd push back";
  _values.push_back(std::forward<T>(value));
}

// template <typename T>
// iterator ValueVector<T>::begin() noexcept { return _values.begin(); }

// template <typename T>
// iterator ValueVector<T>::end() noexcept { return _values.end(); }

// template <typename T>
// T& ValueVector<T>::operator[] (size_t n) { return _values[n]; }

// template <typename T>
// const T& ValueVector<T>::operator[] (size_t n) const { return _values[n]; }

EXPLICITLY_INSTANTIATE_DATA_TYPES(ValueVector);


}  // namespace opossum
