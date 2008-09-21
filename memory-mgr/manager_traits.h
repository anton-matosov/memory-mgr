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

#ifndef MGR_MANAGER_TRAITS_HEADER
#define MGR_MANAGER_TRAITS_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

namespace memory_mgr
{
	//Forward declaration
	template<class MemMgr> 
	class block_id_converter;

	/**
	   @brief memory_manager traits
	   @details all associated types and constants are accessible using this type
	*/
	template<class MemMgr>
	struct manager_traits
	{
		/**
		   @brief Memory manager type
		*/
		typedef MemMgr										manager_type;

		/**
		   @brief Memory manager class, that was decorated
		*/
		typedef void										base_manager_type;

		/**
		   @brief memory manager category tag
		*/
		typedef typename manager_type::manager_category		manager_category;

		/**
			@brief memory block type
			@see static_bitset::block_type
		*/
		typedef typename manager_type::chunk_type			chunk_type;		
		
		/**
		   @brief Type used to store size, commonly std::size_t
		   @see static_bitset::size_type
		*/
		typedef typename manager_type::size_type			size_type;
		
		/**
		   @brief Type of synchronization object passed as template
		   parameter                                               
		*/
		typedef typename manager_type::sync_object_type		sync_object_type;

		/**
		   @brief memory offset type passed as template parameter
		*/
		typedef typename manager_type::offset_type			offset_type;

		/**
		@brief	memory block id type
		@detail objects of this type identify memory blocks
		e.g. objects of this type are retured by allocate method
		*/
		typedef typename manager_type::block_id_type		block_id_type;

		/**
		   @add_comments
		*/
		typedef block_id_converter<manager_type>			block_id_converter_type;

		/**
		@brief Type of synchronization object passed as template
		parameter                                               
		*/
		typedef typename manager_type::sync_object_type		sync_object_type;

		/**
		@brief lockable type, used for synchronization
		*/
		typedef typename manager_type::lockable_type		lockable_type;

		/**
		@brief lock type, used for synchronization
		*/
		typedef typename manager_type::lock_type			lock_type;

		enum
		{
			chunk_size			= manager_type::chunk_size		/**< size of memory chunk*/,
			memory_size			= manager_type::memory_size		/**< memory size in bytes available to manager*/,
			num_chunks			= manager_type::num_chunks		/**< number of memory chunks managed by memory manager*/,
			memory_overhead		= manager_type::memory_overhead /**< memory overhead per allocated memory block in bytes*/,
			allocable_memory	= manager_type::allocable_memory /**< size of memory that can be allocated*/,
			allocable_chunks	= manager_type::allocable_chunks /**< number of memory chunks that can be allocated*/,
			num_segments		= 1 /**< maximum number of segments managed by this manager*/
		};
	};

	/**
	@brief Converter class which should be used to convert objects of block_id_type
	*/
	template<class MemMgr> 
	class block_id_converter
	{
	public:
		typedef		MemMgr		mgr_type;
		typedef	typename manager_traits<mgr_type>::block_id_type	block_id_type;
		/**
		@add_comments
		*/
		template<class MgrT>
		static inline block_id_type	to_block_id( block_id_type id, MgrT& /*mgr*/ )
		{
			return id;
		}
		/**
		@add_comments
		*/
		template<class MgrT>
		static inline block_id_type	to_block_id( void* id, MgrT& mgr )
		{
			return detail::to_offset( id, mgr );
		}
	};
}

#endif //MGR_MANAGER_TRAITS_HEADER
