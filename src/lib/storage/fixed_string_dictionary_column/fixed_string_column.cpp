#include "fixed_string_column.hpp"

#include <memory>
#include <string>

#include "resolve_type.hpp"
#include "storage/vector_compression/base_compressed_vector.hpp"
#include "type_cast.hpp"
#include "utils/assert.hpp"
#include "utils/performance_warning.hpp"

namespace opossum {

template <typename T>
FixedStringColumn<T>::FixedStringColumn(const std::shared_ptr<const FixedStringVector>& dictionary,
                                        const std::shared_ptr<const BaseCompressedVector>& attribute_vector,
                                        const ValueID null_value_id)
    : BaseDictionaryColumn(data_type_from_type<std::string>()),
      _dictionary{dictionary},
      _attribute_vector{attribute_vector},
      _null_value_id{null_value_id} {}

template <typename T>
const AllTypeVariant FixedStringColumn<T>::operator[](const ChunkOffset chunk_offset) const {
  PerformanceWarning("operator[] used");

  DebugAssert(chunk_offset != INVALID_CHUNK_OFFSET, "Passed chunk offset must be valid.");

  auto decoder = _attribute_vector->create_base_decoder();
  const auto value_id = decoder->get(chunk_offset);

  if (value_id == _null_value_id) {
    return NULL_VALUE;
  }

  return _dictionary->get_string_at(value_id);
}

template <typename T>
std::shared_ptr<const pmr_vector<std::string>> FixedStringColumn<T>::dictionary() const {
  return _dictionary->dictionary();
}

template <typename T>
size_t FixedStringColumn<T>::size() const {

    std::cout << "scheiss" << std::endl;
    std::cout << _dictionary->size() << std::endl;
  return _attribute_vector->size();
}

template <typename T>
std::shared_ptr<BaseColumn> FixedStringColumn<T>::copy_using_allocator(
    const PolymorphicAllocator<size_t>& alloc) const {
  auto new_attribute_vector_ptr = _attribute_vector->copy_using_allocator(alloc);
  auto new_attribute_vector_sptr = std::shared_ptr<const BaseCompressedVector>(std::move(new_attribute_vector_ptr));
  auto new_dictionary = FixedStringVector(*_dictionary);
  auto new_dictionary_ptr = std::allocate_shared<FixedStringVector>(alloc, std::move(new_dictionary));
  return std::allocate_shared<FixedStringColumn<T>>(alloc, new_dictionary_ptr, new_attribute_vector_sptr,
                                                    _null_value_id);
}

template <typename T>
size_t FixedStringColumn<T>::estimate_memory_usage() const {
  return sizeof(*this) + _dictionary->data_size() + _attribute_vector->data_size();
}

template <typename T>
CompressedVectorType FixedStringColumn<T>::compressed_vector_type() const {
  return _attribute_vector->type();
}

template <typename T>
EncodingType FixedStringColumn<T>::encoding_type() const {
  return EncodingType::FixedStringDictionary;
}

template <typename T>
ValueID FixedStringColumn<T>::lower_bound(const AllTypeVariant& value) const {
  DebugAssert(!variant_is_null(value), "Null value passed.");

  const auto typed_value = type_cast<std::string>(value);

  auto it = std::lower_bound(_dictionary->cbegin(), _dictionary->cend(), typed_value);
  if (it == _dictionary->cend()) return INVALID_VALUE_ID;
  return static_cast<ValueID>(std::distance(_dictionary->cbegin(), it));
}

template <typename T>
ValueID FixedStringColumn<T>::upper_bound(const AllTypeVariant& value) const {
  DebugAssert(!variant_is_null(value), "Null value passed.");

  const auto typed_value = type_cast<std::string>(value);

  auto it = std::upper_bound(_dictionary->cbegin(), _dictionary->cend(), typed_value);
  if (it == _dictionary->cend()) return INVALID_VALUE_ID;
  return static_cast<ValueID>(std::distance(_dictionary->cbegin(), it));
}

template <typename T>
size_t FixedStringColumn<T>::unique_values_count() const {
  return _dictionary->size();
}

template <typename T>
std::shared_ptr<const BaseCompressedVector> FixedStringColumn<T>::attribute_vector() const {
  return _attribute_vector;
}

template <typename T>
const ValueID FixedStringColumn<T>::null_value_id() const {
  return _null_value_id;
}

template class FixedStringColumn<std::string>;

}  // namespace opossum
