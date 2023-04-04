#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "secret.h"

#ifdef DEBUG
void prep_test_secret(secrets_t *s)
{
	int i=0;
	s->secret[i++] = 0x438cd33ddc99c7ffULL;
	s->secret[i++] = 0x43a9aa8f59a79025ULL;
	s->secret[i++] = 0x37eb66f0acf19270ULL;
	s->secret[i++] = 0x6a195fc527620c58ULL;
	s->secret[i++] = 0x59304c63384b369eULL;
	s->secret[i++] = 0xdc2ad3d6a43c073fULL;
	s->secret[i++] = 0xdb14aaadc7eef3c1ULL;
	s->secret[i++] = 0xf7859f74cf3782fcULL;
	s->secret[i++] = 0xa6f29a9598937e56ULL;
	s->secret[i++] = 0x8b28012713ded2c6ULL;
	s->secret[i++] = 0x9b4fec22a5871c9cULL;
	s->secret[i++] = 0x3ad6bc86eeea03f0ULL;
	s->secret[i++] = 0xc6c30542d15abc32ULL;
	s->secret[i++] = 0xf3d3057ff45c833cULL;
	s->secret[i++] = 0x8790fe608de0a44eULL;
	s->secret[i++] = 0xf0d38c3e827f5053ULL;
	s->secret[i++] = 0x85c512304f804990ULL;
	s->secret[i++] = 0x0823b24f60cea0a1ULL;
	s->secret[i++] = 0xffb99f7343a17bdcULL;
	s->secret[i++] = 0xfbdba78496f9a2e4ULL;
	s->secret[i++] = 0xf16d52663c5d6a36ULL;
	s->secret[i++] = 0xf69c9d22e735cf89ULL;
	s->secret[i++] = 0xd913f01c2b65a79aULL;
	s->secret[i++] = 0x15fa42bec29ee091ULL;
	s->secret[i++] = 0x80a8d27dcb9690cdULL;
	s->secret[i++] = 0x2da658afa54b4dceULL;
	s->secret[i++] = 0xfd1f9b25747fd6c3ULL;
	s->secret[i++] = 0xaf796e2f066da652ULL;
	s->secret[i++] = 0x8d3777299244dbedULL;
	s->secret[i++] = 0xd4863c8908779642ULL;
	s->secret[i++] = 0x1088ee2e6873838dULL;
	s->secret[i++] = 0x8656cf3fbd119912ULL;
	s->loaded = 1;
}
#endif

void dbg_print_secret(secrets_t *s)
{
#ifdef DEBUG
#ifdef DEBUG_SECRETS
/* DO NOT ENABLE THIS IN PRODUCTION */
	int i;
	for(i=0; i<SECRET_ULONGS; i++) {
		printf("0x%016llX", s->secret[i]);
		if((i % 4) == 3) { printf("\n"); }
		else { printf(" "); }
	}
	printf("\n");
#endif
#endif
}

static void dbg_print_secret_load(secrets_t *s, int i)
{
#ifdef DEBUG
#ifdef DEBUG_SECRETS
/* DO NOT ENABLE THIS IN PRODUCTION */
	printf("Loading %lu bytes of secret (slot %4d): 0x%016llX\n", sizeof(sword), i+1, s->secret[i]);
#endif
#endif
}

static void secret_error(FILE *f, int i)
{
	fprintf(stderr, "Secrets not filled! (Got %ld of %ld bytes)\n", i*sizeof(sword), SECRET_ULONGS*sizeof(sword));
	if(feof(f)) {
		fprintf(stderr, "fread() failed: EOF!\n");
	} else {
		fprintf(stderr, "fread() failed: %s!\n", strerror(errno));
	}
}

int load_secret(secrets_t *otp, char *path)
{
	int i, r;
	FILE *f = fopen(path, "r");
	if(!f) {
		fprintf(stderr, "fopen(%s, r) failed: %s!\n", path, strerror(errno));
		return 1;
	}

/*
	At the moment this code is not portable between different BE/LE architectures
	If you need to support BE and LE this fread() loop will need to be re-written.
	2 options to accomplish this:
	 - 1) Read 1 byte at a time
	 - 2) Swap bytes on LE to BE format (as if you were reading bytes from the wire)
*/
	for(i=0; i<SECRET_ULONGS; i++) {
		r = fread(&otp->secret[i], sizeof(sword), 1, f);
		if(r != 1) { secret_error(f, i); return 2; }
		dbg_print_secret_load(otp, i);
	}

	otp->loaded = 1;
	fclose(f);
	return 0;
}
