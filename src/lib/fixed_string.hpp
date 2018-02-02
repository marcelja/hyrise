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
  FixedString(const std::string &string) : _is_reference(true), _reference({(char*)string.c_str(), string.size()}) {
    // std::cout << "FixedString::constructor std::string" << std::endl;
    // std::cout << "bla" << string << std::endl;
  }

  FixedString(char *mem, size_t string_length) : _is_reference(true), _reference({mem, string_length}) {
    // std::cout << "FixedString::constructor mem*" << std::endl;
  }

  ~FixedString() {
    // if (_delete) {
    //   std::cout << "FixedString::delete destructor" << std::endl;
    //   delete [] _mem;
    //   // std::memcpy(_mem, "ffffffffff", _string_length);
    //   // std::cout << string();
    // }
  }

  // FixedString(FixedString &other) : _mem(new char[other._string_length]{}), _string_length(other._string_length), _delete(true) {
  //   std::cout << "FixedString::copy constructor" << std::endl;
  //   std::memcpy(_mem, other._mem, _string_length);
  // }

  // // copy constructor
  // FixedString(const FixedString &other) : _mem(new char[other._string_length]{}), _string_length(other._string_length), _delete(true) {
  //   std::cout << "FixedString::copy constructor 2" << std::endl;
  //   std::memcpy(_mem, other._mem, _string_length);
  // }

  // move constructor
  FixedString(const FixedString &&other) : _is_reference(false), _char_vector(other.char_vector()) {
    // std::cout << "FixedString::move constructor" << std::endl;
    // std::cout << other << std::endl;
    // std::cout << "bla" << std::endl;

    // std::memcpy(_mem, other._mem, _string_length);
  }

  size_t copy(char *s, size_t len, size_t pos = 0) const {
    auto copied_length = len < _reference._string_length - pos ? len : _reference._string_length - pos;
    std::memcpy(s, _reference._mem + pos, copied_length);
    return copied_length;
  }

  size_t size() const {
    return _reference._string_length;
  }

  std::string string() const {
    // I didn't include an operator std::string on purpose. That led to expensive, unexpected comparisons.
    // std::cout << _is_reference;
    return std::string(_reference._mem, _reference._string_length);
  }
  
  void bla234123() {
        std::cout << _is_reference;

  }

  const std::vector<char> char_vector() const {
    // std::cout << "_reference._mem adr: " << &_reference._mem[0] << std::endl;
    // std::cout << "_reference._mem adr: " << &_reference._mem[0] + _reference._string_length << std::endl;
    // std::cout << "_reference._mem adr: " << &_reference._mem << std::endl;
    return std::vector<char>(&_reference._mem[0], &_reference._mem[0] + _reference._string_length);
  }

  FixedString &operator=(const FixedString &other) {
    // std::cout << "1111\n";
    auto copied_length = other.size() < _reference._string_length ? other.size() : _reference._string_length;
    other.copy(_reference._mem, copied_length, 0);
    if (copied_length < _reference._string_length) {
      memset(_reference._mem + copied_length, '\0', _reference._string_length - copied_length);
    }
    return *this;
  }

  bool operator<(const FixedString &other) const {
    const char *this_mem = _reference._mem;
    const char *other_mem = other._reference._mem;
    return memcmp(this_mem, other_mem, _reference._string_length) < 0;
  }

  bool operator==(const FixedString &other) const {
    const char *this_mem = _reference._mem;
    const char *other_mem = other._reference._mem;
    return memcmp(this_mem, other_mem, _reference._string_length) == 0;
  }

  friend std::ostream &operator<<(std::ostream &os, const FixedString &obj) {
    return os << obj.string();
  }

  void swap(const FixedString &other) const;

 private:
  struct Reference {
     char *const _mem;
     const size_t _string_length;    
   };
  const bool _is_reference;   
  union {
    std::vector<char> _char_vector;   
    Reference _reference;    
   };
};

}  // namespace opossum
