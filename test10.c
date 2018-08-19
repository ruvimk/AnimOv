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
	if (argc < 3){ 
		printf ("2 arguments expected; had %d. \r\n", argc - 1); 
		ExitProcess (0); 
	} 
	char * vct= file_get_contents ("vctdebug.txt"); 
	char string1[512]; 
	LookUpPropertyEx (vct, argv[1], atof (argv[2]), string1); 
	printf ("Number: %s\r\n", string1); 
	GlobalFree (vct); 
	ExitProcess (0); 
} 