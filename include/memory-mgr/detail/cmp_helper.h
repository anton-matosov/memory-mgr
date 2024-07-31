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

namespace memory_mgr
{
	namespace detail
	{
		template< class ClassToHelp >
		class cmp_helper
		{
			typedef ClassToHelp rhs_operand_type;
			inline const rhs_operand_type& get_lhs() const
			{
				return static_cast<const rhs_operand_type&>( *this );
			}
		public:
			inline bool operator!=( const rhs_operand_type& rhs ) const
			{
				return !( get_lhs() == rhs);
			}

			inline bool operator<=( const rhs_operand_type& rhs ) const
			{
				return !(get_lhs() > rhs);
			}

			inline bool operator>=( const rhs_operand_type& rhs ) const
			{
				return !(get_lhs() < rhs);
			}
		};
	}//namespace detail

}//namespace memory_mgr

