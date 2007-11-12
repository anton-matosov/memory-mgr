#pragma once

namespace managers
{
	template< class MemMgr >
	class managed_base
	{
	protected:
		~managed_base()
		{}

		typedef MemMgr mem_mgr;
	public:	
		static void* operator new( size_t size )/*throw( std::bad_alloc )*/
		{
			return mem_mgr::instance().allocate( size ).get_ptr( mem_mgr::instance() );			
		}

		static void* operator new( size_t size, const std::nothrow_t& nothrow ) /*throw()*/
		{
			return mem_mgr::instance().allocate( size, nothrow ).get_ptr( mem_mgr::instance() );			
		}

		static void* operator new(  size_t, void* p ) /*throw()*/
		{
			return p;
		}

		static void operator delete( void* p, size_t size )
		{
			mem_mgr::instance().deallocate( mem_mgr::ptr_t( mem_mgr::instance(), p ), size );
		}
	};
}