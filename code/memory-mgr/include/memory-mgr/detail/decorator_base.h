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

#pragma once

#include "memory-mgr/manager_category.h"
#include "memory-mgr/manager_traits.h"
#include "memory-mgr/detail/static_assert.h"

namespace memory_mgr
{
	namespace detail
	{
		template< class MemMgr >
		class decorator_base
			:public MemMgr
		{
		protected:
			typedef MemMgr mgr_type;
			typedef decorator_base	decorated_mgr;

			decorated_mgr& get_decorated_mgr()
			{
				return *this;
			}
		public:	
			//Type definitions to make definition of classes drived from decorator_base easier
			typedef typename manager_traits<mgr_type>::size_type size_type;
			typedef typename manager_traits<mgr_type>::block_offset_type block_offset_type;

			typedef typename manager_traits<mgr_type>::sync_object_type sync_object_type;
			typedef typename manager_traits<mgr_type>::lockable_type lockable_type;
			typedef typename manager_traits<mgr_type>::lock_type lock_type;


			/**
			   @brief Default constructor, creates memory manager 
			   @remarks Can be used only if decorates memory manager with 
						attached memory segment
			   @see memory_manager::memory_segment                        
			*/
			inline decorator_base()
			{
				STATIC_ASSERT( (is_category_supported< mgr_type, memory_segment_tag >::value),
					Memory_manager_does_not_have_attached_memory_segment );
			}

			///One more constructor that delegates parameters to the base class
			inline explicit decorator_base( void* segment_base )
				:mgr_type( segment_base )
			{}
		};

	}

	/**
	   @brief memory_manager + decorator_base traits
	*/
	template<class MemMgr>
	struct manager_traits< detail::decorator_base< MemMgr > > 
		: public manager_traits< MemMgr >
	{
		/**
		   @brief Memory manager class, that was decorated
		*/
		typedef MemMgr base_manager_type;
	};
}
