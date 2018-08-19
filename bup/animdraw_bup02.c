#define                 BIN_ELLIPSOID      0x27 

void CopyMemory01 (void * to, void * from, int the_size, void * dest_base){ 
	int * pTo= (int *)(to); 
	int * pFrom= (int *)(from); 
	char * cTo= (char *)(to); 
	char * cFrom= (char *)(from); 
	int i; 
	int sizeI1= the_size / sizeof (int) - 1; 
	int max_dest= GlobalSize (dest_base); 
	if ((long)(to) < (long)(dest_base)){ 
		printf (":copy error: destination pointer below buffer\r\n"); 
		return; 
	} 
	if ((long)(to) + the_size > (long)(dest_base) + max_dest){ 
		printf (":copy error: destination pointer above buffer\r\n"); 
		return; 
	} 
	printf ("copy to %d from %d len %d\r\n", pTo, pFrom, the_size); 
	for (i= 0; i < sizeI1; i++){ 
		if ((long)(&pTo[i]) > (long)(dest_base) + max_dest){ 
			printf (":copy error: buffer overrun\r\n"); 
			return; 
		} 
		// printf ("int i %d, ", i); 
		pTo[i]= pFrom[i]; 
	} 
	for (i *= sizeof (int); i < the_size; i++){ 
		if ((long)(&pTo[i]) > (long)(dest_base) + max_dest){ 
			printf (":copy error: buffer overrun\r\n"); 
			return; 
		} 
		// printf ("byte i %d\r\n", i); 
		cTo[i]= cFrom[i]; 
	} 
	printf ("%d bytes copied\r\n", i); 
} 

