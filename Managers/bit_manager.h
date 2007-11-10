#pragma once

#include "static_bitset.h"

namespace managers
{
	enum bitMgrMemCtrl{ mcAuto = StaticArray, mcNone = CustomArray };
	template< class BlockType, int BitsCount, bitMgrMemCtrl memoryCtrl = mcAuto >
	class bit_manager
	{
		typedef static_bitset< BlockType, BitsCount, arrayType(memoryCtrl) > bitset_t;
		bitset_t m_bitset;
	public:
		typedef typename bitset_t::block_type			block_type;
		typedef typename bitset_t::block_ptr_type		block_ptr_type;
		typedef typename bitset_t::size_type			size_type;

		enum {
			memory_usage = bitset_t::memory_usage
		};

		const static size_type npos = bitset_t::npos;

		bit_manager()			
		{
			m_bitset.set();
		}

		bit_manager( block_ptr_type ptr )
			:m_bitset( ptr )
		{
			m_bitset.set();
		}

		size_type requires_bytes()
		{
			return memory_usage;
		}

		size_type allocate( size_type bits_count )
		{
			size_type pos = m_bitset.find_n( bits_count );
			if( pos != npos )
			{
				m_bitset.reset( pos, bits_count );
			}
			return pos;
		}

		void deallocate( size_type pos, size_type bits_count )
		{
			m_bitset.set( pos, bits_count );
		}

		std::ostream& print( std::ostream& ostr ) const
		{
			return ostr << m_bitset;
		}
	};

	template< class BlockType, int BitsCount, bitMgrMemCtrl memoryCtrl >
	std::ostream& operator<<( std::ostream& ostr, const bit_manager<BlockType, BitsCount, memoryCtrl>& b )
	{
		return b.print( ostr );
	}
}
