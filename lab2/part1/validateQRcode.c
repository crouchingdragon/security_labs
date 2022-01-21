#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include "lib/sha1.h"


static int
validateTOTP(char * secret_hex, char * TOTP_string)
{
	time_t to = 0;
	time_t t = time(NULL);
	time_t tx = 30;

	// the current time step
	time_t ct = (t - to)/tx;

	// enough bytes to hold time
	uint8_t time[sizeof(time_t)];
	int i; int j = 0;

	// assignment will take only the least significant byte of whatever it's given (that's why I right shift)
	for(i=0; i<sizeof(time_t); i++)
		time[i] = ct >> (56 - 8*i);

	
	// Validating the TOTP_string based on the key/secret_hex
	SHA1_INFO	ctx;
	uint8_t		sha[SHA1_DIGEST_LENGTH];

	// Assuming they mean an array of 8 bit integers as input again (binary??)
	char temp[] = "hi";
	uint8_t inner[65];
	uint8_t outer[65];
	memset(inner, 0x36, 65);
	memset(outer, 0x5c, 65);

	// might be a better way of doing this, but this worked so selavi
	for(i = 0; i < strlen(secret_hex) - 1; i+=2){
		memcpy((void*)temp, (void*)secret_hex + i, 2);
		inner[j] ^= strtol(temp, NULL, 16);
		outer[j] ^= strtol(temp, NULL, 16);
		j++;
	}

	sha1_init(&ctx);
	sha1_update(&ctx, inner, 64);
	sha1_update(&ctx, time, 8);
	sha1_final(&ctx, sha);

	sha1_init(&ctx);
	sha1_update(&ctx, outer, 64);
	sha1_update(&ctx, sha, 20);
	sha1_final(&ctx, sha);

	// from the pdf about sha, this is the way they truncate their values
	int offset = sha[SHA1_DIGEST_LENGTH - 1] & 0xf;
	int binary = ((sha[offset] & 0x7f) << 24) | ((sha[offset + 1] & 0xff) << 16) | ((sha[offset + 2] & 0xff) << 8) | (sha[offset + 3] & 0xff);

	int otp = binary % (1000000);

	char compare[sizeof TOTP_string];
	sprintf(compare, "%d", otp);
	if (strcmp(compare, TOTP_string) == 0) return 1;

	// return 1 for valid, return 0 for invalid
	return (0);
}


int
main(int argc, char * argv[])
{
	if ( argc != 3 ) {
		printf("Usage: %s [secretHex] [TOTP]\n", argv[0]);
		return(-1);
	}

	char *	secret_hex = argv[1];
	char *	TOTP_value = argv[2];

	assert (strlen(secret_hex) <= 20);
	assert (strlen(TOTP_value) == 6);

	printf("\nSecret (Hex): %s\nTOTP Value: %s (%s)\n\n",
		secret_hex,
		TOTP_value,
		validateTOTP(secret_hex, TOTP_value) ? "valid" : "invalid");

	return(0);
}
