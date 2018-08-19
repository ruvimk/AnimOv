#include <windows.h> 
#include <string.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <math.h> 

typedef struct { 
	unsigned char b; 
	unsigned char g; 
	unsigned char r; 
	unsigned char a; 
} BGRA; 

char * file_get_contents (char * filename){ 
	HANDLE hFile; 
	OFSTRUCT ofs; 
	int sFile; 
	char * pFile; 
	unsigned long bRead; 
	hFile= (HANDLE)(OpenFile (filename, &ofs, 0)); 
	sFile= GetFileSize (hFile, 0); 
	pFile= GlobalAlloc (GMEM_ZEROINIT, sFile + 16); 
	ReadFile (hFile, pFile, sFile, &bRead, 0); 
	CloseHandle (hFile); 
	return pFile; 
} 

void file_put_contents (char * filename, char * text, int the_size){ 
	HANDLE hFile; 
	OFSTRUCT ofs; 
	unsigned long bWritten; 
	hFile= (HANDLE)(OpenFile (filename, &ofs, 0x1001)); 
	SetEndOfFile (hFile); 
	WriteFile (hFile, text, the_size, &bWritten, 0); 
	CloseHandle (hFile); 
} 

#include "/fseeko1.c" 

#include "/wminiz.c" 

#include "misc/BmpToPngInMemory.c" 

int main (int argc, char * argv []){ 
	if (argc < 3){ 
		printf ("2 arguments expected; had %d. \r\n", argc - 1); 
	} 
	size_t sPng; 
	void * pBmp= file_get_contents (argv[1]); 
	void * pPng= BmpToPngInMemory (pBmp, &sPng); 
	file_put_contents (argv[2], pPng, sPng); 
	GlobalFree (pBmp); 
	MZ_FREE (pPng); 
	ExitProcess (0); 
} 