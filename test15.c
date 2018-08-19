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

int main (int argc, char * argv []){ 
	if (argc > 2){ 
		char string1[512]; 
		char string2[512]; 
		int i; 
		int j; 
		char path[512]; 
		char * nset= "two"; 
		char * text= file_get_contents ("input.txt"); 
		char ** buff= ImportPropertiesBin (text); 
		double where= 0; 
		double number; 
		int a1= sscanf (argv[1], " %lf", &where); 
		if (!a1){ 
			sscanf (argv[2], " %lf", &where); 
			strcpy (path, argv[1]); 
		} else { 
			strcpy (path, argv[2]); 
		} 
		// EnsurePath (buff, path, 0, where, 1); 
		number= LookUpNumberExBin (buff, path, where); 
		// LookUpPropertyExBin (buff, path, where, string2); 
		printf ("Number: %f\r\n", number); 
		// printf ("Text: %s\r\n", string2); 
		GlobalFree (text); 
		DestroyPropertiesBin (buff); 
	} 
	ExitProcess (0); 
} 