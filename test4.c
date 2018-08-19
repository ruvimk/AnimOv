#include <windows.h> 
#include <string.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <math.h> 

#include "toonhead.h" 

#include "/fseeko1.c" 

#include "/wminiz.c" 

char main_debug[8192]; 

#include "include/cmd.c" 
#include "include/wlib.c" 

int main (){ 
	char string1[512]; 
	char string2[512]; 
	int i; 
	int j; 
	char * nset= "two"; 
	char * path= "stick-figure-1.body.neck.head.face.eyes.y"; 
	char * text= file_get_contents ("input.txt"); 
	char * buff= GlobalAlloc (GMEM_ZEROINIT, strlen (text) + 1024); 
	double where= 0.032619; 
	strcpy (buff, text); 
	EnsurePath (buff, path, "-21", where, 0); 
	file_put_contents ("output.txt", buff, strlen (buff)); 
	GlobalFree (text); 
	GlobalFree (buff); 
	ExitProcess (0); 
} 