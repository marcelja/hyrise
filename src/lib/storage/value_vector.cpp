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
T& ValueVector<T>::at(const ChunkOffset chunk_offset) {
  return _values.at(chunk_offset);
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
typename ValueVector<T>::const_iterator ValueVector<T>::cbegin() noexcept {
  return _values.cbegin();
}

template <typename T>
typename ValueVector<T>::const_iterator ValueVector<T>::cend() noexcept {
  return _values.cend();
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
size_t ValueVector<T>::size() const {
  return _values.size();
}

template <typename T>
size_t ValueVector<T>::capacity() const {
  return _values.capacity();
}

template <typename T>
void ValueVector<T>::shrink_to_fit() {
  _values.shrink_to_fit();
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

FixedString ValueVector<FixedString>::at(const ChunkOffset chunk_offset) {
  return FixedString((char*)&_vector.at(chunk_offset * _string_length), _string_length);
}

ValueVector<FixedString>::iterator ValueVector<FixedString>::begin() noexcept {
  return iterator(_string_length, _vector, 0);
}

ValueVector<FixedString>::iterator ValueVector<FixedString>::end() noexcept {
  return iterator(_string_length, _vector, _vector.size());
}

ValueVector<FixedString>::iterator ValueVector<FixedString>::cbegin() noexcept {
  return iterator(_string_length, _vector, 0);
}

ValueVector<FixedString>::iterator ValueVector<FixedString>::cend() noexcept {
  return iterator(_string_length, _vector, _vector.size());
}

typedef boost::reverse_iterator<ValueVector<FixedString>::iterator> reverse_iterator;
reverse_iterator ValueVector<FixedString>::rbegin() noexcept {
  return reverse_iterator(end());
}

reverse_iterator ValueVector<FixedString>::rend() noexcept {
  return reverse_iterator(begin());
}

FixedString ValueVector<FixedString>::operator[](size_t n) {
  return {FixedString(&_vector[n * _string_length], _string_length)};
}

const FixedString ValueVector<FixedString>::operator[](size_t n) const {
  return {FixedString((char*)&_vector[n * _string_length], _string_length)};
}

size_t ValueVector<FixedString>::size() const { return _vector.size() / _string_length; }

size_t ValueVector<FixedString>::capacity() const { return _vector.capacity(); }

void ValueVector<FixedString>::erase(const iterator start, const iterator end) {
    auto it = _vector.begin();
    std::advance(it, _vector.size() - std::distance(start, end) * _string_length);
    _vector.erase(it, _vector.end());
}

void ValueVector<FixedString>::shrink_to_fit() {
  _vector.shrink_to_fit();
}

EXPLICITLY_INSTANTIATE_DATA_TYPES(ValueVector);

}  // namespace opossum
