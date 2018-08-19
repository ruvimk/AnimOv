#ifndef cos 
	#include <math.h> 
#endif 
#ifndef strcpy 
	#include <string.h> 
#endif 
#ifndef printf 
	#include <stdio.h> 
#endif 

#ifndef PX_UNDEFINED 
	#define PX_UNDEFINED      -65537 
#endif 

#ifndef DP_SCALAR 
	#define DP_SCALAR         1 
#endif 

#ifndef PI 
	#define PI                3.14159265 
#endif 

#ifndef NaN 
	#define NaN               1<<(sizeof (int) * 8 - 1) 
#endif 

#ifndef STRING_LIMIT 
	#define STRING_LIMIT      512 
#endif 

#define DEPTH_MAP_FILE_TYPE   0x4D444F41 
#define DEPTH_MAP_FILE_TYPE_B 0x414F444D 

#define MPS_OPENFILE OpenFile 
#define MPS_READFILE ReadFile 
#define MPS_WRITEFILE WriteFile 
#define MPS_CLOSEHANDLE CloseHandle 
#define MPS_FILE_GET_CONTENTS file_get_contents 
#define MPS_FILE_PUT_CONTENTS file_put_contents 

#define MPS_ALLOC GlobalAlloc 
#define MPS_FREE GlobalFree 

typedef struct { 
	long cbSize; 
	long reserved1; 
	char type[8]; 
	void * h; 
	void * hStream; 
	long oBaseMap; 
	long sBaseMap; 
	long oTileMap; 
	long sTileMap; 
	long oFrameCacheTable; 
	long sFrameCacheTable; 
	long oStreamDescriptorTable; 
	long sStreamDescriptorTable; 
	long oStream; 
} MPS_CONTEXT; 

typedef struct { 
	double x; 
	double y; 
	double z; 
} POINT3D; 

typedef struct { 
	double x; 
	double y; 
} POINT2D; 

typedef struct { 
	double x; 
	double y; 
	double z; 
	double angle; 
} PT_ANGLE; 

typedef struct { 
	unsigned char r; 
	unsigned char g; 
	unsigned char b; 
	unsigned char reserved; 
	long depth; 
} DEPTH_PIXEL; 

typedef struct { 
	unsigned char b; 
	unsigned char g; 
	unsigned char r; 
	unsigned char a; 
} BGRA; 

typedef struct { 
	long file_type; 
	long file_size; 
	long w; 
	long h; 
	long depth_scalar; 
} DEPTH_MAP_FILE_HEADER; 

typedef struct { 
	int startX; 
	int startY; 
	int stopX; 
	int stopY; 
} POINT_RANGE; 

long RenderEllipsoid_UsefulCasts= 0; 
long RenderEllipsoid_TotalCasts= 0; 

long profile_raycasting_before= 0; 
long profile_raycasting_during= 0; 

#define PROFILE_ON 1 

#define MAX_UNKNOWN_X         100 

// The RGB toon (contour) outline color: 
unsigned char toon_outline[4]= {0, 0, 0}; 

void LookUpProperty (char * definitions, char * name, char * output); 
void RotateAboutOrigin (double * pt, double * angles); 
char * scan_line (char * buffer, char * output, int max_output_size); 

/*
void file_put_contents (char * filename, char * text, int the_size); 

double LookUpNumber (char * definitions, char * name); 
void LookUpProperty (char * definitions, char * name, char * output); 

int FindParentObject (char * name, char * output); 

void Rel2Abs (double * pt, char * fg, char * fig); 
void RotateAboutOrigin (double * pt, double * angles); 

int RREF (double * matrix, int h, int w); 
int MatMult (double * matA, int hA, int wA, double * matB, int hB, int wB, double * result); 

char * file_get_contents (char * filename); 
char * scan_line (char * buffer, char * output, int max_output_size); 
*/

void debug_line (int number){ 
	#ifndef ALLOW_DEBUG_LINE 
		return; 
	#endif 
	unsigned long bWritten; 
	char string1[512]; 
	sprintf (string1, "data: line:%d\r\n\r\n", number); 
	WriteFile (GetStdHandle (STD_OUTPUT_HANDLE), string1, strlen (string1), &bWritten, 0); 
} 
void debug_file (char * fn){ 
	#ifndef ALLOW_DEBUG_FILE 
		return; 
	#endif 
	unsigned long bWritten; 
	char string1[512]; 
	sprintf (string1, "data: file:%s\r\n\r\n", fn); 
	WriteFile (GetStdHandle (STD_OUTPUT_HANDLE), string1, strlen (string1), &bWritten, 0); 
} 

HANDLE hStdOut= 0; 
long cprint (char * msg){ 
	unsigned long bWritten; 
	if (!hStdOut) hStdOut= GetStdHandle (STD_OUTPUT_HANDLE); 
	WriteFile (hStdOut, msg, strlen (msg), &bWritten, 0); 
	// if (bWritten != strlen (msg)) printf (":Last Error: %d\r\n", GetLastError ()); 
	if (GetEnvironmentVariable ("REMOTE_ADDR", 0, 0)){ 
		if (!FlushFileBuffers (hStdOut)){ 
			// printf (":Last Error: %d\r\n", GetLastError ()); 
			return 0; 
		} 
	} else if (!bWritten) return -1; 
	if (!msg[0]) return -1; 
	// printf (":%d bytes written, %d bytes expected\r\n", bWritten, strlen (msg)); 
	return (long)(bWritten); 
} 
long cwrite (void * the_data, int the_size){ 
	unsigned long bWritten; 
	if (!hStdOut) hStdOut= GetStdHandle (STD_OUTPUT_HANDLE); 
	WriteFile (hStdOut, the_data, the_size, &bWritten, 0); 
	if (GetEnvironmentVariable ("REMOTE_ADDR", 0, 0)){ 
		if (!FlushFileBuffers (hStdOut)) return 0; 
	} 
	if (!the_size) return -1; 
	return (long)(bWritten); 
} 