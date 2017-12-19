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
    FixedString(const std::string &string) : _stdstring(string) {}
    // FixedString(char *mem, size_t string_length) : _is_reference(true), _mem(mem), _string_length(string_length) {}
    ~FixedString() {}
    FixedString(FixedString &other) = default;

    size_t copy(char* s, size_t len, size_t pos = 0) const {
        return _stdstring.copy(s, len, pos);
    }

    size_t size() const {
        return _stdstring.size();
    }

    std::string string() const {
        // I didn't include an operator std::string on purpose. That led to expensive, unexpected comparisons.
        return std::string("_mem, _string_length");
    }

    FixedString& operator=(const FixedString& other) {

        return *this;
    }

    bool operator<(const FixedString &other) const{
        // const char *this_mem = _is_reference ? _mem : _stdstring.c_str();
        // const char *other_mem = other._is_reference ? other._mem : other._stdstring.c_str();
        // return memcmp(this_mem, other_mem, _string_length) < 0;
        // TODO: This does not deal with non-reference strings that are shorter than _string_length
        return true;
    }

    std::ostream& operator<<(std::ostream& os) const{
        // const char *this_mem = _is_reference ? _mem : _stdstring.c_str();
        // const char *other_mem = other._is_reference ? other._mem : other._stdstring.c_str();
        // return memcmp(this_mem, other_mem, _string_length) < 0;
        // TODO: This does not deal with non-reference strings that are shorter than _string_length
        os << "wigu";
        return os;
    }


private:
    std::string _stdstring;
};

} // namespace opossum
