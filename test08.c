#include <windows.h> 
#include <string.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <math.h> 

#include "include/object.c" 
#include "include/xmllib.c" 

int main (int argc, char * argv []){ 
	HANDLE xdoc; 
	HANDLE hFile; 
	HANDLE dump; 
	HANDLE a; 
	HANDLE b; 
	int sFile= 0; 
	char * buffer; 
	wchar_t * buff; 
	unsigned long bRead; 
	int i; 
	int j; 
	char string1[512]; 
	char string2[512]; 
	if (argc > 1){ 
		hFile= CreateFile (argv[1], GENERIC_READ, 7, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0); 
		if (hFile){ 
			// fseek (hFile, 0, SEEK_END); 
			// sFile= ftell (hFile); 
			sFile= (int)(GetFileSize (hFile, 0)); 
			if (sFile != -1){ 
				buffer= OBJ_ALLOC (sFile + 16); 
				buff= OBJ_ALLOC (sFile * sizeof (wchar_t) + 16); 
				ReadFile (hFile, buffer, sFile, &bRead, 0); 
				for (i= 0; buffer[i]; i++) buff[i]= buffer[i]; 
				xdoc= ParseXML (buff); 
				OBJ_FREE (buffer); 
				OBJ_FREE (buff); 
				printf ("ParseXML () returned; type a DOM path to peek, or type 'exit' to quit. \r\n\r\n"); 
				do { 
					scanf ("%s", string1); 
					if (!memcmp (string1, "exit", 4) && (string1[4] == 32 || string1[4] == 9 || string1[4] == 13 || string1[4] == 10 || !string1[4])) break; 
					i= 0; 
					a= xdoc; 
					do { 
						j= 0; 
						for (; string1[i] && string1[i] != 46; i++) string2[j]= string1[i]; 
						if (string2[0] >= 48 && string2[0] < 58) b= Number (atof (string2)); 
						else b= Buffer (string2, j); 
						a= ObjectGetProperty (a, b); 
					} while (a && string1[i]); 
					b= ObjectToString (a); 
					wprintf (L"=> %s\r\n\r\n", (wchar_t *)(((char **)(b))[5])); 
					ObjectDelete (b); 
				} while (1); 
				ObjectDelete (xdoc); 
			} else printf ("Error:  Could not open file %s for reading. \r\n", argv[1]); 
			CloseHandle (hFile); 
		} 
		// strcpy (string1, argv[1]); 
		// strcat (string1, ".dump"); 
		// hFile= CreateFile (string1, GENERIC_WRITE, 7, 0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0); 
		// if (hFile){ 
			// WriteFile (hFile, (((char **)(dump))[5]), (int)(((char **)(dump))[6]) * 2, &bRead, 0); 
			// CloseHandle (hFile); 
		// } 
		// ObjectDelete (dump); 
	} 
	ExitProcess (0); 
} 