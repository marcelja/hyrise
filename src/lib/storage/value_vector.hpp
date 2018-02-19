#pragma once
#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "fixed_string.hpp"
#include "types.hpp"
#include "utils/assert.hpp"

namespace opossum {

template <typename T, typename Vec = pmr_vector<T> >
class ValueVector {
 public:
  using iterator = typename pmr_concurrent_vector<T>::iterator;
  using const_iterator = typename pmr_concurrent_vector<T>::const_iterator;
  using reverse_iterator = typename pmr_concurrent_vector<T>::reverse_iterator;

  ValueVector();

  template< class Iter >
  ValueVector(Iter first, Iter last) {
    while (first != last) {
      push_back(*first);
      ++first;
    }
  }

  ValueVector(const size_t& elements);

  ValueVector(pmr_vector<T>&& vector) : _values(vector) {};

  // explicit ValueVector(const PolymorphicAllocator<T>& alloc);

  void push_back(const T& value);

  void push_back(T&& value);

  T& at(const ChunkOffset chunk_offset);
  
  const T& at(const ChunkOffset chunk_offset) const;

  iterator begin() noexcept;

  iterator end() noexcept;

  reverse_iterator rbegin() noexcept;

  reverse_iterator rend() noexcept;

  const_iterator cbegin() const noexcept;

  const_iterator cend() const noexcept;

  const_iterator begin() const noexcept;

  const_iterator end() const noexcept;

  T& operator[](size_t n);

  const T& operator[](size_t n) const;

  size_t size() const;

  size_t capacity() const;

  void resize(size_t n);

  void reserve(size_t n) {
    _values.reserve(n);
  }

  void erase(const iterator start, const iterator end);

  void shrink_to_fit();

  pmr_vector<T>& pmr_vector_values() {
    return _values;
  }

  const pmr_vector<T>& pmr_vector_values() const {
    return _values;
  }

  T* data() noexcept {
    return _values.data();
  }

  const T* data() const noexcept {
    return _values.data();
  }

  void erase(const_iterator start, const_iterator end);

  PolymorphicAllocator<T> get_allocator();

 protected:
  // std::vector<T, Alloc> _values;
  Vec _values;
};

template <>
class ValueVector<FixedString> {
 public:
  explicit ValueVector(size_t string_length) : _string_length(string_length) {}

  template< class Iter >
  ValueVector(Iter first, Iter last, size_t string_length) : _string_length(string_length) {
    while (first != last) {
      push_back(*first);
      ++first;
    }
  }

  template< class Iter >
  ValueVector(Iter first, Iter last) {
    _string_length = first->size();
    while (first != last) {
      push_back(*first);
      ++first;
    }
  }

  ValueVector(const ValueVector&& other) : _string_length(other._string_length), _vector(other._vector) {
  }

  ValueVector(const ValueVector& other) : _string_length(other._string_length), _vector(other._vector) {}

  void push_back(const FixedString& value);

  void push_back(FixedString&& string);

  void push_back(const std::string& string);

  FixedString at(const ChunkOffset chunk_offset);

  class iterator : public boost::iterator_facade<iterator, FixedString, std::random_access_iterator_tag, FixedString> {
   public:
    iterator(size_t string_length, const pmr_vector<char>& vector, size_t pos = 0)
        : _string_length(string_length), _vector(vector), _pos(pos) {}

    iterator& operator=(const iterator& other) {
      DebugAssert(_string_length == other._string_length && &_vector == &other._vector, "can't convert pointers from different vectors");
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

  iterator cbegin() const noexcept;

  iterator cend() const noexcept;

  iterator begin() const noexcept;

  iterator end() const noexcept;

  typedef boost::reverse_iterator<iterator> reverse_iterator;
  reverse_iterator rbegin() noexcept;

  reverse_iterator rend() noexcept;

  FixedString operator[](size_t n);

  const FixedString operator[](size_t n) const;

  size_t size() const;

  size_t capacity() const;

  void erase(const iterator start, const iterator end);

  void shrink_to_fit();

  void reserve(size_t n) {
    _vector.reserve(n * _string_length);
  }

  PolymorphicAllocator<FixedString> get_allocator();

 private:
  size_t _string_length;
  // pmr_vector<char> _vector;
  pmr_concurrent_vector<char> _vector;
};

}  // namespace opossum
