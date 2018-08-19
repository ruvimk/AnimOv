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

int iLast= 0; 
int iFilt= 0; 

int id; 

int main (){ 
	HANDLE hFile; 
	unsigned long sFile; 
	unsigned long bRead; 
	char path1[512]; 
	char string1[1024]; 
	char * text; 
	char * pScan; 
	char line[1024]; 
	int c; 
	orgCmdLine= GetCommandLine (); 
	CmdLine= EnsureQueryHTTP (orgCmdLine); 
	filt= SwitchValue (CmdLine, "filt"); 
	if (!filt) filt= SwitchValue (CmdLine, "filter"); 
	last= SwitchValue (CmdLine, "last"); 
	if (filt && !strcmp (filt, "vect")) iFilt= 1; 
	if (last) iLast= atoi (last); 
	if (filt) GlobalFree (filt); 
	if (last) GlobalFree (last); 
	id= SwitchNumber (CmdLine, "id"); 
	strcpy (path1, "C:/WBord/wwwroot"); 
	GetEnvironmentVariable ("DOCUMENT_ROOT", path1, 512); 
	strcat (path1, "/AnimOv/data/id"); 
	sprintf (string1, "%d", id); 
	strcat (path1, string1); 
	strcat (path1, ".aoc"); 
	cprint ("Content-Type: text/plain\r\n\r\n"); 
	if (id){ 
		text= file_get_contents (path1); 
		pScan= text; 
		while (pScan= scan_line (pScan, line, sizeof (line))){ 
			if (!strstr (line, "<stop>")) continue; 
			if (sscanf (line, " %d %[^<>]<stop>", &c, string1) > 0){ 
				if (c > iLast){ 
					if (!iFilt){ 
						cprint ("\t"); 
						cprint (string1); 
						cprint ("<stop>\r\n"); 
					} else if (iFilt == 1){ 
						if (!strstr (string1, "set")){ 
							cprint ("\t"); 
							cprint (string1); 
							cprint ("<stop>\r\n"); 
						} 
					} 
				} 
			} else if (sscanf (line, " %[^<>]<stop>", string1) > 0){ 
				if (!iFilt){ 
					cprint ("\t"); 
					cprint (string1); 
					cprint ("<stop>\r\n"); 
				} else if (iFilt == 1){ 
					if (!strstr (string1, "set")){ 
						cprint ("\t"); 
						cprint (string1); 
						cprint ("<stop>\r\n"); 
					} 
				} 
			} 
		} 
		cprint ("<end of message>"); 
	} 
	ExitProcess (0); 
} 