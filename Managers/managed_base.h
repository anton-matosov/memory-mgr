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
		static void* operator new( size_t size )
		{
			return mem_mgr::instance().allocate( size ).get_ptr( mem_mgr::instance() );
		}

		static void* operator new( size_t size, const std::nothrow_t& )
		{
			try
			{
				return mem_mgr::instance().allocate( size ).get_ptr( mem_mgr::instance() );
			}
			catch (...)
			{
				return 0;	
			}		
		}

		static void* operator new(  size_t, void* p )
		{
			return p;
		}

		static void operator delete( void* p, size_t size )
		{
			mem_mgr::instance().deallocate( mem_mgr::ptr_t( mem_mgr::instance(), p ), size );
		}
	};
}