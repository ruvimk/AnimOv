#include <windows.h> 
#include <string.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <math.h> 

#include "/fseeko2.c" 

#include "toonhead.h" 

#include "/wminiz.c" 

int debug_mode= 0; 

int all_stop= 0; 

double timeAt= 0; 

int render_start= 1; 
int render_stop= -1; 

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
char ** vctbin; 

int stroke_outlines= 0; 

DEPTH_PIXEL * fbuf_p; 

char llc_file[512]; 
char output_mpc[512]; 
char output_png[512]; 
char output_bmp[512]; 
char extra_string[512]; 
char extra_string2[512]; 
char extra_string3[512]; 
char extra_string4[512]; 

#include "include/cmd.c" 
#include "include/wlib.c" 

HANDLE hStdOut= 0; 
long cprint (char * msg){ 
	unsigned long bWritten; 
	if (!hStdOut) hStdOut= GetStdHandle (STD_OUTPUT_HANDLE); 
	WriteFile (hStdOut, msg, strlen (msg), &bWritten, 0); 
	if (GetEnvironmentVariable ("REMOTE_ADDR", 0, 0)){ 
		if (!FlushFileBuffers (hStdOut)) return 0; 
	} 
	return (long)(bWritten); 
} 

#include "animdraw.c" 
#include "animcomp.c" 

