/* 
Generic STL (genericstl)
http://genericstl.sourceforge.net/
Copyright (c) 2007, 2008 Anton (shikin) Matosov

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


Please feel free to contact me via e-mail: shikin at users.sourceforge.net
*/

#ifndef GSTL_OPERATIONS_TRACER_HEADER
#define GSTL_OPERATIONS_TRACER_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <gstl/algorithm>

namespace gstl
{
	namespace test
	{
		template<class T>
		class operations_tracer
		{
		private:
			typedef T					value_type;
			typedef operations_tracer	self_type;
			

			value_type	value_;				// value to be sorted
			int			generation_;		// generation of this tracer
			static long created_;		// number of constructor calls
			static long destroyed_;		// number of destructor calls
			static long assigned_;		// number of assignments
			static long le_compared_;	// number of less comparisons
			static long eq_compared_;	// number of equal comparisons
			static long max_live_;		// maximum of existing objects

			static long throw_ctor_;		// when becomes zero, an exception will be thrown
			static long throw_assign_;		// when becomes zero, an exception will be thrown

			// recompute maximum of existing objects
			static void update_max_live()
			{
				max_live_ = gstl::max( max_live_, created_ - destroyed_ );				
			}

			static void check_throw( long& value )
			{
				if( !--value )
				{
					throw test_exception();
				}
			}

		public:
			class test_exception
			{};

			//statistics methods
			static long creations()
			{ 
				return created_; 
			}

			static long destructions()
			{ 
				return destroyed_; 
			}

			static long assignments()
			{ 
				return assigned_; 
			}

			static long le_comparisons()
			{ 
				return le_compared_; 
			}

			static long eq_comparisons()
			{ 
				return eq_compared_; 
			}

			static long max_live()
			{ 
				return max_live_; 
			}

			static void clear()
			{
				created_ = 0;
				destroyed_ = 0;
				assigned_ = 0;
				le_compared_ = 0;
				max_live_ = 0;
				eq_compared_ = 0;

				//throw counters should be set to -1 to prevent throwing
				throw_ctor_ = -1;
				throw_assign_ = -1;
			}

			//set throw counters method
			static void set_throw_ctor( long val )
			{
				throw_ctor_ = val;
			}

			static void set_throw_assign( long val )
			{
				throw_assign_ = val;
			}

			// constructor
			operations_tracer( const value_type& v = 0 )
				: value_( v ),
				generation_(1)
			{
				++created_;
				update_max_live();
				check_throw( throw_ctor_ );
			}

			// copy constructor
			operations_tracer( self_type const& b ) 
				: value_( b.value_ ),
				generation_( b.generation_ + 1 )
			{
				++created_;
				update_max_live();
				check_throw( throw_ctor_ );
			}

			// destructor
			~operations_tracer()
			{
				++destroyed_;
				update_max_live();
			}

			// assignment
			self_type& operator=( self_type const& rhs )
			{
				++assigned_;
				value_ = rhs.value_;
				check_throw( throw_assign_ );
				return *this;
			}

			// less then comparison
			friend bool operator<( self_type const& lhs, self_type const& rhs )
			{
					++le_compared_;
					return lhs.value_ < rhs.value_;
			}

			// equality comparison
			friend bool operator==( self_type const& lhs, self_type const& rhs )
			{
					++eq_compared_;
					return lhs.value_ == rhs.value_;
			}

			int value() const
			{ 
				return value_;
			}
		};

		template<class T>
		long operations_tracer<T>::created_ = 0;

		template<class T>
		long operations_tracer<T>::destroyed_ = 0;

		template<class T>
		long operations_tracer<T>::assigned_ = 0;

		template<class T>
		long operations_tracer<T>::le_compared_ = 0;

		template<class T>
		long operations_tracer<T>::eq_compared_ = 0;

		template<class T>
		long operations_tracer<T>::max_live_ = 0;

		template<class T>
		long operations_tracer<T>::throw_ctor_ = -1;

		template<class T>
		long operations_tracer<T>::throw_assign_ = -1;
	}
}

#endif //GSTL_OPERATIONS_TRACER_HEADER