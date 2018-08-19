#include <windows.h> 
#include <string.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <math.h> 

void Anim_ReloadLLC (); 
void Anim_ReloadVCT (); 

#include "fseeko2.c" 

#include "toonhead.h" 

#include "wminiz.c" 

// Last known good backup: bup05 

int debug_mode= 0; 
int use_debug_dots= 0; 

int all_stop= 0; 

double timeAt= 0; 

int render_start= 1; 
int render_stop= -1; 

double d_render_start= 1; 
double d_render_stop= -1; 

int f_draw; 
int f_compress; 

double d_draw; 

int fbuf_max_bytes= 1024 * 1024 * 48; 
int fbuf_unit_size; 
int fbuf_max; 
int fbuf_draw; 
int fbuf_compress; 

int draw_complete; 
int compress_complete; 

int draw_start_frame= 0; 
int draw_last_frame= 0; 

double fps; 
double len; 

char * llcbuf; 
char * vctbuf; 
char ** vctbin; 

long * llcbuf2; 

int stroke_outlines= 0; 

int use_pk= 0; 
int prev_mode= 0; 

// Work mode; 1 = render/save , 2 = live preview 
int work_mode= 1; 

int prev_id= 0; 
int prev_fps= 0; 

int id= 0; 

int AnimOv_Commands_len; 

FILETIME aoc_last; 
char aoc_fn[1024]; 

char PrevCmdFn[1024]; 

DEPTH_PIXEL * fbuf_p; 

char sel_vect[512]; 

char llc_file[512]; 
char llcb_file[512]; 
char output_mpc[512]; 
char output_png[512]; 
char output_bmp[512]; 
char output_zip[512]; 
char extra_string[512]; 
char extra_string2[512]; 
char extra_string3[512]; 
char extra_string4[512]; 

char base_frame_dir[512]; 

// void debug_line (int number){ 
	// unsigned long bWritten; 
	// char string1[512]; 
	// sprintf (string1, "data: line:%d\r\n\r\n", number); 
	// WriteFile (GetStdHandle (STD_OUTPUT_HANDLE), string1, strlen (string1), &bWritten, 0); 
// } 
// void debug_file (char * fn){ 
	// unsigned long bWritten; 
	// char string1[512]; 
	// sprintf (string1, "data: file:%s\r\n\r\n", fn); 
	// WriteFile (GetStdHandle (STD_OUTPUT_HANDLE), string1, strlen (string1), &bWritten, 0); 
// } 

char main_debug[8192]; 

#include "include/cmd.c" 
#include "include/wlib.c" 

#include "include/xmllib.c" 
#include "animface.c" 

// HANDLE hStdOut= 0; 
// long cprint (char * msg){ 
	// unsigned long bWritten; 
	// if (!hStdOut) hStdOut= GetStdHandle (STD_OUTPUT_HANDLE); 
	// WriteFile (hStdOut, msg, strlen (msg), &bWritten, 0); 
	// // if (bWritten != strlen (msg)) printf (":Last Error: %d\r\n", GetLastError ()); 
	// if (GetEnvironmentVariable ("REMOTE_ADDR", 0, 0)){ 
		// if (!FlushFileBuffers (hStdOut)){ 
			// // printf (":Last Error: %d\r\n", GetLastError ()); 
			// return 0; 
		// } 
	// } else if (!bWritten) return -1; 
	// if (!msg[0]) return -1; 
	// // printf (":%d bytes written, %d bytes expected\r\n", bWritten, strlen (msg)); 
	// return (long)(bWritten); 
// } 
// long cwrite (void * the_data, int the_size){ 
	// unsigned long bWritten; 
	// if (!hStdOut) hStdOut= GetStdHandle (STD_OUTPUT_HANDLE); 
	// WriteFile (hStdOut, the_data, the_size, &bWritten, 0); 
	// if (GetEnvironmentVariable ("REMOTE_ADDR", 0, 0)){ 
		// if (!FlushFileBuffers (hStdOut)) return 0; 
	// } 
	// if (!the_size) return -1; 
	// return (long)(bWritten); 
// } 

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

DWORD mainThreadID; 

void Anim_UpdateVCT (); 

