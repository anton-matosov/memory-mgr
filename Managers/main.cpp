// BitManager.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
// #include <boost/dynamic_bitset.hpp>
// #include <boost/pool/pool.hpp>

#include <vector>
#include "static_bitset.h"

#include "memory_manager.h"
#include "bit_manager.h"
#include "../perf_timer.h"



//template class managers::memory_manager<size_t, 1024, 4>;

typedef managers::static_bitset<unsigned char/*_int64*/, 80, managers::CustomArray> bitset_t;

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
	return 0;
}

