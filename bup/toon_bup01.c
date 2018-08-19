#include <windows.h> 
#include <string.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <math.h> 

#include "include/cmd.h" 
#include "include/wlib.h" 

#include "include/cmd.c" 
#include "include/wlib.c" 

#define NO_INTERSECT      65536 

typedef struct { 
	void * pLast; 
	void * pNext; 
	double x; 
	double y; 
	double z; 
	double opc; 
	char vct[256]; 
} POINT_LINK; 

HINSTANCE hInstance; 
LPSTR CommandLine; 

int line_width= 1; 

int main_w= 400; 
int main_h= 300; 

char * RenderMode= 0; 
char * SourceFile= 0; 
char * SourceFormat= 0; 
char * DrawOutlines= 0; 
char * OutputBitmap= 0; 

char * OutputW= 0; 
char * OutputH= 0; 

char * InputBuffer= 0; 

char * BitmapData= 0; 

unsigned char objColor[3]; 
unsigned char objOutline[3]; 

double objLine; 

char objVector[1024]; 

double atX, atY, atZ; 
double aboutX, aboutY, aboutZ; 
double axisX, axisY, axisZ; 

char cmdString[1024]; 
char cmdTail[1024]; 

POINT_LINK * pFirst= 0; 

double matrix1[3*6]= { 
	8, 1, 4, 1, 0, 0, 
	7, 4, 1, 0, 1, 0, 
	3, 9, 6, 0, 0, 1 
}; 
int h1= 3; 
int w1= 6; 

double matrix2[2*3]= { 
	1, 2, 7, 
	3, 4, 8 
}; 
int h2= 2; 
int w2= 3; 

double mat1[]= { 
	1, 2, 3, 
	4, 5, 6, 
	7, 8, 9 
}; 
int mat1h= 3; 
int mat1w= 3; 

double mat2[]= { 
	11, 12, 13, 
	14, 15, 16, 
	17, 18, 19 
}; 
int mat2h= 3; 
int mat2w= 3; 

double mat3[9]= {0}; 

