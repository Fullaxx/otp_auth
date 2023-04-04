#ifndef __MT19937_64_H__
#define __MT19937_64_H__

#define NN (312)
#define MM (156)
#define MATRIX_A (0xB5026F5AA96619E9ULL)
#define UM (0xFFFFFFFF80000000ULL) /* Most significant 33 bits */
#define LM (0x7FFFFFFFULL) /* Least significant 31 bits */
#define ONE (1ULL)

typedef struct mtstate {
	/* The array for the state vector */
	unsigned long long mt[NN];
	/* mti==NN+1 means mt[NN] is not initialized */
	/* int mti=NN+1; This was moved to init_by_array64() */
	int mti;
} mt64_t;

/* initialize by an array with array-length */
/* init_key is the array for initializing keys */
/* key_length is its length */
void init_by_array64(mt64_t *s, unsigned long long init_key[], unsigned long long key_length);

/* generates a random number on [0, 2^64-1]-interval */
unsigned long long genrand64_uint64(mt64_t *s);

/* generates a random number on [0, 2^63-1]-interval */
long long genrand64_int63(mt64_t *s);

/* generates a random number on [0,1]-real-interval */
double genrand64_real1(mt64_t *s);

/* generates a random number on [0,1)-real-interval */
double genrand64_real2(mt64_t *s);

/* generates a random number on (0,1)-real-interval */
double genrand64_real3(mt64_t *s);

#endif	/* __MT19937_64_H__ */
