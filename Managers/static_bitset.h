#pragma once

#include <limits>
#include <ostream>
#include <assert.h>
#include "../static_assert.h"
#include "helpers.h"

namespace managers
{
	enum arrayType{ StaticArray, DynamicArray, CustomArray };
	namespace detail
	{
		//Structure used to calculate number of blocks
		//required to store requested number of bits
		template<size_t BitsCount, size_t BitsPerBlock>
		struct calc_num_blocks
		{
			enum {result = (BitsCount / BitsPerBlock) + (BitsCount % BitsPerBlock ? 1 : 0)};
		};

		//Structure used to calculate bit index
		//in block
		template< size_t Pos, size_t BitsPerBlock >
		struct bit_index
		{
			enum{ result = Pos % BitsPerBlock };
		};

		//Structure used to calculate number of extra bits in block
		template<size_t BitsCount, size_t BitsPerBlock>
		struct extra_bits
		{
			enum { result = bit_index< BitsCount, BitsPerBlock >::result };
		};

		//Set strategy
		struct set_op 
		{
			template<class T>
			static void update( T& lhs, const T rhs )
			{
				lhs |= rhs;
			}
		};

		//Reset strategy
		struct reset_op 
		{
			template<class T>
			static void update( T& lhs, const T rhs )
			{
				lhs &= ~rhs;
			}
		};

		//Helper structure used for functions overriding
		//based on integer values
		template<int i>
		struct int2type
		{
			enum{ value = i };
		};

		//Type traits for all array types
		template< class BlockType, int BitsCount >
		struct ArrayTraits
		{
			typedef BlockType				block_type;
			typedef block_type&				block_ref_type;
			typedef const block_type&		const_block_ref_type;

			typedef block_type*				block_ptr_type;
			typedef const block_type*		const_block_ptr_type;

			enum {			
				bits_per_block = std::numeric_limits<block_type>::digits,
				num_bits = BitsCount,
				num_blocks = detail::calc_num_blocks<num_bits, bits_per_block>::result,
				max_bits = num_blocks * bits_per_block,
				memory_usage = sizeof(block_type) * num_blocks
			};
		};

		//Basic array structure for array specializations 
		template< class BlockType, size_t BitsCount, arrayType Type >
		struct Array{};

		//Array specializations for static array
		template< class BlockType, size_t BitsCount >
		struct Array< BlockType, BitsCount, StaticArray> : public ArrayTraits<BlockType, BitsCount>
		{			
			block_type m_bits[num_blocks];
		};

		//Array specializations for dynamic array
		template< class BlockType, size_t BitsCount >
		struct Array< BlockType, BitsCount, DynamicArray> : public ArrayTraits<BlockType, BitsCount>
		{			
			Array()
				:m_bits( 0 )
			{ m_bits = new block_type[num_blocks]; }

			~Array()
			{ delete[] m_bits; }

			block_type* m_bits;
		};

		//Array specializations for custom array
		template< class BlockType, size_t BitsCount >
		struct Array< BlockType, BitsCount, CustomArray> : public ArrayTraits<BlockType, BitsCount>
		{		
			Array( block_ptr_type arr_ptr )
				:m_bits( arr_ptr )
			{}

			block_type* m_bits;		
		};
	}

	template< class BlockType, size_t BitsCount, arrayType StaticArr = StaticArray >
	class static_bitset: public detail::Array< BlockType, BitsCount, StaticArr >
	{		
		STATIC_ASSERT( BitsCount != 0, Bitset_cant_be_empty );

		static_bitset( const static_bitset& );
		static_bitset& operator=( const static_bitset& );
	public:	
		typedef static_bitset	self_t;		
		typedef self_t&		self_ref_t;

		typedef Array< BlockType, BitsCount, StaticArr >	base_t;

		typedef typename base_t::block_type				block_type;
		
