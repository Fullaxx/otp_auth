#include <stdint.h>
#include "secret.h"

#if defined(TOTP)
#include <time.h>
#define TIMESTEP (60)
#endif

/* Take in a single value from the seed, mix with bits and return the new value */
static sword sprinkle_bits(sword orig, unsigned char bits)
{
	sword retval;
	unsigned char lo = orig & 0xFF;
	sword temp = orig >> 8;
	retval = (temp << 8) | (lo ^ bits);
	return retval;
}

/* Pick a manipulation start location based on the secret and the code */
static uint32_t calc_start_location(secrets_t *s, uint64_t code)
{
	int i;
	uint64_t sum = code;
	for(i=0; i<SECRET_ULONGS; i++) {
		sum += (s->secret[i] & 0xFFFF);
	}
	return (sum % SECRET_ULONGS);
}

/*
	If we are using TOTP or HOTP
	we will use manipulate_seed() to tweak the seed based on known shared information.
	Either the current time (TOTP) or a known counter (HOTP)
*/
int manipulate_seed(secrets_t *seed)
{
	int retval = 0;
	unsigned char b;

	/* Get the current time code, valid for interval TIMESTEP */
#if defined(TOTP)
	time_t now = time(NULL);
	uint64_t code = now / TIMESTEP;
	retval = TIMESTEP-(now % TIMESTEP); /* amt of time left */
#endif
#if defined(HOTP)
	uint64_t code = seed->hotp_counter;
#endif

	/* Where shall we start? */
	uint32_t i = calc_start_location(seed, code);

	/* Adjust the seed based on the current time */
	while(code > 0) {
		b  = code & 0xFF;
		seed->secret[i] = sprinkle_bits(seed->secret[i], b);
		i = ((i+1)%SECRET_ULONGS);
		code = code >> 8;
	}

	return retval;
}
