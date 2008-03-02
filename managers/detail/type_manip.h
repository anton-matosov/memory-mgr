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
		struct yes_type{};
		struct no_type{};

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
		namespace detail
		{
			template<bool, typename T, typename U>
			struct select_impl 
			{ typedef T result; };

			template<typename T, typename U>
			struct select_impl<false, T, U>
			{ typedef U result; };		
		}

		template <bool flag, typename T, typename U>
		struct select
		{
			typedef typename detail::select_impl<flag, T, U>::result result;
		};


		////////////////////////////////////////////////////////////////////////////////
		// class template is_same_type
		// Return true if two given types are the same
		// Invocation: is_same_type<T, U>::value
		// where:
		// T and U are types
		// Result evaluates to true if U == T (types equal)
		////////////////////////////////////////////////////////////////////////////////
		namespace detail
		{
			template<typename T, typename U>
			struct is_same_type_impl 
			{ enum { value = false }; };

			template<typename U>
			struct is_same_type_impl<U, U>
			{ enum { value = true }; };
		}

		template <typename T, typename U>
		struct is_same_type
		{
			enum { value = detail::is_same_type_impl<T, U>::value };
		};

		////////////////////////////////////////////////////////////////////////////////
		// Helper types small_type and big_type - guarantee that sizeof(small_type) < sizeof(big_type)
		////////////////////////////////////////////////////////////////////////////////
		namespace detail
		{
			typedef char small_type;
			class big_type { char dummy[2]; };

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
		template <class From, class To>
		class is_convertible
		{
			struct void_replace{};

			typedef typename select
				<
				detail::is_void<From>::result,
				void_replace, From
				>
				::result From1;

			typedef typename select
				<
				detail::is_void<To>::result,
				void_replace, To
				>
				::result To1;

			static detail::big_type   test(...);
			static detail::small_type test(To1);
			static From1 MakeFrom();

		public:       
			enum { exists = sizeof(test(MakeFrom())) == sizeof(detail::small_type) };
		};

		//////////////////////////////////////////////////////////////////////////
		// Is type T a class
		//////////////////////////////////////////////////////////////////////////
		template <class T>
		class is_class
		{
			template<class U> static detail::big_type test(...);
			template<class U> static detail::small_type test(void(U::*)(void));			
		public:       
			enum { value = sizeof(test<T>(0)) == sizeof(detail::small_type) };
		};
		

		////////////////////////////////////////////////////////////////////////////////
		// class template conversion
		// Figures out the conversion relationships between two types
		// Invocations (FromType and ToType are types):
		// a) conversion<FromType, ToType>::exists
		// returns (at compile time) true if there is an implicit conversion from FromType
		// to ToType (example: Derived to Base)
		// b) conversion<FromType, ToType>::exists2Way
		// returns (at compile time) true if there are both conversions from FromType
		// to ToType and from ToType to FromType (example: int to char and back)
		// c) conversion<FromType, ToType>::same_type
		// returns (at compile time) true if FromType and ToType represent the same type
		//
		// Caveat: might not work if FromType and ToType are in a private inheritance hierarchy.
		////////////////////////////////////////////////////////////////////////////////
		template <class FromType, class ToType>
		struct conversion
		{
			enum { exists = (is_convertible<FromType,ToType>::exists) };
			enum { exists2Way = (exists && is_convertible<ToType, FromType>::exists) };
			enum { same_type = (is_same_type<FromType, ToType>::value) };
		};

		////////////////////////////////////////////////////////////////////////////////
		// class template super_subclass
		// Invocation: super_subclass<B(ase), D(erived)>::value where B(ase) and D(erived) are types. 
		// Returns true if B(ase) is a public base of D(erived), or if B(ase) and D(erived) are aliases of the 
		// same type.
		//
		// Caveat: might not work if B(ase) and D are in a private inheritance hierarchy.
		////////////////////////////////////////////////////////////////////////////////
		template <class B/*Base*/, class D/*Derived*/>
		struct super_subclass
		{
			enum { value = (conversion<const volatile D*, const volatile B*>::exists &&
				!conversion<const volatile B*, const volatile void*>::same_type) };
		};

		////////////////////////////////////////////////////////////////////////////////
		// class template super_subclass_strict
		// Invocation: super_subclass_strict<B(ase), D(erived)>::value where B(ase) and D(erived) are types. 
		// Returns true if B(ase) is a public base of D(erived).
		//
		// Caveat: might not work if B(ase) and D(erived) are in a private inheritance hierarchy.
		////////////////////////////////////////////////////////////////////////////////
		template <class B/*Base*/, class D/*Derived*/>
		struct super_subclass_strict
		{
			enum { value = (conversion<const volatile D*, const volatile B*>::exists &&
				!conversion<const volatile B*, const volatile void*>::same_type &&
				!conversion<const volatile B*, const volatile D*>::same_type) };
		};



	}
}

#endif //MGR_TYPE_MANIP_HEADER
