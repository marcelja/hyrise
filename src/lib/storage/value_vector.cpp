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
ValueVector<T>::ValueVector() {}

template <typename T>
ValueVector<T>::ValueVector(iterator begin, iterator end) {
  while (begin != end) {
    push_back(*begin);
    ++begin;
  }
}

template <typename T>
ValueVector<T>::ValueVector(typename std::vector<T>::iterator begin, typename std::vector<T>::iterator end) {
  while (begin != end) {
    push_back(*begin);
    ++begin;
  }
}

template <typename T>
ValueVector<T>::ValueVector(const_iterator cbegin, const_iterator cend) {
  while (cbegin != cend) {
    push_back(*cbegin);
    ++cbegin;
  }
}

template <typename T>
ValueVector<T>::ValueVector(typename std::vector<T>::const_iterator cbegin,
                            typename std::vector<T>::const_iterator cend) {
  while (cbegin != cend) {
    push_back(*cbegin);
    ++cbegin;
  }
}

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
typename ValueVector<T>::reverse_iterator ValueVector<T>::rbegin() noexcept {
  return _values.rbegin();
}

template <typename T>
typename ValueVector<T>::reverse_iterator ValueVector<T>::rend() noexcept {
  return _values.rend();
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

template <typename T>
PolymorphicAllocator<T> ValueVector<T>::get_allocator() {
  return _values.get_allocator();
}

// Implementation of ValueVector<FixedString> starts here

ValueVector<FixedString>::ValueVector(typename std::vector<FixedString>::iterator begin,
                                      typename std::vector<FixedString>::iterator end, size_t string_length)
    : _string_length(string_length) {
  while (begin != end) {
    push_back(*begin);
    ++begin;
  }
}

ValueVector<FixedString>::ValueVector(pmr_vector<FixedString>::iterator begin, pmr_vector<FixedString>::iterator end,
                                      size_t string_length)
    : _string_length(string_length) {
  while (begin != end) {
    push_back(*begin);
    ++begin;
  }
}

ValueVector<FixedString>::ValueVector(pmr_vector<FixedString>::const_iterator cbegin,
                                      pmr_vector<FixedString>::const_iterator cend, size_t string_length)
    : _string_length(string_length) {
  while (cbegin != cend) {
    push_back(*cbegin);
    ++cbegin;
  }
}

ValueVector<FixedString>::ValueVector(typename std::vector<FixedString>::const_iterator cbegin,
                                      typename std::vector<FixedString>::const_iterator cend, size_t string_length)
    : _string_length(string_length) {
  while (cbegin != cend) {
    push_back(*cbegin);
    ++cbegin;
  }
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
reverse_iterator ValueVector<FixedString>::rbegin() noexcept { return reverse_iterator(end()); }

reverse_iterator ValueVector<FixedString>::rend() noexcept { return reverse_iterator(begin()); }

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

void ValueVector<FixedString>::shrink_to_fit() { _vector.shrink_to_fit(); }

PolymorphicAllocator<FixedString> ValueVector<FixedString>::get_allocator() { return _vector.get_allocator(); }

EXPLICITLY_INSTANTIATE_DATA_TYPES(ValueVector);

}  // namespace opossum