void MainInit (){ 
	LPSTR CmdLine= GetCommandLine (); 
	LPSTR orgCmdLine= CmdLine; 
	CmdLine= EnsureQueryHTTP (CmdLine); 
	char * AnimOv_Commands; 
	char * CmdsFile; 
	char * OutputBMP; 
	char * OutputPNG; 
	char * OutputMPC; 
	char * InputLLC; 
	char * RenderStart; 
	char * RenderStop; 
	int a01; 
	char string01[2048]; 
	int in_rel= 0; 
	llcbuf= ""; 
	f_draw= 1; 
	f_compress= 1; 
	GetCurrentDirectory (2048, string01); 
	if (strstr (string01, "AnimOv")) in_rel= 1; 
	// if (debug_mode) printf ("place marker %d\r\n", 1); 
	// fbuf_p= GlobalAlloc (GMEM_ZEROINIT, fbuf_max_bytes); 
	// if (!fbuf_p){ 
		// if (debug_mode) printf ("Content-Type: text/plain\r\n\r\nInternal server error. \r\n"); 
		// ExitProcess (1); 
	// } 
	if (SwitchNumber (CmdLine, "outline")) stroke_outlines= 1; 
	OutputPNG= SwitchValue (CmdLine, "png"); 
	if (OutputPNG){ 
		strncpy (output_png, OutputPNG, 512); 
		if (in_rel){ 
			if (strstr (output_png, "data\\") != output_png && strstr (output_png, "data/") != output_png){ 
				strcpy (extra_string, "data\\"); 
				strcat (extra_string, output_png); 
				strcpy (output_png, extra_string); 
			} 
		} else { 
			if (strstr (output_png, "AnimOv\\data\\") != output_png && strstr (output_png, "AnimOv/data/") != output_png){ 
				strcpy (extra_string, "AnimOv\\data\\"); 
				strcat (extra_string, output_png); 
				strcpy (output_png, extra_string); 
			} 
		} 
		GlobalFree (OutputPNG); 
	} else output_png[0]= 0; 
	OutputBMP= SwitchValue (CmdLine, "bmp"); 
	if (OutputBMP){ 
		strncpy (output_bmp, OutputBMP, 512); 
		if (in_rel){ 
			if (strstr (output_bmp, "data\\") != output_bmp && strstr (output_bmp, "data/") != output_bmp){ 
				strcpy (extra_string, "data\\"); 
				strcat (extra_string, output_bmp); 
				strcpy (output_bmp, extra_string); 
			} 
		} else { 
			if (strstr (output_bmp, "AnimOv\\data\\") != output_bmp && strstr (output_bmp, "AnimOv/data/") != output_bmp){ 
				strcpy (extra_string, "AnimOv\\data\\"); 
				strcat (extra_string, output_bmp); 
				strcpy (output_bmp, extra_string); 
			} 
		} 
		GlobalFree (OutputBMP); 
	} else output_bmp[0]= 0; 
	InputLLC= SwitchValue (CmdLine, "llc"); 
	if (InputLLC){ 
		strncpy (llc_file, InputLLC, 512); 
		if (in_rel){ 
			if (strstr (llc_file, "data\\") != llc_file && strstr (llc_file, "data/") != llc_file){ 
				strcpy (extra_string, "data\\"); 
				strcat (extra_string, llc_file); 
				strcpy (llc_file, extra_string); 
			} 
		} else { 
			if (strstr (llc_file, "AnimOv\\data\\") != llc_file && strstr (llc_file, "AnimOv/data/") != llc_file){ 
				strcpy (extra_string, "AnimOv\\data\\"); 
				strcat (extra_string, llc_file); 
				strcpy (llc_file, extra_string); 
			} 
		} 
		if (!strstr (llc_file, ".llc")){ 
			strcat (llc_file, ".llc"); 
		} 
		llcbuf= file_get_contents (llc_file); 
		GlobalFree (InputLLC); 
	} 
	// if (debug_mode) printf ("place marker %d\r\n", 2); 
	CmdsFile= SwitchValue (CmdLine, "aoc"); 
	if (!CmdsFile){ 
		a01= -1; 
		CmdsFile= SwitchValue (CmdLine, "id"); 
		if (CmdsFile){ 
			sscanf (CmdsFile, " %d", &a01); 
			GlobalFree (CmdsFile); 
			if (a01 != -1){ 
				CmdsFile= GlobalAlloc (GMEM_ZEROINIT, 512); 
				if (in_rel) sprintf (CmdsFile, "data\\id%d.aoc", a01); 
				else sprintf (CmdsFile, "AnimOv\\data\\id%d.aoc", a01); 
			} 
		} 
	} 
	// if (debug_mode) printf ("place marker %d\r\n", 3); 
	if (CmdsFile){ 
		AnimOv_Commands= file_get_contents (CmdsFile); 
		GlobalFree (CmdsFile); 
		if (AnimOv_Commands && AnimOv_Commands[0]){ 
			InspectAnimOvCommands (AnimOv_Commands); 
			vctbuf= ParseAnimOvCommands (AnimOv_Commands); 
			GlobalFree (AnimOv_Commands); 
			if (!vctbuf){ 
				cprint ("Content-Type: text/plain\r\n\r\nError:  Could not process the command file. \r\n"); 
				ExitProcess (1); 
			} 
		} else { 
			cprint ("Content-Type: text/plain\r\n\r\nError:  Command file is blank or does not exist. \r\n"); 
			ExitProcess (1); 
		} 
	} else { 
		cprint ("Content-Type: text/plain\r\n\r\nError:  No AnimOv command file specified. \r\n"); 
		ExitProcess (1); 
	} 
	// if (debug_mode) printf ("place marker %d\r\n", 4); 
	OutputMPC= SwitchValue (CmdLine, "mps"); 
	if (!OutputMPC){ 
		cprint ("Content-Type: text/plain\r\n\r\nError:  No MPC file specified. To specify an MPC file, use the -mpc switch. \r\n"); 
		ExitProcess (1); 
	} 
	strncpy (output_mpc, OutputMPC, 512); 
	GlobalFree (OutputMPC); 
	RenderStart= SwitchValue (CmdLine, "start"); 
	RenderStop= SwitchValue (CmdLine, "stop"); 
	if (RenderStart){ 
		sscanf (RenderStart, " %d", &render_start); 
		GlobalFree (RenderStart); 
	} 
	if (RenderStop){ 
		sscanf (RenderStop, " %d", &render_stop); 
		GlobalFree (RenderStop); 
	} 
	f_draw= render_start; 
	f_compress= render_start; 
	// if (debug_mode) printf ("place marker %d\r\n", 5); 
	InputBuffer= vctbuf; 
	main_w= (int)(SwitchNumber (CmdLine, "w")); 
	main_h= (int)(SwitchNumber (CmdLine, "h")); 
	if (!main_w) main_w= 640; 
	if (!main_h) main_h= 480; 
	fbuf_unit_size= main_w * main_h * 8; 
	fbuf_max= fbuf_max_bytes / fbuf_unit_size; 
	fbuf_max= 2; 
	// if (debug_mode) printf ("place marker %d\r\n", 6); 
	fps= LookUpNumber (vctbuf, "FPS"); 
	len= LookUpNumber (vctbuf, "LEN"); 
	if (!fps) fps= 12; 
	if (!len) len= 1; 
	fbuf_draw= f_draw - 1; 
	fbuf_compress= f_compress - 1; 
	while (fbuf_draw >= fbuf_max) fbuf_draw -= fbuf_max; 
	while (fbuf_compress >= fbuf_max) fbuf_compress -= fbuf_max; 
	draw_complete= 0; 
	compress_complete= 0; 
	// if (debug_mode) printf ("place marker %d\r\n", 7); 
	cprint ("Content-Type: text/event-stream\r\n"); 
	cprint ("Expires: -1\r\n"); 
	cprint ("\r\n"); 
	if (debug_mode) printf (":fbuf_max: %d\r\n", fbuf_max); 
	if (debug_mode) printf (":fbuf_unit_size: %d\r\n", fbuf_unit_size); 
	if (debug_mode) printf (":fbuf_max_bytes: %d\r\n", fbuf_max_bytes); 
	sprintf (string01, "data: 0 of %d; msg: Render initiated; rendering... \r\n\r\n", ((render_stop == -1)? (int)(len * fps):render_stop) - render_start + 1); 
	cprint (string01); 
	// sprintf (string01, "data: fps=%f, len=%f\r\n\r\n", fps, len); 
	// cprint (string01); 
	// sprintf (string01, "data: outputPNG=%s\r\n", output_png); 
	// cprint (string01); 
	vctbin= ImportPropertiesBin (vctbuf); 
	InputBuffer= (char *)(vctbin); 
	file_put_contents ("vctdebug.txt", vctbuf, strlen (vctbuf)); 
	char * tmp= ExportPropertiesBin (vctbin); 
	file_put_contents ("vctbindb.txt", tmp, strlen (tmp)); 
	GlobalFree (tmp); 
	CreateThread (0, 0, &WorkDraw, 0, 0, 0); 
	// CreateThread (0, 0, &WorkCompress, 0, 0, 0); 
	compress_complete= 1; 
	if (debug_mode) printf ("place marker %d\r\n", 8); 
	if (CmdLine != orgCmdLine) GlobalFree (CmdLine); 
	// UseDebugBox= 1; 
} 

