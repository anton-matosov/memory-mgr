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

#include "stdafx.h"
#include <memory-mgr/detail/math.h>
#include <boost/type_traits/make_unsigned.hpp>


#include <sstream>

#pragma warning(push)
#pragma warning(disable:4996)
#include <boost/archive/impl/archive_serializer_map.ipp>
#include <boost/archive/polymorphic_binary_iarchive.hpp>
#include <boost/archive/polymorphic_binary_oarchive.hpp>
#include <boost/archive/polymorphic_oarchive.hpp>
// #include <boost/archive/xml_iarchive.hpp>
// #include <boost/archive/xml_oarchive.hpp>
// #include <boost/archive/xml_wiarchive.hpp>
// #include <boost/archive/xml_woarchive.hpp>
#include <boost/serialization/array.hpp>
#include <boost/serialization/export.hpp>
#pragma warning(pop)

#include <gstl\string>

typedef sing_heap_sz_mgr MemMgr;

typedef memory_mgr::allocator<int, MemMgr> void_allocator;
typedef gstl::basic_string<wchar_t, gstl::char_traits<wchar_t>, void_allocator> string_t;

#define _SERIALIZATION_NVP
//#define _SERIALIZATION_NVP BOOST_SERIALIZATION_NVP

namespace boost
{
	namespace serialization
	{           
		template<class Archive, class CharT, class Traits, class Alloc>
		inline void save(
			Archive& ar,
			const gstl::basic_string<CharT, Alloc, Traits>& str,
			const unsigned int version
			)
		{
 			std::basic_string<CharT> chars( str.c_str(), str.length() );
 			ar & _SERIALIZATION_NVP( chars );

		//	size_t length = str.length();
		//	ar & /*_SERIALIZATION_NVP*/( length );
			//'boost::archive::polymorphic_oarchive_impl::save' : cannot access
			//private member declared in class 'boost::archive::polymorphic_oarchive_impl'
		//	ar.save /*_SERIALIZATION_NVP*/( str.c_str() ) ;
// 			size_t length = str.length();
// 			boost::serialization::array<const CharT> chars( str.c_str(), length );
// 			ar & /*_SERIALIZATION_NVP*/( length );
// 			ar & /*_SERIALIZATION_NVP*/( chars );
		}

		template<class Archive, class CharT, class Traits, class Alloc>
		inline void load(
			Archive& ar,
			gstl::basic_string<CharT, Alloc, Traits>& str,
			const unsigned int version
			)
		{
			std::basic_string<CharT> chars;
			ar & _SERIALIZATION_NVP( chars );
			str.assign( chars.c_str(), chars.length() );


// 			size_t length = 0;
// 			ar & /*_SERIALIZATION_NVP*/( length );
// 			str.resize( length );
// 
// 			boost::serialization::array<CharT> chars( const_cast<CharT*>( str.c_str() ), length );
// 			ar & /*_SERIALIZATION_NVP*/( chars );
		}

		// split non-intrusive serialization function member into separate
		// non intrusive save/load member functions
		template<class Archive, class CharT, class Traits, class Alloc>
		inline void serialize(
			Archive & ar,
			gstl::basic_string<CharT, Alloc, Traits> & str,
			const unsigned int version
			)
		{
			boost::serialization::split_free(ar, str, version);
		}

		template <class CharT, class Traits, class Alloc>                                     
		struct implementation_level_impl< const gstl::basic_string<CharT, Alloc, Traits> >  
		{
			typedef mpl::integral_c_tag tag;
			typedef mpl::int_< boost::serialization::object_serializable > type;
			BOOST_STATIC_CONSTANT(
				int,
				value = implementation_level_impl::type::value
				);
		};

 		template <class CharT, class Traits, class Alloc> 
 		struct tracking_level< const gstl::basic_string<CharT, Alloc, Traits> >  
 		{
 			typedef mpl::integral_c_tag tag;
 			typedef mpl::int_<boost::serialization::track_never> type;
 			BOOST_STATIC_CONSTANT(
 				int,
 				value = tracking_level::type::value
 			);
 		};

	} // serialization
} // namespace boost

template<class T>
void BinarySave( std::ostream& out, T val )
{
	out.write( reinterpret_cast<const char*>( &val ), sizeof( val ) );
}

template<class T>
void BinaryLoad( std::istream& in, T& val )
{
	in.read( reinterpret_cast<char*>( &val ), sizeof( val ) );
}

