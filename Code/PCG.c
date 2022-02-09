#include "PCG.h"


static uint64_t mcg_state = 0xcafef00dd15ea5e5u;	// Must be odd
static uint64_t const multiplier = 6364136223846793005u;


uint32_t fast_pcg32(void) // Fast 32-bit Permuted Congruential generator
{
	uint64_t x = mcg_state;
	unsigned count = (unsigned)(x >> 61);

	mcg_state = x * multiplier;
	x ^= x >> 22;
	return (uint32_t)(x >> (22 + count));
}

void pcg32_fast_init(uint64_t seed) // Initialization of PCG
{
	mcg_state = 2*seed + 1;
	(void)fast_pcg32();
}

