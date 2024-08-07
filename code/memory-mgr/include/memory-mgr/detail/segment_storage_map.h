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

#include "memory-mgr/detail/segment_storage_base.h"
#include <map>

namespace memory_mgr
{

	namespace detail
	{
		template
		<
			class MemMgr,
			size_t SegmentsCount
		>
		class segment_storage_map
			:protected segment_storage_base<MemMgr, SegmentsCount>
		{
			typedef segment_storage_base<MemMgr, SegmentsCount> 	base_type;
			typedef typename base_type::seg_base_type		seg_base_type;

			typedef std::map<seg_base_type, size_t>			seg_bases_type;

			seg_bases_type						m_bases;
		public:
			enum
			{
				num_segments		= base_type::num_segments,
				segment_size		= base_type::segment_size,
				allocable_memory	= base_type::allocable_memory
				//offset_mask,
				//segment_mask
			};
			typedef typename base_type::segment_ptr_type		segment_ptr_type;
			typedef typename base_type::size_type			size_type;
			typedef typename base_type::offset_type			offset_type;
			typedef typename base_type::segment_data_type		segment_data_type;

			segment_storage_map()
			{}

			/**
			@add_comments
			@todo get rid of runtime polymorphism
			*/
			void on_new_segment( segment_ptr_type segment, size_type seg_id )
			{
				m_bases[segment->get_offset_base()] = seg_id;
			}

			inline std::pair<segment_data_type, bool> find_segment( const void* ptr )
			{
				const char* p = detail::char_cast( ptr );
				typename seg_bases_type::const_iterator fres = m_bases.upper_bound( detail::unconst_char( p ) );
				if( fres != m_bases.begin() )
				{
					--fres;
					if( in_segment( fres->first, p ) )
					{
						return std::make_pair( *fres, true );
					}
				}
				return std::make_pair( segment_data_type(), false );
			}

			void delete_segments()
			{
				base_type::delete_segments();
				m_bases.clear();
			}

		};

	}
}