template<class StringT>
void BinarySaveString( std::ostream& out, const StringT& str )
{
	BinarySave( out, str.length() );
	if( str.length() )
	{
		out.write( reinterpret_cast<const char*>( str.data() ), 
			static_cast<int>( str.length() * sizeof( wchar_t ) ) );
	}
}

template<class StringT>
void BinaryLoadString( std::istream& in, StringT& str )
{
	size_t length = 0;
	BinaryLoad( in, length );
	str.resize( length );
	if( length )
	{
		in.read( reinterpret_cast<char*>( const_cast<wchar_t*>( str.data() ) ),
			static_cast<int>( length * sizeof( wchar_t ) ) );
	}
}

class SerializableClass;
class SerializableBase
{
public:
	virtual void Load( boost::archive::polymorphic_iarchive& in ) = 0;
	virtual void Save( boost::archive::polymorphic_oarchive& out ) = 0;

	SerializableBase()
		:i_(0xFFEEFFEE)
	{

	}
	virtual ~SerializableBase()
	{

	}

	int i_;
// 	template<class Archive>
// 	void serialize( Archive & ar, const int version )
// 	{
// 		ar.register_type(static_cast<SerializableClass*>(NULL));
// 	}
};
//BOOST_CLASS_TRACKING( SerializableBase, boost::serialization::track_always );
//BOOST_CLASS_EXPORT( SerializableBase );

enum EOutConstruct { OutConstruct };
enum EInConstruct { InConstruct };
class SerializableClass
	:public SerializableBase
{
public:
	SerializableClass( EOutConstruct = OutConstruct )
		:string1_( L"111" )
		,string2_( L"test string 2" )
		,string3_( L"test string 3" )
		,string4_( L"test string 4" )
		,string5_( L"test string 5" )
	{

	}

	SerializableClass(EInConstruct)
	{

	}

	virtual void Load( boost::archive::polymorphic_iarchive& in )
	{
		in >> *this;
	}

	virtual void Save( boost::archive::polymorphic_oarchive& out )
	{
		out << *this;
	}

	template<class Archive>
	void serialize( Archive & ar, const int version )
	{
		boost::serialization::void_cast_register<SerializableClass, SerializableBase>();
		//ar & boost::serialization::base_object<SerializableBase>(*this);
		//ar.register_type(static_cast<SerializableClass*>(NULL));

		ar & _SERIALIZATION_NVP( i_ );

// 		if( version >= 2 )
// 		{
// 			//assert( "version" );
//  			ar & _SERIALIZATION_NVP( string2_ );
//  			ar & _SERIALIZATION_NVP( string3_ );
//  			ar & _SERIALIZATION_NVP( string4_ );
//  			ar & _SERIALIZATION_NVP( string5_ );
// 		}
	}

	friend std::ostream& operator<<( std::ostream& out, SerializableClass& rhs )
	{
		BinarySaveString( out, rhs.string1_ );
		BinarySaveString( out, rhs.string2_ );
		BinarySaveString( out, rhs.string3_ );
		BinarySaveString( out, rhs.string4_ );
		BinarySaveString( out, rhs.string5_ );
		return out;
	}

	friend std::istream& operator>>( std::istream& in, SerializableClass& rhs )
	{
		BinaryLoadString( in, rhs.string1_ );
		BinaryLoadString( in, rhs.string2_ );
		BinaryLoadString( in, rhs.string3_ );
		BinaryLoadString( in, rhs.string4_ );
		BinaryLoadString( in, rhs.string5_ );
		return in;
	}
private:
	string_t string1_;
	string_t string2_;
	string_t string3_;
	string_t string4_;
	string_t string5_;
};

BOOST_CLASS_EXPORT( SerializableClass );
//BOOST_CLASS_EXPORT_IMPLEMENT( SerializableClass );

BOOST_CLASS_VERSION( SerializableClass, 0xBB );
//BOOST_CLASS_IMPLEMENTATION( SerializableClass, boost::serialization::object_serializable );
BOOST_CLASS_TRACKING( SerializableClass, boost::serialization::track_never );

class SerializableClass2
	:public SerializableBase
{
public:
	SerializableClass2( EOutConstruct = OutConstruct )
		:string1_( L"222" )
	{
		i_ = 0xBBAABBAA;
	}

	virtual void Load( boost::archive::polymorphic_iarchive& in )
	{

	}

	virtual void Save( boost::archive::polymorphic_oarchive& out )
	{

	}

	template<class Archive>
	void serialize( Archive & ar, const int version )
	{
		boost::serialization::void_cast_register<SerializableClass2, SerializableBase>();

		ar & _SERIALIZATION_NVP( i_ );
	}
	string_t string1_;
};

