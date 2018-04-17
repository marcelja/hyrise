#pragma once

#include <memory>

#include "base_encoded_column.hpp"

namespace opossum {

class BaseCompressedVector;

/**
 * @brief Base class of DictionaryColumn<T> exposing type-independent interface
 */
class BaseDictionaryColumn : public BaseEncodedColumn {
 public:
  using BaseEncodedColumn::BaseEncodedColumn;

  virtual EncodingType encoding_type() const override = 0;

  void visit(ColumnVisitable& visitable, std::shared_ptr<ColumnVisitableContext> context = nullptr) const override;

  /**
   * @brief Returns index (i.e. ValueID) of first dictionary entry >= search value
   *
   * @param value the search value
   * @return INVALID_VALUE_ID if entries are smaller than value
   */
  virtual ValueID lower_bound(const AllTypeVariant& value) const = 0;

  /**
   * @brief Returns index (i.e. ValueID) of first dictionary entry > search value
   *
   * @param value the search value
   * @return INVALID_VALUE_ID if entries are smaller than or equal to value
   */
  virtual ValueID upper_bound(const AllTypeVariant& value) const = 0;

  /**
   * @brief The size of the dictionary
   */
  virtual size_t unique_values_count() const = 0;

  virtual std::shared_ptr<const BaseCompressedVector> attribute_vector() const = 0;

  /**
   * @brief Returns encoding specific null value ID
   */
  virtual const ValueID null_value_id() const = 0;
};
}  // namespace opossum
