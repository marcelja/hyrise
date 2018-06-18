#include "fixed_string_column.hpp"

#include "fixed_string_vector_iterator.hpp"

#include <memory>
#include <string>

#include "resolve_type.hpp"
#include "storage/vector_compression/base_compressed_vector.hpp"
#include "type_cast.hpp"
#include "utils/assert.hpp"
#include "utils/performance_warning.hpp"

namespace opossum {

using facade = boost::iterator_facade<FixedStringIterator, FixedString, std::random_access_iterator_tag, FixedString>;

FixedStringIterator::FixedStringIterator(size_t string_length, const pmr_vector<char>& vector, size_t pos = 0)
    : _string_length(string_length), _chars(vector), _pos(pos) {}

FixedStringIterator& FixedStringIterator::operator=(const FixedStringIterator& other) {
  DebugAssert(_string_length == other._string_length && &_chars == &other._chars,
              "can't convert pointers from different vectors");
  _pos = other._pos;
  return *this;
}

bool FixedStringIterator::equal(FixedStringIterator const& other) const { return this->_pos == other._pos; }
typename facade::difference_type FixedStringIterator::distance_to(FixedStringIterator const& other) const {
  if (_string_length == 0) return 0;
  return (std::intptr_t(other._pos) - std::intptr_t(this->_pos)) / std::intptr_t(_string_length);
}
void FixedStringIterator::advance(typename facade::difference_type n) { _pos += n * _string_length; }

void FixedStringIterator::increment() { _pos += _string_length; }

void FixedStringIterator::decrement() { _pos -= _string_length; }

FixedString FixedStringIterator::dereference() const {
  return FixedString(const_cast<char*>(&_chars[_pos]), _string_length);
}

}  // namespace opossum
