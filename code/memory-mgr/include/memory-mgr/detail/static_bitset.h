/*
Generic Memory Manager (memory-mgr)
http://memory-mgr.sourceforge.net/
Copyright (c) 2007-2008 Anton (shikin) Matosov

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 3, 29 June 2007 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA <http://fsf.org/>


Please feel free to contact me via e-mail: shikin@users.sourceforge.net
*/

#pragma once

#include "memory-mgr/detail/assert.h"
#include "memory-mgr/detail/helpers.h"
#include "memory-mgr/detail/math.h"
#include "memory-mgr/detail/type_manip.h"
#include "memory-mgr/detail/types.h"

#include <boost/type_traits/make_unsigned.hpp>
#include <ostream>
#include <type_traits>

namespace memory_mgr
{
enum array_type
{
  static_array,
  dynamic_array,
  external_buffer
};
namespace detail
{
template <size_t BitsPerBlock>
static inline size_t block_index(size_t pos)
{
  return pos / BitsPerBlock;
}

// Structure used to calculate number of blocks
// required to store requested number of bits
template <size_t BitsCount, size_t BitsPerBlock>
struct calc_num_blocks
{
  enum
  {
    result = (BitsCount / BitsPerBlock) + (BitsCount % BitsPerBlock ? 1 : 0)
  };
};

// Structure used to calculate bit index
// in block
template <size_t Pos, size_t BitsPerBlock>
struct bit_index
{
  enum
  {
    result = Pos % BitsPerBlock
  };
};

// Structure used to calculate number of extra bits in block
template <size_t BitsCount, size_t BitsPerBlock>
struct extra_bits
{
  enum
  {
    result = bit_index<BitsCount, BitsPerBlock>::result
  };
};

// Set strategy
struct set_op
{
  template <class T>
  static void update(T& lhs, const T rhs)
  {
    lhs |= rhs;
  }
};

// Reset strategy
struct reset_op
{
  template <class T>
  static void update(T& lhs, const T rhs)
  {
    lhs &= ~rhs;
  }
};

template <class BlockType, class ExtraBits>
struct do_zero_unused_bits
{
  void operator()(BlockType& highest_block)
  {
    using block_type = BlockType;
    BlockType highest_bits_mask = ~(~block_type(0) << ExtraBits::value);
    highest_block &= highest_bits_mask;
  }
};

template <class BlockType>
struct do_zero_unused_bits<BlockType, ::memory_mgr::type_manip::int2type<0> >
{
  void operator()(BlockType&) {}
};

// Type traits for all array types
template <class BlockType, long BitsCount>
struct array_traits
{
  using block_type = typename boost::make_unsigned<BlockType>::type;
  using block_ref_type = block_type&;
  using const_block_ref_type = const block_type&;

  using block_ptr_type = block_type*;
  using const_block_ptr_type = const block_type*;

  /**
     @brief compile time computed constants
  */
  enum
  {
    bits_per_block = sizeof(block_type) * 8,
    num_bits = BitsCount,
    num_blocks = detail::calc_num_blocks<num_bits, bits_per_block>::result,
    max_bits = num_blocks * bits_per_block,
    memory_usage = sizeof(block_type) * num_blocks
  };
};

// Basic array structure for array specializations
template <class BlockType, size_t BitsCount, array_type Type>
struct array
{
};

// Array specializations for static array
template <class BlockType, long BitsCount>
struct array<BlockType, BitsCount, static_array> : public array_traits<BlockType, BitsCount>
{
  using array_traits = array_traits<BlockType, BitsCount>;
  using array_traits::num_blocks;
  using typename array_traits::block_type;

  block_type m_bits[num_blocks ? num_blocks : 1];
};

template <class BlockType>
struct array<BlockType, 0, static_array> : public array_traits<BlockType, 0>
{
  using array_traits = array_traits<BlockType, 0>;
  using typename array_traits::block_type;

  block_type m_bits[1];
};

// Array specializations for dynamic array
template <class BlockType, long BitsCount>
struct array<BlockType, BitsCount, dynamic_array> : public array_traits<BlockType, BitsCount>
{
  using array_traits = array_traits<BlockType, BitsCount>;
  using array_traits::num_blocks;
  using typename array_traits::block_type;

  array() : m_bits(0) { construct(); }