#include "animdraw.c" 
#include "animcomp.c" 

void Anim_ReloadLLC (){ 
	GlobalFree (llcbuf); 
	llcbuf= file_get_contents (llc_file); 
	GlobalFree (llcbuf2); 
	llcbuf2= (long *)(file_get_contents (llcb_file)); 
} 
void Anim_UpdateVCT (){ 
	char * AnimOv_Commands; 
	LARGE_INTEGER sFile; 
	HANDLE hFile= CreateFile (aoc_fn, GENERIC_READ, 7, 0, 3, 128, 0); 
	char * pNewCmd; 
	int to_read; 
	unsigned long bRead= 0; 
	sFile.LowPart= GetFileSize (hFile, (unsigned long *)(&sFile.HighPart)); 
	// GetFileSizeEx (hFile, &sFile); 
	SetFilePointer (hFile, AnimOv_Commands_len, 0, 0); 
	to_read= sFile.LowPart - AnimOv_Commands_len; 
	pNewCmd= GlobalAlloc (GMEM_ZEROINIT, to_read + 16); 
	AnimOv_Commands= pNewCmd; 
	ReadFile (hFile, pNewCmd, to_read, &bRead, 0); 
	CloseHandle (hFile); 
	AnimOv_Commands_len += bRead; 
	InspectAnimOvCommands (AnimOv_Commands); 
	ExecAnimOvCommands (AnimOv_Commands, vctbuf, vctbin, &vctbuf, &vctbin); 
	InputBuffer= (char *)(vctbin); 
	fps= LookUpNumber (vctbuf, "FPS"); 
	len= LookUpNumber (vctbuf, "LEN"); 
	if (!fps) fps= 12; 
	if (!len) len= 1; 
	GlobalFree (AnimOv_Commands); 
} 
void Anim_ReloadVCT (){ 
	// Anim_UpdateVCT (); 
	// return; 
	char * AnimOv_Commands; 
	AnimOv_Commands= file_get_contents (aoc_fn); 
	if (AnimOv_Commands){ 
		// file_put_contents ("animov_cmd_debug2.txt", AnimOv_Commands, strlen (AnimOv_Commands)); 
		InspectAnimOvCommands (AnimOv_Commands); 
		// file_put_contents ("animov_cmd_debug.txt", AnimOv_Commands, strlen (AnimOv_Commands)); 
			if (vctbuf) GlobalFree (vctbuf); 
			// main_debug[0]= 0; 
		vctbuf= ParseAnimOvCommands (AnimOv_Commands); 
		fps= LookUpNumber (vctbuf, "FPS"); 
		len= LookUpNumber (vctbuf, "LEN"); 
		if (!fps) fps= 12; 
		if (!len) len= 1; 
			if (vctbin) DestroyPropertiesBin (vctbin); 
		vctbin= ImportPropertiesBin (vctbuf); 
		InputBuffer= (char *)(vctbin); 
		AnimOv_Commands_len= strlen (AnimOv_Commands); 
		GlobalFree (AnimOv_Commands); 
	} 
	
	if (!cam_rh) cam_rh= PI / 4; 
	if (!cam_rv) cam_rv= PI * 3 / 16; 
	
	// file_put_contents ("vctdebug.txt", vctbuf, strlen (vctbuf)); 
	// char * tmp= ExportPropertiesBin (vctbin); 
	// file_put_contents ("vctbindb.txt", tmp, strlen (tmp)); 
	// GlobalFree (tmp); 
	
	/*AnimOv_Commands= file_get_contents_modified (aoc_fn, &aoc_last); 
	// strncpy (aoc_fn, CmdsFile, sizeof (aoc_fn)); 
	// GlobalFree (CmdsFile); 
	if (AnimOv_Commands && AnimOv_Commands[0]){ 
		InspectAnimOvCommands (AnimOv_Commands); 
		vctbuf= ParseAnimOvCommands (AnimOv_Commands); 
		AnimOv_Commands_len= strlen (AnimOv_Commands); 
		GlobalFree (AnimOv_Commands); 
		// if (!vctbuf){ 
			// cprint ("Content-Type: text/plain\r\n\r\nError:  Could not process the command file. \r\n"); 
			// ExitProcess (1); 
		// } 
	} 
	// if (debug_mode) printf ("place marker %d\r\n", 4); 
	// OutputMPC= SwitchValue (CmdLine, "mps"); 
	// if (!OutputMPC && work_mode != 2){ 
		// cprint ("Content-Type: text/plain\r\n\r\nError:  No MPC file specified. To specify an MPC file, use the -mpc switch. \r\n"); 
		// ExitProcess (1); 
	// } else if (!OutputMPC){ 
		// OutputMPC= GlobalAlloc (GMEM_ZEROINIT, 512); 
		// strcpy (OutputMPC, "1"); 
	// } 
	// strncpy (output_mpc, OutputMPC, 512); 
	// GlobalFree (OutputMPC); 
	// RenderStart= SwitchValue (CmdLine, "start"); 
	// RenderStop= SwitchValue (CmdLine, "stop"); 
	// if (RenderStart){ 
		// sscanf (RenderStart, " %d", &render_start); 
		// GlobalFree (RenderStart); 
	// } 
	// if (RenderStop){ 
		// sscanf (RenderStop, " %d", &render_stop); 
		// GlobalFree (RenderStop); 
	// } 
	// f_draw= render_start; 
	// f_compress= render_start; 
	// d_draw= d_render_start; 
	InputBuffer= vctbuf; 
	// if (!main_w) main_w= 640; 
	// if (!main_h) main_h= 480; 
	fps= LookUpNumber (vctbuf, "FPS"); 
	len= LookUpNumber (vctbuf, "LEN"); 
	if (!fps) fps= 12; 
	if (!len) len= 1; 
	vctbin= ImportPropertiesBin (vctbuf); 
	InputBuffer= (char *)(vctbin); 
	// file_put_contents ("vctdebug.txt", vctbuf, strlen (vctbuf)); 
	// char * tmp= ExportPropertiesBin (vctbin); 
	// file_put_contents ("vctbindb.txt", tmp, strlen (tmp)); 
	// GlobalFree (tmp); */
} 

