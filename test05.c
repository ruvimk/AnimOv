#include <windows.h> 
#include <string.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <math.h> 

#include "include/object.c" 
#include "include/xmllib.c" 

int main (){ 
	HANDLE obj= Object (); 
	HANDLE a; 
	char * ind= "something"; 
	ObjectAddParentCount (obj, 1); 
	printf ("... \r\n"); 
	a= ObjectGetProperty (obj, Buffer (ind, -1)); 
	printf ("obj[%s]=%d\r\n", ind, (int)((char *)(a))); 
	printf ("main %d\r\n", (int)((((char ***)(obj))[2][0]))); 
	ObjectSetProperty (obj, Buffer (ind, -1), Buffer ("abcd", -1)); 
	printf ("main %d\r\n", (int)((((char ***)(obj))[2][0]))); 
	printf ("...\r\n"); 
	printf ("main %d\r\n", (int)((((char ***)(obj))[2][0]))); 
	printf ("%d\r\n...\r\n", (int)(((char ***)(obj))[2][0])); 
	printf ("(offset = %d)\r\n", (int)(&(((char ***)(obj))[2][0]))); 
	printf ("main %d\r\n", (int)((((char ***)(obj))[2][0]))); 
	a= ObjectGetProperty (obj, Buffer (ind, -1)); 
	printf ("main %d\r\n", (int)((((char ***)(obj))[2][0]))); 
	printf ("obj[%s]=%d\r\n", ind, (int)((char *)(a))); 
	printf ("%d\r\n...\r\n", (int)(((char ***)(obj))[2][0])); 
	printf ("obj[%s]=%s\r\n", ind, ((char **)(a))[5]); 
	ObjectDelete (obj); 
	ExitProcess (0); 
} 