  array(const array& rhs) : m_bits(0)
  {
    construct();
    *this = rhs;
  }

  array& operator=(const array& rhs)
  {
    if (this != &rhs)
    {
      copy(rhs);
    }
    return *this;
  }

  ~array() { delete[] this->m_bits; }

  block_type* m_bits;

 private:
  void construct() { this->m_bits = new block_type[num_blocks]; }

  void destruct() { delete[] this->m_bits; }

  void copy(const array& rhs)
  {
    for (int i = 0; i < num_blocks; ++i)
    {
      this->m_bits[i] = rhs.m_bits[i];
    }
  }
};

// Array specializations for custom array
template <class BlockType, long BitsCount>
struct array<BlockType, BitsCount, external_buffer> : public array_traits<BlockType, BitsCount>
{
  using array_traits = array_traits<BlockType, BitsCount>;
  using array_traits::num_blocks;
  using typename array_traits::block_ptr_type;
  using typename array_traits::block_type;

  array(block_ptr_type arr_ptr) : m_bits(arr_ptr) {}

  block_type* m_bits;

 private:
  // noncopyable
  array(const array&);
  array& operator=(const array&);
};

}  // namespace detail

template <class BlockType, long BitsCount, array_type StaticArr = static_array>
class static_bitset : public detail::array<BlockType, BitsCount, StaticArr>
{
 public:
  using self_type = static_bitset;
  using self_ref_type = self_type&;

  using base_type = detail::array<BlockType, BitsCount, StaticArr>;

  /**
     @brief memory block type
  */
  using block_type = typename base_type::block_type;

  /**
     @brief memory block reference type
  */
  using block_ref_type = typename base_type::block_ref_type;

  /**
     @brief memory block const reference type
  */
  using const_block_ref_type = typename base_type::const_block_ref_type;

  /**
     @brief memory block pointer type
  */
  using block_ptr_type = typename base_type::block_ptr_type;

  /**
     @brief memory block const pointer type
  */
  using const_block_ptr_type = typename base_type::const_block_ptr_type;

  /**
     @brief Type used to store size, commonly std::size_t
  */
  using size_type = size_t;
  using bit_position_type = ulong;
  using block_id_type = bit_position_type;
  using difference_type = long;

  /**
     @brief invalid index value (null pos)
  */
  constexpr static bit_position_type npos = ~bit_position_type(0);

  /**
     @brief compile time computed constants
  */
  enum
  {
    bits_per_block = base_type::bits_per_block,
    num_bits = base_type::num_bits,
    num_blocks = base_type::num_blocks,
    max_bits = base_type::max_bits,
    memory_usage = base_type::memory_usage
  };

  // Default constructor
  // Resets all bits
  inline static_bitset() { static_assert(num_bits != 0, Bitset_cant_be_empty); }

  // Constructor used only by custom arrays to initialize
  // array's pointer
  template<typename U = block_ptr_type>
  inline explicit static_bitset(block_ptr_type bits_ptr, std::enable_if_t<std::is_constructible<base_type, U>::value>* = 0) : base_type(bits_ptr) { static_assert(num_bits != 0, Bitset_cant_be_empty); }

  inline ~static_bitset() {}

  inline bit_position_type size() const { return num_bits; }

  inline bool test(bit_position_type pos) const
  {
    MGR_ASSERT(pos < num_bits, "Bit position is out of range");
    return unchecked_test(pos);
  }

  inline bool test(bit_position_type pos, bit_position_type count) const
  {
    MGR_ASSERT(pos < num_bits, "Bit position is out of range");
    MGR_ASSERT(count > 0, "Count should be bigger than null");

    bit_position_type blk = block_index(pos);
    bit_position_type ind = pos - blk * bits_per_block;  // bit_index( pos );
    difference_type left = count;
    while (left > 0)
    {
      if (contains_bits(blk, ind, left))
      {
        ++blk;
        left -= bits_per_block - ind;
        ind = 0;
      }
      else
      {
        return false;
      }
    }
    return true;
  }

  inline self_ref_type set()
  {
    std::fill(this->m_bits, this->m_bits + num_blocks, ~block_type(0));
    zero_unused_bits();
    return *this;
  }

