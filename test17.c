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

int main (){ 
	char string1[512]; 
	strcpy (string1, "1"); 
	EnsureDigits (string1, 4); 
	printf (string1); 
	ExitProcess (0); 
} 