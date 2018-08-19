#include <windows.h> 
#include <string.h> 

int main (){ 
	HANDLE hIn= CreateFile ("CONIN$", GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0); 
	char string1[512]; 
	unsigned long bRead; 
	HANDLE hOut; 
	unsigned long bWritten; 
	int i; 
	ReadFile (hIn, string1, 512, &bRead, 0); 
	CloseHandle (hIn); 
	FreeConsole (); 
	for (i= 0; string1[i] && string1[i] != 13 && string1[i] != 10; i++); 
	string1[i]= 0; 
	AllocConsole (); 
	hOut= CreateFile ("CONOUT$", GENERIC_WRITE, FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0); 
	WriteFile (hOut, "You entered: ", 13, &bWritten, 0); 
	WriteFile (hOut, string1, strlen (string1), &bWritten, 0); 
	CloseHandle (hOut); 
	Sleep (500); 
	FreeConsole (); 
	Sleep (1000); 
} 