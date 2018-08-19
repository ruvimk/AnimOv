#include <windows.h> 
#include <string.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <math.h> 

#include "toonhead.h" 

#include "include/wlib.c" 

int main (int argc, char * argv []){ 
	if (argc > 2){ 
		char string1[512]; 
		char string2[512]; 
		int i; 
		int j; 
		char path[512]; 
		char * nset= "two"; 
		char * text= file_get_contents ("input.txt"); 
		char * buff= GlobalAlloc (GMEM_ZEROINIT, strlen (text) + 1024); 
		double where= 0; 
		double number; 
		int a1= sscanf (argv[1], " %lf", &where); 
		if (!a1){ 
			sscanf (argv[2], " %lf", &where); 
			strcpy (path, argv[1]); 
		} else { 
			strcpy (path, argv[2]); 
		} 
		strcpy (buff, text); 
		//EnsurePath (buff, path, 0, where, 1); 
		LookUpPropertyEx (buff, path, where, string2); 
		//printf ("Number: %f\r\n", number); 
		printf ("Text: %s\r\n", string2); 
		GlobalFree (text); 
		GlobalFree (buff); 
	} 
	ExitProcess (0); 
} 