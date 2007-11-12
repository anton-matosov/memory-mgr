#pragma once

#include "memory_manager.h"
#include "singleton_manager.h"
#include "../scoped_ptr.h"


namespace managers
{

	template< class BlockType, size_t MemorySize, size_t ChunkSize >	
	class heap_memory_manager
	{
		typedef memory_manager<BlockType, MemorySize, ChunkSize>		mgr_t;
		typedef heap_memory_manager<BlockType, MemorySize, ChunkSize>	self_type;
				
		scoped_ptr<char, ::detail::array_deleter> m_memory;
		scoped_ptr<mgr_t> m_mgr;
	public:	
		typedef typename mgr_t::block_ptr_type		block_ptr_type;		
		typedef typename mgr_t::size_type			size_type;

		typedef typename mgr_t::ptr_t				ptr_t;

		heap_memory_manager()
			:m_memory( new char[MemorySize] ),
			m_mgr( new mgr_t( &*m_memory ) )
		{}

		~heap_memory_manager()
		{}

		operator mgr_t&()
		{
			return *m_mgr;
		}

		//Call this method to allocate memory block
		//size - block size in bytes
 		ptr_t allocate( size_type size )
 		{
 			return m_mgr->allocate( size );
 		}
 
 		//Call this method to deallocate memory block
 		//size - block size in bytes
 		void deallocate( const ptr_t ptr, size_type size )
 		{
 			m_mgr->deallocate( ptr, size );
 		}
  
  		//Returns base address
  		const char* get_base() const
  		{
  			return m_mgr->get_base();
  		}

		bool empty()
		{
			return m_mgr->empty();
		}

		bool free()
		{
			return m_mgr->free();
		}
	};

	typedef singleton_manager< heap_memory_manager<size_t, 1024 * 1024, 4> > def_heap_mgr;
}