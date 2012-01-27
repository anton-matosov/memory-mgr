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
#include <memory-mgr/detail/ptr_casts.h>
#include <memory-mgr/offset_ptr.h>

namespace memory_mgr
{
	/**
	   @brief Implementation details
	*/
	namespace detail
	{
		enum internal_ptrs
		{
			internal_ptr_named_objects,
			internal_ptr_lfm_pools,


			internal_ptrs_count
		};


		//Rounds integer value to upper value using specified integer base
		template<size_t value, size_t base>
		struct round_int
		{
			enum {result = ( (value / base) * base) + (value % base ? base : 0)};
		};

		enum bitMgrMemCtrl
		{
			mcAuto = static_array,
			mcNone = external_buffer
		};


		template< class ChunkType, size_t BitsCount, bitMgrMemCtrl memoryCtrl = mcAuto >
		class bit_manager
		{
			typedef static_bitset< ChunkType, BitsCount, array_type(memoryCtrl) > bitset_t;
			/**
			   @brief constants
			*/
			enum isInitialized
			{
				not_initialized = 0,
				initialized
			};
		public:
			typedef bit_manager this_type;
			/**
			   @brief memory block type
			   @see static_bitset::block_type
			*/
			typedef typename bitset_t::block_type			chunk_type;

			/**
			   @brief memory block pointer type
			   @see static_bitset::block_ptr_type
			*/
			typedef typename bitset_t::block_ptr_type		chunk_ptr_type;

			/**
			  @brief Type used to store size, commonly std::size_t
			  @see static_bitset::size_type
			*/
			typedef typename bitset_t::size_type			size_type;

			/**
			   @brief compile time computed constants
			*/
			enum
			{
				aux_data_size = 8 /**< size of auxiliary data required to store bit_manager internal data*/,
				memory_usage = round_int<bitset_t::memory_usage + aux_data_size, 32>::result /**< amount of memory in bytes used by bit_manager*/,
				num_bits = BitsCount /**< number of bits available for allocations*/,
			};

			enum
			{
				npos = bitset_t::npos
			};
			//const static size_type npos = bitset_t::npos;

			/**
			   @brief Default constructor, initializes bit manager
			  
			   @exception newer throws
			*/
			bit_manager()
				:m_is_init( not_initialized ),
				m_bit_hint( 0 )
			{
				if( m_is_init == not_initialized )
				{
					clear();
					m_is_init = initialized;
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
				MGR_ASSERT( ( this->m_bitset.test( pos, bits_count ) == false ),
					"Bits are already deallocated or invalid size." );
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

			inline bool is_free()
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

			bool is_constructed() const { return m_is_init == initialized; }

			offset_ptr<void>& get_internal_ptr( internal_ptrs id )
			{
				return m_internal_pointers[id];
			}
		private:
			//Bitset
			isInitialized m_is_init;
			size_type m_bit_hint;

			offset_ptr<void> m_internal_pointers[internal_ptrs_count];
			bitset_t m_bitset;


			static inline size_type block_index(size_type pos) 
			{
				return detail::block_index<bitset_t::bits_per_block>(pos);
			}

			static inline chunk_ptr_type block_ptr_cast( void* ptr )
			{
				return static_cast<chunk_ptr_type>( ptr );
			}
		};

		template< class Ch, class Tr, class ChunkType, size_t BitsCount, bitMgrMemCtrl memoryCtrl >
		static inline std::basic_ostream<Ch, Tr>& operator<<( std::basic_ostream<Ch, Tr>& ostr,
			const bit_manager<ChunkType, BitsCount, memoryCtrl>& b )
		{
			return b.print( ostr );
		}
	}
}

#endif// MGR_BIT_MANAGER_HEADER
