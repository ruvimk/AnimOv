#include <windows.h> 
#include <string.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <math.h> 

#include "toonhead.h" 

#include "include/wlib.c" 

int main (){ 
	char string1[512]; 
	char string2[512]; 
	int i; 
	int j; 
	char * nset= "figure1.abcd01something"; 
	char * path= "figure1.branch3"; 
	char * text= file_get_contents ("input.txt"); 
	char * buff= GlobalAlloc (GMEM_ZEROINIT, strlen (text) + 1024); 
	double where= 0; 
	strcpy (buff, text); 
	//EnsurePath (buff, path, 0, where, 1); 
	//RenameProperties (buff, path, nset); 
	DeleteProperties (buff, path); 
	file_put_contents ("output.txt", buff, strlen (buff)); 
	GlobalFree (text); 
	GlobalFree (buff); 
	ExitProcess (0); 
} 