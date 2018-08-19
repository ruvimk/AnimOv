#define                 BIN_ELLIPSOID      0x27 

// last known good backup: 03 

long * ProcessToonLowlevelCode2Bin (char * InputCode); 
void ProcessToonLowlevelCodeFromBinRel (long * tlc); 
void ProcessToonLowlevelCodeFromBin (long * tlc); 
void DestroyToonLowlevelCodeFromBin (long * tlc); 

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
			d= 1; 
			a_number= sscanf (cmdTail, " %lf , %lf , %lf , %lf", &a, &b, &c, &d); 
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
			p1->reserved1= d; 
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
		} else if (!strcmp (cmdString, "tricky")){ 
			TrickyCurve (); 
		} else if (!strcmp (cmdString, "strand")){ 
			StrandCurve (); 
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
long * ProcessToonLowlevelCode2Bin (char * InputCode){ 
	F_NaN= sqrt (-1); 
	long * tlc= GlobalAlloc (GMEM_ZEROINIT, 4096); 
	long sTLC= 4096; 
	long cTLC= 4; 
		*tlc= sizeof (char *); 
	long * cp= tlc + cTLC; 
	char * tc; 
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
		if (cTLC + 512 > sTLC){ 
			tlc= GlobalReAlloc (tlc, sTLC + 512, GMEM_ZEROINIT); 
			sTLC += 512; 
		} 
		cp= tlc + cTLC; 
		if (UseVerbose && UseDebug) if (debug_mode) printf ("Scanning Line: %s\r\n", current_line); 
		if (!sscanf (current_line, " %[abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ] %[^;]", cmdString, cmdTail)) continue; 
		if (!strcmp (cmdString, "reset")){ 
			*cp= 1; 
			cTLC += 4; 
			// objColor[0]= objColor[1]= objColor[2]= 255; 
			// objOutline[0]= objOutline[1]= objOutline[2]= 0; 
			// objLine= 1; 
			// objVector[0]= 0; 
			// atX= atY= atZ= aboutX= aboutY= aboutZ= axisX= axisY= axisZ= 0; 
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
			*cp= 2; 
			cp[1]= *((long *)(objColor)); 
			cTLC += 8; 
		} else if (!strcmp (cmdString, "outline")){ 
			a_number= sscanf (cmdTail, " %lf , %lf , %lf , %lf ", &a, &b, &c, &d); 
			if (!a_number) continue; 
			objOutline[0]= (unsigned char)(a); 
			objOutline[1]= (unsigned char)(b); 
			objOutline[2]= (unsigned char)(c); 
			*cp= 3; 
			cp[1]= *((long *)(objOutline)); 
			cTLC += 8; 
		} else if (!strcmp (cmdString, "line")){ 
			a_number= sscanf (cmdTail, " %lf", &a); 
			if (!a_number) continue; 
			objLine= a; 
			*cp= 4; 
			cp[1]= (long)(objLine); 
			cTLC += 8; 
		} else if (!strcmp (cmdString, "vector")){ 
			a_number= sscanf (cmdTail, " %s", objVector); 
			if (!a_number) continue; 
			for (i= strlen (objVector) - 1; i + 1; i--){ 
				if (objVector[i] != 32 && objVector[i] != 9) break; 
				objVector[i]= 0; 
			} 
			*cp= 5; 
			tc= GlobalAlloc (GMEM_ZEROINIT, sizeof (objVector)); 
			strcpy (tc, objVector); 
			cp[1]= (long)(tc); 
			cTLC += 8; 
		} else if (!strcmp (cmdString, "at")){ 
			a_number= sscanf (cmdTail, " %lf , %lf , %lf", &a, &b, &c); 
			if (!a_number) continue; 
			atX= a; 
			atY= b; 
			atZ= c; 
			*cp= 6; 
			((float *)(cp))[1]= (float)(a); 
			((float *)(cp))[2]= (float)(b); 
			((float *)(cp))[3]= (float)(c); 
			cTLC += 16; 
		} else if (!strcmp (cmdString, "about")){ 
			a_number= sscanf (cmdTail, " %lf , %lf , %lf", &a, &b, &c); 
			if (!a_number) continue; 
			aboutX= a; 
			aboutY= b; 
			aboutZ= c; 
			*cp= 7; 
			((float *)(cp))[1]= (float)(a); 
			((float *)(cp))[2]= (float)(b); 
			((float *)(cp))[3]= (float)(c); 
			cTLC += 16; 
		} else if (!strcmp (cmdString, "axes")){ 
			// if (UseVerbose && UseDebug) if (debug_mode) printf ("At axes; loading... \r\n"); 
			a_number= sscanf (cmdTail, " %lf , %lf , %lf", &a, &b, &c); 
			//if (UseVerbose && UseDebug) if (debug_mode) printf ("sscanf (\"%s\", \" f , f , f\", ...) returned %d\r\n", cmdTail, a_number); 
			if (!a_number) continue; 
			//if (UseVerbose && UseDebug) if (debug_mode) printf ("axes: %f, %f, %f\r\n", a, b, c); 
			axisX= a; 
			axisY= b; 
			axisZ= c; 
			*cp= 8; 
			((float *)(cp))[1]= (float)(a); 
			((float *)(cp))[2]= (float)(b); 
			((float *)(cp))[3]= (float)(c); 
			cTLC += 16; 
		} else if (!strcmp (cmdString, "path")){ 
			// p1= pFirst; 
			// do { 
				// if (!p1) break; 
				// p2= p1->pNext; 
				// GlobalFree (p1); 
			// } while (p1= (POINT_LINK *)(p2)); 
			// pFirst= 0; 
			*cp= 9; 
			cTLC += 4; 
		} else if (!strcmp (cmdString, "pt")){ 
			d= 1; 
			a_number= sscanf (cmdTail, " %lf , %lf , %lf , %lf", &a, &b, &c, &d); 
			if (!a_number) continue; 
			// p1= pFirst; 
			// if (p1){ 
				// p2= (void *)(p1); 
				// while (p1= (POINT_LINK *)(p1->pNext)) p2= (void *)(p1); 
				// p1= (POINT_LINK *)(p2); 
				// p2= GlobalAlloc (GMEM_ZEROINIT, sizeof (POINT_LINK)); 
				// p1->pNext= p2; 
				// p3= p1; 
				// p1= (POINT_LINK *)(p2); 
				// p1->pLast= p3; 
			// } else { 
				// pFirst= p1= GlobalAlloc (GMEM_ZEROINIT, sizeof (POINT_LINK)); 
			// } 
			// p1->opc= 1; 
			// p1->x= a; 
			// p1->y= b; 
			// p1->z= c; 
			// strncpy (p1->vct, objVector, 256); 
			*cp= 10; 
			((float *)(cp))[1]= (float)(a); 
			((float *)(cp))[2]= (float)(b); 
			((float *)(cp))[3]= (float)(c); 
			((float *)(cp))[4]= (float)(d); 
			cTLC += 20; 
		} else if (!strcmp (cmdString, "ellipsoid")){ 
			// if (debug_mode) printf ("RenderEllipsoid ();\r\n"); 
			//printf ("render ellipsoid... \r\n"); 
			/*printf ("render ellipsoid (%d, %d, %d), ~(%d, %d, %d), x(%d, %d, %d); \r\n", (int)atX, (int)atY, (int)atZ, (int)aboutX, (int)aboutY, 
				(int)aboutZ, (int)axisX, (int)axisY, (int)axisZ); */
			// UseVerbose= 1; 
			// UseDebug= 1; 
			// RenderEllipsoid (); 
			*cp= 11; 
			cTLC += 4; 
			// UseVerbose= 0; 
			// UseDebug= 0; 
			// if (debug_mode) printf ("Returned. \r\n"); 
		} else if (!strcmp (cmdString, "fill")){ 
			// RenderCurve (); 
			*cp= 12; 
			cTLC += 4; 
		} else if (!strcmp (cmdString, "stroke")){ 
			// StrokeCurve (); 
			*cp= 13; 
			cTLC += 4; 
			// if (debug_mode) printf ("one01 place marker %d\r\n", 2); 
			// GetStdHandle (STD_OUTPUT_HANDLE); 
		} else if (!strcmp (cmdString, "tricky")){ 
			*cp= 14; 
			cTLC += 4; 
		} else if (!strcmp (cmdString, "strand")){ 
			*cp= 15; 
			cTLC += 4; 
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
	return tlc; 
} 
void ProcessToonLowlevelCodeFromBin (long * tlc){ 
	F_NaN= sqrt (-1); 
	long cTLC= 4; 
	long * cp= tlc + cTLC; 
	char * tc; 
	int i; 
	int j; 
	// char * p; 
	// long * lp; 
	// char string1[512]; 
	// char current_line[1024]; 
	// char * pScan; 
	double a, b, c, d; 
	int a_number; 
	POINT_LINK * p1; 
	void * p2; 
	void * p3; 
	char extra_string01[512]; 
	// printf ("tlc=%d\r\n", tlc); 
	// LPSTR CmndLine; 
	// char * CmdFile= 0; 
	// char * CmdBuff= 0; 
	// DEPTH_MAP_FILE_HEADER dmfh; 
	// HANDLE hFile; 
	// OFSTRUCT ofs; 
	// unsigned long extra1; 
	objColor[0]= objColor[1]= objColor[2]= 255; 
	objOutline[0]= objOutline[1]= objOutline[2]= 0; 
	objLine= 1; 
	objVector[0]= 0; 
	atX= atY= atZ= aboutX= aboutY= aboutZ= axisX= axisY= axisZ= 0; 
	// pScan= tlc; 
	//printf ("\r\n\r\nstarting scan loop... \r\n"); 
	while (*(tlc + cTLC)){ 
		// printf ("scanning line: %s\r\n", current_line); 
		cp= tlc + cTLC; 
		// if (UseVerbose && UseDebug) if (debug_mode) printf ("Scanning Line: %s\r\n", current_line); 
		// if (!sscanf (current_line, " %[abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ] %[^;]", cmdString, cmdTail)) continue; 
		// printf ("Opcode: %d\r\n", *cp); 
		if (!*cp){ 
			// printf ("NULL opcode found; stopping... \r\n"); 
			break; 
		} if (*cp == 1){ 
			// *cp= 1; 
			cTLC += 4; 
			objColor[0]= objColor[1]= objColor[2]= 255; 
			objOutline[0]= objOutline[1]= objOutline[2]= 0; 
			objLine= 1; 
			objVector[0]= 0; 
			atX= atY= atZ= aboutX= aboutY= aboutZ= axisX= axisY= axisZ= 0; 
		} else if (*cp == 2){ 
			//a_number= sscanf (cmdTail, " RGB ( %lf , %lf , %lf )", &a, &b, &c); 
			// a_number= sscanf (cmdTail, " %lf , %lf , %lf , %lf ", &a, &b, &c, &d); 
			// if (!a_number){ 
				// // if (debug_mode) printf ("sscanf (\"%s\", \" RGB ( lf , lf , lf )\", ...) \r\n", cmdTail); 
				// // if (debug_mode) printf ("a_number=%d\r\n", a_number); 
				// continue; 
			// } 
			// objColor[0]= (unsigned char)(a); 
			// objColor[1]= (unsigned char)(b); 
			// objColor[2]= (unsigned char)(c); 
			// *cp= 2; 
			// printf ("setting color... \r\n"); 
			*((long *)(objColor))= cp[1]; 
			// printf ("color set; \r\n"); 
			// cp[1]= *((long *)(objColor)); 
			cTLC += 8; 
		} else if (*cp == 3){ 
			// a_number= sscanf (cmdTail, " %lf , %lf , %lf , %lf ", &a, &b, &c, &d); 
			// if (!a_number) continue; 
			// objOutline[0]= (unsigned char)(a); 
			// objOutline[1]= (unsigned char)(b); 
			// objOutline[2]= (unsigned char)(c); 
			// *cp= 3; 
			// cp[1]= *((long *)(objOutline)); 
			*((long *)(objOutline))= cp[1]; 
			cTLC += 8; 
		} else if (*cp == 4){ 
			// a_number= sscanf (cmdTail, " %lf", &a); 
			// if (!a_number) continue; 
			// objLine= a; 
			// *cp= 4; 
			// cp[1]= (long)(objLine); 
			objLine= cp[1]; 
			cTLC += 8; 
		} else if (*cp == 5){ 
			// a_number= sscanf (cmdTail, " %s", objVector); 
			// if (!a_number) continue; 
			// for (i= strlen (objVector) - 1; i + 1; i--){ 
				// if (objVector[i] != 32 && objVector[i] != 9) break; 
				// objVector[i]= 0; 
			// } 
			// *cp= 5; 
			// tc= GlobalAlloc (GMEM_ZEROINIT, sizeof (objVector)); 
			// strcpy (tc, objVector); 
			// cp[1]= tc; 
			strcpy (objVector, (char *)(cp[1])); 
			cTLC += 8; 
		} else if (*cp == 6){ 
			// a_number= sscanf (cmdTail, " %lf , %lf , %lf", &a, &b, &c); 
			// if (!a_number) continue; 
			a= (double)((float *)(cp))[1]; 
			b= (double)((float *)(cp))[2]; 
			c= (double)((float *)(cp))[3]; 
			atX= a; 
			atY= b; 
			atZ= c; 
			// *cp= 6; 
			// ((float *)(cp))[1]= (float)(a); 
			// ((float *)(cp))[2]= (float)(b); 
			// ((float *)(cp))[3]= (float)(c); 
			cTLC += 16; 
		} else if (*cp == 7){ 
			// a_number= sscanf (cmdTail, " %lf , %lf , %lf", &a, &b, &c); 
			// if (!a_number) continue; 
			a= (double)((float *)(cp))[1]; 
			b= (double)((float *)(cp))[2]; 
			c= (double)((float *)(cp))[3]; 
			aboutX= a; 
			aboutY= b; 
			aboutZ= c; 
			// *cp= 7; 
			// ((float *)(cp))[1]= (float)(a); 
			// ((float *)(cp))[2]= (float)(b); 
			// ((float *)(cp))[3]= (float)(c); 
			cTLC += 16; 
		} else if (*cp == 8){ 
			// if (UseVerbose && UseDebug) if (debug_mode) printf ("At axes; loading... \r\n"); 
			// a_number= sscanf (cmdTail, " %lf , %lf , %lf", &a, &b, &c); 
			//if (UseVerbose && UseDebug) if (debug_mode) printf ("sscanf (\"%s\", \" f , f , f\", ...) returned %d\r\n", cmdTail, a_number); 
			// if (!a_number) continue; 
			//if (UseVerbose && UseDebug) if (debug_mode) printf ("axes: %f, %f, %f\r\n", a, b, c); 
			a= (double)((float *)(cp))[1]; 
			b= (double)((float *)(cp))[2]; 
			c= (double)((float *)(cp))[3]; 
			axisX= a; 
			axisY= b; 
			axisZ= c; 
			// *cp= 8; 
			// ((float *)(cp))[1]= (float)(a); 
			// ((float *)(cp))[2]= (float)(b); 
			// ((float *)(cp))[3]= (float)(c); 
			cTLC += 16; 
		} else if (*cp == 9){ 
			p1= pFirst; 
			if (p1){ 
				do { 
					p2= p1->pNext; 
					GlobalFree (p1); 
				} while (p1= (POINT_LINK *)(p2)); 
			} 
			pFirst= 0; 
			// *cp= 9; 
			cTLC += 4; 
		} else if (*cp == 10){ 
			// a_number= sscanf (cmdTail, " %lf , %lf , %lf", &a, &b, &c); 
			// if (!a_number) continue; 
			a= (double)((float *)(cp))[1]; 
			b= (double)((float *)(cp))[2]; 
			c= (double)((float *)(cp))[3]; 
			d= (double)((float *)(cp))[4]; 
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
			p1->reserved1= d; 
			strncpy (p1->vct, objVector, 256); 
			// *cp= 10; 
			// ((float *)(cp))[1]= (float)(a); 
			// ((float *)(cp))[2]= (float)(b); 
			// ((float *)(cp))[3]= (float)(c); 
			cTLC += 20; 
		} else if (*cp == 11){ 
			// if (debug_mode) printf ("RenderEllipsoid ();\r\n"); 
			//printf ("render ellipsoid... \r\n"); 
			/*printf ("render ellipsoid (%d, %d, %d), ~(%d, %d, %d), x(%d, %d, %d); \r\n", (int)atX, (int)atY, (int)atZ, (int)aboutX, (int)aboutY, 
				(int)aboutZ, (int)axisX, (int)axisY, (int)axisZ); */
			// UseVerbose= 1; 
			// UseDebug= 1; 
			// printf ("at: (%f, %f, %f); about: (%f, %f, %f); axes: (%f, %f, %f); \r\n", atX, atY, atZ, aboutX, aboutY, aboutZ, axisX, axisY, axisZ); 
			// printf ("vector: %s\r\n", objVector); 
			// atX= atX; 
			// atY= atY; 
			// atZ= atZ; 
			// aboutX= aboutX; 
			// aboutY= aboutY; 
			// aboutZ= aboutZ; 
			// axisX= axisX; 
			// axisY= axisY; 
			// axisZ= axisZ; 
			// if (f_draw == 1){ 
				// debug_mode= 1; 
				// UseDebug= 1; 
				// UseVerbose= 1; 
			// } else { 
				// debug_mode= 0; 
				// UseDebug= 0; 
				// UseVerbose= 0; 
			// } 
			// sprintf (extra_string01, "data: debug:ellipsoid (vct=%s)\r\n\r\n", objVector); 
			// cprint (extra_string01); 
			RenderEllipsoid (); 
			// *cp= 11; 
			cTLC += 4; 
			// UseVerbose= 0; 
			// UseDebug= 0; 
			// if (debug_mode) printf ("Returned. \r\n"); 
		} else if (*cp == 12){ 
			RenderCurve (); 
			// *cp= 12; 
			cTLC += 4; 
		} else if (*cp == 13){ 
			StrokeCurve (); 
			// *cp= 13; 
			cTLC += 4; 
			// if (debug_mode) printf ("one01 place marker %d\r\n", 2); 
			// GetStdHandle (STD_OUTPUT_HANDLE); 
		} else if (*cp == 14){ 
			TrickyCurve (); 
			cTLC += 4; 
		} else if (*cp == 15){ 
			StrandCurve (); 
			cTLC += 4; 
		} else { 
			// printf ("Unrecognized binary opcode; breaking... \r\n"); 
			break; 
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
void ProcessToonLowlevelCodeFromBinRel (long * tlc){ 
	F_NaN= sqrt (-1); 
	long cTLC= 4; 
	long * cp= tlc + cTLC; 
	char * tc; 
	int i; 
	int j; 
	// char * p; 
	// long * lp; 
	// char string1[512]; 
	// char current_line[1024]; 
	// char * pScan; 
	double a, b, c, d; 
	int a_number; 
	POINT_LINK * p1; 
	void * p2; 
	void * p3; 
	char extra_string01[512]; 
	// printf ("tlc=%d\r\n", tlc); 
	// LPSTR CmndLine; 
	// char * CmdFile= 0; 
	// char * CmdBuff= 0; 
	// DEPTH_MAP_FILE_HEADER dmfh; 
	// HANDLE hFile; 
	// OFSTRUCT ofs; 
	// unsigned long extra1; 
	objColor[0]= objColor[1]= objColor[2]= 255; 
	objOutline[0]= objOutline[1]= objOutline[2]= 0; 
	objLine= 1; 
	objVector[0]= 0; 
	atX= atY= atZ= aboutX= aboutY= aboutZ= axisX= axisY= axisZ= 0; 
	// pScan= tlc; 
	//printf ("\r\n\r\nstarting scan loop... \r\n"); 
	while (*(tlc + cTLC)){ 
		// printf ("scanning line: %s\r\n", current_line); 
		cp= tlc + cTLC; 
		// if (UseVerbose && UseDebug) if (debug_mode) printf ("Scanning Line: %s\r\n", current_line); 
		// if (!sscanf (current_line, " %[abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ] %[^;]", cmdString, cmdTail)) continue; 
		// printf ("Opcode: %d\r\n", *cp); 
		if (!*cp){ 
			// printf ("NULL opcode found; stopping... \r\n"); 
			break; 
		} if (*cp == 1){ 
			// *cp= 1; 
			cTLC += 4; 
			objColor[0]= objColor[1]= objColor[2]= 255; 
			objOutline[0]= objOutline[1]= objOutline[2]= 0; 
			objLine= 1; 
			objVector[0]= 0; 
			atX= atY= atZ= aboutX= aboutY= aboutZ= axisX= axisY= axisZ= 0; 
		} else if (*cp == 2){ 
			//a_number= sscanf (cmdTail, " RGB ( %lf , %lf , %lf )", &a, &b, &c); 
			// a_number= sscanf (cmdTail, " %lf , %lf , %lf , %lf ", &a, &b, &c, &d); 
			// if (!a_number){ 
				// // if (debug_mode) printf ("sscanf (\"%s\", \" RGB ( lf , lf , lf )\", ...) \r\n", cmdTail); 
				// // if (debug_mode) printf ("a_number=%d\r\n", a_number); 
				// continue; 
			// } 
			// objColor[0]= (unsigned char)(a); 
			// objColor[1]= (unsigned char)(b); 
			// objColor[2]= (unsigned char)(c); 
			// *cp= 2; 
			// printf ("setting color... \r\n"); 
			*((long *)(objColor))= cp[1]; 
			// printf ("color set; \r\n"); 
			// cp[1]= *((long *)(objColor)); 
			cTLC += 8; 
		} else if (*cp == 3){ 
			// a_number= sscanf (cmdTail, " %lf , %lf , %lf , %lf ", &a, &b, &c, &d); 
			// if (!a_number) continue; 
			// objOutline[0]= (unsigned char)(a); 
			// objOutline[1]= (unsigned char)(b); 
			// objOutline[2]= (unsigned char)(c); 
			// *cp= 3; 
			// cp[1]= *((long *)(objOutline)); 
			*((long *)(objOutline))= cp[1]; 
			cTLC += 8; 
		} else if (*cp == 4){ 
			// a_number= sscanf (cmdTail, " %lf", &a); 
			// if (!a_number) continue; 
			// objLine= a; 
			// *cp= 4; 
			// cp[1]= (long)(objLine); 
			objLine= cp[1]; 
			cTLC += 8; 
		} else if (*cp == 5){ 
			// a_number= sscanf (cmdTail, " %s", objVector); 
			// if (!a_number) continue; 
			// for (i= strlen (objVector) - 1; i + 1; i--){ 
				// if (objVector[i] != 32 && objVector[i] != 9) break; 
				// objVector[i]= 0; 
			// } 
			// *cp= 5; 
			// tc= GlobalAlloc (GMEM_ZEROINIT, sizeof (objVector)); 
			// strcpy (tc, objVector); 
			// cp[1]= tc; 
			strcpy (objVector, (char *)(cp[1] + (int)(tlc))); 
			cTLC += 8; 
		} else if (*cp == 6){ 
			// a_number= sscanf (cmdTail, " %lf , %lf , %lf", &a, &b, &c); 
			// if (!a_number) continue; 
			a= (double)((float *)(cp))[1]; 
			b= (double)((float *)(cp))[2]; 
			c= (double)((float *)(cp))[3]; 
			atX= a; 
			atY= b; 
			atZ= c; 
			// *cp= 6; 
			// ((float *)(cp))[1]= (float)(a); 
			// ((float *)(cp))[2]= (float)(b); 
			// ((float *)(cp))[3]= (float)(c); 
			cTLC += 16; 
		} else if (*cp == 7){ 
			// a_number= sscanf (cmdTail, " %lf , %lf , %lf", &a, &b, &c); 
			// if (!a_number) continue; 
			a= (double)((float *)(cp))[1]; 
			b= (double)((float *)(cp))[2]; 
			c= (double)((float *)(cp))[3]; 
			aboutX= a; 
			aboutY= b; 
			aboutZ= c; 
			// *cp= 7; 
			// ((float *)(cp))[1]= (float)(a); 
			// ((float *)(cp))[2]= (float)(b); 
			// ((float *)(cp))[3]= (float)(c); 
			cTLC += 16; 
		} else if (*cp == 8){ 
			// if (UseVerbose && UseDebug) if (debug_mode) printf ("At axes; loading... \r\n"); 
			// a_number= sscanf (cmdTail, " %lf , %lf , %lf", &a, &b, &c); 
			//if (UseVerbose && UseDebug) if (debug_mode) printf ("sscanf (\"%s\", \" f , f , f\", ...) returned %d\r\n", cmdTail, a_number); 
			// if (!a_number) continue; 
			//if (UseVerbose && UseDebug) if (debug_mode) printf ("axes: %f, %f, %f\r\n", a, b, c); 
			a= (double)((float *)(cp))[1]; 
			b= (double)((float *)(cp))[2]; 
			c= (double)((float *)(cp))[3]; 
			axisX= a; 
			axisY= b; 
			axisZ= c; 
			// *cp= 8; 
			// ((float *)(cp))[1]= (float)(a); 
			// ((float *)(cp))[2]= (float)(b); 
			// ((float *)(cp))[3]= (float)(c); 
			cTLC += 16; 
		} else if (*cp == 9){ 
			p1= pFirst; 
			if (p1){ 
				do { 
					p2= p1->pNext; 
					GlobalFree (p1); 
				} while (p1= (POINT_LINK *)(p2)); 
			} 
			pFirst= 0; 
			// *cp= 9; 
			cTLC += 4; 
		} else if (*cp == 10){ 
			// a_number= sscanf (cmdTail, " %lf , %lf , %lf", &a, &b, &c); 
			// if (!a_number) continue; 
			a= (double)((float *)(cp))[1]; 
			b= (double)((float *)(cp))[2]; 
			c= (double)((float *)(cp))[3]; 
			d= (double)((float *)(cp))[4]; 
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
			p1->reserved1= d; 
			strncpy (p1->vct, objVector, 256); 
			// *cp= 10; 
			// ((float *)(cp))[1]= (float)(a); 
			// ((float *)(cp))[2]= (float)(b); 
			// ((float *)(cp))[3]= (float)(c); 
			cTLC += 20; 
		} else if (*cp == 11){ 
			// if (debug_mode) printf ("RenderEllipsoid ();\r\n"); 
			//printf ("render ellipsoid... \r\n"); 
			/*printf ("render ellipsoid (%d, %d, %d), ~(%d, %d, %d), x(%d, %d, %d); \r\n", (int)atX, (int)atY, (int)atZ, (int)aboutX, (int)aboutY, 
				(int)aboutZ, (int)axisX, (int)axisY, (int)axisZ); */
			// UseVerbose= 1; 
			// UseDebug= 1; 
			// printf ("at: (%f, %f, %f); about: (%f, %f, %f); axes: (%f, %f, %f); \r\n", atX, atY, atZ, aboutX, aboutY, aboutZ, axisX, axisY, axisZ); 
			// printf ("vector: %s\r\n", objVector); 
			// atX= atX; 
			// atY= atY; 
			// atZ= atZ; 
			// aboutX= aboutX; 
			// aboutY= aboutY; 
			// aboutZ= aboutZ; 
			// axisX= axisX; 
			// axisY= axisY; 
			// axisZ= axisZ; 
			// if (f_draw == 1){ 
				// debug_mode= 1; 
				// UseDebug= 1; 
				// UseVerbose= 1; 
			// } else { 
				// debug_mode= 0; 
				// UseDebug= 0; 
				// UseVerbose= 0; 
			// } 
			RenderEllipsoid (); 
			// *cp= 11; 
			cTLC += 4; 
			// UseVerbose= 0; 
			// UseDebug= 0; 
			// if (debug_mode) printf ("Returned. \r\n"); 
		} else if (*cp == 12){ 
			RenderCurve (); 
			// *cp= 12; 
			cTLC += 4; 
		} else if (*cp == 13){ 
			StrokeCurve (); 
			// *cp= 13; 
			cTLC += 4; 
			// if (debug_mode) printf ("one01 place marker %d\r\n", 2); 
			// GetStdHandle (STD_OUTPUT_HANDLE); 
		} else if (*cp == 14){ 
			TrickyCurve (); 
			cTLC += 4; 
		} else if (*cp == 15){ 
			StrandCurve (); 
			cTLC += 4; 
		} else { 
			// printf ("Unrecognized binary opcode; breaking... \r\n"); 
			break; 
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
void DestroyToonLowlevelCodeFromBin (long * tlc){ 
	F_NaN= sqrt (-1); 
	long cTLC= 4; 
	long * cp= tlc + cTLC; 
	char * tc; 
	int i; 
	int j; 
	// char * p; 
	// long * lp; 
	// char string1[512]; 
	// char current_line[1024]; 
	// char * pScan; 
	double a, b, c, d; 
	int a_number; 
	// POINT_LINK * p1; 
	// void * p2; 
	// void * p3; 
	// LPSTR CmndLine; 
	// char * CmdFile= 0; 
	// char * CmdBuff= 0; 
	// DEPTH_MAP_FILE_HEADER dmfh; 
	// HANDLE hFile; 
	// OFSTRUCT ofs; 
	// unsigned long extra1; 
	// objColor[0]= objColor[1]= objColor[2]= 255; 
	// objOutline[0]= objOutline[1]= objOutline[2]= 0; 
	// objLine= 1; 
	// objVector[0]= 0; 
	// atX= atY= atZ= aboutX= aboutY= aboutZ= axisX= axisY= axisZ= 0; 
	// pScan= InputCode; 
	//printf ("\r\n\r\nstarting scan loop... \r\n"); 
	while (*(tlc + cTLC)){ 
		// printf ("scanning line: %s\r\n", current_line); 
		cp= tlc + cTLC; 
		// if (UseVerbose && UseDebug) if (debug_mode) printf ("Scanning Line: %s\r\n", current_line); 
		// if (!sscanf (current_line, " %[abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ] %[^;]", cmdString, cmdTail)) continue; 
		if (*cp == 1){ 
			// *cp= 1; 
			cTLC += 4; 
			// objColor[0]= objColor[1]= objColor[2]= 255; 
			// objOutline[0]= objOutline[1]= objOutline[2]= 0; 
			// objLine= 1; 
			// objVector[0]= 0; 
			// atX= atY= atZ= aboutX= aboutY= aboutZ= axisX= axisY= axisZ= 0; 
		} else if (*cp == 2){ 
			//a_number= sscanf (cmdTail, " RGB ( %lf , %lf , %lf )", &a, &b, &c); 
			// a_number= sscanf (cmdTail, " %lf , %lf , %lf , %lf ", &a, &b, &c, &d); 
			// if (!a_number){ 
				// // if (debug_mode) printf ("sscanf (\"%s\", \" RGB ( lf , lf , lf )\", ...) \r\n", cmdTail); 
				// // if (debug_mode) printf ("a_number=%d\r\n", a_number); 
				// continue; 
			// } 
			// objColor[0]= (unsigned char)(a); 
			// objColor[1]= (unsigned char)(b); 
			// objColor[2]= (unsigned char)(c); 
			// *cp= 2; 
			// *((long *)(objColor))= cp[1]; 
			// cp[1]= *((long *)(objColor)); 
			cTLC += 8; 
		} else if (*cp == 3){ 
			// a_number= sscanf (cmdTail, " %lf , %lf , %lf , %lf ", &a, &b, &c, &d); 
			// if (!a_number) continue; 
			// objOutline[0]= (unsigned char)(a); 
			// objOutline[1]= (unsigned char)(b); 
			// objOutline[2]= (unsigned char)(c); 
			// *cp= 3; 
			// cp[1]= *((long *)(objOutline)); 
			// *((long *)(objOutline))= cp[1]; 
			cTLC += 8; 
		} else if (*cp == 4){ 
			// a_number= sscanf (cmdTail, " %lf", &a); 
			// if (!a_number) continue; 
			// objLine= a; 
			// *cp= 4; 
			// cp[1]= (long)(objLine); 
			// objLine= cp[1]; 
			cTLC += 8; 
		} else if (*cp == 5){ 
			// a_number= sscanf (cmdTail, " %s", objVector); 
			// if (!a_number) continue; 
			// for (i= strlen (objVector) - 1; i + 1; i--){ 
				// if (objVector[i] != 32 && objVector[i] != 9) break; 
				// objVector[i]= 0; 
			// } 
			// *cp= 5; 
			// tc= GlobalAlloc (GMEM_ZEROINIT, sizeof (objVector)); 
			// strcpy (tc, objVector); 
			// cp[1]= tc; 
			// strcpy (objVector, cp[1]); 
			if (cp[1]) GlobalFree ((char *)(cp[1])); 
			cTLC += 8; 
		} else if (*cp == 6){ 
			// a_number= sscanf (cmdTail, " %lf , %lf , %lf", &a, &b, &c); 
			// if (!a_number) continue; 
			// a= (double)((float *)(cp))[1]; 
			// b= (double)((float *)(cp))[2]; 
			// c= (double)((float *)(cp))[3]; 
			// atX= a; 
			// atY= b; 
			// atZ= c; 
			// *cp= 6; 
			// ((float *)(cp))[1]= (float)(a); 
			// ((float *)(cp))[2]= (float)(b); 
			// ((float *)(cp))[3]= (float)(c); 
			cTLC += 16; 
		} else if (*cp == 7){ 
			// a_number= sscanf (cmdTail, " %lf , %lf , %lf", &a, &b, &c); 
			// if (!a_number) continue; 
			// a= (double)((float *)(cp))[1]; 
			// b= (double)((float *)(cp))[2]; 
			// c= (double)((float *)(cp))[3]; 
			// aboutX= a; 
			// aboutY= b; 
			// aboutZ= c; 
			// *cp= 7; 
			// ((float *)(cp))[1]= (float)(a); 
			// ((float *)(cp))[2]= (float)(b); 
			// ((float *)(cp))[3]= (float)(c); 
			cTLC += 16; 
		} else if (*cp == 8){ 
			// if (UseVerbose && UseDebug) if (debug_mode) printf ("At axes; loading... \r\n"); 
			// a_number= sscanf (cmdTail, " %lf , %lf , %lf", &a, &b, &c); 
			//if (UseVerbose && UseDebug) if (debug_mode) printf ("sscanf (\"%s\", \" f , f , f\", ...) returned %d\r\n", cmdTail, a_number); 
			// if (!a_number) continue; 
			//if (UseVerbose && UseDebug) if (debug_mode) printf ("axes: %f, %f, %f\r\n", a, b, c); 
			// a= (double)((float *)(cp))[1]; 
			// b= (double)((float *)(cp))[2]; 
			// c= (double)((float *)(cp))[3]; 
			// axisX= a; 
			// axisY= b; 
			// axisZ= c; 
			// *cp= 8; 
			// ((float *)(cp))[1]= (float)(a); 
			// ((float *)(cp))[2]= (float)(b); 
			// ((float *)(cp))[3]= (float)(c); 
			cTLC += 16; 
		} else if (*cp == 9){ 
			// p1= pFirst; 
			// do { 
				// if (!p1) break; 
				// p2= p1->pNext; 
				// GlobalFree (p1); 
			// } while (p1= (POINT_LINK *)(p2)); 
			// pFirst= 0; 
			// *cp= 9; 
			cTLC += 4; 
		} else if (*cp == 10){ 
			// a_number= sscanf (cmdTail, " %lf , %lf , %lf", &a, &b, &c); 
			// if (!a_number) continue; 
			// a= (double)((float *)(cp))[1]; 
			// b= (double)((float *)(cp))[2]; 
			// c= (double)((float *)(cp))[3]; 
			// p1= pFirst; 
			// if (p1){ 
				// p2= (void *)(p1); 
				// while (p1= (POINT_LINK *)(p1->pNext)) p2= (void *)(p1); 
				// p1= (POINT_LINK *)(p2); 
				// p2= GlobalAlloc (GMEM_ZEROINIT, sizeof (POINT_LINK)); 
				// p1->pNext= p2; 
				// p3= p1; 
				// p1= (POINT_LINK *)(p2); 
				// p1->pLast= p3; 
			// } else { 
				// pFirst= p1= GlobalAlloc (GMEM_ZEROINIT, sizeof (POINT_LINK)); 
			// } 
			// p1->opc= 1; 
			// p1->x= a; 
			// p1->y= b; 
			// p1->z= c; 
			// strncpy (p1->vct, objVector, 256); 
			// *cp= 10; 
			// ((float *)(cp))[1]= (float)(a); 
			// ((float *)(cp))[2]= (float)(b); 
			// ((float *)(cp))[3]= (float)(c); 
			cTLC += 20; 
		} else if (*cp == 11){ 
			// if (debug_mode) printf ("RenderEllipsoid ();\r\n"); 
			//printf ("render ellipsoid... \r\n"); 
			/*printf ("render ellipsoid (%d, %d, %d), ~(%d, %d, %d), x(%d, %d, %d); \r\n", (int)atX, (int)atY, (int)atZ, (int)aboutX, (int)aboutY, 
				(int)aboutZ, (int)axisX, (int)axisY, (int)axisZ); */
			// UseVerbose= 1; 
			// UseDebug= 1; 
			// RenderEllipsoid (); 
			// // *cp= 11; 
			cTLC += 4; 
			// UseVerbose= 0; 
			// UseDebug= 0; 
			// if (debug_mode) printf ("Returned. \r\n"); 
		} else if (*cp == 12){ 
			// RenderCurve (); 
			// *cp= 12; 
			cTLC += 4; 
		} else if (*cp == 13){ 
			// StrokeCurve (); 
			// *cp= 13; 
			cTLC += 4; 
			// if (debug_mode) printf ("one01 place marker %d\r\n", 2); 
			// GetStdHandle (STD_OUTPUT_HANDLE); 
		} else if (*cp == 14){ 
			cTLC += 4; 
		} else if (*cp == 15){ 
			cTLC += 4; 
		} else { 
			printf ("Unrecognized binary opcode; breaking... \r\n"); 
			break; 
		} 
		// GetStdHandle (STD_OUTPUT_HANDLE); 
	} 
	GlobalFree (tlc); 
	// p1= pFirst; 
	// do { 
		// if (!p1) break; 
		// p2= p1->pNext; 
		// GlobalFree (p1); 
	// } while (p1= (POINT_LINK *)(p2)); 
	// pFirst= 0; 
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
	long * pLLC; 
	HANDLE hStdOut= GetStdHandle (STD_OUTPUT_HANDLE); 
	HANDLE hAOC; 
	char * pPrevCmd= 0; 
	unsigned long extra1; 
	int started_play= 0; 
	double started_t; 
	char * pBase64= 0; 
	int changed= 0; 
	char vct_path[512]; 
	double vct_val; 
	double vct_number; 
	char vct_str[512]; 
	char prev_prop[512]; 
	char next_prop[512]; 
	char * bg_path= "Background"; 
	if (prev_mode) t= (d_draw - 1) / fps; 
	sprintf (extra_str, "%d", (int)(ceil (len * fps))); 
	frame_digits= strlen (extra_str); 
	extra_str[0]= 0; 
	if (debug_mode) printf (":draw allocating %d bytes \r\n", fbuf_unit_size + 16); 
	// GetStdHandle (STD_OUTPUT_HANDLE); 
	BitmapData= (char *)(frameA); 
	if (debug_mode) printf (":draw place marker %d\r\n", 1); 
	// GetStdHandle (STD_OUTPUT_HANDLE); 
	camname[0]= 0; 
	if (!cam_rh) cam_rh= PI / 4; 
	if (!cam_rv) cam_rv= PI * 3 / 16; 
	// long * ProcessToonLowlevelCode2Bin (char * InputCode); 
	// void ProcessToonLowlevelCodeFromBin (long * tlc); 
	// void DestroyToonLowlevelCodeFromBin (long * tlc); 
	// Anim_ReloadLLC (); 
	pLLC= ProcessToonLowlevelCode2Bin (llcbuf); 
	if (work_mode == 1){ 
		for (; t < len && (f_draw <= render_stop || render_stop == -1) && !all_stop; t += ofps){ 
			timeAt= t; 
			if (all_stop) break; 
			if (f_draw == render_start) draw_start_frame= f_draw; 
			draw_last_frame= f_draw; 
			// LookUpPropertyExBin (vctbin, "CAM", t, camname); 
			// // sprintf (extra_str, "Debug:   Camera name (t = %f): %s\r\n", t, camname); 
			// // GetStdHandle (STD_OUTPUT_HANDLE); 
			// // cprint (extra_str); 
			// sprintf (camstr, "[CAM]%s.x", camname); 
			// cam_x= LookUpNumberExBin (vctbin, camstr, t); 
			// sprintf (camstr, "[CAM]%s.y", camname); 
			// cam_y= LookUpNumberExBin (vctbin, camstr, t); 
			// // sprintf (extra_str, "Debug: cam_y(%s)=%f\r\n", camstr, cam_y); 
			// // cprint (extra_str); 
			// sprintf (camstr, "[CAM]%s.z", camname); 
			// cam_z= LookUpNumberExBin (vctbin, camstr, t); 
			// sprintf (camstr, "[CAM]%s.aboutX", camname); 
			// cam_aboutX= LookUpNumberExBin (vctbin, camstr, t); 
			// sprintf (camstr, "[CAM]%s.aboutY", camname); 
			// cam_aboutY= LookUpNumberExBin (vctbin, camstr, t); 
			// sprintf (camstr, "[CAM]%s.aboutZ", camname); 
			// cam_aboutZ= LookUpNumberExBin (vctbin, camstr, t); 
			// sprintf (camstr, "[CAM]%s.rh", camname); 
			// number= LookUpNumberExBin (vctbin, camstr, t); 
			// if (number) cam_rh= number; 
			// sprintf (camstr, "[CAM]%s.rv", camname); 
			// number= LookUpNumberExBin (vctbin, camstr, t); 
			// if (number) cam_rv= number; 
			// sprintf (camstr, "[CAM]%s.zoom", camname); 
			// number= LookUpNumberExBin (vctbin, camstr, t); 
			// cam_zoom= number; 
			// if (!cam_rh) cam_rh= PI / 4; 
			// if (!cam_rv) cam_rv= PI * 3 / 16; 
			// printf ("Camera (%s) Information (t = %f): \r\n\tAt: (%f, %f, %f); \r\n\tAbout: (%f, %f, %f); \r\n\trh: %f, rv: %f; \r\n\tZoom: %f\r\n", 
				// camname, t, cam_x, cam_y, cam_z, cam_aboutX, cam_aboutY, cam_aboutZ, cam_rh, cam_rv, cam_zoom); 
			tfps= t * fps; 
			a= floor (tfps); 
			b= ceil (tfps); 
			if (tfps - a > b - tfps) f= b; 
			else f= a; 
			// if (debug_mode) printf (":draw place marker %d\r\n", 2); 
			// InitBitmapData (frameA); 
			// if (debug_mode) printf (":draw place marker %d b\r\n", 2); 
			// // GetStdHandle (STD_OUTPUT_HANDLE); 
			// // ProcessToonLowlevelCode (llcbuf); 
			// // printf ("about to process low-level binary code... \r\n"); 
			// ProcessToonLowlevelCode (llcbuf); 
			// // ProcessToonLowlevelCodeFromBin (pLLC); 
			// if (llcbuf2) ProcessToonLowlevelCodeFromBinRel (llcbuf2); 
			// if (debug_mode) printf (":draw place marker %d\r\n", 3); 
			// Draw. 
			LookUpPropertyExBin (vctbin, "CAM", t, camname); 
			sprintf (camstr, "[CAM]%s.x", camname); 
			cam_x= LookUpNumberExBin (vctbin, camstr, t); 
			sprintf (camstr, "[CAM]%s.y", camname); 
			cam_y= LookUpNumberExBin (vctbin, camstr, t); 
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
			InitBitmapData (frameA); 
			if (!llcbuf[0]) Anim_ReloadLLC (); 
			// ProcessToonLowlevelCodeFromBin (pLLC); 
			ProcessToonLowlevelCode (llcbuf); 
			if (llcbuf2) ProcessToonLowlevelCodeFromBinRel (llcbuf2); 
			// GetStdHandle (STD_OUTPUT_HANDLE); 
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
				// pBmp= ExportDepthPixelsToBitmap ((stroke_outlines? ((stroke_outlines == 1)? 4:abs (stroke_outlines)):3*2), main_w, main_h, frameA); 
				pBmp= ExportDepthPixelsToBitmap ((stroke_outlines? ((stroke_outlines == 1)? 4:abs (stroke_outlines)):3*2), main_w, main_h, frameA); 
				if (output_bmp[0]){ 
					sprintf (extra_string3, "%s%d.bmp", output_bmp, f_draw); 
					file_put_contents (extra_string3, pBmp, *((long *)((char *)pBmp + 2))); 
				} 
				// file_put_contents ("bmp_debug_1.bmp", pBmp, *((long *)((char *)pBmp + 2))); 
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
				pBmp= ExportDepthPixelsToBitmap ((stroke_outlines? ((stroke_outlines == 1)? 4:abs (stroke_outlines)):3*2), main_w, main_h, frameA); 
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
			// GetStdHandle (STD_OUTPUT_HANDLE); 
		} 
	} else if (work_mode == 2){ 
		// UseDebugBox= 1; 
		hAOC= file_open_read (aoc_fn); 
		// Send the background state values. 
			sprintf (extra_string, "%s.%s", bg_path, "x"); 
			// cprint ("data: step 01\r\n\r\n"); 
			vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
			// cprint ("data: step 02\r\n\r\n"); 
			sprintf (vct_str, "data: bg_%s:%f%s,<%s>\r\n\r\n", "x", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":""), extra_string); 
			if (!cprint (vct_str)){ 
				all_stop= 1; 
				goto live_preview_render_all_stop_now; 
			} 
			debug_file (__FILE__); debug_line (__LINE__); 
			// cprint ("data: step 03\r\n\r\n"); 
			sprintf (extra_string, "%s.%s", bg_path, "y"); 
			vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
			sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "y", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
			if (!cprint (vct_str)){ 
				all_stop= 1; 
				goto live_preview_render_all_stop_now; 
			} 
			sprintf (extra_string, "%s.%s", bg_path, "z"); 
			vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
			sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "z", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
			if (!cprint (vct_str)){ 
				all_stop= 1; 
				goto live_preview_render_all_stop_now; 
			} 
			sprintf (extra_string, "%s.%s", bg_path, "aboutX"); 
			vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
			sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "aboutX", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
			if (!cprint (vct_str)){ 
				all_stop= 1; 
				goto live_preview_render_all_stop_now; 
			} 
			sprintf (extra_string, "%s.%s", bg_path, "aboutY"); 
			vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
			sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "aboutY", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
			if (!cprint (vct_str)){ 
				all_stop= 1; 
				goto live_preview_render_all_stop_now; 
			} 
			sprintf (extra_string, "%s.%s", bg_path, "aboutZ"); 
			vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
			sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "aboutZ", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
			if (!cprint (vct_str)){ 
				all_stop= 1; 
				goto live_preview_render_all_stop_now; 
			} 
			debug_file (__FILE__); debug_line (__LINE__); 
			sprintf (extra_string, "%s.%s", bg_path, "opc"); 
			vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
			sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "opc", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
			if (!cprint (vct_str)){ 
				all_stop= 1; 
				goto live_preview_render_all_stop_now; 
			} 
			sprintf (extra_string, "%s.%s", bg_path, "zoom"); 
			vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
			sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "zoom", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
			if (!cprint (vct_str)){ 
				all_stop= 1; 
				goto live_preview_render_all_stop_now; 
			} 
			LookUpPropertyExBin (vctbin, "BG", 0, extra_string); 
			sprintf (vct_str, "data: BG:%s\r\n\r\n", extra_string); 
			if (!cprint (vct_str)){ 
				all_stop= 1; 
				goto live_preview_render_all_stop_now; 
			} 
			// sprintf (vct_str, "data: alert:%s\r\n\r\n", bg_path); 
			// cprint (vct_str); 
		// Render the preview for the first time. 
			LookUpPropertyExBin (vctbin, "CAM", t, camname); 
			sprintf (camstr, "[CAM]%s.x", camname); 
			cam_x= LookUpNumberExBin (vctbin, camstr, t); 
			sprintf (camstr, "[CAM]%s.y", camname); 
			cam_y= LookUpNumberExBin (vctbin, camstr, t); 
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
			InitBitmapData (frameA); 
			// ProcessToonLowlevelCodeFromBin (pLLC); 
			ProcessToonLowlevelCode (llcbuf); 
			if (llcbuf2) ProcessToonLowlevelCodeFromBinRel (llcbuf2); 
			sprintf (extra_string, "data: profile:%d,%d,%d,%d\r\n\r\n", RenderEllipsoid_UsefulCasts, RenderEllipsoid_TotalCasts, 
				profile_raycasting_before, profile_raycasting_during); 
			cprint (extra_string); 
			// pBmp= ExportDepthPixelsToBitmap ((stroke_outlines? 4:3*2), main_w, main_h, frameA); 
			pBmp= ExportDepthPixelsToBitmap ((stroke_outlines? ((stroke_outlines == 1)? 4:abs (stroke_outlines)):3*2), main_w, main_h, frameA); 
			pPng= BmpToPngInMemory (pBmp, &png_file_size); 
			// pBase64= base64_encode (pBmp, *((long *)((char *)(pBmp) + 2))); 
			pBase64= base64_encode ((unsigned char *)(pPng), (int)(png_file_size)); 
			// sprintf (camstr, "\r\n\r\ndata: alert:Before: %d\r\ndata: After: %d\r\n\r\n", png_file_size, strlen (pBase64)); 
			// cprint (camstr); 
			cprint ("\r\n"); 
			// file_put_contents ("/wbord/wwwroot/debugpng.png", pPng, png_file_size); 
			if (!cprint ("data: data:image/png;base64,")){ 
				GlobalFree (pBase64); GlobalFree (pBmp); MZ_FREE (pPng); 
				all_stop= 1; 
			} 
			if (!cprint (pBase64)){ 
				GlobalFree (pBase64); GlobalFree (pBmp); MZ_FREE (pPng); 
				all_stop= 1; 
			} 
			if (!cprint ("\r\n\r\n")){ 
				GlobalFree (pBase64); GlobalFree (pBmp); MZ_FREE (pPng); 
				all_stop= 1; 
			} 
			GlobalFree (pBase64); 
			GlobalFree (pBmp); 
			MZ_FREE (pPng); 
		// Enter the update loop. 
		while (!all_stop){ 
			debug_file (__FILE__); debug_line (__LINE__); 
			pPrevCmd= file_get_contents (PrevCmdFn); 
			if (strlen (pPrevCmd)){ 
				sprintf (extra_string, "data: debug:pPrevCmd.length:%d\r\n\r\n", strlen (pPrevCmd)); 
				cprint (extra_string); 
			} 
			debug_file (__FILE__); debug_line (__LINE__); 
			// file_put_contents (PrevCmdFn, "", 0); 
			clear_file (PrevCmdFn); 
			sprintf (extra_string, "data: debug: %d\r\n\r\n", file_size (PrevCmdFn)); 
			cprint (extra_string); 
			debug_file (__FILE__); debug_line (__LINE__); 
			// Process the command. 
				changed= 0; 
				changed += sscanf (pPrevCmd, " play %d", &prev_fps); 
				changed += sscanf (pPrevCmd, " seek %lf", &t); 
				if (sscanf (pPrevCmd, " load %d", &i)){ 
					// ToonInit (); 
					Anim_ReloadLLC (); 
					// DestroyToonLowlevelCodeFromBin (pLLC); 
					// pLLC= ProcessToonLowlevelCode2Bin (llcbuf); 
					changed++; 
				} 
				changed += sscanf (pPrevCmd, " vect %s", sel_vect); 
					prev_prop[0]= 0; 
					next_prop[0]= 0; 
				changed += sscanf (pPrevCmd, " prev %s", prev_prop); 
				changed += sscanf (pPrevCmd, " next %s", next_prop); 
					if (prev_prop[0]){ 
						sprintf (extra_string, "%s.%s", sel_vect, prev_prop); 
						t= FindNextKey (vctbin, extra_string, t, -1); 
					} else if (next_prop[0]){ 
						sprintf (extra_string, "%s.%s", sel_vect, next_prop); 
						t= FindNextKey (vctbin, extra_string, t, 1); 
					} 
				if (strstr (pPrevCmd, "play") || strstr (pPrevCmd, "seek") || strstr (pPrevCmd, "load") || /*strstr (pPrevCmd, "vect") || */
					strstr (pPrevCmd, "prev") || strstr (pPrevCmd, "next")) changed= 1; 
				else changed= 0; 
				if (strstr (pPrevCmd, "vect")){ 
					// Update the vector values. 
						cprint ("\r\n\r\n"); 
						sprintf (extra_string, "data: time:%f\r\n\r\n", t); 
						cprint (extra_string); 
						sprintf (vct_path, "%s.%s", sel_vect, "x"); 
						// cprint ("data: step 01\r\n\r\n"); 
						vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
						// cprint ("data: step 02\r\n\r\n"); 
						sprintf (vct_str, "data: %s:%f%s,<%s>\r\n\r\n", "x", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":""), vct_path); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						// debug_file (__FILE__); debug_line (__LINE__); 
						// cprint ("data: step 03\r\n\r\n"); 
						sprintf (vct_path, "%s.%s", sel_vect, "y"); 
						vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
						sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "y", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (vct_path, "%s.%s", sel_vect, "z"); 
						vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
						sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "z", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (vct_path, "%s.%s", sel_vect, "aboutX"); 
						vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
						sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "aboutX", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (vct_path, "%s.%s", sel_vect, "aboutY"); 
						vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
						sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "aboutY", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (vct_path, "%s.%s", sel_vect, "aboutZ"); 
						vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
						sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "aboutZ", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						// debug_file (__FILE__); debug_line (__LINE__); 
						sprintf (vct_path, "%s.%s", sel_vect, "opc"); 
						vct_number= LookUpNumberExBin2 (vctbin, vct_path, t); 
						sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "opc", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (vct_path, "%s.%s", sel_vect, "zoom"); 
						vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
						sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "zoom", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (extra_string, "%s.%s", bg_path, "x"); 
						// cprint ("data: step 01\r\n\r\n"); 
						vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
						// cprint ("data: step 02\r\n\r\n"); 
						sprintf (vct_str, "data: bg_%s:%f%s,<%s>\r\n\r\n", "x", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":""), extra_string); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						// debug_file (__FILE__); debug_line (__LINE__); 
						// cprint ("data: step 03\r\n\r\n"); 
						sprintf (extra_string, "%s.%s", bg_path, "y"); 
						vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
						sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "y", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (extra_string, "%s.%s", bg_path, "z"); 
						vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
						sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "z", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (extra_string, "%s.%s", bg_path, "aboutX"); 
						vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
						sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "aboutX", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (extra_string, "%s.%s", bg_path, "aboutY"); 
						vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
						sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "aboutY", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (extra_string, "%s.%s", bg_path, "aboutZ"); 
						vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
						sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "aboutZ", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						// debug_file (__FILE__); debug_line (__LINE__); 
						sprintf (extra_string, "%s.%s", bg_path, "opc"); 
						vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
						sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "opc", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (extra_string, "%s.%s", bg_path, "zoom"); 
						vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
						sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "zoom", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						LookUpPropertyExBin (vctbin, "BG", 0, extra_string); 
						sprintf (vct_str, "data: BG:%s\r\n\r\n", extra_string); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						// debug_file (__FILE__); debug_line (__LINE__); 
				} 
				// debug_file (__FILE__); debug_line (__LINE__); 
				// sprintf (extra_string, "data: doc:changed=%d\r\n\r\n", changed); 
				// cprint (extra_string); 
				timeAt= t; 
				f_draw= t * fps + 1; 
				sprintf (extra_string, 
					"data: report:changed: %d\r\ndata:prev_fps: %d\r\ndata:t:%f\r\ndata:sel_vect: %s\r\ndata:PrevCmdFn:%s\r\ndata:pPrevCmd:%s\r\n\r\n", 
					changed, prev_fps, t, sel_vect, PrevCmdFn, pPrevCmd); 
				cprint (extra_string); 
				// debug_file (__FILE__); debug_line (__LINE__); 
			// Continue with any rendering. 
			if (prev_fps){ 
				// debug_file (__FILE__); debug_line (__LINE__); 
				ofps= 1 / prev_fps; 
				// f_draw= t * fps + 1; 
				started_play= GetTickCount (); 
				started_t= t; 
				// sprintf (extra_string, "data: alert:play, prev_fps=%d\r\n\r\n", prev_fps); 
				// cprint (extra_string); 
				// started_play= (int)(GetTickCount () - (t * 1000)); 
				for (; t < len && (f_draw <= render_stop || render_stop == -1) && !all_stop && prev_fps; t= (GetTickCount () - started_play + (started_t * 1000)) / 1000){ 
					if (all_stop) break; 
					pPrevCmd= file_get_contents (PrevCmdFn); 
					// file_put_contents (PrevCmdFn, "", 0); 
					clear_file (PrevCmdFn); 
					// Process the command. 
						/*changed= 0; 
						changed += sscanf (pPrevCmd, " play %d", &prev_fps); 
						changed += sscanf (pPrevCmd, " seek %lf", &t); 
						if (sscanf (pPrevCmd, " load %d", &i)){ 
							// ToonInit (); 
							Anim_ReloadLLC (); 
							// DestroyToonLowlevelCodeFromBin (pLLC); 
							// pLLC= ProcessToonLowlevelCode2Bin (llcbuf); 
							changed++; 
						} 
						changed += sscanf (pPrevCmd, " vect %s", sel_vect); 
						timeAt= t; 
						f_draw= t * fps + 1; */
						changed= 0; 
						changed += sscanf (pPrevCmd, " play %d", &prev_fps); 
						changed += sscanf (pPrevCmd, " seek %lf", &t); 
						if (sscanf (pPrevCmd, " load %d", &i)){ 
							// ToonInit (); 
							Anim_ReloadLLC (); 
							// DestroyToonLowlevelCodeFromBin (pLLC); 
							// pLLC= ProcessToonLowlevelCode2Bin (llcbuf); 
							changed++; 
						} 
						changed += sscanf (pPrevCmd, " vect %s", sel_vect); 
						if (strstr (pPrevCmd, "play") || strstr (pPrevCmd, "seek") || strstr (pPrevCmd, "load") || strstr (pPrevCmd, "vect")) changed= 1; 
						else changed= 0; 
						// debug_file (__FILE__); debug_line (__LINE__); 
						// sprintf (extra_string, "data: doc:changed=%d\r\n\r\n", changed); 
						// cprint (extra_string); 
						timeAt= t; 
						f_draw= t * fps + 1; 
						sprintf (extra_string, 
							"data: report:changed: %d\r\ndata:prev_fps: %d\r\ndata:t:%f\r\ndata:sel_vect: %s\r\ndata:PrevCmdFn:%s\r\ndata:pPrevCmd:%s\r\n\r\n", 
							changed, prev_fps, t, sel_vect, PrevCmdFn, pPrevCmd); 
						cprint (extra_string); 
						// debug_file (__FILE__); debug_line (__LINE__); 
					// Output the vector value commands. 
						/*cprint ("\r\n\r\n"); 
						debug_file (__FILE__); debug_line (__LINE__); 
						cprint ("\r\n\r\n"); 
						sprintf (vct_path, "%s.%s", sel_vect, "x"); 
						// cprint ("data: step 01\r\n\r\n"); 
						vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
						// cprint ("data: step 02\r\n\r\n"); 
						sprintf (vct_str, "data: %s:%f%s,<%s>\r\n\r\n", "x", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":""), vct_path); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						debug_file (__FILE__); debug_line (__LINE__); 
						// cprint ("data: step 03\r\n\r\n"); 
						sprintf (vct_path, "%s.%s", sel_vect, "y"); 
						vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
						sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "y", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (vct_path, "%s.%s", sel_vect, "z"); 
						vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
						sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "z", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (vct_path, "%s.%s", sel_vect, "aboutX"); 
						vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
						sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "aboutX", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (vct_path, "%s.%s", sel_vect, "aboutY"); 
						vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
						sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "aboutY", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (vct_path, "%s.%s", sel_vect, "aboutZ"); 
						vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
						sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "aboutZ", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						debug_file (__FILE__); debug_line (__LINE__); 
						sprintf (vct_path, "%s.%s", sel_vect, "opc"); 
						vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
						sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "opc", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (vct_path, "%s.%s", sel_vect, "zoom"); 
						vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
						sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "zoom", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						debug_file (__FILE__); debug_line (__LINE__); */
					// Update the vector values. 
						cprint ("\r\n\r\n"); 
						// debug_file (__FILE__); debug_line (__LINE__); 
						cprint ("\r\n\r\n"); 
						sprintf (extra_string, "data: time:%f\r\n\r\n", t); 
						cprint (extra_string); 
						sprintf (vct_path, "%s.%s", sel_vect, "x"); 
						// cprint ("data: step 01\r\n\r\n"); 
						vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
						// cprint ("data: step 02\r\n\r\n"); 
						sprintf (vct_str, "data: %s:%f%s,<%s>\r\n\r\n", "x", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":""), vct_path); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						// debug_file (__FILE__); debug_line (__LINE__); 
						// cprint ("data: step 03\r\n\r\n"); 
						sprintf (vct_path, "%s.%s", sel_vect, "y"); 
						vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
						sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "y", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (vct_path, "%s.%s", sel_vect, "z"); 
						vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
						sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "z", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (vct_path, "%s.%s", sel_vect, "aboutX"); 
						vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
						sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "aboutX", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (vct_path, "%s.%s", sel_vect, "aboutY"); 
						vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
						sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "aboutY", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (vct_path, "%s.%s", sel_vect, "aboutZ"); 
						vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
						sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "aboutZ", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						// debug_file (__FILE__); debug_line (__LINE__); 
						sprintf (vct_path, "%s.%s", sel_vect, "opc"); 
						vct_number= LookUpNumberExBin2 (vctbin, vct_path, t); 
						sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "opc", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (vct_path, "%s.%s", sel_vect, "zoom"); 
						vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
						sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "zoom", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (extra_string, "%s.%s", bg_path, "x"); 
						// cprint ("data: step 01\r\n\r\n"); 
						vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
						// cprint ("data: step 02\r\n\r\n"); 
						sprintf (vct_str, "data: bg_%s:%f%s,<%s>\r\n\r\n", "x", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":""), extra_string); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						// debug_file (__FILE__); debug_line (__LINE__); 
						// cprint ("data: step 03\r\n\r\n"); 
						sprintf (extra_string, "%s.%s", bg_path, "y"); 
						vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
						sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "y", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (extra_string, "%s.%s", bg_path, "z"); 
						vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
						sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "z", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (extra_string, "%s.%s", bg_path, "aboutX"); 
						vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
						sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "aboutX", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (extra_string, "%s.%s", bg_path, "aboutY"); 
						vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
						sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "aboutY", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (extra_string, "%s.%s", bg_path, "aboutZ"); 
						vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
						sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "aboutZ", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						// debug_file (__FILE__); debug_line (__LINE__); 
						sprintf (extra_string, "%s.%s", bg_path, "opc"); 
						vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
						sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "opc", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (extra_string, "%s.%s", bg_path, "zoom"); 
						vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
						sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "zoom", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						LookUpPropertyExBin (vctbin, "BG", 0, extra_string); 
						sprintf (vct_str, "data: BG:%s\r\n\r\n", extra_string); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						// debug_file (__FILE__); debug_line (__LINE__); 
					// Work the preview rendering. 
						LookUpPropertyExBin (vctbin, "CAM", t, camname); 
						sprintf (camstr, "[CAM]%s.x", camname); 
						cam_x= LookUpNumberExBin (vctbin, camstr, t); 
						sprintf (camstr, "[CAM]%s.y", camname); 
						cam_y= LookUpNumberExBin (vctbin, camstr, t); 
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
						InitBitmapData (frameA); 
						// ProcessToonLowlevelCodeFromBin (pLLC); 
						ProcessToonLowlevelCode (llcbuf); 
						if (llcbuf2) ProcessToonLowlevelCodeFromBinRel (llcbuf2); 
						sprintf (extra_string, "data: profile:%d,%d,%d,%d\r\n\r\n", RenderEllipsoid_UsefulCasts, RenderEllipsoid_TotalCasts, 
							profile_raycasting_before, profile_raycasting_during); 
						cprint (extra_string); 
						// pBmp= ExportDepthPixelsToBitmap ((stroke_outlines? 4:3*2), main_w, main_h, frameA); 
						pBmp= ExportDepthPixelsToBitmap ((stroke_outlines? ((stroke_outlines == 1)? 4:abs (stroke_outlines)):3*2), main_w, main_h, frameA); 
						pPng= BmpToPngInMemory (pBmp, &png_file_size); 
						// pBase64= base64_encode (pBmp, *((long *)((char *)(pBmp) + 2))); 
						pBase64= base64_encode ((unsigned char *)(pPng), (int)(png_file_size)); 
						// sprintf (camstr, "\r\n\r\ndata: alert:Before: %d\r\ndata: After: %d\r\n\r\n", png_file_size, strlen (pBase64)); 
						// cprint (camstr); 
						cprint ("\r\n"); 
						// file_put_contents ("/wbord/wwwroot/debugpng.png", pPng, png_file_size); 
						if (!cprint ("data: data:image/png;base64,")){ 
							GlobalFree (pBase64); GlobalFree (pBmp); MZ_FREE (pPng); 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						if (!cprint (pBase64)){ 
							GlobalFree (pBase64); GlobalFree (pBmp); MZ_FREE (pPng); 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						if (!cprint ("\r\n\r\n")){ 
							GlobalFree (pBase64); GlobalFree (pBmp); MZ_FREE (pPng); 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						GlobalFree (pBase64); 
						GlobalFree (pBmp); 
						MZ_FREE (pPng); 
					// Continue. 
					if (pPrevCmd){ 
						GlobalFree (pPrevCmd); 
						pPrevCmd= 0; 
					} 
					// f_draw= t * fps + 1; 
					i= (int)(1000 / prev_fps); 
					if (i < 20) i= 20; 
					Sleep (i); 
				} 
				// if (prev_fps) t= timeAt= 0; 
				// else prev_fps= 0; 
				timeAt= t= 0; 
				// Update the vector values. 
					cprint ("\r\n\r\n"); 
					// debug_file (__FILE__); debug_line (__LINE__); 
					cprint ("\r\n\r\n"); 
					sprintf (extra_string, "data: time:%f\r\n\r\n", t); 
					cprint (extra_string); 
					sprintf (vct_path, "%s.%s", sel_vect, "x"); 
					// cprint ("data: step 01\r\n\r\n"); 
					vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
					// cprint ("data: step 02\r\n\r\n"); 
					sprintf (vct_str, "data: %s:%f%s,<%s>\r\n\r\n", "x", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":""), vct_path); 
					if (!cprint (vct_str)){ 
						all_stop= 1; 
						goto live_preview_render_all_stop_now; 
					} 
					// debug_file (__FILE__); debug_line (__LINE__); 
					// cprint ("data: step 03\r\n\r\n"); 
					sprintf (vct_path, "%s.%s", sel_vect, "y"); 
					vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
					sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "y", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
					if (!cprint (vct_str)){ 
						all_stop= 1; 
						goto live_preview_render_all_stop_now; 
					} 
					sprintf (vct_path, "%s.%s", sel_vect, "z"); 
					vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
					sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "z", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
					if (!cprint (vct_str)){ 
						all_stop= 1; 
						goto live_preview_render_all_stop_now; 
					} 
					sprintf (vct_path, "%s.%s", sel_vect, "aboutX"); 
					vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
					sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "aboutX", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
					if (!cprint (vct_str)){ 
						all_stop= 1; 
						goto live_preview_render_all_stop_now; 
					} 
					sprintf (vct_path, "%s.%s", sel_vect, "aboutY"); 
					vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
					sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "aboutY", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
					if (!cprint (vct_str)){ 
						all_stop= 1; 
						goto live_preview_render_all_stop_now; 
					} 
					sprintf (vct_path, "%s.%s", sel_vect, "aboutZ"); 
					vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
					sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "aboutZ", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
					if (!cprint (vct_str)){ 
						all_stop= 1; 
						goto live_preview_render_all_stop_now; 
					} 
					// debug_file (__FILE__); debug_line (__LINE__); 
					sprintf (vct_path, "%s.%s", sel_vect, "opc"); 
					vct_number= LookUpNumberExBin2 (vctbin, vct_path, t); 
					sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "opc", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
					if (!cprint (vct_str)){ 
						all_stop= 1; 
						goto live_preview_render_all_stop_now; 
					} 
					sprintf (vct_path, "%s.%s", sel_vect, "zoom"); 
					vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
					sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "zoom", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
					if (!cprint (vct_str)){ 
						all_stop= 1; 
						goto live_preview_render_all_stop_now; 
					} 
					sprintf (extra_string, "%s.%s", bg_path, "x"); 
					// cprint ("data: step 01\r\n\r\n"); 
					vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
					// cprint ("data: step 02\r\n\r\n"); 
					sprintf (vct_str, "data: bg_%s:%f%s,<%s>\r\n\r\n", "x", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":""), extra_string); 
					if (!cprint (vct_str)){ 
						all_stop= 1; 
						goto live_preview_render_all_stop_now; 
					} 
					// debug_file (__FILE__); debug_line (__LINE__); 
					// cprint ("data: step 03\r\n\r\n"); 
					sprintf (extra_string, "%s.%s", bg_path, "y"); 
					vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
					sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "y", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
					if (!cprint (vct_str)){ 
						all_stop= 1; 
						goto live_preview_render_all_stop_now; 
					} 
					sprintf (extra_string, "%s.%s", bg_path, "z"); 
					vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
					sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "z", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
					if (!cprint (vct_str)){ 
						all_stop= 1; 
						goto live_preview_render_all_stop_now; 
					} 
					sprintf (extra_string, "%s.%s", bg_path, "aboutX"); 
					vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
					sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "aboutX", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
					if (!cprint (vct_str)){ 
						all_stop= 1; 
						goto live_preview_render_all_stop_now; 
					} 
					sprintf (extra_string, "%s.%s", bg_path, "aboutY"); 
					vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
					sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "aboutY", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
					if (!cprint (vct_str)){ 
						all_stop= 1; 
						goto live_preview_render_all_stop_now; 
					} 
					sprintf (extra_string, "%s.%s", bg_path, "aboutZ"); 
					vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
					sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "aboutZ", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
					if (!cprint (vct_str)){ 
						all_stop= 1; 
						goto live_preview_render_all_stop_now; 
					} 
					// debug_file (__FILE__); debug_line (__LINE__); 
					sprintf (extra_string, "%s.%s", bg_path, "opc"); 
					vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
					sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "opc", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
					if (!cprint (vct_str)){ 
						all_stop= 1; 
						goto live_preview_render_all_stop_now; 
					} 
					sprintf (extra_string, "%s.%s", bg_path, "zoom"); 
					vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
					sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "zoom", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
					if (!cprint (vct_str)){ 
						all_stop= 1; 
						goto live_preview_render_all_stop_now; 
					} 
					LookUpPropertyExBin (vctbin, "BG", 0, extra_string); 
					sprintf (vct_str, "data: BG:%s\r\n\r\n", extra_string); 
					if (!cprint (vct_str)){ 
						all_stop= 1; 
						goto live_preview_render_all_stop_now; 
					} 
					// debug_file (__FILE__); debug_line (__LINE__); 
				// Work the preview rendering. 
					LookUpPropertyExBin (vctbin, "CAM", t, camname); 
					sprintf (camstr, "[CAM]%s.x", camname); 
					cam_x= LookUpNumberExBin (vctbin, camstr, t); 
					sprintf (camstr, "[CAM]%s.y", camname); 
					cam_y= LookUpNumberExBin (vctbin, camstr, t); 
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
					InitBitmapData (frameA); 
					// ProcessToonLowlevelCodeFromBin (pLLC); 
					ProcessToonLowlevelCode (llcbuf); 
					if (llcbuf2) ProcessToonLowlevelCodeFromBinRel (llcbuf2); 
					sprintf (extra_string, "data: profile:%d,%d,%d,%d\r\n\r\n", RenderEllipsoid_UsefulCasts, RenderEllipsoid_TotalCasts, 
						profile_raycasting_before, profile_raycasting_during); 
					cprint (extra_string); 
					// pBmp= ExportDepthPixelsToBitmap ((stroke_outlines? 4:3*2), main_w, main_h, frameA); 
					pBmp= ExportDepthPixelsToBitmap ((stroke_outlines? ((stroke_outlines == 1)? 4:abs (stroke_outlines)):3*2), main_w, main_h, frameA); 
					pPng= BmpToPngInMemory (pBmp, &png_file_size); 
					// pBase64= base64_encode (pBmp, *((long *)((char *)(pBmp) + 2))); 
					pBase64= base64_encode ((unsigned char *)(pPng), (int)(png_file_size)); 
					// sprintf (camstr, "\r\n\r\ndata: alert:Before: %d\r\ndata: After: %d\r\n\r\n", png_file_size, strlen (pBase64)); 
					// cprint (camstr); 
					cprint ("\r\n"); 
					// file_put_contents ("/wbord/wwwroot/debugpng.png", pPng, png_file_size); 
					if (!cprint ("data: data:image/png;base64,")){ 
						GlobalFree (pBase64); GlobalFree (pBmp); MZ_FREE (pPng); 
						all_stop= 1; 
						goto live_preview_render_all_stop_now; 
					} 
					if (!cprint (pBase64)){ 
						GlobalFree (pBase64); GlobalFree (pBmp); MZ_FREE (pPng); 
						all_stop= 1; 
						goto live_preview_render_all_stop_now; 
					} 
					if (!cprint ("\r\n\r\n")){ 
						GlobalFree (pBase64); GlobalFree (pBmp); MZ_FREE (pPng); 
						all_stop= 1; 
						goto live_preview_render_all_stop_now; 
					} 
					GlobalFree (pBase64); 
					GlobalFree (pBmp); 
					MZ_FREE (pPng); 
					cprint ("data: stop play\r\n\r\n"); 
				prev_fps= 0; 
			} else { 
				// debug_file (__FILE__); debug_line (__LINE__); 
				if (ModifiedSince (hAOC, &aoc_last)){ 
					// sprintf (extra_string, "%d\r\n", GetTickCount ()); 
					// file_put_contents ("modsince.txt", extra_string, strlen (extra_string)); 
					// debug_file (__FILE__); debug_line (__LINE__); 
					Anim_ReloadVCT (); 
					// debug_file (__FILE__); debug_line (__LINE__); 
					// cprint (main_debug); 
					// Update the vector values. 
						cprint ("\r\n\r\n"); 
						// debug_file (__FILE__); debug_line (__LINE__); 
						cprint ("\r\n\r\n"); 
						sprintf (extra_string, "data: time:%f\r\n\r\n", t); 
						cprint (extra_string); 
						sprintf (vct_path, "%s.%s", sel_vect, "x"); 
						// cprint ("data: step 01\r\n\r\n"); 
						vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
						// cprint ("data: step 02\r\n\r\n"); 
						sprintf (vct_str, "data: %s:%f%s,<%s>\r\n\r\n", "x", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":""), vct_path); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						// debug_file (__FILE__); debug_line (__LINE__); 
						// cprint ("data: step 03\r\n\r\n"); 
						sprintf (vct_path, "%s.%s", sel_vect, "y"); 
						vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
						sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "y", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (vct_path, "%s.%s", sel_vect, "z"); 
						vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
						sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "z", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (vct_path, "%s.%s", sel_vect, "aboutX"); 
						vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
						sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "aboutX", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (vct_path, "%s.%s", sel_vect, "aboutY"); 
						vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
						sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "aboutY", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (vct_path, "%s.%s", sel_vect, "aboutZ"); 
						vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
						sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "aboutZ", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						// debug_file (__FILE__); debug_line (__LINE__); 
						sprintf (vct_path, "%s.%s", sel_vect, "opc"); 
						vct_number= LookUpNumberExBin2 (vctbin, vct_path, t); 
						sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "opc", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (vct_path, "%s.%s", sel_vect, "zoom"); 
						vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
						sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "zoom", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (extra_string, "%s.%s", bg_path, "x"); 
						// cprint ("data: step 01\r\n\r\n"); 
						vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
						// cprint ("data: step 02\r\n\r\n"); 
						sprintf (vct_str, "data: bg_%s:%f%s,<%s>\r\n\r\n", "x", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":""), extra_string); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						// debug_file (__FILE__); debug_line (__LINE__); 
						// cprint ("data: step 03\r\n\r\n"); 
						sprintf (extra_string, "%s.%s", bg_path, "y"); 
						vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
						sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "y", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (extra_string, "%s.%s", bg_path, "z"); 
						vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
						sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "z", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (extra_string, "%s.%s", bg_path, "aboutX"); 
						vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
						sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "aboutX", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (extra_string, "%s.%s", bg_path, "aboutY"); 
						vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
						sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "aboutY", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (extra_string, "%s.%s", bg_path, "aboutZ"); 
						vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
						sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "aboutZ", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						// debug_file (__FILE__); debug_line (__LINE__); 
						sprintf (extra_string, "%s.%s", bg_path, "opc"); 
						vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
						sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "opc", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (extra_string, "%s.%s", bg_path, "zoom"); 
						vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
						sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "zoom", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						LookUpPropertyExBin (vctbin, "BG", 0, extra_string); 
						sprintf (vct_str, "data: BG:%s\r\n\r\n", extra_string); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						// debug_file (__FILE__); debug_line (__LINE__); 
					// Do the drawing, ... 
					LookUpPropertyExBin (vctbin, "CAM", t, camname); 
					sprintf (camstr, "[CAM]%s.x", camname); 
					cam_x= LookUpNumberExBin (vctbin, camstr, t); 
					sprintf (camstr, "[CAM]%s.y", camname); 
					cam_y= LookUpNumberExBin (vctbin, camstr, t); 
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
					InitBitmapData (frameA); 
					// ProcessToonLowlevelCodeFromBin (pLLC); 
					ProcessToonLowlevelCode (llcbuf); 
					if (llcbuf2) ProcessToonLowlevelCodeFromBinRel (llcbuf2); 
					sprintf (extra_string, "data: profile:%d,%d,%d,%d\r\n\r\n", RenderEllipsoid_UsefulCasts, RenderEllipsoid_TotalCasts, 
						profile_raycasting_before, profile_raycasting_during); 
					cprint (extra_string); 
					// pBmp= ExportDepthPixelsToBitmap ((stroke_outlines? 4:3*2), main_w, main_h, frameA); 
					pBmp= ExportDepthPixelsToBitmap ((stroke_outlines? ((stroke_outlines == 1)? 4:abs (stroke_outlines)):3*2), main_w, main_h, frameA); 
					pPng= BmpToPngInMemory (pBmp, &png_file_size); 
					// pBase64= base64_encode (pBmp, *((long *)((char *)(pBmp) + 2))); 
					pBase64= base64_encode ((unsigned char *)(pPng), (int)(png_file_size)); 
					// sprintf (camstr, "\r\n\r\ndata: alert:Before: %d\r\ndata: After: %d\r\n\r\n", png_file_size, strlen (pBase64)); 
					// cprint (camstr); 
					cprint ("\r\n"); 
					// file_put_contents ("/wbord/wwwroot/debugpng.png", pPng, png_file_size); 
					if (!cprint ("data: data:image/png;base64,")){ 
						GlobalFree (pBase64); GlobalFree (pBmp); MZ_FREE (pPng); 
						all_stop= 1; 
						goto live_preview_render_all_stop_now; 
					} 
					if (!cprint (pBase64)){ 
						GlobalFree (pBase64); GlobalFree (pBmp); MZ_FREE (pPng); 
						all_stop= 1; 
						goto live_preview_render_all_stop_now; 
					} 
					if (!cprint ("\r\n\r\n")){ 
						GlobalFree (pBase64); GlobalFree (pBmp); MZ_FREE (pPng); 
						all_stop= 1; 
						goto live_preview_render_all_stop_now; 
					} 
					GlobalFree (pBase64); 
					GlobalFree (pBmp); 
					MZ_FREE (pPng); 
					// debug_file (__FILE__); debug_line (__LINE__); 
				} else if (changed > 0){ 
					// ToonInit (); 
					// Anim_UpdateVCT (); 
					// cprint ("data: alert:changed>0\r\n\r\n"); 
					// Update the vector values. 
						// cprint ("data: sel_vect: "); 
						// cprint (sel_vect); 
						// debug_file (__FILE__); debug_line (__LINE__); 
						cprint ("\r\n\r\n"); 
						sprintf (extra_string, "data: time:%f\r\n\r\n", t); 
						cprint (extra_string); 
						sprintf (vct_path, "%s.%s", sel_vect, "x"); 
						// cprint ("data: step 01\r\n\r\n"); 
						vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
						// cprint ("data: step 02\r\n\r\n"); 
						sprintf (vct_str, "data: %s:%f%s,<%s>\r\n\r\n", "x", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":""), vct_path); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						// debug_file (__FILE__); debug_line (__LINE__); 
						// cprint ("data: step 03\r\n\r\n"); 
						sprintf (vct_path, "%s.%s", sel_vect, "y"); 
						vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
						sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "y", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (vct_path, "%s.%s", sel_vect, "z"); 
						vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
						sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "z", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (vct_path, "%s.%s", sel_vect, "aboutX"); 
						vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
						sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "aboutX", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (vct_path, "%s.%s", sel_vect, "aboutY"); 
						vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
						sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "aboutY", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (vct_path, "%s.%s", sel_vect, "aboutZ"); 
						vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
						sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "aboutZ", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						// debug_file (__FILE__); debug_line (__LINE__); 
						sprintf (vct_path, "%s.%s", sel_vect, "opc"); 
						vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
						sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "opc", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (vct_path, "%s.%s", sel_vect, "zoom"); 
						vct_number= LookUpNumberExBin (vctbin, vct_path, t); 
						sprintf (vct_str, "data: %s:%f%s\r\n\r\n", "zoom", vct_number, ((IsPropertyKey (vctbin, vct_path, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (extra_string, "%s.%s", bg_path, "x"); 
						// cprint ("data: step 01\r\n\r\n"); 
						vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
						// cprint ("data: step 02\r\n\r\n"); 
						sprintf (vct_str, "data: bg_%s:%f%s,<%s>\r\n\r\n", "x", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":""), extra_string); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						// debug_file (__FILE__); debug_line (__LINE__); 
						// cprint ("data: step 03\r\n\r\n"); 
						sprintf (extra_string, "%s.%s", bg_path, "y"); 
						vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
						sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "y", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (extra_string, "%s.%s", bg_path, "z"); 
						vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
						sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "z", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (extra_string, "%s.%s", bg_path, "aboutX"); 
						vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
						sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "aboutX", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (extra_string, "%s.%s", bg_path, "aboutY"); 
						vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
						sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "aboutY", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (extra_string, "%s.%s", bg_path, "aboutZ"); 
						vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
						sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "aboutZ", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						// debug_file (__FILE__); debug_line (__LINE__); 
						sprintf (extra_string, "%s.%s", bg_path, "opc"); 
						vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
						sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "opc", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						sprintf (extra_string, "%s.%s", bg_path, "zoom"); 
						vct_number= LookUpNumberExBin (vctbin, extra_string, t); 
						sprintf (vct_str, "data: bg_%s:%f%s\r\n\r\n", "zoom", vct_number, ((IsPropertyKey (vctbin, extra_string, t))? ",key":"")); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						LookUpPropertyExBin (vctbin, "BG", 0, extra_string); 
						sprintf (vct_str, "data: BG:%s\r\n\r\n", extra_string); 
						if (!cprint (vct_str)){ 
							all_stop= 1; 
							goto live_preview_render_all_stop_now; 
						} 
						// debug_file (__FILE__); debug_line (__LINE__); 
					// Do the drawing, ... 
					LookUpPropertyExBin (vctbin, "CAM", t, camname); 
					sprintf (camstr, "[CAM]%s.x", camname); 
					cam_x= LookUpNumberExBin (vctbin, camstr, t); 
					sprintf (camstr, "[CAM]%s.y", camname); 
					cam_y= LookUpNumberExBin (vctbin, camstr, t); 
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
					// debug_file (__FILE__); debug_line (__LINE__); 
					InitBitmapData (frameA); 
					// ProcessToonLowlevelCodeFromBin (pLLC); 
					// debug_file (__FILE__); debug_line (__LINE__); 
					ProcessToonLowlevelCode (llcbuf); 
					if (llcbuf2) ProcessToonLowlevelCodeFromBinRel (llcbuf2); 
					sprintf (extra_string, "data: profile:%d,%d,%d,%d\r\n\r\n", RenderEllipsoid_UsefulCasts, RenderEllipsoid_TotalCasts, 
						profile_raycasting_before, profile_raycasting_during); 
					cprint (extra_string); 
					// debug_file (__FILE__); debug_line (__LINE__); 
					// pBmp= ExportDepthPixelsToBitmap ((stroke_outlines? 4:3*2), main_w, main_h, frameA); 
					pBmp= ExportDepthPixelsToBitmap ((stroke_outlines? ((stroke_outlines == 1)? 4:abs (stroke_outlines)):3*2), main_w, main_h, frameA); 
					pPng= BmpToPngInMemory (pBmp, &png_file_size); 
					// debug_file (__FILE__); debug_line (__LINE__); 
					// pBase64= base64_encode (pBmp, *((long *)((char *)(pBmp) + 2))); 
					pBase64= base64_encode ((unsigned char *)(pPng), (int)(png_file_size)); 
					// debug_file (__FILE__); debug_line (__LINE__); 
					// sprintf (camstr, "\r\n\r\ndata: alert:Before: %d\r\ndata: After: %d\r\n\r\n", png_file_size, strlen (pBase64)); 
					// cprint (camstr); 
					cprint ("\r\n"); 
					// file_put_contents ("/wbord/wwwroot/debugpng.png", pPng, png_file_size); 
					if (!cprint ("data: data:image/png;base64,")){ 
						GlobalFree (pBase64); GlobalFree (pBmp); MZ_FREE (pPng); 
						all_stop= 1; 
						goto live_preview_render_all_stop_now; 
					} 
					if (!cprint (pBase64)){ 
						GlobalFree (pBase64); GlobalFree (pBmp); MZ_FREE (pPng); 
						all_stop= 1; 
						goto live_preview_render_all_stop_now; 
					} 
					if (!cprint ("\r\n\r\n")){ 
						GlobalFree (pBase64); GlobalFree (pBmp); MZ_FREE (pPng); 
						all_stop= 1; 
						goto live_preview_render_all_stop_now; 
					} 
					GlobalFree (pBase64); 
					GlobalFree (pBmp); 
					MZ_FREE (pPng); 
					// debug_file (__FILE__); debug_line (__LINE__); 
				} 
			} 
			// debug_file (__FILE__); debug_line (__LINE__); 
			if (pPrevCmd){ 
				GlobalFree (pPrevCmd); 
				pPrevCmd= 0; 
			} 
			// debug_file (__FILE__); debug_line (__LINE__); 
			if (!cprint (":\r\n")){ 
				all_stop= 1; 
				break; 
			} 
			// debug_file (__FILE__); debug_line (__LINE__); 
			Sleep (100); 
		} 
		; live_preview_render_all_stop_now: ; 
		file_close (hAOC); 
		DeleteFile (PrevCmdFn); 
	} 
	DeleteFile (PrevCmdFn); 
	DestroyToonLowlevelCodeFromBin (pLLC); 
	pLLC= 0; 
	if (debug_mode) printf (":draw place marker %d\r\n", 6); 
	// GetStdHandle (STD_OUTPUT_HANDLE); 
	BitmapData= 0; 
	GlobalFree (frameA); 
	draw_complete= 1; 
	// PostQuitMessage (0); 
	PostThreadMessage (mainThreadID, WM_USER + 1, 0, 0); 
	ExitThread (0); 
} 