// BitManager.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <boost/dynamic_bitset.hpp>
#include <boost/pool/pool.hpp>
#include <vector>
#include "static_bitset.h"

#include "memory_manager.h"
#include "bit_manager.h"
#include "../PerfTimer.h"



template class managers::memory_manager<size_t, 1024, 4>;

typedef managers::static_bitset<unsigned char/*_int64*/, 80, managers::CustomArray> bitset_t;

_int64 FileTimeToQuadWord(PFILETIME pft)
{
	return(Int64ShllMod32(pft->dwHighDateTime, 32) | pft->dwLowDateTime);
}

void PerformLongOperation( bitset_t& bitset, const size_t first, const size_t count )
{
	FILETIME ftKernelTimeStart, ftKernelTimeEnd; 
	FILETIME ftUserTimeStart, ftUserTimeEnd; 
	FILETIME ftDummy;

	_int64 qwKernelTimeElapsed, qwUserTimeElapsed, qwTotalTimeElapsed;

	// получаем начальные показатели времени 
	GetThreadTimes(GetCurrentThread(), &ftDummy, &ftDummy,
		&ftKernelTimeStart, &ftUserTimeStart);

	const int tests_count = 10000;

	for ( int ti = 0; ti < tests_count; ++ti )
	{
		bitset.set( first, count );
	}	

	// получаем конечные показатели времени
	GetThreadTimes(GetCurrentThread(), &ftDummy, &ftDummy,
		&ftKernelTimeEnd, &ftUserTimeEnd);

	// получаем значении времени, затраченного на выполнение ядра и User, 
	// преобразуя начальные и конечные показатели времени из FILETIME 
	// в учетверенные слова, а затем вычитая начальные показатели из конечных 
	qwKernelTimeElapsed = FileTimeToQuadWord(&ftKernelTimeEnd) -
		FileTimeToQuadWord(&ftKernelTimeStart);

	qwUserTimeElapsed = FileTimeToQuadWord(&ftUserTimeEnd) -
		FileTimeToQuadWord(&ftUserTimeStart);

	// получаем общее время, складывая время выполнения ядра и User 
	qwTotalTimeElapsed = qwKernelTimeElapsed + qwUserTimeElapsed;

	// общее время хранится в qwTotalTimeElapsed

}


void PerfTest( bitset_t& bitset, const size_t first, const size_t count )
{
	CPerfTimer timer;

	double myset = 0.;
	double defset = 0.;
	const size_t end = first + count;
	const int tests_count = 10000;

	
	timer.Start( true );	
	bitset.set( first, count );	
	timer.Stop();
	myset += timer.Elapsedus();

	timer.Start( true );
	for ( size_t i = first; i < end; ++i )
	{
		//bitset.set( i );
	}
	
	timer.Stop();
	defset += timer.Elapsedus();
	
	//myset /= tests_count;
	//defset /= tests_count;

	std::cout << "set(" << first << ", " << count << "), time = " <<  myset << "\n";
	std::cout << "set(i) from " << first << " to " << count << ", time = " <<  defset << "\n";

	std::ostream_iterator<char> ostr_beg = std::cout;
	std::fill_n( ostr_beg, 40, '-' );
	std::cout << '\n';
}

bool allocate( bitset_t& bitset, const size_t bit_count )
{
	std::cout << "Allocating " << bit_count << " bits\n";
	bitset_t::size_type pos = bitset.find_n( bit_count );
	if( pos != bitset_t::npos )
	{		
		std::cout << "(" << pos << ", " << pos + bit_count << ")\n";
		std::cout << "old bitset = " << bitset << "\n";
		bitset.reset( pos, bit_count );
		std::cout << "new bitset = " << bitset << "\n\n";
		return true;
	}
	else
	{
		std::cout << "throw bad_alloc\n\n";
	}
	return false;
}