void MainInit (){ 
	LPSTR CmdLine= GetCommandLine (); 
	LPSTR orgCmdLine= CmdLine; 
	CmdLine= EnsureQueryHTTP (CmdLine); 
	char * AnimOv_Commands; 
	char * CmdsFile; 
	char * OutputBMP; 
	char * OutputPNG; 
	char * OutputMPC; 
	char * OutputZIP; 
	char * InputLLC; 
	char * RenderStart; 
	char * RenderStop; 
	int a01; 
	char string01[2048]; 
	DWORD processes[512]; 
	DWORD plsize; 
	DWORD * procID; 
	DWORD attributes; 
	HANDLE hProc; 
	// HANDLE hThreadSnap; 
	int in_rel= 0; 
	// UseDebugBox= 1; 
	llcbuf= ""; 
	llcbuf2= 0; 
	f_draw= 1; 
	f_compress= 1; 
	GetCurrentDirectory (2048, string01); 
	if (strstr (string01, "AnimOv")) in_rel= 1; 
	sel_vect[0]= 0; 
	max_smart_area= pow (1024, 3); 
	// file_put_contents ("cmd_debug_anim.txt", CmdLine, strlen (CmdLine)); 
	// if (debug_mode) printf ("place marker %d\r\n", 1); 
	// fbuf_p= GlobalAlloc (GMEM_ZEROINIT, fbuf_max_bytes); 
	// if (!fbuf_p){ 
		// if (debug_mode) printf ("Content-Type: text/plain\r\n\r\nInternal server error. \r\n"); 
		// ExitProcess (1); 
	// } 
	// ;;Now interrupt any other rendering process working on this same animation. 
	// Now exit if there is already another rendering process working on this same animation. 
		// EnumProcesses (processes, sizeof (processes), &plsize); 
		// for (procID= processes; (int)(procID - processes) < plsize; procID += sizeof (DWORD)){ 
			
		// } 
		// BroadcastSystemMessage (BSF_IGNORECURRENTTASK | BSF_POSTMESSAGE, 0, WM_ERASEBKGND, 0, id); 
		// ;; Do this later. 
	// Go on with other things. 
	if (SwitchNumber (CmdLine, "prev")) prev_mode= 1; 
	if (prev_id= (int)(SwitchNumber (CmdLine, "outline")) == 1) stroke_outlines= 1; 
	else stroke_outlines= prev_id; 
	if (prev_id= (int)(SwitchNumber (CmdLine, "prev_id"))){ 
		work_mode= 2; 
		if (in_rel) sprintf (PrevCmdFn, "prev/c%d.txt", prev_id); 
		else sprintf (PrevCmdFn, "AnimOv/prev/c%d.txt", prev_id); 
		file_put_contents (PrevCmdFn, "", 0); 
	} 
	id= (int)(SwitchNumber (CmdLine, "id")); 
	d_render_start= SwitchNumber (CmdLine, "start"); 
	d_render_stop= SwitchNumber (CmdLine, "stop"); 
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
		// llcbuf= file_get_contents (llc_file); 
		llcbuf= GlobalAlloc (GMEM_ZEROINIT, 16); 
		GlobalFree (InputLLC); 
	} 
	InputLLC= SwitchValue (CmdLine, "lcb"); 
	if (InputLLC){ 
		strncpy (llcb_file, InputLLC, 512); 
		if (in_rel){ 
			if (strstr (llcb_file, "data\\") != llcb_file && strstr (llcb_file, "data/") != llcb_file){ 
				strcpy (extra_string, "data\\"); 
				strcat (extra_string, llcb_file); 
				strcpy (llcb_file, extra_string); 
			} 
		} else { 
			if (strstr (llcb_file, "AnimOv\\data\\") != llcb_file && strstr (llcb_file, "AnimOv/data/") != llcb_file){ 
				strcpy (extra_string, "AnimOv\\data\\"); 
				strcat (extra_string, llcb_file); 
				strcpy (llcb_file, extra_string); 
			} 
		} 
		if (!strstr (llcb_file, ".lcb")){ 
			strcat (llcb_file, ".lcb"); 
		} 
		llcbuf2= (long *)(file_get_contents (llcb_file)); 
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
	OutputZIP= SwitchValue (CmdLine, "zip"); 
	if (OutputZIP){ 
		strncpy (output_zip, OutputZIP, 512); 
		if (in_rel){ 
			// if (strstr (output_zip, "data\\") != output_zip && strstr (output_zip, "data/") != output_zip){ 
				// strcpy (extra_string, "data\\"); 
				// strcat (extra_string, output_zip); 
				// strcpy (output_zip, extra_string); 
			// } 
		} else { 
			// if (strstr (output_zip, "AnimOv\\data\\") != output_zip && strstr (output_zip, "AnimOv/data/") != output_zip){ 
				// strcpy (extra_string, "AnimOv\\data\\"); 
				// strcat (extra_string, output_zip); 
				// strcpy (output_zip, extra_string); 
			// } 
		} 
		if (!strstr (output_zip, ".zip")){ 
			strcat (output_zip, ".zip"); 
		} 
		use_pk= 1; 
		GlobalFree (OutputZIP); 
	} else output_zip[0]= 0; 
	// if (debug_mode) printf ("place marker %d\r\n", 3); 
	if (CmdsFile){ 
		AnimOv_Commands= file_get_contents_modified (CmdsFile, &aoc_last); 
		strncpy (aoc_fn, CmdsFile, sizeof (aoc_fn)); 
		GlobalFree (CmdsFile); 
		if (AnimOv_Commands && AnimOv_Commands[0]){ 
			InspectAnimOvCommands (AnimOv_Commands); 
			vctbuf= ParseAnimOvCommands (AnimOv_Commands); 
			AnimOv_Commands_len= strlen (AnimOv_Commands); 
			GlobalFree (AnimOv_Commands); 
			if (!vctbuf){ 
				cprint ("Content-Type: text/plain\r\n\r\nError:  Could not process the command file. \r\n"); 
				ExitProcess (1); 
			} 
		} else if (AnimOv_Commands){ 
			vctbuf= ParseAnimOvCommands (AnimOv_Commands); 
			AnimOv_Commands_len= 0; 
			GlobalFree (AnimOv_Commands); 
			if (!vctbuf){ 
				cprint ("Content-Type: text/plain\r\n\r\nError:  Could not process the command file. \r\n"); 
				ExitProcess (1); 
			} 
		} else { 
			cprint ("Content-Type: text/plain\r\n\r\nError:  Command file is blank or does not exist. \r\n"); 
			// sprintf (extra_string, "Filename: %s\r\n", aoc_fn); 
			// cprint (extra_string); 
			ExitProcess (1); 
		} 
	} else { 
		cprint ("Content-Type: text/plain\r\n\r\nError:  No AnimOv command file specified. \r\n"); 
		ExitProcess (1); 
	} 
	// if (debug_mode) printf ("place marker %d\r\n", 4); 
	OutputMPC= SwitchValue (CmdLine, "mps"); 
	if (!OutputMPC && work_mode != 2){ 
		cprint ("Content-Type: text/plain\r\n\r\nError:  No MPC file specified. To specify an MPC file, use the -mpc switch. \r\n"); 
		ExitProcess (1); 
	} else if (!OutputMPC){ 
		OutputMPC= GlobalAlloc (GMEM_ZEROINIT, 512); 
		strcpy (OutputMPC, "1"); 
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
	d_draw= d_render_start; 
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
	if (work_mode == 1){ 
		sprintf (string01, "data: 0 of %d; msg: Render initiated; rendering... \r\n\r\n", ((render_stop == -1)? (int)(len * fps):render_stop) - render_start + 1); 
		cprint (string01); 
	} 
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
	if (output_zip[0] && work_mode == 1){ 
		// extra_string[0]= 0; 
		strcpy (extra_string, "C:\\WBord\\wwwroot"); 
		GetEnvironmentVariable ("DOCUMENT_ROOT", extra_string, 512); 
		if ((extra_string2[0]= extra_string[strlen (extra_string) - 1]) != 92 && extra_string2[0] != 47) strcat (extra_string, "/"); 
		sprintf (extra_string2, "%s%s%d", extra_string, "AnimOv/data/id", id); 
		strcpy (base_frame_dir, extra_string2); 
		attributes= GetFileAttributes (base_frame_dir); 
		if (attributes != -1 && (attributes & FILE_ATTRIBUTE_DIRECTORY)){ 
			all_stop= 1; 
			// DestroyWindow (hSecretWnd); 
			// while (!draw_complete) Sleep (20); 
			// Sleep (100); 
			GlobalFree (vctbuf); 
			DestroyPropertiesBin (vctbin); 
			ClearDir (base_frame_dir); 
			sprintf (extra_string, "data: halt; msg: Busy. \r\n\r\n"); 
			cprint (extra_string); 
			ExitProcess (0); 
		} 
		CreateDirectory (extra_string2, 0); 
		// file_put_contents ("animdebug1.txt", extra_string2, strlen (extra_string2)); 
	} 
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

LRESULT CALLBACK WndProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){ 
	return DefWindowProc (hWnd, uMsg, wParam, lParam); 
} 

int main (){ 
	STARTUPINFO s_i; 
	PROCESS_INFORMATION p_i; 
	int i; 
	unsigned long exit_code; 
	MSG msg; 
	WNDCLASSEX wc= {sizeof (WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, &WndProc, 0, 0, 0, 0, 0, (HBRUSH)(COLOR_WINDOW + 1), 
		0, "SecretWindowClasss", 0}; 
		wc.hInstance= hInstance; 
		// wc.hIcon= wc.hIconSm= LoadIcon (0, IDI_APPLICATION); 
		// wc.hCursor= LoadCursor (0, IDC_ARROW); 
	// ATOM wcls= RegisterClassEx (&wc); 
	unsigned long error_code; 
	char string1[512]; 
	char * s1; 
		// sprintf (string1, "AnimOv Render Process"); 
	// HWND hSecretWnd= CreateWindowEx (0, "SecretWindowClasss", string1, WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, 0, 0, hInstance, 0); 
	// sprintf (string1, "New window error: %d\r\n", GetLastError ()); 
	// if (!hSecretWnd || (int)(hSecretWnd) == -1) file_put_contents ("anim_debug_02.txt", string1, strlen (string1)); 
	// ShowWindow (hSecretWnd, SW_HIDE); 
	// UpdateWindow (hSecretWnd); 
	mainThreadID= GetCurrentThreadId (); 
	MainInit (); 
	if (debug_mode) printf ("place marker %d\r\n", 9); 
	// file_put_contents ("vctdebug.txt", vctbuf, strlen (vctbuf)); 
	// char * tmp= ExportPropertiesBin (vctbin); 
	// file_put_contents ("vctbindb.txt", tmp, strlen (tmp)); 
	// GlobalFree (tmp); 
	while (GetMessage (&msg, 0, 0, 0)){ 
		TranslateMessage (&msg); 
		if (msg.message == WM_CLOSE || msg.message == WM_QUIT || msg.message == WM_DESTROY){ 
			ReplyMessage (0); 
			// file_put_contents ("anim_debug_02.txt", "WM_CLOSE message received. ", 27); 
			all_stop= 1; 
			// DestroyWindow (hSecretWnd); 
			while (!draw_complete) Sleep (20); 
			// Sleep (100); 
			GlobalFree (vctbuf); 
			DestroyPropertiesBin (vctbin); 
			ClearDir (base_frame_dir); 
			sprintf (extra_string, "data: halt; msg: Interrupted. \r\n\r\n"); 
			cprint (extra_string); 
			ExitProcess (0); 
		} else if (msg.message == WM_USER + 1){ 
			ReplyMessage (0); 
			break; 
		} else if (msg.message == WM_USER + 2){ 
			// The request ID message. 
			ReplyMessage (id); 
		} else if (msg.message == WM_NULL){ 
			ReplyMessage (0); 
			s1= file_get_contents ("anim_debug_03.txt"); 
			sprintf (string1, "WM_NULL received; wParam: %d; lParam: %d;\r\n", s1, msg.wParam, msg.lParam); 
			s1= GlobalReAlloc (s1, strlen (s1) + strlen (string1) + 16, GMEM_ZEROINIT); 
			strcat (s1, string1); 
			file_put_contents ("anim_debug_03.txt", string1, strlen (string1)); 
			GlobalFree (s1); 
			if (msg.wParam == id){ 
				// PostThreadMessage (mainThreadID, WM_CLOSE, 0, 0); 
				all_stop= 1; 
				// DestroyWindow (hSecretWnd); 
				while (!draw_complete) Sleep (20); 
				// Sleep (100); 
				GlobalFree (vctbuf); 
				DestroyPropertiesBin (vctbin); 
				ClearDir (base_frame_dir); 
				sprintf (extra_string, "data: halt; msg: Interrupted. \r\n\r\n"); 
				cprint (extra_string); 
				ExitProcess (0); 
			} 
		} else if (msg.message == WM_ERASEBKGND){ 
			ReplyMessage (0); 
			s1= file_get_contents ("anim_debug_03.txt"); 
			sprintf (string1, "WM_ERASEBKGND received; wParam: %d; lParam: %d;\r\n", s1, msg.wParam, msg.lParam); 
			s1= GlobalReAlloc (s1, strlen (s1) + strlen (string1) + 16, GMEM_ZEROINIT); 
			strcat (s1, string1); 
			file_put_contents ("anim_debug_03.txt", string1, strlen (string1)); 
			GlobalFree (s1); 
			if (msg.wParam == id){ 
				// PostThreadMessage (mainThreadID, WM_CLOSE, 0, 0); 
				all_stop= 1; 
				// DestroyWindow (hSecretWnd); 
				while (!draw_complete) Sleep (20); 
				// Sleep (100); 
				GlobalFree (vctbuf); 
				DestroyPropertiesBin (vctbin); 
				ClearDir (base_frame_dir); 
				sprintf (extra_string, "data: halt; msg: Interrupted. \r\n\r\n"); 
				cprint (extra_string); 
				ExitProcess (0); 
			} 
		} else { 
			ReplyMessage (0); 
		} 
	} 
	// DestroyWindow (hSecretWnd); 
	// while (!(draw_complete && compress_complete) && !all_stop){ 
		// Sleep (500); 
	// } 
	if (debug_mode) printf ("Finished! \r\n"); 
	GlobalFree (vctbuf); 
	DestroyPropertiesBin (vctbin); 
	if (output_zip[0]){ 
		if (!all_stop){ 
			sprintf (extra_string, "data: %d of %d; msg: Packing data... \r\n\r\n", f_draw - render_start, 
				((render_stop + 1)? render_stop + 1 - render_start: (int)(len * fps))); 
			cprint (extra_string); 
		} else { 
			cprint ("data: Stopped. \r\n\r\n"); 
			ExitProcess (0); 
		} 
		for (i= 0; i < sizeof (STARTUPINFO); i++) ((char *)(&s_i))[i]= 0; 
		for (i= 0; i < sizeof (PROCESS_INFORMATION); i++) ((char *)(&p_i))[i]= 0; 
		s_i.cb= sizeof (STARTUPINFO); 
		// SetCurrentDirectory ("AnimOv"); 
		// SetCurrentDirectory ("data"); 
		{ 
			char outZipPath [32 + strlen (output_zip)]; 
			sprintf (outZipPath, "data/%s", output_zip); 
			DeleteFile (outZipPath); 
		} 
		// SetCurrentDirectory (base_frame_dir); 
		/*if (output_png[0]) */sprintf (extra_string, "7za a -tzip -mx9 ../%s *.png", output_zip, id); // Work relative to id%s directory, so 7za puts all the PNGs in the root folder of the output ZIP. 
		cprint ("data: Executing command: "); 
		cprint (extra_string); 
		cprint ("\r\n\r\n"); 
		// else if (output_bmp[0]) sprintf (extra_string, "7za a -tzip -mx9 ../%s *.png", output_zip, id); 
		// else { 
			// extra_string[0]= 0; 
			// sprintf (extra_string, "data: done; msg: Render Complete! (frames %d..%d)\r\n\r\n", draw_start_frame, draw_last_frame); 
			// if (!all_stop) cprint (extra_string); 
			// else cprint ("data: Stopped. \r\n\r\n"); 
			// extra_string[0]= 0; 
			// ExitProcess (0); 
		// } 
		for (i= 0; extra_string[i]; i++){ 
			if (!memcmp (&extra_string[i], "AnimOv\\data\\", 12) || !memcmp (&extra_string[i], "AnimOv/data/", 12)){ 
				memmove (&extra_string[i], &extra_string[i+12], 12); 
				i--; 
			} 
		} 
		// strcpy (extra_string2, extra_string); 
		char workingDir [32]; 
		sprintf (workingDir, "data/id%d", id); 
		if (CreateProcess ("7za.exe", extra_string, 0, 0, 0, CREATE_NO_WINDOW, 0, workingDir, &s_i, &p_i)){ 
			do { 
				if (!GetExitCodeProcess (p_i.hProcess, &exit_code)){ 
					exit_code= STILL_ACTIVE; 
					break; 
				} 
			} while (exit_code == STILL_ACTIVE); 
			cprint (":\r\n\r\n"); 
			if (!exit_code){ 
				sprintf (extra_string, "data: done; msg: Render Complete! (frames %d..%d)\r\n\r\n", draw_start_frame, draw_last_frame); 
				cprint (extra_string); 
			} else { 
				sprintf (extra_string, "data: done; msg: Archive Packing Failed! (Exit Code: %d)\r\n\r\n", exit_code); 
				cprint (extra_string); 
				ClearDir (base_frame_dir); 
				ExitProcess (exit_code); 
			} 
		} else { 
			sprintf (extra_string, "data: done; msg: Error: Could not start archive-packaging process. (Error Code: %d)\r\n\r\n", GetLastError ()); 
			cprint (extra_string); 
			ClearDir (base_frame_dir); 
			ExitProcess (1); 
		} 
	} 
	sprintf (extra_string, "data: done; msg: Render Complete! (frames %d..%d)\r\n\r\n", draw_start_frame, draw_last_frame); 
	if (!all_stop) cprint (extra_string); 
	else cprint ("data: Stopped. \r\n\r\n"); 
	ClearDir (base_frame_dir); 
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