BOOST_CLASS_EXPORT( SerializableClass2 );
BOOST_CLASS_VERSION( SerializableClass2, 0xEE );
BOOST_CLASS_TRACKING( SerializableClass2, boost::serialization::track_never );

namespace
{
	struct stub
	{
		static bool  do_work_;
		static void work( int ii )
		{
			if( do_work_ )
			{
				std::vector<char> s( ii && 0 + 1 );
			}

		}
	};
	bool stub::do_work_ = false;

	template<class TestType>
	long double test_lowest_bit( const int op_repeat, const int per_alloc )
	{
		typedef	TestType test_type;

		enum
		{
			numBytes = sizeof( test_type ),
			bitsInByte = 8,
			numBits = numBytes * bitsInByte
		};
		for( int i = 0; i < numBits; ++i )
		{
			test_type var = test_type(1) << i;
			int ii = 0;
			TEST_START_LOOP( op_repeat, per_alloc, char );
			{
				ii = memory_mgr::math::lowest_bit2<boost::make_unsigned<test_type>::type >( var );
			
				//This is required because otherwise the call will be removed by the optimizer	
				stub::work( ii );
			}
			TEST_END_LOOP( std::wcout );

		}
		return 0;//TEST_ELAPCED_MCS;

		//memory_mgr::math::lowest_bit2( var );

	}

	std::string testStdString = "test string";
	//	std::wstring testWString = L"test string";
	string_t testString = L"test string";
	// 	gstl::wstring testWString = L"test string";

	long double test_out_stream( const int op_repeat, const int per_alloc )
	{
		std::ostringstream outStream( std::ios_base::out | std::ios_base::binary );

		TEST_START_LOOP( op_repeat, per_alloc, char );
		{
			BinarySaveString( outStream, testString );
		}
		TEST_END_LOOP( std::wcout );

		return TEST_ELAPCED_MCS;
	}

	long double test_in_stream( const int op_repeat, const int per_alloc )
	{
		std::ostringstream outStream( std::ios_base::out | std::ios_base::binary );
		{
			TEST_START_LOOP( op_repeat, per_alloc, char );
			{
				BinarySaveString( outStream, testString );
			}
			TEST_END_LOOP_NO_PRINT;
		}

		std::istringstream inStream( outStream.str(), std::ios_base::out | std::ios_base::binary );

		TEST_START_LOOP( op_repeat, per_alloc, char );
		{
			string_t inString;
			BinaryLoadString( inStream, inString );
		}
		TEST_END_LOOP( std::wcout );

		return TEST_ELAPCED_MCS;
	}

	//////////////////////////////////////////////////////////////////////////
	long double test_out_OBJECT_stream( const int op_repeat, const int per_alloc )
	{
		SerializableClass outObject;
		std::ostringstream outStream( std::ios_base::out | std::ios_base::binary );

		TEST_START_LOOP( op_repeat, per_alloc, char );
		{
			//outStream << testString;
			outStream << outObject;
			//outStream.rdbuf()->sputn( testString.c_str(), testString.size() );
		}
		TEST_END_LOOP( std::wcout );

		return TEST_ELAPCED_MCS;
	}

	long double test_in_OBJECT_stream( const int op_repeat, const int per_alloc )
	{
		SerializableClass outObject;
		std::ostringstream outStream( std::ios_base::out | std::ios_base::binary );
		{
			TEST_START_LOOP( op_repeat, per_alloc, char );
			{
				outStream << outObject;
			}
			TEST_END_LOOP_NO_PRINT;
		}


		std::istringstream inStream( outStream.str(), std::ios_base::out | std::ios_base::binary );

		TEST_START_LOOP( op_repeat, per_alloc, char );
		{
			SerializableClass inObject( InConstruct );
			inStream >> inObject;
		}
		TEST_END_LOOP( std::wcout );

		return TEST_ELAPCED_MCS;
	}
	//////////////////////////////////////////////////////////////////////////
	
	long double test_out_polymorphic_archive( const int op_repeat, const int per_alloc )
	{
		std::ostringstream outStream( std::ios_base::out | std::ios_base::binary );
		boost::archive::polymorphic_binary_oarchive outArchive( outStream );
		boost::archive::polymorphic_oarchive& out( outArchive );
		//boost::archive::binary_oarchive outArchive( outStream );
		//boost::archive::xml_woarchive outArchive( outStream );

		TEST_START_LOOP( op_repeat, per_alloc, char );
		{
			//outArchive << testString;
			out & /*_SERIALIZATION_NVP*/( testString );
			
		}
		TEST_END_LOOP( std::wcout );
		//std::cout << outStream.str();

		return TEST_ELAPCED_MCS;
		return 0;
	}

