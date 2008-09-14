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

#ifndef MGR_SEGMENT_STORAGE_HEADER
#define MGR_SEGMENT_STORAGE_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif


#include <memory-mgr/config/config.h>
#include <memory-mgr/manager_traits.h>
#include <memory-mgr/manager_category.h>
#include <vector>


namespace memory_mgr
{

	namespace detail
	{
		template
			<
				class MemMgr,
				size_t SegmentsCount
			>
		class segment_storage_base
		{
		public:
			typedef MemMgr			segment_type;
			typedef segment_type*	segment_ptr_type;

			typedef char*		seg_base_type;

			typedef typename manager_traits<segment_type>::size_type		size_type;
			typedef typename manager_traits<segment_type>::offset_type		offset_type;
			typedef std::pair<seg_base_type, size_type>						segment_data_type;

			enum
			{
				num_segments = SegmentsCount,
				segment_size = manager_traits<segment_type>::memory_size,
				allocable_memory = manager_traits<segment_type>::allocable_memory
				//offset_mask,
				//segment_mask
			};


		private:
			segment_ptr_type	m_segments[num_segments];
			template<class Func>
			class func_wrapper_base
			{
			protected:
				Func* func_;
			public:
				func_wrapper_base( Func& func )
					:func_( &func )
				{}
			};

			template<class Func>
			class if_not_null
				:public func_wrapper_base<Func>
			{
				typedef func_wrapper_base<Func> base_t;
			public:
				if_not_null( Func& func )
					:base_t( func )
				{}

				void operator()( segment_ptr_type& segment )
				{
					if( segment )
					{
						(*this->func_)( segment );
					}
				}
			};

			template<class Func>
			class if_not_null_and_res
				:public func_wrapper_base<Func>
			{
				bool result;
				typedef func_wrapper_base<Func> base_t;
			public:
				if_not_null_and_res( Func& func )
					:base_t( func ),
					result( true )
				{}

				bool operator()( segment_ptr_type& segment )
				{
					if( segment )
					{
						result &= (*this->func_)( segment );
					}
					return !result;
				}
			};

			static inline void delete_segment( segment_ptr_type& segment )
			{
				delete segment;
				segment = 0;
			}
		public:

			segment_storage_base()
			{
				std::fill<segment_ptr_type*,segment_ptr_type>( m_segments, m_segments + num_segments, 0 );
			}

			~segment_storage_base()
			{
				delete_segments();
			}

			void delete_segments()
			{
				for_each( boost::bind( delete_segment, _1 ) );
			}

			bool in_segment( const char* base, const char* ptr )
			{
				if ( ptr >= base )
				{
					if( (ptr - base) <= allocable_memory )
					{
						return true;
					}
				}
				return false;
			}

			template<class Func>
			void for_each( Func func )
			{
				std::for_each( m_segments, m_segments + num_segments, if_not_null<Func>(func) );
			}

			template<class Func>
			bool for_each_if( Func func )
			{
				segment_ptr_type* end = m_segments + num_segments;
				return std::find_if( m_segments, end, if_not_null_and_res<Func>(func) ) == end;
			}


			inline segment_ptr_type get_segment( size_type seg_id )
			{
				segment_ptr_type segment = m_segments[seg_id];
				if( !segment )
				{
					segment = m_segments[seg_id] = new segment_type( seg_id );
					on_new_segment_( segment, seg_id );
				}
				return segment;
			}

			boost::signal<void (segment_ptr_type, size_type)>	on_new_segment_;
		};

	}
}

#endif //MGR_SEGMENT_STORAGE_HEADER


