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
	cmd= SwitchValue (CmdLine, "text"); 
	if (!cmd){ 
		cprint ("Content-Type: text/plain\r\n\r\nWarning:  Command text is not provided. \r\n"); 
		cprint ("Debug: "); 
		cprint (CmdLine); 
		ExitProcess (0); 
	} 
	EnsureNoQuotes (cmd); 
	if (!*cmd){ 
		cprint ("Content-Type: text/plain\r\n\r\nWarning:  Command text is blank. \r\n"); 
		GlobalFree (cmd); 
		ExitProcess (0); 
	} 
	if (!(tmp= strstr (cmd, "<end of message>"))){ 
		cprint ("Content-Type: text/plain\r\n\r\nError:  Command text is invalid (no end of message marker). \r\n"); 
		GlobalFree (cmd); 
		ExitProcess (0); 
	} 
	*(tmp)= 0; 
	strcpy (path1, "C:/WBord/wwwroot"); 
	GetEnvironmentVariable ("DOCUMENT_ROOT", path1, 512); 
	strcat (path1, "/AnimOv/data/id"); 
	sprintf (string1, "%d", id); 
	strcat (path1, string1); 
	strcat (path1, ".aoc"); 
	cprint ("Content-Type: text/plain\r\n\r\n"); 
	if (id){ 
		text= file_get_contents (path1); 
		if (!text){ 
			text= GlobalAlloc (GMEM_ZEROINIT, 64); 
			strcpy (text, "<end of message>"); 
		} 
		if (!(tmp= strstr (text, "<end of message>"))){ 
			cprint ("Content-Type: text/plain\r\n\r\nError:  Data file corrupt (no end of message marker). \r\n"); 
			GlobalFree (text); 
			GlobalFree (cmd); 
			ExitProcess (0); 
		} 
		text3= GlobalAlloc (GMEM_ZEROINIT, strlen (tmp) + 16); 
		strcpy (text3, tmp); 
		*(tmp)= 0; 
		text2= GlobalAlloc (GMEM_ZEROINIT, strlen (text) + strlen (cmd) + strlen (text3) + 64); 
		strcpy (text2, text); 
		c= 0; 
		// Scan the number of commands already present. 
		pScan= text; 
		while (pScan= scan_line (pScan, line, sizeof (line))){ 
			if (!strstr (line, "<stop>")) continue; 
			if (sscanf (line, " %d %[^<>]<stop>", &c, string1) > 0){ 
				
			} else if (sscanf (line, " %[^<>]<stop>", string1) > 0){ 
				c++; 
			} 
		} 
		// Append the new commands to the data. 
		pScan= cmd; 
		while (pScan= scan_line (pScan, line, sizeof (line))){ 
			if (!strstr (line, "<stop>")) continue; 
			c++; 
			if (sscanf (line, " %*d %[^<>]<stop>", string1) > 0){ 
				sprintf (string2, "%d\t%s<stop>\r\n", c, string1); 
				strcat (text2, string2); 
			} else if (sscanf (line, " %[^<>]<stop>", string1) > 0){ 
				sprintf (string2, "%d\t%s<stop>\r\n", c, string1); 
				strcat (text2, string2); 
			} 
		} 
		strcat (text2, text3); 
		GlobalFree (cmd); 
		GlobalFree (text); 
		GlobalFree (text3); 
		file_put_contents (path1, text2, strlen (text2)); 
		GlobalFree (text2); 
	} 
	ExitProcess (0); 
} 