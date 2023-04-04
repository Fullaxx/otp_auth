#ifndef __OTP_SECRET_H__
#define __OTP_SECRET_H__

/* The number of swords in the MT init array to seed with */
#define SECRET_ULONGS (32)

#if SECRET_ULONGS < 8
#error SECRET_ULONGS must be >= 8
#endif

/* The MT uses unsigned long long as its data type */
typedef unsigned long long sword;

/*
	This is our struct of secret data.
	It will be used to seed the MT.
	If we are using TOTP method
	the amt of time left is put in totp_time_left for caller
	If using the HOTP method, the changing bits are set by the caller in hotp_counter
*/
typedef struct {
	sword secret[SECRET_ULONGS];
	int loaded;

	int totp_time_left;			/* TOTP method only */
	unsigned long hotp_counter;	/* HOTP method only */
} secrets_t;

void dbg_print_secret(secrets_t *otp);
int load_secret(secrets_t *otp, char *path);

#endif	/* __OTP_SECRET_H__ */