  inline self_ref_type set(bit_position_type pos)
  {
    MGR_ASSERT(pos < num_bits, "Bit position is out of range");

    this->m_bits[block_index(pos)] |= bit_mask(pos);
    return *this;
  }

  inline self_ref_type set(bit_position_type pos, bit_position_type count) { return do_set<detail::set_op>(pos, count); }

  inline self_ref_type reset(bit_position_type pos)
  {
    MGR_ASSERT(pos < num_bits, "Bit position is out of range");
    this->m_bits[block_index(pos)] &= ~bit_mask(pos);
    return *this;
  }

  inline self_ref_type reset(bit_position_type pos, bit_position_type count) { return do_set<detail::reset_op>(pos, count); }

  inline self_ref_type reset()
  {
    std::fill(this->m_bits, this->m_bits + num_blocks, block_type(0));
    return *this;
  }

  inline self_ref_type flip(bit_position_type pos)
  {
    MGR_ASSERT(pos < num_bits, "Bit position is out of range");
    this->m_bits[block_index(pos)] ^= bit_mask(pos);
    return *this;
  }

  inline self_ref_type flip()
  {
    for (bit_position_type i = 0; i < num_blocks; ++i)
    {
      this->m_bits[i] = ~this->m_bits[i];
    }
    zero_unused_bits();
    return *this;
  }

  inline bit_position_type find_n(bit_position_type count, /*bit_position_type first_block = 0,*/ bit_position_type bit_hint = 0)
  {
    if (count == 0)
    {
      return npos;
    }

    // Calculate the first block id to start search with
    bit_position_type first_block = block_index(bit_hint);
    // Calculate the first free bit id in the first block
    bit_position_type lowest_bit = bit_hint - first_block * bits_per_block;  // bit_index( bit_hint );

    // Check if the bit specified in the hint is really free
    if (!unchecked_test(first_block, bit_hint))
    {
      // If not find, another one
      lowest_bit = do_find_from(first_block);
    }

    bit_position_type blk = first_block;
    bit_position_type ind = lowest_bit;
    difference_type left = count;
    while ((left > 0) && (lowest_bit != npos))
    {
      if (contains_bits(blk, ind, left))
      {
        ++blk;
        left -= bits_per_block - ind;
        ind = 0;
      }
      else
      {
        lowest_bit = ind = do_find_next(ind + 1, blk);
        left = count;
        first_block = blk;
      }
    }

    return (lowest_bit != npos) ? first_block * bits_per_block + lowest_bit : npos;
  }

  inline bit_position_type find_first() const
  {
    bit_position_type blk_index = 0;
    return blk_index * bits_per_block + do_find_from(blk_index);
  }

  inline bit_position_type find_next(bit_position_type pos) const
  {
    ++pos;

    bit_position_type blk_index = block_index(pos);
    return blk_index * bits_per_block + do_find_next(bit_index(pos), blk_index);
  }

  // Return true if all bits are set to 0
  inline bool empty()
  {
    // Is there at least one block with bit set to 1
    return find_first_block(0) >= num_blocks;
  }

  template <class StreamT>
  friend StreamT& operator<<(StreamT& stream, const self_type& bitset)
  {
    for (int i = 0; i < self_type::num_blocks; ++i)
    {
      stream << bitset.m_bits[i];
    }
    return stream;
  }

  template <class StreamT>
  friend StreamT& operator>>(StreamT& stream, self_ref_type bitset)
  {
    for (int i = 0; i < self_type::num_blocks; ++i)
    {
      stream >> bitset.m_bits[i];
    }
    return stream;
  }

 private:
  static const block_type one = block_type(1);
  static const block_type block_max = static_cast<block_type>(~block_type(0));

  static inline bit_position_type block_index(bit_position_type pos) { return pos / bits_per_block; }

  static inline block_id_type bit_index(bit_position_type pos) { return pos % bits_per_block; }

  static inline block_type bit_mask(bit_position_type pos) { return unchecked_bit_mask(bit_index(pos)); }

  static inline block_type unchecked_bit_mask(bit_position_type pos) { return static_cast<block_type>(one << pos); }

  static inline block_type bit_mask(bit_position_type pos, bit_position_type count)
  {
    block_type bit_mask_for_count_bits = 0;
    if (count < bits_per_block)
    {
      bit_mask_for_count_bits = ((one << count) - 1);
    }
    else
    {
      bit_mask_for_count_bits = block_max;
    }
    return bit_mask_for_count_bits << bit_index(pos);
  }

