#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "lib/encoding.h"

int
main(int argc, char * argv[])
{
	if ( argc != 4 ) {
		printf("Usage: %s [issuer] [accountName] [secretHex]\n", argv[0]);
		return(-1);
	}

	char *	issuer = argv[1];
	char *	accountName = argv[2];
	char *	secret_hex = argv[3];
	int i; int j = 0;
	int8_t nice[10];
	
	char enc_secret[100];
	int nret;
	char temp[] = "hi";
	
	for (i = 0; i < strlen(secret_hex) - 1; i+=2){
		memcpy((void*)temp, (void*)secret_hex + i, 2);
		nice[j] = strtol(temp, NULL, 16);
		j++;
	}
	//FIXME: Probably should change size of enc_secret after reading piazza post
	nret = base32_encode((int8_t*)nice, 10, enc_secret, sizeof enc_secret);
	
//	printf("\n\nENCODED:\n%s\n\n", enc_secret);

	assert (strlen(secret_hex) <= 20);

	printf("\nIssuer: %s\nAccount Name: %s\nSecret (Hex): %s\n\n",
		issuer, accountName, secret_hex);

	// Create an otpauth:// URI and display a QR code that's compatible
	// with Google Authenticator
	
	char buf[1024];
	memset(buf, 0, 1024);
	sprintf(buf, "otpauth://totp/%s?issuer=%s&secret=%s&period=30", urlEncode(accountName), urlEncode(issuer), enc_secret);

	displayQRcode(buf);


//	displayQRcode("otpauth://testing");

	return (0);
}
