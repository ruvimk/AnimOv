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

LPSTR orgCmdLine; 
LPSTR CmdLine; 

char * filt; 
char * last; 

char * cmd; 

int iLast= 0; 
int iFilt= 0; 

int id; 

int main (){ 
	HANDLE hFile; 
	unsigned long sFile; 
	unsigned long bRead; 
	char path1[512]; 
	char string1[1024]; 
	char string2[1024]; 
	char * text; 
	char * text2; 
	char * text3; 
	char * pScan; 
	char * tmp; 
	char line[1024]; 
	int c; 
	orgCmdLine= GetCommandLine (); 
	CmdLine= EnsureQueryHTTP (orgCmdLine); 
	filt= SwitchValue (CmdLine, "filt"); 
	last= SwitchValue (CmdLine, "last"); 
	if (filt && !strcmp (filt, "vect")) iFilt= 1; 
	if (last) iLast= atoi (last); 
	if (filt) GlobalFree (filt); 
	if (last) GlobalFree (last); 
	id= SwitchNumber (CmdLine, "id"); 
	cprint ("Content-Type: text/plain\r\n\r\n"); 
	cprint ("Query Data: "); 
	cprint (CmdLine); 
	cmd= SwitchValue (CmdLine, "text"); 
	if (!cmd) ExitProcess (0); 
	EnsureNoQuotes (cmd); 
	cprint ("\r\n\r\ncmd="); 
	cprint (cmd); 
	ExitProcess (0); 
} 