  static inline block_type higher_bit_mask(bit_position_type pos) { return block_type(block_max << bit_index(pos)); }

  static inline block_type lower_bit_mask(bit_position_type pos) { return ~higher_bit_mask(pos); }

  static inline bit_position_type blocks_count(bit_position_type bits_count) { return block_index(bits_count); }

  inline bool unchecked_test(bit_position_type pos) const { return unchecked_test(block_index(pos), bit_index(pos)); }

  inline bool unchecked_test(bit_position_type block_ind, bit_position_type bit_ind) const { return (this->m_bits[block_ind] & unchecked_bit_mask(bit_ind)) != 0; }

  // Find first block with at least one bit set to 1
  inline bit_position_type find_first_block(bit_position_type from) const
  {
    // skip null blocks
    while (from < num_blocks && this->m_bits[from] == 0)
    {
      ++from;
    }
    return from;
  }

  // finds the first bit set to 1
  inline bit_position_type do_find_from(bit_position_type& first_block) const
  {
    first_block = find_first_block(first_block);

    if (first_block >= num_blocks)
    {
      return npos;  // not found
    }

    return math::lowest_bit(this->m_bits[first_block]);
  }

  // finds the next bit set to 1
  inline bit_position_type do_find_next(bit_position_type bit_ind, bit_position_type& blk_ind) const
  {
    if (bit_ind >= num_bits)
    {
      return npos;
    }
    const block_type fore = this->m_bits[blk_ind] & (higher_bit_mask(bit_ind));

    return fore ? math::lowest_bit(fore) : do_find_from(++blk_ind);
  }

  inline bool contains_bits(bit_position_type block, bit_position_type first, bit_position_type count) const
  {
    const block_type mask = bit_mask(first, count);
    return (this->m_bits[block] & mask) == mask;
  }

  // If size() is not a multiple of bits_per_block
  // then not all the bits in the last block are used.
  // This function resets the unused bits (convenient
  // for the implementation of many member functions)
  //
  inline void zero_unused_bits()
  {
    // if != 0 this is the number of bits used in the last block
    detail::do_zero_unused_bits<block_type, type_manip::int2type<detail::extra_bits<num_bits, bits_per_block>::result> >()(highest_block());
  }

  // gives a reference to the highest block
  //
  inline block_ref_type highest_block() { return this->m_bits[num_blocks - 1]; }

  // gives a const-reference to the highest block
  //
  inline const_block_ref_type highest_block() const { return this->m_bits[num_blocks - 1]; }

  template <class set_op>
  inline self_ref_type do_set(bit_position_type pos, bit_position_type count)
  {
    MGR_ASSERT(pos < num_bits, "Bit position is out of range");
    count = (count != npos ? count : num_bits - pos);

    MGR_ASSERT((pos + count) <= num_bits, "Specified range is out of range");

    bit_position_type block_ind = block_index(pos);
    const bit_position_type block_end = block_index(pos + count);
    const bit_position_type last_index = bit_index(pos + count);

    bit_position_type bits_fill_count = count;
    do
    {
      set_op::update(this->m_bits[block_ind++], bit_mask(pos, bits_fill_count));
      pos = 0;
      bits_fill_count = (block_ind != block_end) ? bits_per_block : last_index;
    } while (block_ind <= block_end);

    return *this;
  };
};

template <class Ch, class Tr, class BlockType, size_t BitsCount, array_type Type>
std::basic_ostream<Ch, Tr>& operator<<(std::basic_ostream<Ch, Tr>& ostr, const static_bitset<BlockType, BitsCount, Type>& b)
{
  using char_type = Ch;
  char_type one = '1';
  char_type zero = '0';
  char_type space = ' ';

  using bitset_type = static_bitset<BlockType, BitsCount, Type>;
  using bit_position_type = typename bitset_type::bit_position_type;

  for (bit_position_type i = bitset_type::num_bits - 1; (i != bitset_type::npos) && ostr; --i)
  {
    ostr << (b.test(i) ? one : zero);
    if ((i % 8) == 0)
    {
      ostr << space;
    }
  }
  return ostr;
}
}  // namespace memory_mgr
