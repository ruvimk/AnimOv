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
	char * text= file_get_contents ("input1.txt"); 
	char * buff; 
	double where= 0; 
	//EnsurePath (buff, path, 0, where, 1); 
	//RenameProperties (buff, path, nset); 
	//DeleteProperties (buff, path); 
	buff= ParseAnimOvCommands (text); 
	file_put_contents ("output1.txt", buff, strlen (buff)); 
	GlobalFree (text); 
	GlobalFree (buff); 
	ExitProcess (0); 
} 