void InitBitmapData (DEPTH_PIXEL * BmpDat){ 
	long * lp; 
	DEPTH_PIXEL * p; 
	int i; 
	int j; 
	// for (i= 0; i < main_w; i++){ 
		// for (j= 0; j < main_h; j++){ 
			// //if (debug_mode) printf ("InitBitmapData (%d, %d) <%d>\r\n", i, j, main_w * 8 * j + 8 * i + 4); 
			// // lp= (long *)(BmpDat + main_w * 8 * j + 8 * i + 0); 
			// // *lp= 0; 
			// // lp= (long *)(BmpDat + main_w * 8 * j + 8 * i + 4); 
			// // *lp= PX_UNDEFINED; 
			// // if (debug_mode) printf ("(%d,%d);", i, j); 
			// asm { 
				// mov eax, dword ptr [BmpDat] 
				// mov ebx, eax 
				// mov eax, dword ptr [main_w] 
				// mov ecx, 8 
				// mul ecx 
				// mov ecx, eax 
				// mov eax, dword ptr [j] 
				// mul ecx 
				// add ebx, eax 
				// mov eax, dword ptr [i] 
				// mov ecx, 8 
				// mul ecx 
				// add ebx, eax 
				// mov dword ptr [ebx+0], 0 
				// mov dword ptr [ebx+4], PX_UNDEFINED 
			// } 
		// } 
	// } 
	for (i= 0; i < main_w * main_h; i++){ 
		p= &BmpDat[i]; 
		*((long *)(p))= 0; 
		p->depth= PX_UNDEFINED; 
	} 
	if (debug_mode) printf ("InitBitmapData complete; \r\n"); 
} 
void ProcessToonLowlevelCode (char * InputCode){ 
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
	objColor[0]= objColor[1]= objColor[2]= 255; 
	objOutline[0]= objOutline[1]= objOutline[2]= 0; 
	objLine= 1; 
	objVector[0]= 0; 
	atX= atY= atZ= aboutX= aboutY= aboutZ= axisX= axisY= axisZ= 0; 
	pScan= InputCode; 
	//printf ("\r\n\r\nstarting scan loop... \r\n"); 
	while (pScan= scan_line (pScan, current_line, sizeof (current_line))){ 
		// printf ("scanning line: %s\r\n", current_line); 
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
			//printf ("render ellipsoid... \r\n"); 
			/*printf ("render ellipsoid (%d, %d, %d), ~(%d, %d, %d), x(%d, %d, %d); \r\n", (int)atX, (int)atY, (int)atZ, (int)aboutX, (int)aboutY, 
				(int)aboutZ, (int)axisX, (int)axisY, (int)axisZ); */
			// UseVerbose= 1; 
			// UseDebug= 1; 
			RenderEllipsoid (); 
			// UseVerbose= 0; 
			// UseDebug= 0; 
			// if (debug_mode) printf ("Returned. \r\n"); 
		} else if (!strcmp (cmdString, "fill")){ 
			RenderCurve (); 
		} else if (!strcmp (cmdString, "stroke")){ 
			StrokeCurve (); 
			// if (debug_mode) printf ("one01 place marker %d\r\n", 2); 
			// GetStdHandle (STD_OUTPUT_HANDLE); 
		} 
		GetStdHandle (STD_OUTPUT_HANDLE); 
	} 
	p1= pFirst; 
	do { 
		if (!p1) break; 
		p2= p1->pNext; 
		GlobalFree (p1); 
	} while (p1= (POINT_LINK *)(p2)); 
	pFirst= 0; 
} 
double * ImportToonLowLevelCodeBin (char * InputCode){ 
	int i; 
	int cnt= 1; 
	int ibin= 0; 
	double * bin; 
	int maxbin; 
		for (i= 0; InputCode[i]; i++) if (InputCode[i] == 10) cnt++; 
		maxbin= cnt * 512 + cnt * 12 * sizeof (double); 
		bin= GlobalAlloc (GMEM_ZEROINIT, maxbin); 
	F_NaN= sqrt (-1); 
	// int i; 
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
	objColor[0]= objColor[1]= objColor[2]= 255; 
	objOutline[0]= objOutline[1]= objOutline[2]= 0; 
	objLine= 1; 
	objVector[0]= 0; 
	atX= atY= atZ= aboutX= aboutY= aboutZ= axisX= axisY= axisZ= 0; 
	pScan= InputCode; 
	//printf ("\r\n\r\nstarting scan loop... \r\n"); 
	while (pScan= scan_line (pScan, current_line, sizeof (current_line))){ 
		// printf ("scanning line: %s\r\n", current_line); 
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
			//printf ("render ellipsoid... \r\n"); 
			/*printf ("render ellipsoid (%d, %d, %d), ~(%d, %d, %d), x(%d, %d, %d); \r\n", (int)atX, (int)atY, (int)atZ, (int)aboutX, (int)aboutY, 
				(int)aboutZ, (int)axisX, (int)axisY, (int)axisZ); */
			// UseVerbose= 1; 
			// UseDebug= 1; 
			// // RenderEllipsoid (); 
			bin[ibin+0]= BIN_ELLIPSOID; 
			bin[ibin+1]= atX; 
			bin[ibin+2]= atY; 
			bin[ibin+3]= atZ; 
			bin[ibin+4]= aboutX; 
			bin[ibin+5]= aboutY; 
			bin[ibin+6]= aboutZ; 
			bin[ibin+7]= axisX; 
			bin[ibin+8]= axisY; 
			bin[ibin+9]= axisZ; 
			((long *)(&bin[ibin+10]))[0]= *((long *)(objColor)); 
			((long *)(&bin[ibin+10]))[1]= *((long *)(objOutline)); 
			bin[ibin+11]= objLine; 
			strncpy ((char *)(&bin[ibin+12]), objVector, 416); 
			ibin += 64; 
			// UseVerbose= 0; 
			// UseDebug= 0; 
			// if (debug_mode) printf ("Returned. \r\n"); 
		} else if (!strcmp (cmdString, "fill")){ 
			// RenderCurve (); 
			
		} else if (!strcmp (cmdString, "stroke")){ 
			// StrokeCurve (); 
			
			// if (debug_mode) printf ("one01 place marker %d\r\n", 2); 
			// GetStdHandle (STD_OUTPUT_HANDLE); 
		} 
		GetStdHandle (STD_OUTPUT_HANDLE); 
	} 
	p1= pFirst; 
	do { 
		if (!p1) break; 
		p2= p1->pNext; 
		GlobalFree (p1); 
	} while (p1= (POINT_LINK *)(p2)); 
	pFirst= 0; 
} 
unsigned long WINAPI WorkDraw (void * param){ 
	double t= (f_draw - 1) / fps; 
	double ofps= 1 / fps; 
	double tfps; 
	char camname[1024]; 
	char camstr[1024]; 
	char string001[1024]; 
	double number; 
	int a, b; 
	int f; 
	int i; 
	DEPTH_PIXEL * pFrame; 
	DEPTH_PIXEL * frameA= GlobalAlloc (GMEM_ZEROINIT, fbuf_unit_size + 16); 
	size_t png_file_size; 
	void * pBmp; 
	void * pPng; 
	char extra_str[2048]; 
	char extra_str2[512]; 
	int frame_digits= 1; 
	if (prev_mode) t= (d_draw - 1) / fps; 
	sprintf (extra_str, "%d", (int)(ceil (len * fps))); 
	frame_digits= strlen (extra_str); 
	extra_str[0]= 0; 
	if (debug_mode) printf (":draw allocating %d bytes \r\n", fbuf_unit_size + 16); 
	GetStdHandle (STD_OUTPUT_HANDLE); 
	BitmapData= (char *)(frameA); 
	if (debug_mode) printf (":draw place marker %d\r\n", 1); 
	GetStdHandle (STD_OUTPUT_HANDLE); 
	camname[0]= 0; 
	if (!cam_rh) cam_rh= PI / 4; 
	if (!cam_rv) cam_rv= PI * 3 / 16; 
	for (; t < len && (f_draw <= render_stop || render_stop == -1) && !all_stop; t += ofps){ 
		timeAt= t; 
		if (all_stop) break; 
		if (f_draw == render_start) draw_start_frame= f_draw; 
		draw_last_frame= f_draw; 
		LookUpPropertyExBin (vctbin, "CAM", t, camname); 
		// sprintf (extra_str, "Debug:   Camera name (t = %f): %s\r\n", t, camname); 
		GetStdHandle (STD_OUTPUT_HANDLE); 
		// cprint (extra_str); 
		sprintf (camstr, "[CAM]%s.x", camname); 
		cam_x= LookUpNumberExBin (vctbin, camstr, t); 
		sprintf (camstr, "[CAM]%s.y", camname); 
		cam_y= LookUpNumberExBin (vctbin, camstr, t); 
		// sprintf (extra_str, "Debug: cam_y(%s)=%f\r\n", camstr, cam_y); 
		// cprint (extra_str); 
		sprintf (camstr, "[CAM]%s.z", camname); 
		cam_z= LookUpNumberExBin (vctbin, camstr, t); 
		sprintf (camstr, "[CAM]%s.aboutX", camname); 
		cam_aboutX= LookUpNumberExBin (vctbin, camstr, t); 
		sprintf (camstr, "[CAM]%s.aboutY", camname); 
		cam_aboutY= LookUpNumberExBin (vctbin, camstr, t); 
		sprintf (camstr, "[CAM]%s.aboutZ", camname); 
		cam_aboutZ= LookUpNumberExBin (vctbin, camstr, t); 
		sprintf (camstr, "[CAM]%s.rh", camname); 
		number= LookUpNumberExBin (vctbin, camstr, t); 
		if (number) cam_rh= number; 
		sprintf (camstr, "[CAM]%s.rv", camname); 
		number= LookUpNumberExBin (vctbin, camstr, t); 
		if (number) cam_rv= number; 
		sprintf (camstr, "[CAM]%s.zoom", camname); 
		number= LookUpNumberExBin (vctbin, camstr, t); 
		cam_zoom= number; 
		// printf ("Camera (%s) Information (t = %f): \r\n\tAt: (%f, %f, %f); \r\n\tAbout: (%f, %f, %f); \r\n\trh: %f, rv: %f; \r\n\tZoom: %f\r\n", 
			// camname, t, cam_x, cam_y, cam_z, cam_aboutX, cam_aboutY, cam_aboutZ, cam_rh, cam_rv, cam_zoom); 
		tfps= t * fps; 
		a= floor (tfps); 
		b= ceil (tfps); 
		if (tfps - a > b - tfps) f= b; 
		else f= a; 
		if (debug_mode) printf (":draw place marker %d\r\n", 2); 
		InitBitmapData (frameA); 
		if (debug_mode) printf (":draw place marker %d b\r\n", 2); 
		GetStdHandle (STD_OUTPUT_HANDLE); 
		ProcessToonLowlevelCode (llcbuf); 
		if (debug_mode) printf (":draw place marker %d\r\n", 3); 
		GetStdHandle (STD_OUTPUT_HANDLE); 
		// asm { 
			// mov eax, dword ptr [fbuf_p] 
			// mov ebx, eax 
			// mov eax, dword ptr [fbuf_draw] 
			// mov ecx, eax 
			// mov eax, dword ptr [main_w] 
			// mul ecx 
			// mov ecx, eax 
			// mov eax, dword ptr [main_h] 
			// mul ecx 
			// mov ecx, 8 
			// mul ecx 
			// add ebx, eax 
			// mov eax, ebx 
			// mov dword ptr [pFrame], eax 
		// } 
		// pFrame= fbuf_p; 
		// pFrame += fbuf_draw * fbuf_unit_size; 
		// pFrame= fbuf_p + fbuf_draw * main_w * main_h * 8; 
		// if (debug_mode) printf (":rel pFrame: %d; pFrame - fbuf_p: %d; fbuf_draw: %d\r\n", fbuf_draw * fbuf_unit_size, pFrame - fbuf_p, fbuf_draw); 
		// if (debug_mode){ 
			// printf (":\tfbuf_p size: %d\r\n", GlobalSize (fbuf_p)); 
			// printf (":\tpFrame size: %d\r\n", GlobalSize (pFrame)); 
			// printf (":\tframeA size: %d\r\n", GlobalSize (frameA)); 
		// } 
		// memmove (pFrame, frameA, fbuf_unit_size); 
		// CopyMemory01 (pFrame, frameA, fbuf_unit_size, fbuf_p); 
		if (output_png[0]){ 
			sprintf (extra_string2, "%s%d.png", output_png, f_draw); 
			if (prev_mode){ 
				sprintf (extra_str2, "%f", d_draw); 
				i= (int)(strchr (extra_str2, '.')); 
				if (i){ 
					i -= (int)(extra_str2); 
					extra_str2[i+3]= 0; 
				} 
				sprintf (extra_string2, "%s%s.png", output_png, extra_str2); 
			} 
			if (debug_mode) printf (":Output PNG: %s\r\n", extra_string2); 
			pBmp= ExportDepthPixelsToBitmap ((stroke_outlines? 4:3*2), main_w, main_h, frameA); 
			if (output_bmp[0]){ 
				sprintf (extra_string3, "%s%d.bmp", output_bmp, f_draw); 
				file_put_contents (extra_string3, pBmp, *((long *)((char *)pBmp + 2))); 
			} 
			// img_offset= (BGRA *)((char *)(pBmp) + *((long *)((char *)(pBmp) + 10))); 
			// for (i= 0; i < main_w * main_h; i++){ 
				// clr= &img_offset[i]; 
				// j= clr->r; 
				// clr->r= clr->b; 
				// clr->b= j; 
			// } 
			// pPng= tdefl_write_image_to_png_file_in_memory (img_offset, main_w, main_h, 4, &png_file_size); 
			// printf ("\tpBmp: %d\r\n", (int)(pBmp)); 
			pPng= BmpToPngInMemory (pBmp, &png_file_size); 
			// printf ("\tpPng: %d\r\n", (int)(pPng)); 
			file_put_contents (extra_string2, pPng, png_file_size); 
			if (use_pk){ 
				sprintf (extra_string, "%d", f_draw); 
				EnsureDigits (extra_string, frame_digits); 
				sprintf (extra_string2, "%s/frame%s.png", base_frame_dir, extra_string); 
				file_put_contents (extra_string2, pPng, png_file_size); 
			} 
			// printf ("\tWritten to file. \r\n"); 
			MZ_FREE (pPng); 
			GlobalFree (pBmp); 
			sprintf (string001, "data: %d of %d; msg: Rendering... \r\n\r\n", f_draw - render_start, 
				((render_stop + 1)? render_stop + 1 - render_start: (int)(len * fps))); 
			cprint (string001); 
			// if (!cprint (string001)) goto compress__stop_script_immediately; 
			// goto cont00101; 
		} else if (use_pk){ 
			// sprintf (extra_string2, "%s%d.png", output_png, f_draw); 
			// if (debug_mode) printf (":Output PNG: %s\r\n", extra_string2); 
			pBmp= ExportDepthPixelsToBitmap ((stroke_outlines? 4:3*2), main_w, main_h, frameA); 
			// if (output_bmp[0]){ 
				// sprintf (extra_string3, "%s%d.bmp", output_bmp, f_draw); 
				// file_put_contents (extra_string3, pBmp, *((long *)((char *)pBmp + 2))); 
			// } 
			// img_offset= (BGRA *)((char *)(pBmp) + *((long *)((char *)(pBmp) + 10))); 
			// for (i= 0; i < main_w * main_h; i++){ 
				// clr= &img_offset[i]; 
				// j= clr->r; 
				// clr->r= clr->b; 
				// clr->b= j; 
			// } 
			// pPng= tdefl_write_image_to_png_file_in_memory (img_offset, main_w, main_h, 4, &png_file_size); 
			// printf ("\tpBmp: %d\r\n", (int)(pBmp)); 
			pPng= BmpToPngInMemory (pBmp, &png_file_size); 
			// printf ("\tpPng: %d\r\n", (int)(pPng)); 
			// file_put_contents (extra_string2, pPng, png_file_size); 
			// if (use_pk){ 
				sprintf (extra_string, "%d", f_draw); 
				EnsureDigits (extra_string, frame_digits); 
				sprintf (extra_string2, "%s/frame%s.png", base_frame_dir, extra_string); 
				file_put_contents (extra_string2, pPng, png_file_size); 
			// } 
			// printf ("\tWritten to file. \r\n"); 
			MZ_FREE (pPng); 
			GlobalFree (pBmp); 
			sprintf (string001, "data: %d of %d; msg: Rendering... \r\n\r\n", f_draw - render_start, 
				((render_stop + 1)? render_stop + 1 - render_start: (int)(len * fps))); 
			cprint (string001); 
			// if (!cprint (string001)) goto compress__stop_script_immediately; 
			// goto cont00101; 
		} 
		if (debug_mode) printf (":draw place marker %d\r\n", 4); 
		// if (output_png[0]){ 
			// sprintf (extra_string4, "%s%d.bmp", output_png, f_draw); 
			// pBmp= ExportDepthPixelsToBitmap ((stroke_outlines? 4:3), main_w, main_h, (DEPTH_PIXEL *)BitmapData); 
			// //pPng= tdefl_write_image_to_png_file_in_memory (pBmp, main_w, main_h, 4, &png_file_size); 
			// file_put_contents (extra_string4, pBmp, *((long *)((char *)pBmp + 2))); 
			// //MZ_FREE (pPng); 
			// GlobalFree (pBmp); 
		// } 
		// if (output_png[0]){ 
			// sprintf (extra_string2, "%s%d.png", output_png, f_draw); 
			// pBmp= ExportDepthPixelsToBitmap ((stroke_outlines? 4:3), main_w, main_h, pFrame); 
			// pPng= tdefl_write_image_to_png_file_in_memory (pBmp, main_w, main_h, 4, &png_file_size); 
			// file_put_contents (extra_string2, pPng, png_file_size); 
			// MZ_FREE (pPng); 
			// GlobalFree (pBmp); 
		// } 
		f_draw++; 
		fbuf_draw++; 
		if (fbuf_draw >= fbuf_max) fbuf_draw= 0; 
		// while (fbuf_draw == fbuf_compress) Sleep (100); 
		// Sleep (100); 
		// if (f_draw == 20) PostThreadMessage (mainThreadID, WM_CLOSE, 0, 0); 
		if (debug_mode) printf (":draw place marker %d\r\n", 5); 
		GetStdHandle (STD_OUTPUT_HANDLE); 
	} 
	if (debug_mode) printf (":draw place marker %d\r\n", 6); 
	GetStdHandle (STD_OUTPUT_HANDLE); 
	BitmapData= 0; 
	GlobalFree (frameA); 
	draw_complete= 1; 
	// PostQuitMessage (0); 
	PostThreadMessage (mainThreadID, WM_USER + 1, 0, 0); 
	ExitThread (0); 
} 