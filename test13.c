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
	char * vct= file_get_contents ("data/id17.aoc"); 
	InspectAnimOvCommands (vct); 
	file_put_contents ("data/id17_inspected.aoc", vct, strlen (vct)); 
	GlobalFree (vct); 
	ExitProcess (0); 
} 