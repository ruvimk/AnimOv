#include <windows.h> 
#include <string.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <math.h> 

#include "toonhead.h" 

#include "include/cmd.c" 
#include "include/wlib.c" 

HINSTANCE hInstance; 
LPSTR CommandLine; 

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

int startTime; 
int mainProfile; 

int main (){ 
	startTime= GetTickCount (); 
	F_NaN= sqrt (-1); 
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
	LPSTR CmndLine; 
	char * CmdFile= 0; 
	char * CmdBuff= 0; 
	DEPTH_MAP_FILE_HEADER dmfh; 
	HANDLE hFile; 
	OFSTRUCT ofs; 
	unsigned long extra1; 
	DepthScalar= DP_SCALAR; 
	hInstance= GetModuleHandle (0); 
	CmndLine= CommandLine= GetCommandLine (); 
	CmdFile= SwitchValue (CommandLine, "CmdFile"); 
	if (CmdFile){ 
		CmdBuff= file_get_contents (CmdFile); 
		GlobalFree (CmdFile); 
		if (CmdBuff && CmdBuff[0]){ 
			CommandLine= CmdBuff; 
		} 
	} 
	printf ("Starting up... \r\n"); 
	objColor[0]= objColor[1]= objColor[2]= 255; 
	RenderMode= SwitchValue (CommandLine, "RenderMode"); 
	SourceFile= SwitchValue (CommandLine, "SourceFile"); 
	printf (".\r\n"); 
	SourceFormat= SwitchValue (CommandLine, "SourceFormat"); 
	DrawOutlines= SwitchValue (CommandLine, "DrawOutlines"); 
	OutputBitmap= SwitchValue (CommandLine, "OutputBitmap"); 
	ExportBmMode= SwitchValue (CommandLine, "ExportBitmap"); 
	OutputMap= SwitchValue (CommandLine, "OutputMap"); 
	verbose= SwitchValue (CommandLine, "verbose"); 
	OutputW= SwitchValue (CommandLine, "w"); 
	OutputH= SwitchValue (CommandLine, "h"); 
	printf (".\r\n"); 
	cam_x= SwitchNumber (CommandLine, "cx"); 
	cam_y= SwitchNumber (CommandLine, "cy"); 
	cam_z= SwitchNumber (CommandLine, "cz"); 
	cam_aboutX= SwitchNumber (CommandLine, "crx"); 
	cam_aboutY= SwitchNumber (CommandLine, "cry"); 
	cam_aboutZ= SwitchNumber (CommandLine, "crz"); 
	cam_rh= SwitchNumber (CommandLine, "rh"); 
	cam_rv= SwitchNumber (CommandLine, "rv"); 
	cam_zoom= SwitchNumber (CommandLine, "czm"); 
	printf ("..\r\n"); 
	if (verbose) UseVerbose= 1; 
	else UseVerbose= 0; 
	if (!cam_rh) cam_rh= PI / 4; 
	if (!cam_rv) cam_rv= PI * 3 / 16; 
	printf ("Command Line:  %s\r\n\r\n", CommandLine); 
	if (!RenderMode || !SourceFile){ 
		printf ("Usage:  toon  -RenderMode render_mode -SourceFile source_filename -SourceFormat source_mime_type -OutputBitmap output_bitmap_filename "); 
		printf ("-ExportBitmap export_mode -OutputMap output_map_file_name [-verbose] [-CmdFile command_line_file_name] ... "); 
		printf ("\r\n\r\n"); 
		printf ("Render Modes: \r\n\t"); 
			printf ("default\tUse default toon rendering. \r\n\t"); 
		printf ("\r\n"); 
		printf ("Export Modes: \r\n\t"); 
			printf ("toon\tPlain color. \r\n\t"); 
			printf ("depth\tDefault; grayscale depth. \r\n\t"); 
			printf ("color\tObject color scaled according to depth. \r\n\t"); 
			printf ("linear-color\tObject color scaled linearly according to depth. \r\n\t"); 
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
			//if (DrawOutlines) printf ("Draw Outlines Enabled\r\n\t"); 
			// else printf ("Draw Outlines Disabled\r\n\t"); 
			if (OutputMap) printf ("Output Map: %s\r\n\r\n", OutputMap); 
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
			if (!strcmp (RenderMode, "default")){ 
				// Render mode default. 
				if (!SourceFormat || !strcmp (SourceFormat, "text/line-structured")){ 
					// Default source format. 
					BitmapData= GlobalAlloc (GMEM_ZEROINIT, main_w * main_h * 8 + 16); 
					for (i= 0; i < main_w; i++){ 
						for (j= 0; j < main_h; j++){ 
							lp= (long *)(BitmapData + main_w * 8 * j + 8 * i + 4); 
							*lp= PX_UNDEFINED; 
						} 
					} 
					if (ExportBmMode){ 
						if (!strcmp (ExportBmMode, "toon")) BmpExpMode= 0; 
						else if (!strcmp (ExportBmMode, "depth")) BmpExpMode= 1; 
						else if (!strcmp (ExportBmMode, "color")) BmpExpMode= 2; 
						else if (!strcmp (ExportBmMode, "linear-color")) BmpExpMode= 3; 
					} else BmpExpMode= 1; 
					printf ("\r\n\r\nRendering... \r\n\r\n"); 
					pScan= InputBuffer; 
					while (pScan= scan_line (pScan, current_line, sizeof (current_line))){ 
						if (UseVerbose && UseDebug) printf ("Scanning Line: %s\r\n", current_line); 
						if (!sscanf (current_line, " %[abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ] %[^;]", cmdString, cmdTail)) continue; 
						if (!strcmp (cmdString, "reset")){ 
							objColor[0]= objColor[1]= objColor[2]= 255; 
							objOutline[0]= objOutline[1]= objOutline[2]= 0; 
							objLine= 1; 
							objVector[0]= 0; 
							atX= atY= atZ= aboutX= aboutY= aboutZ= axisX= axisY= axisZ= 0; 
						} else if (!strcmp (cmdString, "color")){ 
							//a_number= sscanf (cmdTail, " RGB ( %lf , %lf , %lf )", &a, &b, &c); 
							a_number= sscanf (cmdTail, " %lf , %lf , %lf , %lf ", &a, &b, &c, &d); 
							if (!a_number){ 
								// printf ("sscanf (\"%s\", \" RGB ( lf , lf , lf )\", ...) \r\n", cmdTail); 
								// printf ("a_number=%d\r\n", a_number); 
								continue; 
							} 
							objColor[0]= (unsigned char)(a); 
							objColor[1]= (unsigned char)(b); 
							objColor[2]= (unsigned char)(c); 
						} else if (!strcmp (cmdString, "outline")){ 
							a_number= sscanf (cmdTail, " %lf , %lf , %lf , %lf ", &a, &b, &c, &d); 
							if (!a_number) continue; 
							objOutline[0]= (unsigned char)(a); 
							objOutline[1]= (unsigned char)(b); 
							objOutline[2]= (unsigned char)(c); 
						} else if (!strcmp (cmdString, "line")){ 
							a_number= sscanf (cmdTail, " %lf", &a); 
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
							a_number= sscanf (cmdTail, " %lf , %lf , %lf", &a, &b, &c); 
							if (!a_number) continue; 
							atX= a; 
							atY= b; 
							atZ= c; 
						} else if (!strcmp (cmdString, "about")){ 
							a_number= sscanf (cmdTail, " %lf , %lf , %lf", &a, &b, &c); 
							if (!a_number) continue; 
							aboutX= a; 
							aboutY= b; 
							aboutZ= c; 
						} else if (!strcmp (cmdString, "axes")){ 
							// if (UseVerbose && UseDebug) printf ("At axes; loading... \r\n"); 
							a_number= sscanf (cmdTail, " %lf , %lf , %lf", &a, &b, &c); 
							//if (UseVerbose && UseDebug) printf ("sscanf (\"%s\", \" f , f , f\", ...) returned %d\r\n", cmdTail, a_number); 
							if (!a_number) continue; 
							//if (UseVerbose && UseDebug) printf ("axes: %f, %f, %f\r\n", a, b, c); 
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
							pFirst= 0; 
						} else if (!strcmp (cmdString, "pt")){ 
							a_number= sscanf (cmdTail, " %lf , %lf , %lf", &a, &b, &c); 
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
							// printf ("RenderEllipsoid ();\r\n"); 
							RenderEllipsoid (); 
							// printf ("Returned. \r\n"); 
						} else if (!strcmp (cmdString, "fill")){ 
							RenderCurve (); 
						} else if (!strcmp (cmdString, "stroke")){ 
							StrokeCurve (); 
						} 
					} 
					if (OutputMap){ 
						dmfh.file_type= DEPTH_MAP_FILE_TYPE; 
						dmfh.file_size= sizeof (DEPTH_MAP_FILE_HEADER) + main_w * main_h * 8; 
						dmfh.w= main_w; 
						dmfh.h= main_h; 
						dmfh.depth_scalar= DepthScalar; 
						hFile= (HANDLE)(OpenFile (OutputMap, &ofs, 0x1001)); 
						if (hFile){ 
							WriteFile ((HANDLE)(hFile), &dmfh, sizeof (DEPTH_MAP_FILE_HEADER), &extra1, 0); 
							WriteFile ((HANDLE)(hFile), BitmapData, main_w * main_h * 8, &extra1, 0); 
							CloseHandle ((HANDLE)(hFile)); 
						} else printf ("Error:  Could not open file %s for writing. \r\n\r\nSystem Error Code: %d \r\n\r\n", OutputMap, GetLastError ()); 
					} 
					if (OutputBitmap){ 
						BitmapBuffer= ExportDepthPixelsToBitmap (BmpExpMode, main_w, main_h, (DEPTH_PIXEL *)(BitmapData)); 
						file_put_contents (OutputBitmap, BitmapBuffer, (int)(*((long *)((char *)(BitmapBuffer) + 2)))); 
						if (UseVerbose && UseDebug) printf ("OutputBitmap: %s\r\nBitmap File Size: %d\r\n", 
							OutputBitmap, 
							(int)(*((long *)((char *)(BitmapBuffer) + 2)))); 
					} 
					if (UseVerbose){ 
						printf ("Useful Ray-Casts: %d\r\nTotal Ray-Casts: %d\r\n", RenderEllipsoid_UsefulCasts, RenderEllipsoid_TotalCasts); 
					} 
				} 
			} 
		} else printf ("Source file not found or is blank; quitting... \r\n"); 
		/*printf ("RREF matrix: \r\n%f\t%f\t%f\r\n%f\t%f\t%f\r\n", 
				matrix2[0], matrix2[1], matrix2[2], 
				matrix2[3], matrix2[4], matrix2[5]); */
		//if (RREF (matrix1, h1, w1)){ 
			/*printf ("RREF matrix: \r\n%f\t%f\t%f\r\n%f\t%f\t%f\r\n", 
				matrix2[0], matrix2[1], matrix2[2], 
				matrix2[3], matrix2[4], matrix2[5]); */
			/*printf ("RREF matrix: \r\n%f\t%f\t%f\r\n%f\t%f\t%f\r\n%f\t%f\t%f\r\n", 
				matrix1[3], matrix1[4], matrix1[5], 
				matrix1[9], matrix1[10], matrix1[11], 
				matrix1[15], matrix1[16], matrix1[17]); 
		} */
		/*if (MatMult (mat1, mat1h, mat1w, mat2, mat2h, mat2w, &mat3[0])){ 
			printf ("Matrix Multiplication Result: \r\n%f\t%f\t%f\r\n%f\t%f\t%f\r\n%f\t%f\t%f\r\n", 
				mat3[0], mat3[1], mat3[2], 
				mat3[3], mat3[4], mat3[5], 
				mat3[6], mat3[7], mat3[8]); 
		} */
		//printf ("FindParentObject () => %d\r\n", FindParentObject (SourceFile, string1)); 
		//printf ("Parent (%s) = %s\r\n", SourceFile, string1); 
		//printf ("%s.%s", "one", "two"); 
	} 
	if (RenderMode) GlobalFree (RenderMode); 
	if (SourceFile) GlobalFree (SourceFile); 
	if (SourceFormat) GlobalFree (SourceFormat); 
	if (OutputBitmap) GlobalFree (OutputBitmap); 
	if (ExportBmMode) GlobalFree (ExportBmMode); 
	if (DrawOutlines) GlobalFree (DrawOutlines); 
	if (OutputMap) GlobalFree (OutputMap); 
	if (OutputW) GlobalFree (OutputW); 
	if (OutputH) GlobalFree (OutputH); 
	if (InputBuffer) GlobalFree (InputBuffer); 
	if (BitmapData) GlobalFree (BitmapData); 
	if (CmdBuff) GlobalFree (CmdBuff); 
	if (verbose) GlobalFree (verbose); 
	p1= pFirst; 
	do { 
		if (!p1) break; 
		p2= p1->pNext; 
		GlobalFree (p1); 
	} while (p1= (POINT_LINK *)(p2)); 
	mainProfile= GetTickCount () - startTime; 
	sprintf (string1, "toon.exe main profile: %d\r\n", mainProfile); 
	file_put_contents ("C:\\WBord\\wwwroot\\AnimOv\\toonprof.txt", string1, strlen (string1)); 
	ExitProcess (0); 
} 