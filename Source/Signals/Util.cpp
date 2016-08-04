#include "Signals.h"
#include "Util.h"

UUtil::UUtil(const class FObjectInitializer & PCIP)
: Super(PCIP)
{
	
}

/*static*/
int UUtil::GetNumberOfSetBits(int value)
{
	unsigned i = unsigned(value);
	i = i - ((i >> 1) & 0x55555555);
	i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
	return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

/*static*/
int UUtil::GetNextSetBitIndex(int start, int bits)
{
	if (bits == 0)
	{
		return -1;
	}

	unsigned value = unsigned(bits);
	for (int i = start + 1; ; i = (i + 1) % 32)
	{
		unsigned mask = 1u << i;
		if (value & mask)
		{
			return(i);
		}
	}

	// Should never get here.
	check(false);

	return(-1);
}