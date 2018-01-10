#pragma once

#include <cstring>
#include <chrono>
#include <iostream>
#include <tuple>
#include <utility>
#include <vector>
#include <boost/iterator/iterator_facade.hpp>

// #include "all_type_variant.hpp"
// #include "types.hpp"

namespace opossum {

// template <typename T>
// class DictionaryVector;

class FixedString {
public:
    FixedString(const std::string &string) : _is_reference(false), _string_length(0), _stdstring(string) {}
    FixedString(char *mem, size_t string_length) : _is_reference(true), _string_length(string_length), _mem(mem) {}
    ~FixedString() {}
    FixedString(FixedString &other) = default;

    size_t copy(char* s, size_t len, size_t pos = 0) const {
        if (_is_reference) {
            auto copied_length = len < _string_length - pos ? len : _string_length - pos;
            std::memcpy(s, _mem + pos, copied_length);
            return copied_length;
        } else {
            return _stdstring.copy(s, len, pos);
        }
    }

    size_t size() const {
        if (_is_reference) {
            return _string_length;
        } else {
            return _stdstring.size();
        }
    }

    std::string string() const {
        // I didn't include an operator std::string on purpose. That led to expensive, unexpected comparisons.
        return std::string(_mem, _string_length);
    }

    FixedString& operator=(const FixedString& other) {
        if (_is_reference) {
            auto copied_length = other.size() < _string_length ? other.size() : _string_length;
            other.copy(_mem, copied_length, 0);
            if (copied_length < _string_length) {
                memset(_mem + copied_length, '\0', _string_length - copied_length);
            }
        } else {
            _stdstring = other._stdstring;
        }
        return *this;
    }

    bool operator<(const FixedString &other) const{
        const char *this_mem = _is_reference ? _mem : _stdstring.c_str();
        const char *other_mem = other._is_reference ? other._mem : other._stdstring.c_str();
        return memcmp(this_mem, other_mem, _string_length) < 0;
        // TODO: This does not deal with non-reference strings that are shorter than _string_length
    }

private:
    const bool _is_reference;
    const size_t _string_length;
    union {
        std::string _stdstring;
        // struct {
        char * const _mem;
            // TODO: can we provide this for the other constructor, too?
        // };
    };
};

// std::ostream &operator<<(std::ostream &os, FixedString const &s) { return os << s.string(); }

} // namespace opossum
