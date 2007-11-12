
#include "detail/static_bitset.h"

typedef unsigned char block_type;

static const size_t bits_count = 32;
typedef managers::static_bitset<block_type, bits_count, managers::StaticArray> static_bitset;

bool test_static_bitset()
{
	return true;
}