	long double test_in_polymorphic_archive( const int op_repeat, const int per_alloc )
	{
		std::ostringstream outStream( std::ios_base::out | std::ios_base::binary );
		boost::archive::polymorphic_binary_oarchive outArchive( outStream );
		
		//boost::archive::xml_oarchive outArchive( outStream );

		{
			TEST_START_LOOP( op_repeat, per_alloc, char );
			{
				outArchive & testString;
			}
			TEST_END_LOOP_NO_PRINT;
		}

		std::istringstream inStream( outStream.str(), std::ios_base::in | std::ios_base::binary );
		boost::archive::polymorphic_binary_iarchive inArchive( inStream );
		boost::archive::polymorphic_iarchive& in( inArchive );
		//boost::archive::xml_iarchive inArchive( inStream );

		TEST_START_LOOP( op_repeat, per_alloc, char );
		{
			string_t inString;
			in & inString;
		}
		TEST_END_LOOP( std::wcout );

		return TEST_ELAPCED_MCS;
	}

	//////////////////////////////////////////////////////////////////////////
	long double test_out_OBJECT_polymorphic_archive( const int op_repeat, const int per_alloc )
	{
		SerializableBase* outObject = new SerializableClass();
		SerializableBase* outObject2 = new SerializableClass2();

		std::ostringstream outStream( std::ios_base::out | std::ios_base::binary );
		size_t size = 10 * 1024*1024;
		char* buffer = new char[ size ];
		outStream.rdbuf()->pubsetbuf( buffer, size );

		boost::archive::polymorphic_binary_oarchive outArchive( outStream );
		boost::archive::polymorphic_oarchive& out( outArchive );
		//boost::archive::binary_oarchive outArchive( outStream );
		//boost::archive::xml_woarchive outArchive( outStream );

		TEST_START_LOOP( op_repeat, per_alloc, char );
		{
			//outArchive << testString;
			//outObject->Save( out );

			out & outObject;
			out & outObject2;
		}
		TEST_END_LOOP( std::wcout );
		//std::cout << outStream.str();

		return TEST_ELAPCED_MCS;
		return 0;
	}

	long double test_in_OBJECT_polymorphic_archive( const int op_repeat, const int per_alloc )
	{
		std::ostringstream outStream( std::ios_base::out | std::ios_base::binary );
		boost::archive::polymorphic_binary_oarchive outArchive( outStream );

		{
// 			SerializableBase* outObject = new SerializableClass();
// 			SerializableBase* outObject2 = new SerializableClass2();
			SerializableClass outObject;// = new SerializableClass();
			SerializableClass2 outObject2;// = new SerializableClass2();
			TEST_START_LOOP( op_repeat, per_alloc, char );
			{
				//outObject->Save( outArchive );
				outArchive & outObject;
				outArchive & outObject2;
			}
			TEST_END_LOOP_NO_PRINT;
		}

		std::istringstream inStream( outStream.str(), std::ios_base::in | std::ios_base::binary );
		boost::archive::polymorphic_binary_iarchive inArchive( inStream );
		boost::archive::polymorphic_iarchive& in( inArchive );
		//boost::archive::xml_iarchive inArchive( inStream );

		//SerializableBase* outObject = new SerializableClass();
		TEST_START_LOOP( op_repeat, per_alloc, char );
		{
			SerializableClass inObject( InConstruct );
			SerializableClass2 inObject2;

			in & inObject2;
			in & inObject;
			//inObject.Load( in );
		}
		TEST_END_LOOP( std::wcout );

		return TEST_ELAPCED_MCS;
	}