		typedef typename base_t::block_ref_type			block_ref_type;
		typedef typename base_t::const_block_ref_type	const_block_ref_type;
		
		typedef typename base_t::block_ptr_type			block_ptr_type;
		typedef typename base_t::const_block_ptr_type	const_block_ptr_type;

		typedef size_t		size_type;
		typedef size_type	block_width_type;

		const static size_type npos = ~size_type(0);
	
		//Default constructor
		//Resets all bits
		static_bitset()
		{ reset(); }

		//Constructor used only by custom arrays to initialize
		//array's pointer
		explicit static_bitset( block_ptr_type bits_ptr )
			:base_t(bits_ptr)
		{ reset(); }

		~static_bitset()
		{}

		size_type size() const
		{ return num_bits; }

		bool test( size_type pos ) const
		{
			assert(pos < num_bits);
			return unchecked_test(pos);
		}

		bool test( size_type pos, size_type count ) const
		{
			assert(pos < num_bits);
			assert(count > 0);

			size_type blk = block_index( pos );
			size_type ind = pos;
			ptrdiff_t left = count;
			while( left > 0 )
			{
				if ( contains_bits(blk, ind, left) )
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
		
		self_ref_t set()
		{
			std::fill(m_bits, m_bits + num_blocks, ~block_type(0));
			zero_unused_bits();
			return *this;
		}

		self_ref_t set( size_type pos)
		{
			assert(pos < num_bits);

			m_bits[block_index(pos)] |= bit_mask(pos);
			return *this;
		}


		self_ref_t set( size_type pos, size_type count )
		{			
			return do_set<detail::set_op>( pos, count );
		}

		self_ref_t reset(size_type pos)
		{
			assert(pos < num_bits);
			m_bits[block_index(pos)] &= ~bit_mask(pos);
			return *this;
		}

		self_ref_t reset( size_type pos, size_type count )
		{			
			return do_set<detail::reset_op>( pos, count );
		}

		self_ref_t reset()
		{
			std::fill(m_bits, m_bits + num_blocks, block_type(0));
			return *this;
		}

		self_ref_t flip(size_type pos)
		{
			assert(pos < num_bits);
			m_bits[block_index(pos)] ^= bit_mask(pos);
			return *this;
		}

		self_ref_t flip()
		{
			for (size_type i = 0; i < num_blocks; ++i)
				m_bits[i] = ~m_bits[i];
			zero_unused_bits();
			return *this;
		}


		size_type find_n( size_type count,  size_type first_block = 0 )
		{
			if( count == 0 )
			{
				return npos;
			}
			size_type lowest_bit = do_find_from(first_block);

			size_type blk = first_block;
			size_type ind = lowest_bit;
			ptrdiff_t left = count;
			while( (left > 0) && (lowest_bit != npos) )
			{
				if ( contains_bits(blk, ind, left) )
				{
					++blk;
					left -= bits_per_block - ind;
					ind = 0;
				}
				else
				{
					lowest_bit = ind = do_find_next( ind + 1, blk );
					left = count;
					first_block = blk;
				}
			}
			
			return (lowest_bit != npos) ? first_block * bits_per_block + lowest_bit : npos;
		}

		size_type find_first() const
		{
			size_type blk_index = 0;
			return blk_index * bits_per_block + do_find_from( blk_index );
		}

		size_type find_next( size_type pos ) const
		{
			
			++pos;

			size_type blk_index = block_index(pos);
			return blk_index * bits_per_block + do_find_next( bit_index(pos), blk_index );
		}
		
	private:
		static inline size_type block_index(size_type pos) 
		{ return pos / bits_per_block; }
		
		static inline block_width_type bit_index(size_type pos) 
		{ return pos % bits_per_block; }

		static inline block_type bit_mask(size_type pos) 
		{ return static_cast<block_type>( block_type(1) << bit_index(pos) ); }
		
		static inline block_type bit_mask(size_type pos, size_type count) 
		{ return block_type( ((block_type(1) << count ) - 1) << bit_index(pos) ); }

		static inline block_type higher_bit_mask(size_type pos) 
		{ return block_type( ~block_type(0) << bit_index(pos) ); }

		static inline block_type lower_bit_mask(size_type pos) 
		{ return ~higher_bit_mask(pos); }

		static inline size_type blocks_count( size_type bits_count ) 
		{ return block_index( bits_count ); }

		bool unchecked_test(size_type pos) const
		{ return (m_bits[block_index(pos)] & bit_mask(pos)) != 0; }
		

		size_type find_first_block( size_type from ) const
		{
			// skip null blocks
			while (from < num_blocks && m_bits[from] == 0)
			{
				++from;
			}
			return from;
		}

		size_type do_find_from( size_type& first_block ) const
		{
			first_block = find_first_block( first_block );

			if (first_block >= num_blocks)
			{
				return npos; // not found
			}

			return helpers::lowest_bit(m_bits[first_block]);
		}

		size_type do_find_next( size_type bit_ind, size_type& blk_ind ) const
		{
			if ( bit_ind >= num_bits )
			{
				return npos;
			}
			const block_type fore = m_bits[blk_ind] & ( higher_bit_mask(bit_ind) );

			return fore ? helpers::lowest_bit(fore):
				do_find_from( ++blk_ind );
		}

		bool contains_bits( size_type block, size_type first, size_type count ) const
		{
			const block_type mask = bit_mask( first, count );
			return (m_bits[block] &  mask) == mask;
		}

		template< class C >
		void do_zero_unused_bits()
		{
			enum { highest_bits_mask = ~(~block_type(0) << C::value) };
			highest_block() &= highest_bits_mask;
		}

		template<>
		void do_zero_unused_bits< detail::int2type<0> >() 
		{}

		// If size() is not a multiple of bits_per_block
		// then not all the bits in the last block are used.
		// This function resets the unused bits (convenient
		// for the implementation of many member functions)
		//
		void zero_unused_bits()
		{
			// if != 0 this is the number of bits used in the last block
			do_zero_unused_bits< detail::int2type< detail::extra_bits<num_bits, bits_per_block>::result > >();
		}		

		// gives a reference to the highest block
		//
		block_ref_type highest_block()
		{
			return m_bits[num_blocks - 1];
		}

		// gives a const-reference to the highest block
		//
		const_block_ref_type highest_block() const
		{
			return m_bits[num_blocks - 1];
		}

		template< class set_op >
		self_ref_t do_set( size_type pos, size_type count )
		{
			assert(pos < num_bits);
			count = (count != npos ? count : num_bits - pos); 

			assert( (pos + count) < num_bits );

			size_type block_ind = block_index(pos);
			const size_type block_end	= block_index(pos + count);
			const size_type last_index = bit_index(pos + count);


			size_type bits_fill_count = count;
			do
			{
				set_op::update(m_bits[block_ind++], bit_mask(pos, bits_fill_count) );
				pos = 0;
				bits_fill_count = (block_ind != block_end) ? bits_per_block : last_index;
			}
			while (block_ind <= block_end);

			return *this;
		};
	};

	template< class BlockType, int BitsCount, arrayType Type >
	std::ostream& operator<<( std::ostream& ostr, const static_bitset<BlockType, BitsCount, Type>& b )
	{
		char one = '1';
		char zero = '0';
		char space = ' ';

		typedef static_bitset<BlockType, BitsCount, Type> bit_mgr_t;
		typedef typename bit_mgr_t::size_type bitsetsize_type;

		for (bitsetsize_type i = b.size() - 1; ( i != bit_mgr_t::npos ) && ostr; --i) 
		{
			ostr << (b.test(i)? one : zero);
			if ( (i % 8) == 0 )
			{
				ostr << space;
			}
			
		}
		return ostr;
	}
}

