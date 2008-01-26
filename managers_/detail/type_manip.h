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

#ifndef MGR_TYPE_MANIP_HEADER
#define MGR_TYPE_MANIP_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif



namespace memory_mgr
{
	namespace type_manip
	{
		////////////////////////////////////////////////////////////////////////////////
		// class template int2type
		// Converts each integral constant into a unique type, used for functions overriding
		// based on integer values
		// Invocation: int2type<v> where v is a compile-time constant integral
		// Defines 'value', an enum that evaluates to v
		////////////////////////////////////////////////////////////////////////////////
		template <int v>
		struct int2type
		{
			enum { value = v };
		};

		////////////////////////////////////////////////////////////////////////////////
		// class template type2type
		// Converts each type into a unique, insipid type
		// Invocation type2type<T> where T is a type
		// Defines the type original_type which maps back to T
		////////////////////////////////////////////////////////////////////////////////
		template <typename T>
		struct type2type
		{   
			typedef T original_type;
			type2type(){} // VC7
		};

		////////////////////////////////////////////////////////////////////////////////
		// class template select
		// Selects one of two types based upon a boolean constant
		// Invocation: select<flag, T, U>::Result
		// where:
		// flag is a compile-time boolean constant
		// T and U are types
		// Result evaluates to T if flag is true, and to U otherwise.
		////////////////////////////////////////////////////////////////////////////////
		template <bool flag, typename T, typename U>
		struct select
		{
		private:
			template<bool>
			struct in 
			{ typedef T result; };

			template<>
			struct in<false>
			{ typedef U result; };

		public:
			typedef typename in<flag>::result result;
		};


		////////////////////////////////////////////////////////////////////////////////
		// class template is_same_type
		// Return true if two given types are the same
		// Invocation: is_same_type<T, U>::value
		// where:
		// T and U are types
		// Result evaluates to true if U == T (types equal)
		////////////////////////////////////////////////////////////////////////////////
		template <typename T, typename U>
		struct is_same_type
		{
		private:
			template<typename>
			struct in 
			{ enum { value = false }; };

			template<>
			struct in<T>
			{ enum { value = true };  };

		public:
			enum { value = in<U>::value };
		};

		////////////////////////////////////////////////////////////////////////////////
		// Helper types small and big - guarantee that sizeof(small) < sizeof(big)
		////////////////////////////////////////////////////////////////////////////////
		namespace detail
		{
			typedef char small;
			class big { char dummy[2]; };

			template<typename T>
			struct is_void
			{
				enum { result = 
					is_same_type<T, void>::value          ||
					is_same_type<T, const void>::value    ||
					is_same_type<T, volatile void>::value ||
					is_same_type<T, const volatile void>::value
				};
			};
		}

		//
		// is one type convertable to another?
		//
		template <class T, class U>
		class is_convertible
		{
			struct void_replace{};

			typedef typename select
				<
				detail::is_void<T>::result,
				void_replace, T
				>
				::result T1;

			typedef typename select
				<
				detail::is_void<U>::result,
				void_replace, U
				>
				::result U1;

			static detail::big   test(...);
			static detail::small test(U1);
			static T1 MakeT();

		public:       
			enum { exists = sizeof(test(MakeT())) == sizeof(detail::small) };
		};

		////////////////////////////////////////////////////////////////////////////////
		// class template conversion
		// Figures out the conversion relationships between two types
		// Invocations (T and U are types):
		// a) conversion<T, U>::exists
		// returns (at compile time) true if there is an implicit conversion from T
		// to U (example: Derived to Base)
		// b) conversion<T, U>::exists2Way
		// returns (at compile time) true if there are both conversions from T
		// to U and from U to T (example: int to char and back)
		// c) conversion<T, U>::same_type
		// returns (at compile time) true if T and U represent the same type
		//
		// Caveat: might not work if T and U are in a private inheritance hierarchy.
		////////////////////////////////////////////////////////////////////////////////
		template <class T, class U>
		struct conversion
		{
			enum { exists = (is_convertible<T,U>::exists) };
			enum { exists2Way = (exists && is_convertible<U, T>::exists) };
			enum { same_type = (is_same_type<T, U>::value) };
		};

		////////////////////////////////////////////////////////////////////////////////
		// class template super_subclass
		// Invocation: super_subclass<B, D>::value where B and D are types. 
		// Returns true if B is a public base of D, or if B and D are aliases of the 
		// same type.
		//
		// Caveat: might not work if T and U are in a private inheritance hierarchy.
		////////////////////////////////////////////////////////////////////////////////
		template <class B, class D>
		struct super_subclass
		{
			enum { value = (conversion<const volatile D*, const volatile B*>::exists &&
				!conversion<const volatile B*, const volatile void*>::same_type) };
		};

		////////////////////////////////////////////////////////////////////////////////
		// class template super_subclass_strict
		// Invocation: super_subclass_strict<B, D>::value where B and D are types. 
		// Returns true if B is a public base of D.
		//
		// Caveat: might not work if T and U are in a private inheritance hierarchy.
		////////////////////////////////////////////////////////////////////////////////
		template<class B,class D>
		struct super_subclass_strict
		{
			enum { value = (conversion<const volatile D*, const volatile B*>::exists &&
				!conversion<const volatile B*, const volatile void*>::same_type &&
				!conversion<const volatile B*, const volatile U*>::same_type) };
		};



	}
}

#endif //MGR_TYPE_MANIP_HEADER
