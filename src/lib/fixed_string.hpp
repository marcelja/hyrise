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
  FixedString(const std::string &string) : _is_reference(false), _stdstring(string) {}

  FixedString(char *mem, size_t string_length) : _is_reference(true), fixed({mem, string_length}) {}

  ~FixedString() {}

  FixedString(FixedString &other) : _is_reference(true), fixed(other.fixed) {}

  size_t copy(char *s, size_t len, size_t pos = 0) const {
    if (_is_reference) {
      auto copied_length = len < fixed._string_length - pos ? len : fixed._string_length - pos;
      std::memcpy(s, fixed._mem + pos, copied_length);
      return copied_length;
    } else {
      return _stdstring.copy(s, len, pos);
    }
  }

  size_t size() const {
    if (_is_reference) {
      return fixed._string_length;
    } else {
      return _stdstring.size();
    }
  }

  std::string string() const {
    // I didn't include an operator std::string on purpose. That led to expensive, unexpected comparisons.
    if(_is_reference) return std::string(fixed._mem, fixed._string_length);
    return _stdstring;
  }

  FixedString &operator=(const FixedString &other) {
    if (_is_reference) {
      auto copied_length = other.size() < fixed._string_length ? other.size() : fixed._string_length;
      other.copy(fixed._mem, copied_length, 0);
      if (copied_length < fixed._string_length) {
        memset(fixed._mem + copied_length, '\0', fixed._string_length - copied_length);
      }
    } else {
      _stdstring = other._stdstring;
    }
    return *this;
  }

  bool operator<(const FixedString &other) const {
    const char *this_mem = _is_reference ? fixed._mem : _stdstring.c_str();
    const char *other_mem = other._is_reference ? other.fixed._mem : other._stdstring.c_str();
    return memcmp(this_mem, other_mem, fixed._string_length) < 0;
    // TODO: This does not deal with non-reference strings that are shorter than _string_length
  }

  friend std::ostream &operator<<(std::ostream &os, const FixedString &obj) {
    return os << obj.string();
  }

  // inline std::ostream&;

 private:
  struct Fixed {
    char *const _mem;
    const size_t _string_length;
    // TODO: can we provide this for the other constructor, too?
  };

  const bool _is_reference;
  union {
    std::string _stdstring;
    Fixed fixed;
  };
};

} // namespace opossum
