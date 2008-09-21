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

#ifndef MGR_MEMORY_MANAGER_HEADER
#define MGR_MEMORY_MANAGER_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <memory-mgr/detail/ptr_helpers.h>
#include <memory-mgr/detail/bit_manager.h>
#include <memory-mgr/sync/critical_section.h>
#include <memory-mgr/sync/locks.h>
#include <memory-mgr/detail/offset_traits.h>
#include <memory-mgr/manager_category.h>
#include <memory-mgr/detail/decorator_base.h>

/**
   @brief Main namespace
*/
namespace memory_mgr
{
	template
	<
		class  ChunkType, 
		size_t MemorySize,
		size_t ChunkSize
	>
	struct allocable_memory_calc
	{
		enum
		{
			chunk_size  = ChunkSize,
			memory_size = MemorySize,
			num_chunks  = memory_size / chunk_size
		};

		typedef detail::bit_manager<ChunkType, num_chunks, detail::mcNone> bitmgr_type;

		enum
		{
			used_memory = bitmgr_type::memory_usage,
			used_chunks = used_memory / chunk_size,

			allocable_memory = memory_size - used_memory,
			allocable_chunks = num_chunks - used_chunks,
		};

		typedef detail::bit_manager<ChunkType, allocable_chunks, detail::mcNone> allocable_bitmgr_type;

		enum
		{
			required_memory = allocable_bitmgr_type::memory_usage,
			lost_memory_tmp = used_memory - required_memory,
			lost_memory		= lost_memory_tmp > 0 ? lost_memory_tmp : 0
		};
		typedef allocable_memory_calc<ChunkType, lost_memory, chunk_size> lost_calc;

		enum
		{
			result_used_memory		= used_memory + lost_calc::used_memory,
			res_allocable_memory	= allocable_memory + lost_calc::allocable_memory,
			result_lost_memory		= lost_calc::lost_memory,
			result_allocable_chunks = res_allocable_memory / chunk_size,
			result_allocable_memory = result_allocable_chunks * chunk_size
		};
	};

	template<class ChunkType,
		size_t ChunkSize>
	struct allocable_memory_calc< ChunkType, 0, ChunkSize >
	{
		enum
		{
			lost_memory = 0,
			used_memory = 0,
			allocable_memory = 0
		};
	};

	/**
	   @brief Encapsulates work with memory
	   @tparam ChunkType   integral type, that will be used in
	                      static_bitset
	   @tparam MemorySize  memory size in bytes which will be
	                      available to manager
	   @tparam ChunkSize   size of minimum block in bytes, which
	                      manager can allocate
	   @tparam OffsetType  type that will be used to store memory offset
	   @tparam SyncObj     Synchronization object that will be used to
	                      synchronize access to internal structures
	                      during management operations               
	*/
	template
	<
		class ChunkType, 
		size_t MemorySize,
 		size_t ChunkSize,
		class OffsetType = size_t,
		class SyncObj = sync::critical_section
	>
	class memory_manager : protected sync::object_level_lockable<SyncObj>
	{
		typedef memory_mgr::allocable_memory_calc<ChunkType, MemorySize, ChunkSize> calc_type;

		template<class Mgr>
		friend class decorator_base;
	public:
		/**
		   @brief compile time computed constants
		*/
		enum
		{
			chunk_size			= ChunkSize		/**< size of memory chunk*/,
			memory_size			= MemorySize	/**< memory size in bytes*/,
			num_chunks			= memory_size / chunk_size /**< number of memory chunks that can be allocated*/,
			memory_overhead		= 0				/**< memory overhead per allocated memory block in bytes*/,
			allocable_memory	= calc_type::result_allocable_memory /**< size of memory that can be allocated*/,
			allocable_chunks	= calc_type::result_allocable_chunks
		};
		
	private:	

		/**
		   @brief lock type, used for synchronization
		*/
		typedef typename sync::object_level_lockable<SyncObj>::lock lock;
		
		/**
		   @brief bit manager type, used to manipulate chunks bitmap
		*/
		typedef detail::bit_manager<ChunkType, calc_type::result_allocable_chunks, detail::mcNone> bitmgr_type;

