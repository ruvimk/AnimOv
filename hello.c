#include <windows.h> 
#include <string.h> 
#include <stdlib.h> 

int main (){ 
	HANDLE hStdOut= GetStdHandle (STD_OUTPUT_HANDLE); 
	unsigned long bWritten; 
	char * msg= "Content-Type: text/plain\r\n\r\nHello World!\r\n"; 
	WriteFile (hStdOut, msg, strlen (msg), &bWritten, 0); 
	ExitProcess (0); 
} 