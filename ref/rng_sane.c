// avoid the utter aes nonsense in rng.c, and just do buffered output from shake.
// that makes verification elsewhere much less painful

#include "fips202.h"
static uint64_t state[25];
static unsigned char pending[SHAKE256_RATE];
static int ptr = SHAKE256_RATE;

void randombytes_init(unsigned char *entropy_input,
                 unsigned char *personalization_string,
                 int security_strength)
{
    shake256_absorb(state, entropy_input, 48);        
}

int
randombytes(unsigned char *x, unsigned long long xlen)
{
    unsigned char rem[SHAKE256_RATE];
    do {
	for (;(ptr < SHAKE256_RATE) && xlen; xlen--)
	    *x++ = pending[ptr++];
	if (!xlen) break;
	shake256_squeezeblocks(pending, 1, state);
	ptr = 0;
    } while (xlen);
}
