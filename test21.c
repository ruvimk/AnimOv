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
	char * msg= "Hello World!"; 
	file_put_contents ("hello.txt", msg, strlen (msg)); 
	ExitProcess (0); 
} 