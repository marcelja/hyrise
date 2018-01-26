#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "fixed_string.hpp"
#include "types.hpp"

namespace opossum {

template <typename T>
class ValueVector {
 public:
  using iterator = typename pmr_vector<T>::iterator;
  using const_iterator = typename pmr_vector<T>::const_iterator;
  using reverse_iterator = typename pmr_vector<T>::reverse_iterator;

  ValueVector();

  // explicit ValueVector(uint8_t fixed_string_length);

  // void copy_values(pmr_concurrent_vector<T> values) {
  //   _values(std::move(values));
  // };

  void push_back(const T& value);

  void push_back(T&& value);

  T& at(const ChunkOffset chunk_offset);

  iterator begin() noexcept;

  iterator end() noexcept;

  reverse_iterator rbegin() noexcept;

  reverse_iterator rend() noexcept;

  const_iterator cbegin() noexcept;

  const_iterator cend() noexcept;

  T& operator[](size_t n);

  const T& operator[](size_t n) const;

  size_t size() const;

  size_t capacity() const;

  void shrink_to_fit();

 protected:
  pmr_vector<T> _values;
  // std::vector<char> _fixed_string_vector;
  // uint8_t _fixed_string_length;
};

template <>
class ValueVector<FixedString> {
 public:
  explicit ValueVector(size_t string_length) : _string_length(string_length) {}

  void push_back(const FixedString& value);

  void push_back(FixedString&& string);

  void push_back(const std::string& string) { push_back(FixedString(string)); }

  FixedString at(const ChunkOffset chunk_offset);

  class iterator : public boost::iterator_facade<iterator, FixedString, std::random_access_iterator_tag, FixedString> {
   public:
    iterator(size_t string_length, const pmr_vector<char>& vector, size_t pos = 0)
        : _string_length(string_length), _vector(vector), _pos(pos) {}
    iterator& operator=(const iterator& other) {
      if (_string_length != other._string_length || _vector != other._vector)
        throw std::runtime_error("can't convert pointers from different vectors");
      _pos = other._pos;
      return *this;
    }

   private:
    using facade = boost::iterator_facade<iterator, FixedString, std::random_access_iterator_tag, FixedString>;
    friend class boost::iterator_core_access;
    bool equal(iterator const& other) const { return this->_pos == other._pos; }
    typename facade::difference_type distance_to(iterator const& other) const {
      return (int64_t(other._pos) - int64_t(this->_pos)) / int64_t(_string_length);
    }
    void advance(typename facade::difference_type n) { _pos += n * _string_length; }
    void increment() { _pos += _string_length; }
    void decrement() { _pos -= _string_length; }
    FixedString dereference() const { return FixedString((char*)&_vector[_pos], _string_length); }

    const size_t _string_length;
    const pmr_vector<char>& _vector;
    size_t _pos;
  };

  iterator begin() noexcept;

  iterator end() noexcept;

  iterator cbegin() noexcept;

  iterator cend() noexcept;

  typedef boost::reverse_iterator<iterator> reverse_iterator;
  reverse_iterator rbegin() noexcept;

  reverse_iterator rend() noexcept;

  FixedString operator[](size_t n);

  const FixedString operator[](size_t n) const;

  size_t size() const;

  size_t capacity() const;

  void erase(const iterator start, const iterator end);

  void shrink_to_fit();

 private:
  size_t _string_length;
  pmr_vector<char> _vector;
};

}  // namespace opossum
