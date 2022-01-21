PART 1

1.1 - Generate QR

To be able to generate a QR code which communicates the secret key to the Authenticator app, the account name and issuer must be URL encoded and the secret key must be encoded in base 32. To URL encode the name and issuer, I inputted the
name and issuer given in the command line into the provided URL encode function. To encode the secret key, I broke the given string into byte long strings, and converted each of these strings to uint8 format using strtol with base 16.
I saved each byte in a uint8 array, which I then sent into the provided base 32 encode function.

1.2 - Validate QR

To validate that the correct authentication code was inputted, I use the TOTP algorithm as described in the included lab document. To do this, I need to calculate the HMAC using the current UNIX time to calculate the time step, and using
this timestep as the input message. To be able to input the timestep in the sha-1 functions I needed to re-format it in a binary string. To do this I bit shift right by a byte each iteration because the assignment will only take the least significant byte. To be able to split the key into the outer and inner keys, I filled an inner
array with bytes of 0x36, and XOR'd the first 10 bytes with the corresponding bytes of the original key, in a for loop. I used the same method for the outer key, replacing 0x36 with 0x5c. I then used the given SHA-1 functions to first
hash the inner key with the message (timestep), before hashing the output of that operation with the outer key. Next, I truncated the output by using the method given in the document. Then I compared the truncated output with the user
input to validate.

PART 2

2.1 - Setting up the enviornment

2.2 - Validating a Login

getAuthenticatorStatus:
Performed a GET call on the given address. Indexed the returned JSON structure dictionary to find the status, and returned "active" when the status was active, and one or more of face, voice, fingerprint, or eye status was enrolled.

sendSetup:
Performed a POST call to the given address with a login request. Added a global variable to self, verificationId, which held the uuid from the JSON reply.

getSetupStatus:
Performed a GET request given the verificationId from the previous function in the URL. Then returned the user verification status acquired from the returned JSON structure reply.

 
