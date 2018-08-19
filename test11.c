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
	wprint ("Content-Type: text/plain\r\n\r\n"); 
	wprint ("Current Directory: "); 
	GetCurrentDirectory (2048, string1); 
	wprint (string1); 
	wprint ("\r\n"); 
	CmdLine= EnsureQueryHTTP (orgCmdLine); 
	sprintf (string1, "Command Line: %s\r\n", CmdLine); 
	wprint (string1); 
	sprintf (string1, "Original Command Line: %s\r\n", orgCmdLine); 
	wprint (string1); 
	if (CmdLine != orgCmdLine) GlobalFree (CmdLine); 
	ExitProcess (0); 
} 