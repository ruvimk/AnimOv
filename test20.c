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
	if (argc < 2) ExitProcess (0); 
	// char * cmd= strchr (GetCommandLine (), 32) + 1; 
		// if (!cmd) ExitProcess (0); 
	char * a; 
	char * cmd= argv[1]; 
		// while (a= strchr (cmd, 32)) cmd= a + 1; 
	int the_size= 17682; 
	// char * pFile= file_get_contents_size (cmd, &the_size); 
		// SetCurrentDirectory ("C:/WBord/wwwroot/AnimOv"); 
		cmd= "dp.png"; 
	char * pFile= file_get_contents (cmd); 
	char * pBase64= base64_encode ((unsigned char *)(pFile), the_size); 
	file_put_contents ("test20output.txt", pBase64, strlen (pBase64)); 
	GlobalFree (pFile); 
	printf ("Input: %s (%d bytes)\r\nOutput: %s (%d bytes)\r\n", cmd, the_size, "test20output.txt", strlen (pBase64)); 
	GlobalFree (pBase64); 
	ExitProcess (0); 
} 