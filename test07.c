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
	int sFile= 0; 
	char * buffer; 
	wchar_t * buff; 
	unsigned long bRead; 
	int i; 
	char string1[512]; 
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
				// dump= ObjectDump (xdoc, 0, 0); 
				dump= String (L"something", -1); 
				ObjectDelete (xdoc); 
			} else printf ("Error:  Could not open file %s for reading. \r\n", argv[1]); 
			CloseHandle (hFile); 
		} 
		strcpy (string1, argv[1]); 
		strcat (string1, ".dump"); 
		hFile= CreateFile (string1, GENERIC_WRITE, 7, 0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0); 
		if (hFile){ 
			WriteFile (hFile, (((char **)(dump))[5]), (int)(((char **)(dump))[6]) * 2, &bRead, 0); 
			CloseHandle (hFile); 
		} 
		ObjectDelete (dump); 
	} 
	ExitProcess (0); 
} 