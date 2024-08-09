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

#include "memory-mgr/detail/assert.h"

#include <boost/type_traits/make_unsigned.hpp>

#include <limits>
#include <cstdint>

namespace memory_mgr
{
	/**
	@brief Helper classes
	*/
	namespace math
	{
		/**
		@brief Helper classes implementation details
		*/
		namespace detail
		{
			template <typename T>
			static inline int integer_log2_impl(T x, int n)
			{
				int result = 0;
				while( x != 1 )
				{
					const T t = static_cast< const T >( x >> n );
					if( t )
					{
						result += n;
						x = t;
					}
					n /= 2;
				}
				return result;
			}

			//////////////////////////////////////////////////////////////////////////
			template <typename T, T t, T x, int n, T prev_res>
			struct log2_if_t_params
			{
				typedef T T_;
				enum
				{
					t_ = t,
					x_ = x,
					n_ = n,
					prev_res_ = prev_res
				};
			};

			template < class params_t >
			struct log2_if_t
			{
				typedef params_t p;
				enum {result = p::prev_res_ + p::n_};
				enum {new_x = p::t_};
			};

			template <typename T, T x, int n, T prev_res>
			struct log2_if_t< log2_if_t_params<T, 0, x, n, prev_res> >
			{
				enum {result = prev_res};
				enum {new_x = x};
			};

			//////////////////////////////////////////////////////////////////////////
			template <typename T, T x, int n, T prev_res>
			struct int_log2_params
			{
				typedef T T_;
				enum
				{
					x_ = x,
					n_ = n,
					prev_res_ = prev_res
				};

			};

			template < class params_c_t >
			struct int_log2_impl
			{
				typedef params_c_t p;
				enum {t = p::x_ >> p::n_};

				typedef log2_if_t< log2_if_t_params<typename p::T_, t, p::x_, p::n_, p::prev_res_> > u;
				enum{ result = int_log2_impl<
					int_log2_params<typename p::T_, u::new_x, p::n_ / 2, u::result> >::result };

			};


			template <typename T, int n, T prev_res>
			struct int_log2_impl< int_log2_params<T, 1, n, prev_res> >
			{
				enum{ result = prev_res };

			};

			// helper to find the maximum power of two
			// less than p (more involved than necessary,
			// to avoid PTS)
			//
			template <int p, int n>
			struct max_pow2_less
			{
				enum { c = 2*n < p };
				enum { value = c ? (max_pow2_less< c*p, 2*c*n>::value) : n};
			};

			template <>
			struct max_pow2_less<0, 0>
			{
				enum { value = 0};
			};

		} //helpers detail

		// ---------
		// integer_log2
		// ---------------
		//
		template <typename T>
		static inline int integer_log2(T x)
		{
			MGR_ASSERT(x > 0, "Logarithm is defined for values only greater than zero"); // PRE

			const int n = detail::max_pow2_less<std::numeric_limits<T>::digits, 4>::value;

			return detail::integer_log2_impl(x, n);
		}

		template<class T, T x>
		struct int_log2
		{
			static_assert(x > 0, ""); // PRE
			enum { n = detail::max_pow2_less<std::numeric_limits<T>::digits, 4>::value };
			enum { result = detail::int_log2_impl<
				detail::int_log2_params<T, x, n, 0> >::result };
		};


		/**
		   @brief Returns the number of the first bit in the 'x' that is set to 1
		   @details Uses int_log2 to find the bit id
		*/
		template <typename T>
		static inline  int lowest_bit(T x)
		{
			MGR_ASSERT(x >= 1, "prcondition check failed"); // PRE

			using UT = typename boost::make_unsigned<T>::type;

			auto ux = static_cast<UT>(x);

			// clear all bits on except the rightmost one,
			// then calculate the logarithm base 2
			return integer_log2<UT>( ux - ( ux & (ux-1) ) );

		}

		template <typename T>
		static inline  int lowest_bit2(T x)
		{
			MGR_ASSERT(x != 0, "prcondition check failed");

			static const int8_t num_clear_low_bits[] =
					  { -1,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
						4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
						5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
						4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
						6,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
						4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
						5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
						4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
						7,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
						4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
						5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
						4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
						6,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
						4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
						5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
						4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0 };

			enum
			{
				num_bytes = sizeof( T ),
				bits_in_byte = 8
			};
			int result = -1;
			for( int i = 0; i < num_bytes && result < 0; ++i )
			{
				unsigned char val = (x >> (i*bits_in_byte)) & 0xFF;
				if( val )
				{
					result = num_clear_low_bits[val];
					if( result >= 0 )
					{
						result+=(i*(bits_in_byte));
					}

				}

			}

			return result;


		}
	}//math

}//memory_mgr
