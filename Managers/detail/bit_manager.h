/* 
Generic Memory Manager (memory-mgr)
Copyright (c) 2007 Anton Matosov

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

#include "static_bitset.h"

namespace managers
{
	namespace detail
	{
		enum bitMgrMemCtrl{ mcAuto = StaticArray, mcNone = CustomArray };
		template< class BlockType, int BitsCount, bitMgrMemCtrl memoryCtrl = mcAuto >
		class bit_manager
		{
			typedef static_bitset< BlockType, BitsCount, arrayType(memoryCtrl) > bitset_t;			
		public:
			typedef typename bitset_t::block_type			block_type;
			typedef typename bitset_t::block_ptr_type		block_ptr_type;
			typedef typename bitset_t::size_type			size_type;

			enum {
				memory_usage = bitset_t::memory_usage,
				num_bits = BitsCount
			};

			const static size_type npos = bitset_t::npos;

			bit_manager()
				:m_last_block( 0 )
			{
				m_bitset.set();				
			}

			bit_manager( block_ptr_type ptr )
				:m_bitset( ptr ),
				 m_last_block( 0 )
			{
				m_bitset.set();
			}

			size_type requires_bytes()
			{
				return memory_usage;
			}

			size_type allocate( size_type bits_count )
			{
				size_type pos = m_bitset.find_n( bits_count, m_last_block );
				if( pos != npos )
				{
					m_last_block = block_index(pos);
					m_bitset.reset( pos, bits_count );
				}
				else
				{
					if (m_last_block != 0)
					{
						m_last_block = 0;
						return allocate( bits_count );
					}					
				}
				return pos;
			}

			void deallocate( size_type pos, size_type bits_count )
			{
				assert( ( m_bitset.test( pos, bits_count ) == false ) && "Bits are already deallocated or invalid size." );
				m_bitset.set( pos, bits_count );
				m_last_block = block_index(pos);
			}

			std::ostream& print( std::ostream& ostr ) const
			{
				return ostr << m_bitset;
			}

			bool empty()
			{
				return m_bitset.empty();
			}

			bool free()
			{
				return !empty();
			}

		private:
			bitset_t m_bitset;
			size_type m_last_block;

			static inline size_type block_index(size_type pos) 
			{
				return detail::block_index<bitset_t::bits_per_block>(pos);
			}
		};

		template< class BlockType, int BitsCount, bitMgrMemCtrl memoryCtrl >
		std::ostream& operator<<( std::ostream& ostr, const bit_manager<BlockType, BitsCount, memoryCtrl>& b )
		{
			return b.print( ostr );
		}
	}
}

#endif// MGR_BIT_MANAGER_HEADER