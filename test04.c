#include <windows.h> 
#include <string.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <math.h> 

#include "object.c" 

int main (){ 
	HANDLE obj= Object (); 
	HANDLE a; 
	HANDLE b; 
	HANDLE c= Buffer ("...", -1); 
	HANDLE d= ObjectCopy (c); 
	int i; 
	char * ind= "something"; 
	char * val1; 
	ObjectAddParentCount (obj, 1); 
	ObjectAddParentCount (c, 1); 
	// printf ("... \r\n"); 
		// printf ("(%d,%s)\r\n", ((char **)(c))[6], ((char **)(c))[5]); 
		// b= ObjectToString (c); 
		// wprintf (L"%s", ((char **)(b))[5]); 
		// ObjectDelete (b); 
		// printf ("\r\n"); 
		// printf ("(%d,%s)\r\n", ((char **)(c))[6], ((char **)(c))[5]); 
		// b= ObjectToString (c); 
		// wprintf (L"%s", ((char **)(b))[5]); 
		// ObjectDelete (b); 
		// printf ("\r\n"); 
		// printf ("(%d,%s)\r\n", ((char **)(c))[6], ((char **)(c))[5]); 
		// b= ObjectToString (c); 
		// wprintf (L"%s", ((char **)(b))[5]); 
		// ObjectDelete (b); 
		// printf ("\r\n"); 
		// printf ("(%d,%s)\r\n", ((char **)(c))[6], ((char **)(c))[5]); 
		// b= ObjectToString (c); 
		// wprintf (L"obj[%s]=>", ((char **)(b))[5]); 
		// ObjectDelete (b); 
		// b= ObjectToString (ObjectGetProperty (obj, c)); 
		// wprintf (L"%s\r\n", ((char **)(b))[5]); 
		// ObjectDelete (b); 
	ObjectSetProperty (obj, c, Buffer ("Hello, how are you? ", -1)); 
		// printf ("c - d = %d\r\n", memcmp (c, d, 32 * sizeof (char *))); 
		// for (i= 0; i < 32 * sizeof (char *); i++){ 
			// if (((char *)(c))[i] != ((char *)(d))[i]) printf ("(c - d)[%d] = %d\r\n", i, (char)(((char *)(c))[i] - ((char *)(d))[i])); 
		// } 
	ObjectSetProperty (obj, Buffer ("f", -1), ObjectGetProperty (obj, c)); 
		// b= ObjectToString (c); 
		// wprintf (L"obj[%s]=>", ((char **)(b))[5]); 
		// ObjectDelete (b); 
		// b= ObjectToString (ObjectGetProperty (obj, c)); 
		// wprintf (L"%s\r\n", ((char **)(b))[5]); 
		// ObjectDelete (b); 
	printf ("\r\n"); 
	// printf ("something01...\r\n"); 
	a= ObjectGetProperty (obj, Buffer (ind, -1)); 
	printf ("obj[%s]=%d\r\n", ind, (int)((char *)(a))); 
	// printf ("something02...\r\n"); 
	// printf ("main %d\r\n", (int)((((char ***)(obj))[2][0]))); 
	ObjectSetProperty (obj, Buffer (ind, -1), Buffer ("abcd", -1)); 
	// printf ("main %d\r\n", (int)((((char ***)(obj))[2][0]))); 
	// printf ("...\r\n"); 
	// printf ("main %d\r\n", (int)((((char ***)(obj))[2][0]))); 
	// printf ("%d\r\n...\r\n", (int)(((char ***)(obj))[2][0])); 
	// printf ("(offset = %d)\r\n", (int)(&(((char ***)(obj))[2][0]))); 
	// printf ("main %d\r\n", (int)((((char ***)(obj))[2][0]))); 
	// asm { 
		// mov eax, dword ptr [obj] 
		// mov ebx, eax 
		// add ebx, 8 
		// mov eax, dword ptr [ebx] 
		// mov ebx, eax 
		// mov eax, dword ptr [ebx] 
		// mov dword ptr [val1], eax 
	// } 
	// printf ("(main (%d))\r\n", (int)val1); 
	// printf ("main ID %d\r\n", (int)(((char **)(obj))[8])); 
	// printf ("something ID %d\r\n", (int)(((char **)(val1))[8])); 
	// ObjectDelete (Buffer (ind, -1)); 
	// asm { 
		// mov eax, dword ptr [obj] 
		// mov ebx, eax 
		// add ebx, 8 
		// mov eax, dword ptr [ebx] 
		// mov ebx, eax 
		// mov eax, dword ptr [ebx] 
		// mov dword ptr [val1], eax 
	// } 
	// printf ("(main (%d))\r\n", (int)val1); 
	a= ObjectGetProperty (obj, Buffer (ind, -1)); 
	// printf ("main %d\r\n", (int)((((char ***)(obj))[2][0]))); 
	printf ("obj[%s]=%d\r\n", ind, (int)((char *)(a))); 
	// printf ("%d\r\n...\r\n", (int)(((char ***)(obj))[2][0])); 
	ObjectSetProperty (obj, Buffer ("the message", -1), Buffer ("Hello World!", -1)); 
	// printf ("...\r\n"); 
	a= ObjectGetProperty (obj, Buffer ("the message", -1)); 
	// printf ("....\r\n"); 
	printf ("obj[the message]=%d\r\n", (int)((char *)(a))); 
	b= ObjectToString (a); 
	wprintf (L"obj[the message]=>%s\r\n", ((char **)(b))[5]); 
	ObjectDelete (b); 
	b= ObjectToString (Number (7)); 
	wprintf (L"%s\r\n", ((char **)(b))[5]); 
	ObjectDelete (b); 
	printf ("obj["); 
		b= ObjectToString (c); 
		wprintf (L"%s", ((char **)(b))[5]); 
		ObjectDelete (b); 
		printf ("]=>"); 
		b= ObjectToString (ObjectGetProperty (obj, c)); 
		wprintf (L"%s\r\n", ((char **)(b))[5]); 
		ObjectDelete (b); 
	printf ("\r\n"); 
	printf ("obj["); 
		b= ObjectToString (Buffer ("f", -1)); 
		wprintf (L"%s", ((char **)(b))[5]); 
		ObjectDelete (b); 
		printf ("]=>"); 
		b= ObjectToString (ObjectGetProperty (obj, Buffer ("f", -1))); 
		wprintf (L"%s", ((char **)(b))[5]); 
		ObjectDelete (b); 
	printf ("\r\n"); 
	printf ("obj["); 
		b= ObjectToString (Buffer ("...", -1)); 
		wprintf (L"%s", ((char **)(b))[5]); 
		ObjectDelete (b); 
		printf ("]=>"); 
		b= ObjectToString (ObjectGetProperty (obj, Buffer ("...", -1))); 
		wprintf (L"%s", ((char **)(b))[5]); 
		ObjectDelete (b); 
	printf ("\r\n"); 
	printf ("obj["); 
		b= ObjectToString (Buffer ("...", -1)); 
		wprintf (L"%s", ((char **)(b))[5]); 
		ObjectDelete (b); 
		printf ("]=>"); 
		b= ObjectToString (ObjectGetProperty (obj, Buffer ("...", -1))); 
		wprintf (L"%s", ((char **)(b))[5]); 
		ObjectDelete (b); 
	printf ("\r\n"); 
		// printf ("c - d = %d\r\n", memcmp (c, d, 32 * sizeof (char *))); 
		// for (i= 0; i < 32 * sizeof (char *); i++){ 
			// if (((char *)(c))[i] != ((char *)(d))[i]) printf ("(c - d)[%d] = %d\r\n", i, (char)(((char *)(c))[i] - ((char *)(d))[i])); 
		// } 
	ObjectDelete (c); 
	ObjectDelete (obj); 
	printf ("Memory Stats: \r\n\tTotal Objects Constructed: %d\r\n\tTotal Objects Deleted: %d\r\n", Object_UniqueNextAssignID - 1, Object_UniqueDeletedCount); 
	ExitProcess (0); 
} 