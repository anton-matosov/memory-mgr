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

#ifndef MGR_BIT_MANAGER_HEADER
#define MGR_BIT_MANAGER_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <memory-mgr/detail/static_bitset.h>
#include <memory-mgr/detail/ptr_helpers.h>

namespace memory_mgr
{
	/**
	   @brief Implementation details
	*/
	namespace detail
	{
		//Rounds integer value to upper value using specified integer base
		template<size_t value, size_t base>
		struct round_int
		{
			enum {result = ( (value / base) * base) + (value % base ? base : 0)};
		};

		enum bitMgrMemCtrl{ mcAuto = static_array, mcNone = custom_array };
		template< class BlockType, size_t BitsCount, bitMgrMemCtrl memoryCtrl = mcAuto >
		class bit_manager
		{
			typedef static_bitset< BlockType, BitsCount, array_type(memoryCtrl) > bitset_t;
			/**
			   @brief constants
			*/
			enum
			{
				not_initialized = 0,
				initialized
			};
		public:
			/**
			   @brief memory block type
			   @see static_bitset::block_type
			*/
			typedef typename bitset_t::block_type			block_type;

			/**
			   @brief memory block pointer type
			   @see static_bitset::block_ptr_type
			*/
			typedef typename bitset_t::block_ptr_type		block_ptr_type;

			/**
			  @brief Type used to store size, commonly std::size_t
			  @see static_bitset::size_type
			*/
			typedef typename bitset_t::size_type			size_type;

			typedef size_type aux_data_type;
			/**
			   @brief compile time computed constants
			*/
			enum
			{
				aux_data_size = sizeof( aux_data_type ) /**< size of auxiliary data required to store bit_manager internal data*/,
				memory_usage = round_int<bitset_t::memory_usage + aux_data_size, 32>::result /**< amount of memory in bytes used by bit_manager*/,
				num_bits = BitsCount /**< number of bits available for allocations*/
			};

			const static size_type npos = bitset_t::npos;

			/**
			   @brief Default constructor, initializes bit manager
			  
			   @exception newer throws
			*/
			bit_manager()
				:m_is_init(0),
				m_bit_hint( 0 )
			{
				clear();				
			}

			/**
			   @brief Constructor
			  
			   @param	ptr	pointer to memory that will be used to store bitset
			   @attention	First four bytes (dword) of memory segment must be zeroed before bit_manager creation\n
							It will initialize its state only if first four bytes are null\n
							Bitset will be placed starting from the fifth byte (the second dword)

			   @exception newer throws
			*/
			bit_manager( block_ptr_type ptr )
				:m_is_init( detail::size_cast( ptr ) ),
				 m_bitset( block_ptr_cast( detail::shift( ptr, aux_data_size ) ) ),
				 m_bit_hint( 0 )
			{
				if( *m_is_init == not_initialized )
				{
					clear();
					*m_is_init = initialized;
				}
			}

			size_type requires_bytes()
			{
				return memory_usage;
			}

			inline size_type allocate( size_type bits_count )
			{
				size_type pos = m_bitset.find_n( bits_count,  m_bit_hint );
				//if found set bits
				if( pos != npos )
				{
					//cache block index
					m_bit_hint = pos + bits_count;
					m_bitset.reset( pos, bits_count );
				}
				else
				{
					//If cache is used
					if (m_bit_hint != 0)
					{
						//Invalidate cache
						m_bit_hint = 0;
						//And try one more time
						return allocate( bits_count );
					}					
				}
				//return bit index
				return pos;
			}

			inline void deallocate( size_type pos, size_type bits_count )
			{
				assert( ( this->m_bitset.test( pos, bits_count ) == false ) 
					&& "Bits are already deallocated or invalid size." );
				this->m_bitset.set( pos, bits_count );
				//cache block index
				m_bit_hint = pos;
			}

			template <class Ch, class Tr>
			std::basic_ostream<Ch, Tr>& print( std::basic_ostream<Ch, Tr>& ostr ) const
			{
				return ostr << this->m_bitset;
			}
			
			/**
			   @brief Return true if there is no more free bits to allocate
			*/
			inline bool empty()
			{
				return this->m_bitset.empty();
			}

			inline bool free()
			{
				return this->m_bitset.test( 0, num_bits );
			}

			/**
			   @brief Call this method to deallocate all allocated bits
			   @exception newer  throws                                     
			*/
			inline void clear()
			{
				m_bitset.set();
				m_bit_hint = 0;
			}
		private:
			//Bitset
			aux_data_type*		m_is_init;
			bitset_t	m_bitset;

			//Cache
			size_type m_bit_hint;

			static inline size_type block_index(size_type pos) 
			{
				return detail::block_index<bitset_t::bits_per_block>(pos);
			}

			static inline block_ptr_type block_ptr_cast( void* ptr )
			{
				return static_cast<block_ptr_type>( ptr );
			}
		};

		template< class Ch, class Tr, class BlockType, size_t BitsCount, bitMgrMemCtrl memoryCtrl >
		static inline std::basic_ostream<Ch, Tr>& operator<<( std::basic_ostream<Ch, Tr>& ostr,
			const bit_manager<BlockType, BitsCount, memoryCtrl>& b )
		{
			return b.print( ostr );
		}
	}
}

#endif// MGR_BIT_MANAGER_HEADER