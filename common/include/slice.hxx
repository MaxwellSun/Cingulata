/*
    (C) Copyright 2019 CEA LIST. All Rights Reserved.
    Contributor(s): Cingulata team

    This software is governed by the CeCILL-C license under French law and
    abiding by the rules of distribution of free software.  You can  use,
    modify and/ or redistribute the software under the terms of the CeCILL-C
    license as circulated by CEA, CNRS and INRIA at the following URL
    "http://www.cecill.info".

    As a counterpart to the access to the source code and  rights to copy,
    modify and redistribute granted by the license, users are provided only
    with a limited warranty  and the software's author,  the holder of the
    economic rights,  and the successive licensors  have only  limited
    liability.

    The fact that you are presently reading this means that you have had
    knowledge of the CeCILL-C license and that you accept its terms.
*/

#ifndef CI_SLICE
#define CI_SLICE

#include <ci_bit.hxx>

#include <vector>
#include <functional>
#include <optional>

namespace cingulata {

/**
 * @brief      Base class for slices.
 * @details    This class computes slice range (start, stop and stride) as a
 *             function of list size @c p_list_size and optional @c p_start, @c
 *             p_end and @c p_stride parameters. List indexes selected by the
 *             slice are @c k*stride for any @c k (relative slice index) which
 *             verifies @c start<=k*stride<end (@c start>=k*stride>end in case
 *             of negative @c stride value). So, @c k=0...(end-start)/stride.
 *
 *             Default value for @c p_stride parameter is 1. Default values for
 *             @c p_start and @c p_end are @c 0 and @c p_list_size or @c
 *             p_list_size-1 and @c -1 in case of negative stride.
 */
class SliceBase {
public:
  /**
   * @brief      Constructs object.
   *
   * @param[in]  p_list_size  List size
   * @param[in]  p_start      Optional start parameter
   * @param[in]  p_end        Optional end parameter
   * @param[in]  p_stride     Optional stride parameter
   */
  SliceBase(const int p_list_size, const std::optional<int>& p_start,
        const std::optional<int>& p_end, const std::optional<int>& p_stride);

  /**
   * @brief      Get number of elements in slice
   *
   * @return     element count
   */
  unsigned size() const;

  /**
   * @brief      Map relative slice index to absolute list index.
   *
   * @param[in]  idx   slice index (@c 0<=idx<size())
   *
   * @return     absolute index
   */
  unsigned abs_idx(const unsigned idx) const;

protected:
  int m_stride;
  int m_start;
  int m_end;
  unsigned m_size;
};

/* forward declaration */
template<typename list_t>
class CSlice;

/**
 * @brief      Modifiable slice
 *
 * @tparam     list_t  inner list object type. @c list_t object must define @c
 *                     size() and @c at(unsigned) functions
 */
template<typename list_t>
class Slice : public SliceBase {
  friend class CSlice<list_t>;

public:
  /**
   * @brief      Constructs the object.
   *
   * @param[in]  p_obj     Reference to list object
   * @param[in]  p_start   Optional start parameter
   * @param[in]  p_end     Optional end parameter
   * @param[in]  p_stride  Optional stride parameter
   */
  Slice(list_t& p_obj, const std::optional<int>& p_start = {},
        const std::optional<int>& p_end = {}, const std::optional<int>& p_stride = {});

  /**
   * @name Assignment operators
   * @{
   */

  /**
   * @brief      Copy elements pointed by another slice to current slice.
   * @note       Only the first @c min(size(),other.size()) elements are copied.
   *
   * @param[in]  other  Slice object.
   *
   * @return     reference to current object
   */
  Slice& operator=(const Slice& other);

  /**
   * @copydoc operator=
   */
  Slice& operator=(const CSlice<list_t>& other);

  /**
   * @}
   */


  /**
   * @name Slice element selection operators
   * @{
   */

  typename list_t::elem_t& operator[](const unsigned idx);
  const typename list_t::elem_t& operator[](const unsigned idx) const;

  /**
   * @}
   */

private:
  list_t& m_obj;
};

/**
 * @brief      Non-modifiable slice.
 *
 * @tparam     list_t  inner list object type. @c list_t object must define @c
 *                     size() and @c at(unsigned) functions
 */
template<typename list_t>
class CSlice : public SliceBase {
public:
  /**
   * @brief      Constructs the object.
   *
   * @param[in]  p_obj     Constant reference to list object
   * @param[in]  p_start   Optional start parameter
   * @param[in]  p_end     Optional end parameter
   * @param[in]  p_stride  Optional stride parameter
   */
  CSlice(const list_t& p_obj, const std::optional<int>& p_start = {},
        const std::optional<int>& p_end = {}, const std::optional<int>& p_stride = {});

  /**
   * @brief      Copy-constructs object from a modifiable slice.
   * @details    Slice parameters and referenced object are copied.
   *
   * @param[in]  other  modifiable slice to copy
   */
  CSlice(const Slice<list_t>& other);

  /**
   * @name Slice element selection operator
   * @{
   */

  const typename list_t::elem_t& operator[](const unsigned idx) const;

  /**
   * @}
   */

private:
  const list_t& m_obj;
};

} // namespace cingulata

#endif
