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

template <typename T>
ValueVector<T>::ValueVector() {
  // _fixed_string_length = 0;
}

// template <typename T>
// ValueVector<T>::ValueVector(uint8_t fixed_string_length) {
//   _fixed_string_length = 77;
//   // _fixed_string = true;
// }

// template <typename T>
// void ValueVector<T>::copy_valuess(pmr_concurrent_vector<T> values) {
//   // _values(std::move(values));
// }

template <typename T>
void ValueVector<T>::push_back(const T& value) {
  _values.push_back(std::forward<const T>(value));
}

template <typename T>
void ValueVector<T>::push_back(T&& value) {
  _values.push_back(std::forward<T>(value));
}

template <typename T>
typename ValueVector<T>::iterator ValueVector<T>::begin() noexcept {
  return _values.begin();
}

template <typename T>
typename ValueVector<T>::iterator ValueVector<T>::end() noexcept {
  return _values.end();
}

template <typename T>
T& ValueVector<T>::operator[](size_t n) {
  return _values[n];
}

template <typename T>
const T& ValueVector<T>::operator[](size_t n) const {
  return _values[n];
}

template <typename T>
size_t ValueVector<T>::vector_size() const {
  return _values.size();
}

void ValueVector<FixedString>::push_back(const FixedString& value) {
  push_back(std::forward<FixedString>((FixedString&)value));
}

void ValueVector<FixedString>::push_back(FixedString&& string) {
  auto pos = _vector.size();
  _vector.resize(_vector.size() + _string_length);
  string.copy(&_vector[pos], _string_length);
  if (string.size() < _string_length) {
    std::fill(_vector.begin() + pos + string.size(), _vector.begin() + pos + _string_length, '\0');
  }
}

ValueVector<FixedString>::iterator ValueVector<FixedString>::begin() noexcept {
  return iterator(_string_length, _vector, 0);
}

ValueVector<FixedString>::iterator ValueVector<FixedString>::end() noexcept {
  return iterator(_string_length, _vector, _vector.size());
}

FixedString ValueVector<FixedString>::operator[](size_t n) {
  return {FixedString(&_vector[n * _string_length], _string_length)};
}

const FixedString ValueVector<FixedString>::operator[](size_t n) const {
  return {FixedString((char*)&_vector[n * _string_length], _string_length)};
}

size_t ValueVector<FixedString>::vector_size() const { return _vector.size(); }

EXPLICITLY_INSTANTIATE_DATA_TYPES(ValueVector);

}  // namespace opossum