	long double test_in_OBJECT_base_polymorphic_archive( const int op_repeat, const int per_alloc )
	{
		std::ostringstream outStream( std::ios_base::out | std::ios_base::binary );
		boost::archive::polymorphic_binary_oarchive outArchive( outStream );

		{
			SerializableBase* outObject = new SerializableClass();
			TEST_START_LOOP( op_repeat, per_alloc, char );
			{
				outArchive << outObject;
			}
			TEST_END_LOOP_NO_PRINT;
		}

		std::istringstream inStream( outStream.str(), std::ios_base::in | std::ios_base::binary );
		boost::archive::polymorphic_binary_iarchive inArchive( inStream );
		boost::archive::polymorphic_iarchive& in( inArchive );
		//boost::archive::xml_iarchive inArchive( inStream );

		SerializableBase* inObject = 0;//new SerializableClass();
		TEST_START_LOOP( op_repeat, per_alloc, char );
		{
			in >> inObject;
		}
		TEST_END_LOOP( std::wcout );

		return TEST_ELAPCED_MCS;
	}
	//////////////////////////////////////////////////////////////////////////
/*
	long double test_out_binary_archive( const int op_repeat, const int per_alloc )
	{
		std::ostringstream outStream( std::ios_base::out | std::ios_base::binary );
		boost::archive::binary_oarchive outArchive( outStream );
		//boost::archive::xml_oarchive outArchive( outStream );

		TEST_START_LOOP( op_repeat, per_alloc, char );
		{
			outArchive & testString;
		}
		TEST_END_LOOP( std::wcout );
		return TEST_ELAPCED_MCS;
	}

	long double test_in_binary_archive( const int op_repeat, const int per_alloc )
	{
		std::ostringstream outStream( std::ios_base::out | std::ios_base::binary );
		boost::archive::binary_oarchive outArchive( outStream );
		//boost::archive::xml_oarchive outArchive( outStream );

		{
			TEST_START_LOOP( op_repeat, per_alloc, char );
			{
				outArchive & testString;
			}
			TEST_END_LOOP_NO_PRINT;
		}

		std::istringstream inStream( outStream.str(), std::ios_base::in | std::ios_base::binary );
		boost::archive::binary_iarchive inArchive( inStream );
		//boost::archive::xml_iarchive inArchive( inStream );

		TEST_START_LOOP( op_repeat, per_alloc, char );
		{
			string_t inString;
			inArchive & inString;
		}
		TEST_END_LOOP( std::wcout );
		
		return TEST_ELAPCED_MCS;
	}
*/
	const char* lowest_bit_category = "lowest_bit";

	const char* serialization_category = "serialization";
	void run_all_tests( const int op_repeat, const int per_alloc, const int test_repeat )
	{
		// 		run_perf_test( lowest_bit_category, "lowest_bit (log2)",
		// 			test_lowest_bit<unsigned long>, op_repeat, per_alloc, test_repeat );

// 		run_perf_test( serialization_category, "test gstl::string OUT stream",
// 			test_out_stream, op_repeat, per_alloc, test_repeat );
// 
// 		run_perf_test( serialization_category, "test gstl::string IN stream",
// 			test_in_stream, op_repeat, per_alloc, test_repeat );

//  		run_perf_test( serialization_category, "test SerializableClass OUT stream",
//  			test_out_OBJECT_stream, op_repeat, per_alloc, test_repeat );
//  
//  		run_perf_test( serialization_category, "test SerializableClass IN stream",
//  			test_in_OBJECT_stream, op_repeat, per_alloc, test_repeat );

// 		run_perf_test( serialization_category, "test gstl::string OUT polymorphic_archive",
// 			test_out_polymorphic_archive, op_repeat, per_alloc, test_repeat );
// 		run_perf_test( serialization_category, "test gstl::string IN polymorphic_archive",
// 			test_in_polymorphic_archive, op_repeat, per_alloc, test_repeat );


// 		run_perf_test( serialization_category, "test SerializableClass OUT polymorphic_archive",
// 			test_out_OBJECT_polymorphic_archive, op_repeat, per_alloc, test_repeat );
		run_perf_test( serialization_category, "test SerializableClass IN polymorphic_archive",
			test_in_OBJECT_polymorphic_archive, op_repeat, per_alloc, test_repeat );

// 		run_perf_test( serialization_category, "test SerializableClass IN via BASE polymorphic_archive",
// 			test_in_OBJECT_base_polymorphic_archive, op_repeat, per_alloc, test_repeat );

// 		run_perf_test( serialization_category, "test gstl::string  OUT binary_archive",
// 			test_out_binary_archive, op_repeat, per_alloc, test_repeat );
// 
// 		run_perf_test( serialization_category, "test gstl::string IN binary_archive",
// 			test_in_binary_archive, op_repeat, per_alloc, test_repeat );
	}

}


bool test_math( const int op_repeat, const int per_alloc, const int test_repeat )
{	
	print_perf_test_header<memmgr_type>( L"Testing math library",
		0, 0, 0 );
	run_all_tests( op_repeat, per_alloc, test_repeat );

	return true;
}