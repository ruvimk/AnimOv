#include <windows.h> 
#include <string.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <math.h> 

#include "/fseeko1.c" 

#include "toonhead.h" 

#include "/wminiz.c" 

double timeAt= 0; 

int f_draw; 
int f_compress; 

int fbuf_max_bytes= 1024 * 1024 * 48; 
int fbuf_unit_size; 
int fbuf_max; 
int fbuf_draw; 
int fbuf_compress; 

int draw_complete; 
int compress_complete; 

double fps; 
double len; 

char * llcbuf; 
char * vctbuf; 

DEPTH_PIXEL * fbuf_p; 

char output_mpc[512]; 
char extra_string[512]; 

#include "include/cmd.c" 
#include "include/wlib.c" 

HINSTANCE hInstance; 
LPSTR CommandLine; 

int main (){ 
	LPSTR CmdLine; 
	char * mps; 
	char * png; 
	char * bmp; 
	unsigned char * pMps; 
	hInstance= GetModuleHandle (0); 
	CmdLine= CommandLine= GetCommandLine (); 
	mps= SwitchValue (CmdLine, "mps"); 
	png= SwitchValue (CmdLine, "png"); 
	bmp= SwitchValue (CmdLine, "bmp"); 
	pMps= file_get_contents (mps); 
	if (pMps){ 
		if (pMps[0] == 'M' && pMps[1] == 'P' && pMps[2] == 'P' && pMps[3] == 'S'){ 
			
		} else { 
			printf ("Error:  Invalid MPS file. \r\n"); 
			ExitProcess (1); 
		} 
	} else { 
		printf ("Error:  Problem reading input MPS file into memory. \r\n"); 
		ExitProcess (1); 
	} 
	ExitProcess (0); 
} 