int main (){ 
	int i; 
	int j; 
	char * p; 
	long * lp; 
	char string1[512]; 
	char current_line[1024]; 
	char * pScan; 
	double a, b, c, d; 
	int a_number; 
	POINT_LINK * p1; 
	void * p2; 
	void * p3; 
	hInstance= GetModuleHandle (0); 
	CommandLine= GetCommandLine (); 
	RenderMode= SwitchValue (CommandLine, "RenderMode"); 
	SourceFile= SwitchValue (CommandLine, "SourceFile"); 
	SourceFormat= SwitchValue (CommandLine, "SourceFormat"); 
	DrawOutlines= SwitchValue (CommandLine, "DrawOutlines"); 
	OutputBitmap= SwitchValue (CommandLine, "OutputBitmap"); 
	OutputW= SwitchValue (CommandLine, "w"); 
	OutputH= SwitchValue (CommandLine, "h"); 
	if (!RenderMode || !SourceFile){ 
		printf ("Usage:  toon  -RenderMode render_mode -SourceFile source_filename -SourceFormat source_mime_type [-DrawOutlines] -OutputBitmap output_bitmap_filename\r\n\r\n"); 
		printf ("Render Modes: \r\n\t"); 
			printf ("toon\tUse default toon rendering. \r\n\t"); 
		printf ("\r\n"); 
		printf ("Currently-Supported Source Formats: \r\n\t"); 
			printf ("text/line-structured\tDefault. \r\n\t"); 
		printf ("\r\n"); 
	} else { 
		printf ("Command Line Interpretation: \r\n\t"); 
			if (RenderMode) printf ("Render Mode: %s\r\n\t", RenderMode); 
			if (SourceFile) printf ("Source File: %s\r\n\t", SourceFile); 
			if (SourceFormat) printf ("Source Format: %s\r\n\t", SourceFormat); 
			if (OutputBitmap) printf ("Output Bitmap: %s\r\n\t", OutputBitmap); 
			if (DrawOutlines) printf ("Draw Outlines Enabled\r\n\t"); 
			else printf ("Draw Outlines Disabled\r\n\t"); 
		printf ("\r\n\r\n"); 
		InputBuffer= file_get_contents (SourceFile); 
		if (InputBuffer[0]){ 
			if (OutputW) main_w= atoi (OutputW); 
			if (OutputH) main_h= atoi (OutputH); 
			if (!main_w) main_w= 400; 
			if (!main_h) main_h= 300; 
			// BitmapData format is a dynamic-sized array accessed by: 
			//   BitmapData[width * 8 * row + 8 * column] 
			// Each entry is a structure with the following: 
			//   RGB 3-Byte Descriptor, in R, G, B order. 
			//   Reserved Byte. 
			//   Long Depth Value; -1 is default. 
			if (!strcmp (RenderMode, "toon")){ 
				// Render mode toon. 
				if (!SourceFormat || !strcmp (SourceFormat, "text/line-structured")){ 
					// Default source format. 
					BitmapData= GlobalAlloc (GMEM_ZEROINIT, main_w * main_h * 8 + 16); 
					for (i= 0; i < main_w; i++){ 
						for (j= 0; j < main_h; j++){ 
							lp= (long *)(BitmapData + main_w * 8 * j + 8 * i + 4); 
							*lp= -1; 
						} 
					} 
					pScan= InputBuffer; 
					while (pScan= scan_line (pScan, current_line, sizeof (current_line))){ 
						if (!sscanf (current_line, " %[abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ] %s", cmdString, cmdTail)) continue; 
						if (!strcmp (cmdString, "reset")){ 
							objColor[0]= objColor[1]= objColor[2]= 255; 
							objOutline[0]= objOutline[1]= objOutline[2]= 0; 
							objLine= 1; 
							objVector[0]= 0; 
							atX= atY= atZ= aboutX= aboutY= aboutZ= axisX= axisY= axisZ= 0; 
						} else if (!strcmp (cmdString, "color")){ 
							a_number= sscanf (cmdTail, " RGB ( %f , %f , %f )", &a, &b, &c); 
							if (!a_number) a_number= sscanf (cmdTail, " RGBA ( %f , %f , %f , %f )", &a, &b, &c, &d); 
							if (!a_number) continue; 
							objColor[0]= (unsigned char)(a); 
							objColor[1]= (unsigned char)(b); 
							objColor[2]= (unsigned char)(c); 
						} else if (!strcmp (cmdString, "outline")){ 
							a_number= sscanf (cmdTail, " RGB ( %f , %f , %f )", &a, &b, &c); 
							if (!a_number) a_number= sscanf (cmdTail, " RGBA ( %f , %f , %f , %f )", &a, &b, &c, &d); 
							if (!a_number) continue; 
							objOutline[0]= (unsigned char)(a); 
							objOutline[1]= (unsigned char)(b); 
							objOutline[2]= (unsigned char)(c); 
						} else if (!strcmp (cmdString, "line")){ 
							a_number= sscanf (cmdTail, " %f", &a); 
							if (!a_number) continue; 
							objLine= a; 
						} else if (!strcmp (cmdString, "vector")){ 
							a_number= sscanf (cmdTail, " %s", objVector); 
							if (!a_number) continue; 
							for (i= strlen (objVector) - 1; i + 1; i--){ 
								if (objVector[i] != 32 && objVector[i] != 9) break; 
								objVector[i]= 0; 
							} 
						} else if (!strcmp (cmdString, "at")){ 
							a_number= sscanf (cmdTail, " %f , %f , %f", &a, &b, &c); 
							if (!a_number) continue; 
							atX= a; 
							atY= b; 
							atZ= c; 
						} else if (!strcmp (cmdString, "about")){ 
							a_number= sscanf (cmdTail, " %f , %f , %f", &a, &b, &c); 
							if (!a_number) continue; 
							aboutX= a; 
							aboutY= b; 
							aboutZ= c; 
						} else if (!strcmp (cmdString, "axes")){ 
							a_number= sscanf (cmdTail, " %f , %f , %f", &a, &b, &c); 
							if (!a_number) continue; 
							axisX= a; 
							axisY= b; 
							axisZ= c; 
						} else if (!strcmp (cmdString, "path")){ 
							p1= pFirst; 
							do { 
								if (!p1) break; 
								p2= p1->pNext; 
								GlobalFree (p1); 
							} while (p1= (POINT_LINK *)(p2)); 
						} else if (!strcmp (cmdString, "pt")){ 
							a_number= sscanf (cmdTail, " %f , %f , %f", &a, &b, &c); 
							if (!a_number) continue; 
							p1= pFirst; 
							if (p1){ 
								p2= (void *)(p1); 
								while (p1= (POINT_LINK *)(p1->pNext)) p2= (void *)(p1); 
								p1= (POINT_LINK *)(p2); 
								p2= GlobalAlloc (GMEM_ZEROINIT, sizeof (POINT_LINK)); 
								p1->pNext= p2; 
								p3= p1; 
								p1= (POINT_LINK *)(p2); 
								p1->pLast= p3; 
							} else { 
								pFirst= p1= GlobalAlloc (GMEM_ZEROINIT, sizeof (POINT_LINK)); 
							} 
							p1->opc= 1; 
							p1->x= a; 
							p1->y= b; 
							p1->z= c; 
							strncpy (p1->vct, objVector, 256); 
						} else if (!strcmp (cmdString, "ellipsoid")){ 
							
						} else if (!strcmp (cmdString, "fill")){ 
							
						} else if (!strcmp (cmdString, "stroke")){ 
							
						} 
					} 
				} 
			} 
		} else printf ("Source file not found or is blank; quitting... \r\n"); 
		/*printf ("RREF matrix: \r\n%f\t%f\t%f\r\n%f\t%f\t%f\r\n", 
				matrix2[0], matrix2[1], matrix2[2], 
				matrix2[3], matrix2[4], matrix2[5]); */
		if (RREF (matrix1, h1, w1)){ 
			/*printf ("RREF matrix: \r\n%f\t%f\t%f\r\n%f\t%f\t%f\r\n", 
				matrix2[0], matrix2[1], matrix2[2], 
				matrix2[3], matrix2[4], matrix2[5]); */
			printf ("RREF matrix: \r\n%f\t%f\t%f\r\n%f\t%f\t%f\r\n%f\t%f\t%f\r\n", 
				matrix1[3], matrix1[4], matrix1[5], 
				matrix1[9], matrix1[10], matrix1[11], 
				matrix1[15], matrix1[16], matrix1[17]); 
		} 
		if (MatMult (mat1, mat1h, mat1w, mat2, mat2h, mat2w, &mat3[0])){ 
			printf ("Matrix Multiplication Result: \r\n%f\t%f\t%f\r\n%f\t%f\t%f\r\n%f\t%f\t%f\r\n", 
				mat3[0], mat3[1], mat3[2], 
				mat3[3], mat3[4], mat3[5], 
				mat3[6], mat3[7], mat3[8]); 
		} 
		printf ("FindParentObject () => %d\r\n", FindParentObject (SourceFile, string1)); 
		printf ("Parent (%s) = %s\r\n", SourceFile, string1); 
		printf ("%s.%s", "one", "two"); 
	} 
	if (RenderMode) GlobalFree (RenderMode); 
	if (SourceFile) GlobalFree (SourceFile); 
	if (SourceFormat) GlobalFree (SourceFormat); 
	if (OutputBitmap) GlobalFree (OutputBitmap); 
	if (DrawOutlines) GlobalFree (DrawOutlines); 
	if (OutputW) GlobalFree (OutputW); 
	if (OutputH) GlobalFree (OutputH); 
	if (InputBuffer) GlobalFree (InputBuffer); 
	if (BitmapData) GlobalFree (BitmapData); 
	ExitProcess (0); 
} 

