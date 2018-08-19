#include <windows.h> 
#include <string.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <math.h> 

#include "toonhead.h" 

#include "/fseeko1.c" 

#include "/wminiz.c" 

#include "include/cmd.c" 
#include "include/wlib.c" 

int main (){ 
	char * cmd= strchr (GetCommandLine (), 32); 
		if (!cmd) ExitProcess (0); 
	char * a; 
		while (*cmd == 32) cmd++; 
		// while (a= strchr (cmd, 32)) cmd= a + 1; 
	char * pBase64= base64_encode ((unsigned char *)(cmd), strlen (cmd)); 
	printf ("Normal: %s\r\nBase64: %s\r\n", cmd, pBase64); 
	GlobalFree (pBase64); 
	ExitProcess (0); 
} 