		/**
		   @brief Bit Manager used to store information about allocated
		   memory chunks                                               
		*/
		bitmgr_type m_bitmgr;

		/**
		   @brief Pointer to memory base address from which offset is
					calculated                                                
		*/
		char* m_offset_base;
		
		/**
		   @brief Pointer to memory segment base address                                                
		*/
		void* m_segment_base;
	public:
		/**
		   @brief Type of this
		*/
		typedef memory_manager								self_type;
		/**
		   @brief memory manager category tag
		*/
		typedef memory_manager_tag							manager_category;

		/**
			@brief memory block type
			@see static_bitset::block_type
		*/
		typedef typename bitmgr_type::chunk_type			chunk_type;

		/**
		   @brief memory block pointer type
		   @see static_bitset::block_ptr_type
		*/
		typedef typename bitmgr_type::chunk_ptr_type		chunk_ptr_type;

		/**
		   @brief Type used to store size, commonly std::size_t
		   @see static_bitset::size_type
		*/
		typedef typename bitmgr_type::size_type				size_type;
		
		/**
		   @brief Type of synchronization object passed as template
		   parameter                                               
		*/
		typedef SyncObj										sync_object_type;
		
		/**
		   @brief memory offset type passed as template parameter
		*/
		typedef OffsetType									offset_type;
		
		/**
		@brief	memory block id type
		@detail objects of this type identify memory blocks
		e.g. objects of this type are retured by allocate method
		*/
		typedef offset_type									block_id_type;

		/**
		   @brief Default constructor, declared only to
					support decorators technology
		*/
		memory_manager()
		{
			/**
			   @warning If you use explicit template instantiation you will get this error
						don't remove this assert, better remove explicit template instantiation
			*/
			STATIC_ASSERT( false, Default_constructor_cant_be_used_for_creation_of_memory_manage );
		}

		/**
		   @brief Constructor, performs initialization of manager and
					memory passed as parameter
		   @param segment_base  Pointer to memory which will be managed by
		                    manager, before first use First four bytes (dword) of memory
							segment must be zeroed                   
		*/
		explicit memory_manager( void* segment_base )
			:m_bitmgr( static_cast< chunk_ptr_type >( segment_base ) ),
			m_segment_base( segment_base )
		{
			m_offset_base = detail::char_cast( detail::shift( segment_base, bitmgr_type::memory_usage ) );
		}
		
		/**
		   @brief Call this method to allocate memory block
		   @param size size of memory block in bytes
		   @exception bad_alloc if manager went out of memory
		   @return offset in bytes from memory base address.
		*/
		inline offset_type allocate( size_type size )
		{			
			return do_allocate( size, helpers::throw_bad_alloc );
		}


		/**
		   @brief Call this method to allocate memory block
		   @param size    size of memory block in bytes 
		   @param nothrow  unused parameter, just to overload existing
		                   function
		   
		   @exception newer  throws
		   @return offset in bytes from memory base address.          
		*/
		inline offset_type allocate( size_type size, const std::nothrow_t& /*nothrow*/ )/*throw()*/
		{			
			return do_allocate( size, helpers::do_nothing );
		}

		/**
		   @brief Call this method to deallocate memory block
		   @param offset  offset returned by allocate method
		   @param size   size of memory block in bytes
		   @exception newer  throws
		*/
 		inline void deallocate( const offset_type offset, size_type size )
 		{
			if( offset != offset_traits<offset_type>::invalid_offset )
			{
				lock l(*this);
 				m_bitmgr.deallocate( chunk_index( offset ), chunks_required( size ) );
			}
 		}


		/**
		   @add_comments
		*/
		inline void* offset_to_pointer( offset_type offset )
		{
			if( offset == offset_traits<offset_type>::invalid_offset )
			{
				return 0;
			}
			return detail::shift( m_offset_base, offset );
		}

