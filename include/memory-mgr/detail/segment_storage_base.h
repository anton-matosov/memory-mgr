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

#include "memory-mgr/config/config.h"
#include "memory-mgr/detail/math.h"
#include "memory-mgr/manager_traits.h"
#include "memory-mgr/manager_category.h"
#include <vector>


namespace memory_mgr
{

	namespace detail
	{
		/**
		@add_comments
		*/
		template<size_t seg_size>
		struct calc_offset_bits
		{
			enum
			{
				log2	= math::int_log2<size_t, seg_size>::result,
				extra	= seg_size % log2,
				result	= log2 + (extra ? 1 : 0)			
			};
		};

		/**
		@add_comments
		*/
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

			typedef char*			seg_base_type;

			typedef typename manager_traits<segment_type>::size_type			size_type;
			typedef typename manager_traits<segment_type>::block_offset_type	block_offset_type;
			typedef std::pair<seg_base_type, size_type>							segment_data_type;
			typedef size_type													seg_id_type;

			/**
			@brief Type of synchronization object passed as template
			parameter                                               
			*/
			typedef typename manager_traits<segment_type>::sync_object_type		sync_object_type;

			/**
			@brief lockable type, used for synchronization
			*/
			typedef typename manager_traits<segment_type>::lockable_type		lockable_type;

			/**
			@brief lock type, used for synchronization
			*/
			typedef typename manager_traits<segment_type>::lock_type			lock_type;

			enum
			{
				num_segments		= SegmentsCount,
				segment_size		= manager_traits<segment_type>::memory_size,
				allocable_memory	= manager_traits<segment_type>::allocable_memory,

				offset_bits			= calc_offset_bits< allocable_memory >::result,
				segment_mask		= ~seg_id_type(0) << offset_bits,
				offset_mask			= ~segment_mask
			};

			//typedef void (*on_new_segment_fn_type)(segment_ptr_type, size_type);
			
		private:
			//segments array
			segment_ptr_type	m_segments[num_segments];


			/**
			@add_comments
			*/
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

			/**
			@add_comments
			*/
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

			/**
			@add_comments
			*/
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

			/**
			@add_comments
			*/
			static inline void delete_segment( segment_ptr_type& segment )
			{
				delete segment;
				segment = 0;
			}
		public:

			/**
			@add_comments
			*/
			segment_storage_base()
			{
				std::fill<segment_ptr_type*,segment_ptr_type>( m_segments, m_segments + num_segments, 0 );
			}

			/**
			@add_comments
			*/
			~segment_storage_base()
			{
				delete_segments();
			}

			/**
			@add_comments
			*/
			void delete_segments()
			{
				for_each( delete_segment );
			}

			/**
			@add_comments
			*/
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

			/**
			@add_comments
			*/
			template<class Func>
			void for_each( Func func )
			{
				std::for_each( m_segments, m_segments + num_segments, if_not_null<Func>(func) );
			}

			/**
			@add_comments
			*/
			template<class Func>
			bool for_each_if( Func func )
			{
				segment_ptr_type* end = m_segments + num_segments;
				return std::find_if( m_segments, end, if_not_null_and_res<Func>(func) ) == end;
			}


			/**
			@add_comments
			*/
			inline segment_ptr_type get_segment( size_type seg_id )
			{
				segment_ptr_type segment = m_segments[seg_id];
				if( !segment )
				{
					segment = m_segments[seg_id] = new segment_type( seg_id );
					on_new_segment( segment, seg_id );
				}
				return segment;
			}

			/**
			@add_comments
			@todo get rid of runtime polymorphism
			*/
			virtual void on_new_segment( segment_ptr_type, size_type )
			{

			}
		};

	}
}