HINSTANCE hInstance; 
LPSTR CommandLine; 

int startTime; 
int mainProfile; 

int main (){ 
	MainInit (); 
	if (debug_mode) printf ("place marker %d\r\n", 9); 
	// file_put_contents ("vctdebug.txt", vctbuf, strlen (vctbuf)); 
	// char * tmp= ExportPropertiesBin (vctbin); 
	// file_put_contents ("vctbindb.txt", tmp, strlen (tmp)); 
	// GlobalFree (tmp); 
	while (!(draw_complete && compress_complete) && !all_stop){ 
		Sleep (500); 
	} 
	if (debug_mode) printf ("Finished! \r\n"); 
	GlobalFree (vctbuf); 
	DestroyPropertiesBin (vctbin); 
	if (!all_stop) cprint ("data: done; msg: Render Complete!\r\n\r\n"); 
	else cprint ("data: Stopped. \r\n\r\n"); 
	ExitProcess (0); 
} 

int main2 (){ 
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
	if (debug_mode) printf ("Starting up... \r\n"); 
	objColor[0]= objColor[1]= objColor[2]= 255; 
	RenderMode= SwitchValue (CommandLine, "RenderMode"); 
	SourceFile= SwitchValue (CommandLine, "SourceFile"); 
	if (debug_mode) printf (".\r\n"); 
	SourceFormat= SwitchValue (CommandLine, "SourceFormat"); 
	DrawOutlines= SwitchValue (CommandLine, "DrawOutlines"); 
	OutputBitmap= SwitchValue (CommandLine, "OutputBitmap"); 
	ExportBmMode= SwitchValue (CommandLine, "ExportBitmap"); 
	OutputMap= SwitchValue (CommandLine, "OutputMap"); 
	verbose= SwitchValue (CommandLine, "verbose"); 
	OutputW= SwitchValue (CommandLine, "w"); 
	OutputH= SwitchValue (CommandLine, "h"); 
	if (debug_mode) printf (".\r\n"); 
	cam_x= SwitchNumber (CommandLine, "cx"); 
	cam_y= SwitchNumber (CommandLine, "cy"); 
	cam_z= SwitchNumber (CommandLine, "cz"); 
	cam_aboutX= SwitchNumber (CommandLine, "crx"); 
	cam_aboutY= SwitchNumber (CommandLine, "cry"); 
	cam_aboutZ= SwitchNumber (CommandLine, "crz"); 
	cam_rh= SwitchNumber (CommandLine, "rh"); 
	cam_rv= SwitchNumber (CommandLine, "rv"); 
	cam_zoom= SwitchNumber (CommandLine, "czm"); 
	if (debug_mode) printf ("..\r\n"); 
	if (verbose) UseVerbose= 1; 
	else UseVerbose= 0; 
	if (!cam_rh) cam_rh= PI / 4; 
	if (!cam_rv) cam_rv= PI * 3 / 16; 
	if (debug_mode) printf ("Command Line:  %s\r\n\r\n", CommandLine); 
	if (!RenderMode || !SourceFile){ 
		if (debug_mode) printf ("Usage:  toon  -RenderMode render_mode -SourceFile source_filename -SourceFormat source_mime_type -OutputBitmap output_bitmap_filename "); 
		if (debug_mode) printf ("-ExportBitmap export_mode -OutputMap output_map_file_name [-verbose] [-CmdFile command_line_file_name] ... "); 
		if (debug_mode) printf ("\r\n\r\n"); 
		if (debug_mode) printf ("Render Modes: \r\n\t"); 
			if (debug_mode) printf ("default\tUse default toon rendering. \r\n\t"); 
		if (debug_mode) printf ("\r\n"); 
		if (debug_mode) printf ("Export Modes: \r\n\t"); 
			if (debug_mode) printf ("toon\tPlain color. \r\n\t"); 
			if (debug_mode) printf ("depth\tDefault; grayscale depth. \r\n\t"); 
			if (debug_mode) printf ("color\tObject color scaled according to depth. \r\n\t"); 
			if (debug_mode) printf ("linear-color\tObject color scaled linearly according to depth. \r\n\t"); 
		if (debug_mode) printf ("\r\n"); 
		if (debug_mode) printf ("Currently-Supported Source Formats: \r\n\t"); 
			if (debug_mode) printf ("text/line-structured\tDefault. \r\n\t"); 
		if (debug_mode) printf ("\r\n"); 
	} else { 
		// if (debug_mode) printf ("Command Line Interpretation: \r\n\t"); 
			// if (RenderMode) if (debug_mode) printf ("Render Mode: %s\r\n\t", RenderMode); 
			// if (SourceFile) if (debug_mode) printf ("Source File: %s\r\n\t", SourceFile); 
			// if (SourceFormat) if (debug_mode) printf ("Source Format: %s\r\n\t", SourceFormat); 
			// if (OutputBitmap) if (debug_mode) printf ("Output Bitmap: %s\r\n\t", OutputBitmap); 
			//if (DrawOutlines) if (debug_mode) printf ("Draw Outlines Enabled\r\n\t"); 
			// else if (debug_mode) printf ("Draw Outlines Disabled\r\n\t"); 
			// if (OutputMap) if (debug_mode) printf ("Output Map: %s\r\n\r\n", OutputMap); 
		// if (debug_mode) printf ("\r\n\r\n"); 
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
					BitmapData= GlobalAlloc (GMEM_ZEROINIT, fbuf_unit_size + 16); 
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
					if (debug_mode) printf ("\r\n\r\nRendering... \r\n\r\n"); 
					pScan= InputBuffer; 
					while (pScan= scan_line (pScan, current_line, sizeof (current_line))){ 
						if (UseVerbose && UseDebug) if (debug_mode) printf ("Scanning Line: %s\r\n", current_line); 
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
								// if (debug_mode) printf ("sscanf (\"%s\", \" RGB ( lf , lf , lf )\", ...) \r\n", cmdTail); 
								// if (debug_mode) printf ("a_number=%d\r\n", a_number); 
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
							// if (UseVerbose && UseDebug) if (debug_mode) printf ("At axes; loading... \r\n"); 
							a_number= sscanf (cmdTail, " %lf , %lf , %lf", &a, &b, &c); 
							//if (UseVerbose && UseDebug) if (debug_mode) printf ("sscanf (\"%s\", \" f , f , f\", ...) returned %d\r\n", cmdTail, a_number); 
							if (!a_number) continue; 
							//if (UseVerbose && UseDebug) if (debug_mode) printf ("axes: %f, %f, %f\r\n", a, b, c); 
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
							// if (debug_mode) printf ("RenderEllipsoid ();\r\n"); 
							RenderEllipsoid (); 
							// if (debug_mode) printf ("Returned. \r\n"); 
						} else if (!strcmp (cmdString, "fill")){ 
							RenderCurve (); 
						} else if (!strcmp (cmdString, "stroke")){ 
							StrokeCurve (); 
						} 
					} 
					if (OutputMap){ 
						dmfh.file_type= DEPTH_MAP_FILE_TYPE; 
						dmfh.file_size= sizeof (DEPTH_MAP_FILE_HEADER) + fbuf_unit_size; 
						dmfh.w= main_w; 
						dmfh.h= main_h; 
						dmfh.depth_scalar= DepthScalar; 
						hFile= (HANDLE)(OpenFile (OutputMap, &ofs, 0x1001)); 
						if (hFile){ 
							WriteFile ((HANDLE)(hFile), &dmfh, sizeof (DEPTH_MAP_FILE_HEADER), &extra1, 0); 
							WriteFile ((HANDLE)(hFile), BitmapData, fbuf_unit_size, &extra1, 0); 
							CloseHandle ((HANDLE)(hFile)); 
						} else if (debug_mode) printf ("Error:  Could not open file %s for writing. \r\n\r\nSystem Error Code: %d \r\n\r\n", OutputMap, GetLastError ()); 
					} 
					if (OutputBitmap){ 
						BitmapBuffer= ExportDepthPixelsToBitmap (BmpExpMode, main_w, main_h, (DEPTH_PIXEL *)(BitmapData)); 
						file_put_contents (OutputBitmap, BitmapBuffer, (int)(*((long *)((char *)(BitmapBuffer) + 2)))); 
						if (UseVerbose && UseDebug) if (debug_mode) printf ("OutputBitmap: %s\r\nBitmap File Size: %d\r\n", 
							OutputBitmap, 
							(int)(*((long *)((char *)(BitmapBuffer) + 2)))); 
					} 
					if (UseVerbose){ 
						if (debug_mode) printf ("Useful Ray-Casts: %d\r\nTotal Ray-Casts: %d\r\n", RenderEllipsoid_UsefulCasts, RenderEllipsoid_TotalCasts); 
					} 
				} 
			} 
		} else if (debug_mode) printf ("Source file not found or is blank; quitting... \r\n"); 
		/*if (debug_mode) printf ("RREF matrix: \r\n%f\t%f\t%f\r\n%f\t%f\t%f\r\n", 
				matrix2[0], matrix2[1], matrix2[2], 
				matrix2[3], matrix2[4], matrix2[5]); */
		//if (RREF (matrix1, h1, w1)){ 
			/*if (debug_mode) printf ("RREF matrix: \r\n%f\t%f\t%f\r\n%f\t%f\t%f\r\n", 
				matrix2[0], matrix2[1], matrix2[2], 
				matrix2[3], matrix2[4], matrix2[5]); */
			/*if (debug_mode) printf ("RREF matrix: \r\n%f\t%f\t%f\r\n%f\t%f\t%f\r\n%f\t%f\t%f\r\n", 
				matrix1[3], matrix1[4], matrix1[5], 
				matrix1[9], matrix1[10], matrix1[11], 
				matrix1[15], matrix1[16], matrix1[17]); 
		} */
		/*if (MatMult (mat1, mat1h, mat1w, mat2, mat2h, mat2w, &mat3[0])){ 
			if (debug_mode) printf ("Matrix Multiplication Result: \r\n%f\t%f\t%f\r\n%f\t%f\t%f\r\n%f\t%f\t%f\r\n", 
				mat3[0], mat3[1], mat3[2], 
				mat3[3], mat3[4], mat3[5], 
				mat3[6], mat3[7], mat3[8]); 
		} */
		//if (debug_mode) printf ("FindParentObject () => %d\r\n", FindParentObject (SourceFile, string1)); 
		//if (debug_mode) printf ("Parent (%s) = %s\r\n", SourceFile, string1); 
		//if (debug_mode) printf ("%s.%s", "one", "two"); 
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