		/**
		   @add_comments
		*/
		inline offset_type pointer_to_offset( const void* ptr )
		{
			if( ptr )
			{
				assert( ptr >= m_offset_base && "Invalid pointer value");
				assert(ptr < ( m_offset_base + memory_size ) && "Invalid pointer value" );
				return detail::diff( ptr, m_offset_base );
			}
			return offset_traits<offset_type>::invalid_offset;
		}

		/**
		   @add_comments
		*/
		inline void* get_segment_base()
		{
			return m_segment_base;
		}

		/**
		   @brief Call this method to know is there available memory in
		   manager
		   
		   @exception newer  throws
		   @retval true   if there is no more free memory to
		                  allocate
		   @retval false  otherwise                                    
		*/
		inline bool empty()
		{
			return m_bitmgr.empty();
		}

		/**
		   @brief Call this method to know that there is no allocated blocks
		   @exception newer  throws
		   @retval true   no blocks are allocated by this manager
		   @retval false  otherwise                                     
		*/
		inline bool is_free()
		{
			return m_bitmgr.is_free();
		}

		/**
		   @brief Call this method to deallocate all allocated memory
		   @exception newer  throws                                     
		*/
		inline void clear()
		{
			m_bitmgr.clear();
		}

		
		/**
		   @brief Call this method to get memory base address from which offset
		   is calculated
		   @param offset offset for which base address should be returned
		   @exception newer  throws
		   @return pointer to memory base address                               
		*/
		inline const char* get_offset_base( const offset_type /*offset*/ = 0 ) const
		{
			return m_offset_base;
		}

		inline char* get_offset_base( offset_type /*offset*/ = 0 )
		{
			return m_offset_base;
		}


		/**
		   @add_comments
		*/
		inline const char* get_ptr_base( const void* /*ptr*/ ) const
		{
			return m_offset_base;
		}

		inline char* get_ptr_base( void* /*ptr*/ ) const
		{
			return m_offset_base;
		}
	private:
		/**
		   @brief Returns offset by chunk index
		   @param chunk_ind  chunk index
		   @exception newer  throws
		*/
		static inline size_type calc_offset( size_type chunk_ind )
		{ return chunk_ind * chunk_size; }

		/**
		   @brief Returns chunk index by offset
		   @param offset  memory offset in bytes
		   @exception newer  throws
		*/
		static inline size_type chunk_index( size_type offset )
		{ return offset / chunk_size; }

		/**
		   @brief Returns chunks count by bytes count
		   @param size size in bytes
		   @exception newer  throws
		*/
		static inline size_type chunks_count( size_type size )
		{ return chunk_index( size ); }

		/**
		   @brief Returns number of extra bytes
		   @exception newer  throws
		*/
		static inline size_type extra_bytes( size_type size )
		{ return size % chunk_size; }

		/**
		   @brief Returns number of chunks required to store requested
		   bytes number		   
		   @param size size in bytes
		   @exception newer  throws
		*/
		static inline size_type chunks_required( size_type size )
		{
			return chunks_count( size ) + (extra_bytes( size ) ? 1 : 0);
		};

		


		/**
		   @brief Call this method to allocate memory block
		   @return offset in bytes from memory base address.
		   @param size         size of memory block in bytes
		   @param OnNoMemoryOp  Callback for "out of memory" event
		   @exception n/a Callback function can throw an exception, 
		   but by itself method never throws
		*/
		template< class OnNoMemory >
		inline offset_type do_allocate( size_type size, OnNoMemory OnNoMemoryOp )
		{
			lock l(*this);
			size_type chunk_ind = m_bitmgr.allocate( chunks_required( size ) );
			if( chunk_ind == bitmgr_type::npos )
			{
				OnNoMemoryOp();
				return offset_traits<offset_type>::invalid_offset;
			}
			return calc_offset( chunk_ind );
		}
	};	

/**
   @brief Helper macros, using it you can easily create derived class from memory manager
   @details 
*/
#define MGR_DECLARE_MANAGER_CLASS(name, manager_type)\
	struct name:\
		public manager_type\
	{};\
	namespace memory_mgr{\
		template<>\
		struct manager_traits<name>:\
			public manager_traits<manager_type>\
		{};\
	}
}


#endif// MGR_MEMORY_MANAGER_HEADER
