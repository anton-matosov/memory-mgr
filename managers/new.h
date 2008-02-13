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

#ifndef MGR_NEW_HEADER
#define MGR_NEW_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

namespace memory_mgr
{
	namespace detail
	{
		template
		<
			class MemMgr,
			class PointerConvSupp,
			class SizeTrackingSupp
		>
		struct new_helpers
		{
			typedef MemMgr mgr_type;
			static inline void* new_impl( size_t size, mgr_type& mgr )
			{
				STATIC_ASSERT( false, Invalid_manager_type )
			}
			
			static inline void delete_impl( void* p, mgr_type& mgr )
			{
				STATIC_ASSERT( false, Invalid_manager_type )
			}
			
			static inline void delete_arr_impl( void* p, mgr_type& mgr )
			{
				STATIC_ASSERT( false, Invalid_manager_type )
			}			
		};

		template< class MemMgr >
		struct new_helpers<MemMgr, typename is_category_supported<MemMgr, size_tracking_tag>::yes_type,
			typename is_category_supported<MemMgr, pointer_convertion_tag>::yes_type>
		{
			typedef MemMgr mgr_type;
			static inline void* new_impl( size_t size, mgr_type& mgr )
			{
				return mgr.allocate( size );
			}

			template<class T>
			static inline void delete_impl( T* p, mgr_type& mgr )
			{
				p->~T();
				return mgr.deallocate( p );
			}

			template<class T>
			static inline void delete_arr_impl( T* p, mgr_type& mgr )
			{				
				size_t* count = size_cast(p) - 1;
				for( size_t i = 0; i < *count; ++i )
				{
					p[i].~T();
				}
				//count - points to real memory address
				return mgr.deallocate( count );
			}
		};
	

		template<class MemMgr> 
		class mem_mgr_helper
		{			
			mem_mgr_helper& operator=( const mem_mgr_helper& );
		public:
			typedef MemMgr mgr_type;

			typedef typename memory_mgr::is_category_supported
				<
					mgr_type, typename memory_mgr::pointer_convertion_tag
				>::result supports_pointer_conv;

			typedef typename memory_mgr::is_category_supported
				<
					mgr_type, typename memory_mgr::size_tracking_tag
				>::result supports_size_tracking;

			mutable mgr_type& m_mgr;
			mem_mgr_helper( mgr_type& mgr )
				:m_mgr( mgr )
			{}
		};

		
	}

	template<class MemMgr>
	static inline detail::mem_mgr_helper<MemMgr> mem_mgr( MemMgr& mgr )
	{
		typedef MemMgr mgr_type;
		return detail::mem_mgr_helper<mgr_type>( mgr );
	};
}

template<class MemMgr>
void* operator new( size_t size, const memory_mgr::detail::mem_mgr_helper<MemMgr>& mgr )
{
	typedef MemMgr mgr_type;
	typedef memory_mgr::detail::mem_mgr_helper<mgr_type> mgr_helper_type;
	typedef memory_mgr::detail::new_helpers
		<
			mgr_type,
			typename mgr_helper_type::supports_size_tracking,
			typename mgr_helper_type::supports_pointer_conv
		> helper_type;
	
	return helper_type::new_impl( size, mgr.m_mgr );
}

template<class MemMgr>
void* operator new[]( size_t size, const memory_mgr::detail::mem_mgr_helper<MemMgr>& mgr )
{
	typedef MemMgr mgr_type;
	typedef memory_mgr::detail::mem_mgr_helper<mgr_type> mgr_helper_type;
	typedef memory_mgr::detail::new_helpers
		<
		mgr_type,
		typename mgr_helper_type::supports_size_tracking,
		typename mgr_helper_type::supports_pointer_conv
		> helper_type;

	return helper_type::new_impl( size, mgr.m_mgr );
}

// template<class MemMgr>
// void do_delete( memory_mgr::managed_base<MemMgr>* p, const memory_mgr::detail::mem_mgr_helper<MemMgr>& mgr )
// {
// 	//typedef MemMgr mgr_type;
// 	delete p;
// }

template<class T, class MemMgr>
void do_delete( T* p, const memory_mgr::detail::mem_mgr_helper<MemMgr>& mgr )
{
	typedef MemMgr mgr_type;
	typedef memory_mgr::detail::mem_mgr_helper<mgr_type> mgr_helper_type;
	typedef memory_mgr::detail::new_helpers
		<
		mgr_type,
		typename mgr_helper_type::supports_size_tracking,
		typename mgr_helper_type::supports_pointer_conv
		> helper_type;

	
	return helper_type::delete_impl( p, mgr.m_mgr );
}

template<class T, class MemMgr>
void do_delete_arr( T* p, const memory_mgr::detail::mem_mgr_helper<MemMgr>& mgr )
{
	typedef MemMgr mgr_type;
	typedef memory_mgr::detail::mem_mgr_helper<mgr_type> mgr_helper_type;
	typedef memory_mgr::detail::new_helpers
		<
		mgr_type,
		typename mgr_helper_type::supports_size_tracking,
		typename mgr_helper_type::supports_pointer_conv
		> helper_type;


	return helper_type::delete_arr_impl( p, mgr.m_mgr );
}

// template<class MemMgr>
// void do_delete( memory_mgr::managed_base<MemMgr> p[], const memory_mgr::detail::mem_mgr_helper<MemMgr>& mgr )
// {
// 	typedef MemMgr mgr_type;
// 	delete[] p;
// }

#endif //MGR_NEW_HEADER
