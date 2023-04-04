#define _XOPEN_SOURCE (700)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(TOTP)
#include <time.h>
#endif

#include "mt19937-64.h"
#include "secret.h"

#if defined(TOTP) || defined(HOTP)
int manipulate_seed(secrets_t *seed);
#endif

/* The amount of output from the MT that you want to use */
#define OUTPUT_ULONGS (4)

#if OUTPUT_ULONGS < 1
#error OUTPUT_ULONGS must be >= 1
#endif

static char* gen_otp(secrets_t *s, unsigned long spin)
{
	mt64_t t;
	secrets_t seed;
	char *otp_str;
	int i, n=0, otp_str_len;

	if(!s->loaded) { fprintf(stderr, "Secret not loaded!\n"); return NULL; }

	/* Prepare the seed, based on the initial secret */
	memcpy(&seed, s, sizeof(secrets_t));

	dbg_print_secret(s);
#if defined(TOTP) || defined(HOTP)
	s->totp_time_left = manipulate_seed(&seed);
#endif
	dbg_print_secret(&seed);

	/* Init the MT with seed and spin */
	init_by_array64(&t, seed.secret, SECRET_ULONGS);
	while(spin-- > 0) { (void)genrand64_uint64(&t); }

	/* Extract values to use and generate output string */
	otp_str_len = (OUTPUT_ULONGS*8*2)+1;
	otp_str = malloc(otp_str_len);
	i = OUTPUT_ULONGS;
	while(i-- > 0) {
		n += snprintf(&otp_str[n], otp_str_len-n, "%016llX", genrand64_uint64(&t));
	}

	memset(&t,    0, sizeof(t));
	memset(&seed, 0, sizeof(seed));
	return otp_str;
}

static void init_hotp(secrets_t *s)
{
#if defined(HOTP)
	char *hotp_counter_str = getenv("HOTP_COUNTER");
	if(hotp_counter_str) {
		s->hotp_counter = strtoul(hotp_counter_str, NULL, 10);
	} else {
		fprintf(stderr, "HOTP_COUNTER is not set!\n");
		exit(1);
	}
#endif
}

static void init_totp(void)
{
#if defined(TOTP)
	if(sizeof(time_t) == 4) {
		fprintf(stderr, "time_t is only 32-bit!\n");
		fprintf(stderr, "This system is subject to the Y2038 problem\n");
		fprintf(stderr, "Look here for more info: https://en.wikipedia.org/wiki/Year_2038_problem\n");
	}
#endif
}

int main(int argc, char *argv[])
{
	secrets_t s;

	init_totp();

	if(argc < 3) {
		fprintf(stderr, "%s: <SECRET FILE> <SPIN VALUE>\n", argv[0]);
		exit(1);
	}

	/* Initialize Variables */
	memset(&s, 0, sizeof(secrets_t));
	char *secret_path = argv[1];
	unsigned long spin = strtoul(argv[2], NULL, 10);
	init_hotp(&s);

	/* Load Secret File */
	int z = load_secret(&s, secret_path);
	if(z != 0) {
		fprintf(stderr, "load_secret(%s) failed!\n", secret_path);
		exit(1);
	}

	/* Generate OTP */
	char *otp_str = gen_otp(&s, spin);
	if(!otp_str) {
		fprintf(stderr, "OTP GENERATION FAILED!\n");
		memset(&s, 0, sizeof(secrets_t));
		exit(1);
	}

#if defined(TOTP)
	printf("TOTP: %s (valid for %4ds)\n", otp_str, s.totp_time_left);
#elif defined(HOTP)
	printf("HOTP: %s\n", otp_str);
#else
	printf("Static Key: %s\n", otp_str);
#endif

	/* Wipe memory */
	memset(otp_str, 0, strlen(otp_str));
	free(otp_str);
	memset(&s, 0, sizeof(secrets_t));
	return 0;
}
