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
	char string2[512]; 
	int i; 
	int j; 
	char * nset= "two"; 
	char * path= "figure1.branch1.path"; 
	char * text= file_get_contents ("input.txt"); 
	char ** buff= ImportPropertiesBin (text); 
	double where= 1.5; 
	GlobalFree (text); 
	EnsurePathBin (buff, path, 0, where, 1); 
	text= ExportPropertiesBin (buff); 
	// printf ("%s", ((char *)(((char **)(buff[1]))[1])) + sizeof (double)); 
	DestroyPropertiesBin (buff); 
	file_put_contents ("output.txt", text, strlen (text)); 
	GlobalFree (text); 
	ExitProcess (0); 
} 