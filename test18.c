#include <windows.h> 

void ClearDir (char * path){ 
	char p[512]; 
	strcpy (p, path); 
	p[strlen (p) + 1]= 0; 
	SHFILEOPSTRUCT ops= { 
		0, 
		FO_DELETE, 
		0, 
		"\0\0", 
		FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT, 
		0, 
		0, 
		0 
	}; 
	ops.pFrom= p; 
	SHFileOperation (&ops); 
} 

int main (){ 
	ClearDir ("useless"); 
} 