int _tmain(int /*argc*/, _TCHAR* /*argv*/[])
{
	//typedef boost::pool<> pool_t;
	typedef unsigned char uchar;



	std::vector<unsigned char> vec(1000);
	bitset_t bitset( &*vec.begin() );

	managers::bit_manager<unsigned char, 80> mgr( &*vec.begin() );
//	bitset.set();

//	bitset_t::block_type bm1 = bitset.bit_mask( 5, 10 );
//	bitset_t::block_type bm2 = bitset.lower_bit_mask( 5 );
//	bitset_t::block_type bm3 = bitset.higher_bit_mask( 5 );

//	PerformLongOperation( bitset, 5, 1 );

	bitset.set();

	PerfTest( bitset, 0, 1 );
 	PerfTest( bitset, 2, 2 );
// 	bitset.reset();
// 
 	PerfTest( bitset, 5, 3);
// 	bitset.reset();
// 
 	PerfTest( bitset, 9, 4 );
// 	bitset.reset();
// 
 	//PerfTest( bitset, 15, 5 );
// 	bitset.reset();
// 
 	PerfTest( bitset, 23, 20 );
// 	bitset.reset();

	PerfTest( bitset, 38, 11 );
// 	bitset.reset();
// 
// 	PerfTest( bitset, 5, 20 );
// 	bitset.reset();
// 
// 	PerfTest( bitset, 5, 30 );
// 	bitset.reset();
// 
// 	PerfTest( bitset, 5, 40 );
	//bitset.reset();

// 	PerfTest( bitset, 5, 100 );
// 	bitset.reset();
// 
// 	PerfTest( bitset, 5, 300 );
// 	bitset.reset();
// 
// 	PerfTest( bitset, 5, 800 );
// 	bitset.reset();
// 
// 	PerfTest( bitset, 5, 1500 );

	std::cout << "bitset = " << bitset << "\n";

 	size_t ind = bitset.find_first();
 	ind = bitset.find_next( ind );
 	ind = bitset.find_next( ind );
 	ind = bitset.find_next( ind );
 	ind = bitset.find_next( ind );
 	ind = bitset.find_next( ind );
 	ind = bitset.find_next( ind );
 	ind = bitset.find_next( ind );
 	ind = bitset.find_next( ind );
 	ind = bitset.find_next( ind );
 	ind = bitset.find_next( ind );
 	ind = bitset.find_next( ind );
 	ind = bitset.find_next( ind );

 	size_t bit4 = mgr.allocate( 4 );
	std::cout << "mgr = " << mgr << "\n";
 	size_t bit1 = mgr.allocate( 1 );
	size_t bit3 = mgr.allocate( 3 );
 	size_t bit26 = mgr.allocate( 26 );
 	size_t bit7 = mgr.allocate( 7 );
 	size_t bit5 = mgr.allocate( 5 ); 	

	mgr.deallocate( bit3, 3 );
	mgr.deallocate( bit5, 5 );

 	size_t bit2 = mgr.allocate( 2 );
 	size_t bit3_2 = mgr.allocate( 3 );

	mgr.deallocate( bit4, 4 );
 	size_t bit4_2= mgr.allocate( 4 );
 	size_t bit300 = mgr.allocate( 300 );
// 
// 	PerfTest( bitset, 5, 15000 );

	FILETIME ftCreationTime, ftExitTime, ftKernelTime, ftUserTime;

	GetThreadTimes(GetCurrentThread(),
		&ftCreationTime,
		&ftExitTime,
		&ftKernelTime,
		&ftUserTime);
	//mem.reset( mem.size()/2 );

	//bitset_t submem;
	//submem.set();

 	//size_t first = mem.find_first();
	//mem.flip( first );

	//unsigned char mask = mem.bit_mask( 3, 4 );
// 	if( first != bitset_t::npos )
// 	{
// 		mem[first].flip();
// 	}
// 	size_t next = mem.find_first();

	std::wcout << L"bits_per_block = " << bitset_t::bits_per_block << L"\n";
	std::wcout << L"blocks_count = " << bitset_t::num_blocks << L"\n";
	std::wcout << L"bits_count = " << bitset_t::num_bits << L"\n";
	//std::wcout << L"bits_per_block = " << bitset_t::bits_per_block << L"\n";
	std::cout << "bitset = " << bitset << "\n";
	
	_getch();
	return 0;
}

