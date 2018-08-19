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

HANDLE hStdOut= 0; 
long cprint (char * msg){ 
	unsigned long bWritten; 
	if (!hStdOut) hStdOut= GetStdHandle (STD_OUTPUT_HANDLE); 
	WriteFile (hStdOut, msg, strlen (msg), &bWritten, 0); 
	return (long)(bWritten); 
} 

int main (int argc, char * argv []){ 
	HINSTANCE hInstance= GetModuleHandle (0); 
	LPSTR CmdLine= GetCommandLine (); 
	LPSTR orgCmdLine= CmdLine; 
	char string1[2048]; 
	char * a; 
	cprint ("Content-Type: text/plain\r\n\r\n"); 
	CmdLine= EnsureQueryHTTP (orgCmdLine); 
	a= SwitchValue (CmdLine, "a"); 
	if (a){ 
		urldecode (a, a, 0, strlen (a)); 
		sprintf (string1, "a: %s\r\n", a); 
		cprint (string1); 
		GlobalFree (a); 
	} 
	sprintf (string1, "Command Line: %s\r\n", CmdLine); 
	cprint (string1); 
	sprintf (string1, "Original Command Line: %s\r\n", orgCmdLine); 
	cprint (string1); 
	if (CmdLine != orgCmdLine) GlobalFree (CmdLine); 
	ExitProcess (0); 
} 