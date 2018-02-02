#pragma once

#include <memory>
#include <string>
#include <vector>
#include <iostream>

#include "all_type_variant.hpp"
#include "types.hpp"

namespace opossum {

class FixedString {
 public:
  FixedString(const std::string &string) : _mem((char*)string.c_str()), _string_length(string.size()), _delete(false) {
  }

  FixedString(char *mem, size_t string_length) : _mem(mem), _string_length(string_length), _delete(false) {}

  ~FixedString() {
    if (_delete) {
      // std::cout << "FixedString::delete destructor" << std::endl;
      delete [] _mem;
      // std::memcpy(_mem, "ffffffffff", _string_length);
      // std::cout << string();
    }
  }

  FixedString(FixedString &other) : _mem(new char[other._string_length]{}), _string_length(other._string_length), _delete(true) {
    std::cout << "FixedString::copy constructor" << std::endl;
    std::memcpy(_mem, other._mem, _string_length);
  }

  // copy constructor
  FixedString(const FixedString &other) : _mem(new char[other._string_length]{}), _string_length(other._string_length), _delete(true) {
    std::cout << "FixedString::copy constructor 2" << std::endl;
    std::memcpy(_mem, other._mem, _string_length);
  }

  // move constructor
  FixedString(const FixedString &&other) : _mem(new char[other._string_length]{}), _string_length(other._string_length), _delete(true) {
    // std::cout << "FixedString::move constructor" << std::endl;
    std::memcpy(_mem, other._mem, _string_length);
  }

  size_t copy(char *s, size_t len, size_t pos = 0) const {
    auto copied_length = len < _string_length - pos ? len : _string_length - pos;
    std::memcpy(s, _mem + pos, copied_length);
    return copied_length;
  }

  size_t size() const {
    return _string_length;
  }

  std::string string() const {
    // I didn't include an operator std::string on purpose. That led to expensive, unexpected comparisons.
    return std::string(_mem, _string_length);
  }

  const std::vector<char> char_vector() const {
    // std::cout << "_mem adr: " << &_mem[0] << std::endl;
    // std::cout << "_mem adr: " << &_mem[0] + _string_length << std::endl;
    // std::cout << "_mem adr: " << &_mem << std::endl;
    return std::vector<char>(&_mem[0], &_mem[0] + _string_length);
  }

  FixedString &operator=(const FixedString &other) {
    auto copied_length = other.size() < _string_length ? other.size() : _string_length;
    other.copy(_mem, copied_length, 0);
    if (copied_length < _string_length) {
      memset(_mem + copied_length, '\0', _string_length - copied_length);
    }
    return *this;
  }

  bool operator<(const FixedString &other) const {
    const char *this_mem = _mem;
    const char *other_mem = other._mem;
    return memcmp(this_mem, other_mem, _string_length) < 0;
  }

  bool operator==(const FixedString &other) const {
    const char *this_mem = _mem;
    const char *other_mem = other._mem;
    return memcmp(this_mem, other_mem, _string_length) == 0;
  }

  friend std::ostream &operator<<(std::ostream &os, const FixedString &obj) {
    return os << obj.string();
  }

  void swap(const FixedString &other) const;

 private:
  char *const _mem;
  const size_t _string_length;
  const bool _delete;
};

}  // namespace opossum