double RayEllipsoid (double s0, double s1, double s2, double rX, double rY, double rZ, 
	double a, double b, double c, double re0, double re1, double re2, double rd0, double rd1, double rd2, double * result1, double * result2){ 
	*result1= NO_INTERSECT; 
	*result2= NO_INTERSECT; 
	if (!(a * b * c)) return NO_INTERSECT; 
	if (!(rd0 + rd1 + rd2)) return NO_INTERSECT; 
	double cosRX= cos (rX); 
	double sinRX= sin (rX); 
	double cosRY= cos (rY); 
	double sinRY= sin (rY); 
	double cosRZ= cos (rZ); 
	double sinRZ= sin (rZ); 
	double r00= cosRY * cosRZ; 
	double r01= cosRY * sinRZ; 
	double r02= -sinRY; 
	double r10= sinRX * sinRY * cosRZ - cosRX * sinRZ; 
	double r11= sinRX * sinRY * sinRZ + cosRX * cosRZ; 
	double r12= sinRX * cosRY; 
	double r20= cosRX * sinRY * cosRZ + sinRX * sinRZ; 
	double r21= cosRX * sinRY * sinRZ - sinRX * cosRZ; 
	double r22= cosRX * cosRY; 
	double sre0= re0 - s0; 
	double sre1= re1 - s1; 
	double sre2= re2 - s2; 
	double e0= r00 * sre0 + r01 * sre1 + r02 * sre2; 
	double e1= r10 * sre0 + r11 * sre1 + r12 * sre2; 
	double e2= r20 * sre0 + r21 * sre1 + r22 * sre2; 
	double d0= r00 * rd0 + r01 * rd1 + r02 * rd2; 
	double d1= r10 * rd0 + r11 * rd1 + r12 * rd2; 
	double d2= r20 * rd0 + r21 * rd1 + r22 * rd2; 
	double a4= pow (a, 4); 
	double b4= pow (b, 4); 
	double c4= pow (c, 4); 
	double oa4= 1 / a4; 
	double ob4= 1 / b4; 
	double oc4= 1 / c4; 
	double d02= pow (d0, 2); 
	double d12= pow (d1, 2); 
	double d22= pow (d2, 2); 
	double e02= pow (e0, 2); 
	double e12= pow (e1, 2); 
	double e22= pow (e2, 2); 
	double qA= oa4 * d02 + ob4 * d12 + oc4 * d22; 
	double qB= oa4 * 2 * e0 * d0 + ob4 * 2 * e1 * d1 + oc4 * 2 * e2 * d2; 
	double qC= oa4 * e02 + ob4 * e12 + oc4 * e22 - 1; 
	double srParam= qB * qB - 4 * qA * qC; 
	if (srParam < 0) return NO_INTERSECT; 
	double srValue= sqrt (srParam); 
	double qDivisor= 2 * qA; 
	if (srParam == 0){ 
		*result1= (-qB + srValue) / qDivisor; 
		return *result1; 
	} 
	double t0= (-qB + srValue) / qDivisor; 
	double t1= (-qB - srValue) / qDivisor; 
	*result1= t0; 
	*result2= t1; 
	return t0; 
} 

