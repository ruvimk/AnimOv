typedef struct { 
	char signature[4]; 
	long file_size; 
	long screen_w; 
	long screen_h; 
	short px_size; 
	short reserved; 
	float total_len; 
	long rel_ptr_stream; 
} MPS_FILE_HEADER; 

typedef struct { 
	short flags; 
	short compressed_size; 
} TILE_HEADER; 

typedef struct { 
	DWORD data[256]; 
} TILE_DATA; 

typedef struct { 
	short cbSize; 
	unsigned char flags; 
	unsigned char param; 
} STREAMCOMMANDSMALL; 

typedef struct { 
	short cbSize; 
	unsigned char flags; 
	unsigned char reserved; 
	DWORD param; 
} STREAMCOMMAND; 

typedef struct { 
	short cbSize; 
	unsigned char flags; 
	unsigned char reserved; 
	DWORD paramW; 
	DWORD paramN; 
	DWORD paramE; 
} STREAMCOMMANDEX; 

typedef struct { 
	char name[8]; 
} STREAM_DESCRIPTOR; 

typedef struct { 
	long x; 
	long y; 
	long w; 
	long h; 
} LARGEST_OBJECT; 

void PrefilterLongs (long * ls, int len){ 
	int i; 
	for (i= 1; i < len; i++){ 
		ls[i] -= ls[i-1]; 
	} 
} 

int ChooseCompressOutput (int * results, unsigned long * sizes, int len, unsigned long uncompressed_size){ 
	int iMin; 
	int vMin; 
	int a= 0; 
	int i; 
	for (i= 0; i < len; i++) if (results[i] == Z_OK) a= 1; 
	if (!a) return -1; 
	iMin= -1; 
	vMin= -1; 
	for (i= 0; i < len; i++){ 
		if (results[i] != Z_OK) continue; 
		if (iMin == -1 || sizes[i] < vMin){ 
			iMin= i; 
			vMin= sizes[i]; 
		} 
	} 
	if (iMin == -1) return 0; 
	if (vMin < uncompressed_size) return iMin + 1; 
	else return 0; 
} 

int FindExactTileMatch (unsigned char * tiles, long tiles_size, TILE_HEADER * pTileHeader, DWORD * pTileData){ 
	int i= 0; 
	unsigned char * p; 
	short sval; 
	long a; 
	long b= 0; 
	asm { 
		mov eax, dword ptr [tiles] 
		mov dword ptr [p], eax 
		lp1: 
			mov eax, dword ptr [p] 
			sub eax, dword ptr [tiles] 
			sub eax, dword ptr [tiles_size] 
			jnl lp1s 
			
			mov eax, dword ptr [pTileHeader] 
			mov ebx, eax 
			mov eax, dword ptr [pTileData] 
			mov edx, eax 
			lea eax, [p] 
			
			push ebp 
			mov ebp, esp 
			
			push dword ptr pTileHeader 
			push dword ptr pTileData 
			push dword ptr eax 
			
			push dword ptr sizeof (TILE_HEADER) 
			push dword ptr [ebp-4] 
			push dword ptr [ebp-12] 
			call memcmp 
			sub eax, 0 
			jnz lp1cont1 
			
			mov eax, dword ptr [ebp-4] 
			mov ebx, eax 
			xor eax, eax 
			mov ax, word ptr [ebx+2] 
			push dword ptr eax 
			push dword ptr [ebp-8] 
			mov eax, dword ptr [p] 
			add eax, sizeof (TILE_HEADER) 
			push dword ptr eax 
			call memcmp 
			sub eax, 0 
			jnz lp1cont1 
			
			mov esp, ebp 
			pop ebp 
			mov dword ptr [b], 1 
			jmp lp1s 
			
			lp1cont1: 
			
			mov esp, ebp 
			pop ebp 
			
			lp1cont: 
			inc dword ptr [i] 
			mov eax, dword ptr [p] 
			mov ebx, eax 
			xor eax, eax 
			mov ax, word ptr [ebx+2] 
			add eax, sizeof (TILE_HEADER) 
			add dword ptr [p], eax 
			jmp lp1 
		lp1s: 
	} 
	if (b) return i; 
	// for (p= tiles; (int *)(p - tiles) < (int *)(tiles_size); i++, p += (((TILE_HEADER *)(p)->compressed_size) + sizeof (TILE_HEADER))){ 
		// if (memcmp (p, pTileHeader, sizeof (TILE_HEADER))) continue; 
		// if (memcmp (p + sizeof (TILE_HEADER), pTileData, pTileHeader->compressed_size)) continue; 
		// return i; 
	// } 
	return -1; 
} 

void FindLargestObject (DEPTH_PIXEL * picture, int w, int h, LARGEST_OBJECT * obj, int step){ 
	long * lp; 
	int i; 
	int j; 
	int l_lXstart= 0; 
	int l_lXstop= 0; 
	int l_maxDXrow= 0; 
	int last_Xstart= 0; 
	int l_lYstart= 0; 
	int l_lYstop= 0; 
	int l_maxDYcol= 0; 
	int last_Ystart= 0; 
	// lp= (long *)(BmpDat + main_w * 8 * j + 8 * i + 4); 
	// lp= ((px[8, 0]).depth) * 
	// i= 8 
	// j= 0 
	// l_lXstart= 2 
	// l_lXstop= 3 
	// l_maxDXrow= 0 
	// last_Xstart= 2 
	// Line Number: 36 
	for (j= 0; j < h; j += step){ 
		for (i= 0; i < w; i += step){ 
			// lp= (long *)(picture + w * 8 * j + 8 * i + 4); 
			// if (*lp == PX_UNDEFINED){ 
				// if (i > 0 && *(lp - 8) != PX_UNDEFINED){ 
					// if (i - last_Xstart > l_lXstop - l_lXstart){ 
						// l_lXstart= last_Xstart; 
						// l_lXstop= i; 
						// l_maxDXrow= j; 
					// } 
				// } 
			// } else { 
				// if (i - 1 < 0 || *(lp - 8) == PX_UNDEFINED) last_Xstart= i; 
			// } 
			asm { 
				mov eax, dword ptr [picture] 
				mov ebx, eax 
				mov eax, dword ptr [w] 
				mov ecx, 8 
				mul ecx 
				mov ecx, eax 
				mov eax, dword ptr [j] 
				mul ecx 
				add ebx, eax 
				mov eax, dword ptr [i] 
				mov ecx, 8 
				mul ecx 
				add ebx, eax 
				
				mov eax, dword ptr [ebx+4] 
				cmp eax, PX_UNDEFINED 
				jnz px_is_defined 
					mov eax, dword ptr [i] 
					cmp eax, 0 
					jng px_def_finish 
					
					mov eax, dword ptr [ebx-4] 
					cmp eax, PX_UNDEFINED 
					jz px_def_finish 
					
					mov eax, dword ptr [i] 
					sub eax, dword ptr [last_Xstart] 
					mov ecx, eax 
					
					mov eax, dword ptr [l_lXstop] 
					sub eax, dword ptr [l_lXstart] 
					xchg eax, ecx 
					
					cmp eax, ecx 
					jng px_def_finish 
					
					mov eax, dword ptr [last_Xstart] 
					mov dword ptr [l_lXstart], eax 
					
					mov eax, dword ptr [i] 
					mov dword ptr [l_lXstop], eax 
					
					mov eax, dword ptr [j] 
					mov dword ptr [l_maxDXrow], eax 
					
					jmp px_def_finish 
				px_is_defined: 
					mov eax, dword ptr [i] 
					dec eax 
					sub eax, 0 
					jl px_def_not_finish 
					
					mov eax, dword ptr [ebx-4] 
					cmp eax, PX_UNDEFINED 
					jnz px_def_finish 
					
					px_def_not_finish: 
					
					mov eax, dword ptr [i] 
					mov dword ptr [last_Xstart], eax 
					
				px_def_finish: 
			} 
		} 
	} 
	for (i= 0; i < w; i += step){ 
		for (j= 0; j < h; j += step){ 
			// lp= (long *)(picture + w * 8 * j + 8 * i + 4); 
			// if (*lp == PX_UNDEFINED){ 
				// if (j > 0 && *((long *)(picture + w * 8 * (j - 1) + 8 * i + 4)) != PX_UNDEFINED){ 
					// if (j - last_Ystart > l_lYstop - l_lYstart){ 
						// l_lYstart= last_Ystart; 
						// l_lYstop= j; 
						// l_maxDYcol= i; 
					// } 
				// } 
			// } else { 
				// if (j - 1 < 0 || *((long *)(picture + w * 8 * (j - 1) + 8 * i + 4)) == PX_UNDEFINED) last_Ystart= j; 
			// } 
			asm { 
				mov eax, dword ptr [picture] 
				mov ebx, eax 
				mov eax, dword ptr [w] 
				mov ecx, 8 
				mul ecx 
				mov ecx, eax 
				mov eax, dword ptr [j] 
				mul ecx 
				add ebx, eax 
				mov eax, dword ptr [i] 
				mov ecx, 8 
				mul ecx 
				add ebx, eax 
				
				mov eax, dword ptr [ebx+4] 
				cmp eax, PX_UNDEFINED 
				jnz px_is_defined2 
					mov eax, dword ptr [j] 
					cmp eax, 0 
					jng px_def_finish2 
					
					mov eax, dword ptr [ebx-4] 
					cmp eax, PX_UNDEFINED 
					jz px_def_finish2 
					
					mov eax, dword ptr [j] 
					sub eax, dword ptr [last_Ystart] 
					mov ecx, eax 
					
					mov eax, dword ptr [l_lYstop] 
					sub eax, dword ptr [l_lYstart] 
					xchg eax, ecx 
					
					cmp eax, ecx 
					jng px_def_finish2 
					
					mov eax, dword ptr [last_Ystart] 
					mov dword ptr [l_lYstart], eax 
					
					mov eax, dword ptr [j] 
					mov dword ptr [l_lYstop], eax 
					
					mov eax, dword ptr [i] 
					mov dword ptr [l_maxDYcol], eax 
					
					jmp px_def_finish2 
				px_is_defined2: 
					mov eax, dword ptr [j] 
					dec eax 
					sub eax, 0 
					jl px_def_not_finish2 
					
					mov eax, dword ptr [ebx-4] 
					cmp eax, PX_UNDEFINED 
					jnz px_def_finish2 
					
					px_def_not_finish2: 
					
					mov eax, dword ptr [j] 
					mov dword ptr [last_Ystart], eax 
					
				px_def_finish2: 
			} 
		} 
	} 
	obj->w= l_lXstop - l_lXstart; 
	obj->h= l_lYstop - l_lYstart; 
	obj->x= l_lXstart + (obj->w) / 2; 
	obj->y= l_lYstart + (obj->h) / 2; 
} 

unsigned long WINAPI WorkCompress (void * param){ 
	MPS_FILE_HEADER file_head; 
	double t= (f_compress - 1) / fps; 
	double ofps= 1 / fps; 
	LARGEST_OBJECT base_obj; 
	LARGEST_OBJECT frame_obj; 
	DEPTH_PIXEL * pFrame; 
	DEPTH_PIXEL * frame= GlobalAlloc (GMEM_ZEROINIT, fbuf_unit_size + 16); 
	DEPTH_PIXEL * prevFrame= GlobalAlloc (GMEM_ZEROINIT, fbuf_unit_size + 16); 
	int dmapW= main_w / 16 + main_w % 16; 
	int dmapH= main_h / 16 + main_h % 16; 
	int dmapA= dmapW * dmapH; 
	int * dmap= GlobalAlloc (GMEM_ZEROINIT, dmapW * dmapH * sizeof (int) + 16); 
	long cBaseMap= 0; 
	int sBaseMap= fbuf_unit_size + 16; 
	DEPTH_PIXEL * pBaseMap= GlobalAlloc (GMEM_ZEROINIT, sBaseMap); 
	DEPTH_PIXEL * pBase; 
	DEPTH_PIXEL * pCompressedBaseMap; 
	unsigned long sCompressedBaseMap; 
	int cBaseArr= 0; 
	int * pBaseArr= GlobalAlloc (GMEM_ZEROINIT, (cBaseArr * 2 + 1) * sizeof (int)); 
	void * pTmp; 
	int iMinBase= -2; 
	int nMinBase= main_w * main_h; 
	int sStream= 0; 
	int aStream= 8192; 
	unsigned char * pStream= GlobalAlloc (GMEM_ZEROINIT, 8192); 
	STREAMCOMMANDSMALL * scmd1; 
	STREAMCOMMAND * cmd1; 
	STREAMCOMMANDEX * ecmd1; 
	TILE_HEADER thead; 
	TILE_DATA tdata1; 
	TILE_DATA tdata2; 
	DWORD cdata1[1024]; 
	DWORD cdata2[1024]; 
	DWORD cdata3[1024]; 
	DWORD cdata4[1024]; 
	char string001[2048]; 
	unsigned long cdata1len; 
	unsigned long cdata2len; 
	unsigned long cdata3len; 
	unsigned long cdata4len; 
	int cr1; 
	int cr2; 
	int cr3; 
	int cr4; 
	int cc; 
	DWORD * pcdata; 
	long cTiles= 0; 
	long sTiles= 0; 
	int aTiles= 131072; 
	unsigned char * pTiles= GlobalAlloc (GMEM_ZEROINIT, 131072); 
	int i; 
	int j; 
	int k; 
	int dy; 
	int dx; 
	int x; 
	int y; 
	int base_x; 
	int base_y; 
	long val; 
	int n; 
	long * pLong; 
	unsigned char * pUC; 
	long * pLong2; 
	unsigned char * pUC2; 
	STREAM_DESCRIPTOR sdesc; 
	HANDLE hFile; 
	OFSTRUCT ofs; 
	unsigned long bWritten; 
	long extra_long; 
	void * pBmp; 
	size_t png_file_size; 
	void * pPng; 
	BGRA * img_offset; 
	BGRA * clr; 
	memmove (file_head.signature, "MPPS", 4); 
	file_head.screen_w= main_w; 
	file_head.screen_h= main_h; 
	file_head.file_size= 0; 
	file_head.total_len= len; 
	file_head.rel_ptr_stream= 0; 
	cBaseMap++; 
	if (debug_mode) printf ("main_w: %d; main_h: %d; \r\n"); 
	if (debug_mode) printf ("compress base map size: %d\r\n", GlobalSize (pBaseMap)); 
	if (debug_mode) printf ("compress frame size: %d\r\n", GlobalSize (pBaseMap)); 
	// if (debug_mode) printf ("compress place marker %d\r\n", -15); 
	GetStdHandle (STD_OUTPUT_HANDLE); 
	InitBitmapData (pBaseMap); 
	// if (debug_mode) printf ("compress place marker %d\r\n", -14); 
	GetStdHandle (STD_OUTPUT_HANDLE); 
	InitBitmapData (frame); 
	// if (debug_mode) printf ("compress place marker %d\r\n", 1); 
	for (; t < len && (f_compress <= render_stop || render_stop == -1); t += ofps){ 
		while (fbuf_draw == fbuf_compress) Sleep (100); 
		// if (debug_mode) printf ("compress place marker %d\r\n", 2); 
		GetStdHandle (STD_OUTPUT_HANDLE); 
		// asm { 
			// mov eax, dword ptr [fbuf_p] 
			// mov ebx, eax 
			// mov eax, dword ptr [fbuf_compress] 
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
		pFrame= fbuf_p; 
		pFrame += fbuf_compress * fbuf_unit_size; 
		memmove (prevFrame, frame, fbuf_unit_size); 
		memmove (frame, pFrame /*fbuf_p + fbuf_compress * main_w * main_h * 8*/, fbuf_unit_size); 
		if (output_png[0]){ 
			sprintf (extra_string2, "%s%d.png", output_png, f_compress); 
			if (debug_mode) printf (":Output PNG: %s\r\n", extra_string2); 
			pBmp= ExportDepthPixelsToBitmap ((stroke_outlines? 4:3), main_w, main_h, frame); 
			if (output_bmp[0]){ 
				sprintf (extra_string3, "%s%d.bmp", output_bmp, f_compress); 
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
			// printf ("\tWritten to file. \r\n"); 
			MZ_FREE (pPng); 
			GlobalFree (pBmp); 
			sprintf (string001, "data: %d of %d; msg: Rendering... \r\n\r\n", f_compress - render_start, 
				((render_stop + 1)? render_stop + 1 - render_start: (int)(len * fps))); 
			if (!cprint (string001)) goto compress__stop_script_immediately; 
			goto cont00101; 
		} 
		// if (debug_mode) printf ("compress place marker %d\r\n", 3); 
		GetStdHandle (STD_OUTPUT_HANDLE); 
		FindLargestObject (frame, main_w, main_h, &frame_obj, 1); 
		GlobalFree (pBaseArr); 
		// if (debug_mode) printf ("compress place marker %d\r\n", 4); 
		GetStdHandle (STD_OUTPUT_HANDLE); 
		cBaseArr= 0; 
		pBaseArr= GlobalAlloc (GMEM_ZEROINIT, (cBaseArr * 2 + 5) * sizeof (int)); 
		// if (debug_mode) printf ("compress place marker %d\r\n", 5); 
		GetStdHandle (STD_OUTPUT_HANDLE); 
			pBase= prevFrame; 
			FindLargestObject (pBase, main_w, main_h, &base_obj, 16); 
			// if (debug_mode) printf ("compress place marker %d\r\n", 6); 
			GetStdHandle (STD_OUTPUT_HANDLE); 
			// f - (base buffer position + (object positions, base - f)) 
			dy= frame_obj.y - base_obj.y; 
			dx= frame_obj.x - base_obj.x; 
			for (k= 0; k < dmapA; k++) dmap[k]= 0; 
			for (x= 0; x < main_w; x++){ 
				for (y= 0; y < main_h; y++){ 
					if ((base_y= y - dy) < 0) continue; 
					if ((base_x= x - dx) < 0) continue; 
					if (base_y >= main_h) continue; 
					if (base_x >= main_w) continue; 
					// val= *((long *)(frame + main_w * 8 * y + 8 * x + 4)) - *((long *)(pBase + main_w * 8 * (base_y) + 8 * (base_x) + 4)); 
					asm { 
						mov eax, dword ptr [frame] 
						mov ebx, eax 
						mov eax, dword ptr [main_w] 
						mov ecx, 8 
						mul ecx 
						mov ecx, eax 
						mov eax, dword ptr [y] 
						mul ecx 
						add ebx, eax 
						mov eax, dword ptr [x] 
						mov ecx, 8 
						mul ecx 
						add ebx, eax 
						mov eax, dword ptr [ebx+4] 
						push eax 
						
						mov eax, dword ptr [pBase] 
						mov ebx, eax 
						mov eax, dword ptr [main_w] 
						mov ecx, 8 
						mul ecx 
						mov ecx, eax 
						mov eax, dword ptr [base_y] 
						mul ecx 
						add ebx, eax 
						mov eax, dword ptr [base_x] 
						mov ecx, 8 
						mul ecx 
						add ebx, eax 
						mov eax, dword ptr [ebx+4] 
						mov ecx, eax 
						
						pop eax 
						sub eax, ecx 
						mov dword ptr [val], eax 
					} 
					if (val){ 
						dmap[dmapW * (y / 16) + (x / 16)]= 1; 
					} 
				} 
			} 
			// if (debug_mode) printf ("compress place marker %d\r\n", 7); 
			n= 0; 
			for (k= 0; k < dmapA; k++) if (dmap[k]) n++; 
			// if (n < main_w * main_h / (sizeof (TILE_HEADER) / 4 + 256)){ 
				// pTmp= pBaseArr; 
				// pBaseArr= GlobalAlloc (GMEM_ZEROINIT, (cBaseArr * 2 + 3) * sizeof (int)); 
				// memmove (pBaseArr, pTmp, cBaseArr * 2 * sizeof (int)); 
				// pBaseArr[cBaseArr * 2 + 0]= -1; 
				// pBaseArr[cBaseArr * 2 + 1]= n; 
				// cBaseArr; 
				// GlobalFree (pTmp); 
			// } 
			// if (debug_mode) printf ("compress place marker %d\r\n", 8); 
		for (j= 0; j < cBaseMap; j++){ 
			if (debug_mode) printf ("compress place marker %d\r\n", 9); 
			pBase= pBaseMap + j * fbuf_unit_size; 
			FindLargestObject (pBase, main_w, main_h, &base_obj, 16); 
			// f - (base buffer position + (object positions, base - f)) 
			dy= frame_obj.y - base_obj.y; 
			dx= frame_obj.x - base_obj.x; 
			for (k= 0; k < dmapA; k++) dmap[k]= 0; 
			if (debug_mode) printf ("compress place marker %d\r\n", 10); 
			for (x= 0; x < main_w; x++){ 
				for (y= 0; y < main_h; y++){ 
					if ((base_y= y - dy) < 0) continue; 
					if ((base_x= x - dx) < 0) continue; 
					if (base_y >= main_h) continue; 
					if (base_x >= main_w) continue; 
					// val= *((long *)(frame + main_w * 8 * y + 8 * x + 4)) - *((long *)(pBase + main_w * 8 * (base_y) + 8 * (base_x) + 4)); 
					asm { 
						mov eax, dword ptr [frame] 
						mov ebx, eax 
						mov eax, dword ptr [main_w] 
						mov ecx, 8 
						mul ecx 
						mov ecx, eax 
						mov eax, dword ptr [y] 
						mul ecx 
						add ebx, eax 
						mov eax, dword ptr [x] 
						mov ecx, 8 
						mul ecx 
						add ebx, eax 
						mov eax, dword ptr [ebx+4] 
						push eax 
						
						mov eax, dword ptr [pBase] 
						mov ebx, eax 
						mov eax, dword ptr [main_w] 
						mov ecx, 8 
						mul ecx 
						mov ecx, eax 
						mov eax, dword ptr [base_y] 
						mul ecx 
						add ebx, eax 
						mov eax, dword ptr [base_x] 
						mov ecx, 8 
						mul ecx 
						add ebx, eax 
						mov eax, dword ptr [ebx+4] 
						mov ecx, eax 
						
						pop eax 
						sub eax, ecx 
						mov dword ptr [val], eax 
					} 
					if (val){ 
						dmap[dmapW * (y / 16) + (x / 16)]= 1; 
					} 
				} 
			} 
			if (debug_mode) printf ("compress place marker %d\r\n", 11); 
			n= 0; 
			for (k= 0; k < dmapA; k++) if (dmap[k]) n++; 
			if (n < main_w * main_h / (sizeof (TILE_HEADER) / 4 + 256)){ 
				pBaseArr= GlobalReAlloc (pBaseArr, (cBaseArr * 2 + 3) * sizeof (int), GMEM_ZEROINIT); 
				pBaseArr[cBaseArr * 2 + 0]= j; 
				pBaseArr[cBaseArr * 2 + 1]= n; 
				cBaseArr++; 
				// pTmp= pBaseArr; 
				// pBaseArr= GlobalAlloc (GMEM_ZEROINIT, (cBaseArr * 2 + 3) * sizeof (int)); 
				// memmove (pBaseArr, pTmp, cBaseArr * 2 * sizeof (int)); 
				// pBaseArr[cBaseArr * 2 + 0]= j; 
				// pBaseArr[cBaseArr * 2 + 1]= n; 
				// cBaseArr; 
				// GlobalFree (pTmp); 
			} 
			if (debug_mode) printf ("compress place marker %d\r\n", 12); 
		} 
		if (debug_mode) printf ("compress place marker %d\r\n", 13); 
		iMinBase= -2; 
		nMinBase= main_w * main_h; 
		for (j= 0; j < cBaseArr; j++){ 
			if (pBaseArr[j * 2 + 1] < nMinBase){ 
				iMinBase= pBaseArr[j * 2 + 0]; 
				nMinBase= pBaseArr[j * 2 + 1]; 
			} 
		} 
		if (debug_mode) printf ("compress place marker %d\r\n", 14); 
		cmd1= 0; 
		ecmd1= 0; 
		if (iMinBase == -2){ 
			// Add a new base. 
			pTmp= pBaseMap; 
			pBaseMap= GlobalAlloc (GMEM_ZEROINIT, sBaseMap + fbuf_unit_size); 
			memmove (pBaseMap, pTmp, sBaseMap); 
			GlobalFree (pTmp); 
			sBaseMap += fbuf_unit_size; 
			pBase= pBaseMap + cBaseMap * fbuf_unit_size; 
			cBaseMap++; 
			memmove (pBase, frame, fbuf_unit_size); 
			cmd1= (STREAMCOMMAND *)(pStream + sStream); 
			cmd1->cbSize= sizeof (STREAMCOMMAND); 
			cmd1->flags= 0x01; 
			*(long *)(&cmd1->param)= cBaseMap - 1; 
			sStream += sizeof (STREAMCOMMAND); 
			if (debug_mode) printf ("compress place marker %d\r\n", 15); 
		} else { 
			// Use an existing base. 
			if (iMinBase != -1) pBase= pBaseMap + iMinBase * fbuf_unit_size; 
			else pBase= prevFrame; 
			FindLargestObject (pBase, main_w, main_h, &base_obj, 1); 
			if (debug_mode) printf ("compress place marker %d\r\n", 16); 
			if (frame_obj.x == base_obj.x && frame_obj.y == base_obj.y){ 
				cmd1= (STREAMCOMMAND *)(pStream + sStream); 
				cmd1->cbSize= sizeof (STREAMCOMMAND); 
				cmd1->flags= 0x01; 
				*(long *)(&cmd1->param)= iMinBase; 
				dy= 0; 
				dx= 0; 
				sStream += sizeof (STREAMCOMMAND); 
				if (debug_mode) printf ("compress place marker %d\r\n", 17); 
			} else { 
				cmd1= (STREAMCOMMAND *)(pStream + sStream); 
				cmd1->cbSize= sizeof (STREAMCOMMAND); 
				cmd1->flags= 0x01; 
				*(long *)(&cmd1->param)= 0; 
				sStream += sizeof (STREAMCOMMAND); 
				ecmd1= (STREAMCOMMANDEX *)(pStream + sStream); 
				ecmd1->cbSize= sizeof (STREAMCOMMANDEX); 
				ecmd1->flags= 0x02; 
				dy= frame_obj.y - base_obj.y; 
				dx= frame_obj.x - base_obj.x; 
				*((short *)(&ecmd1->paramW) + 2)= dx; 
				*((short *)(&ecmd1->paramW) + 0)= dy; 
				*((short *)(&ecmd1->paramN) + 2)= main_w; 
				*((short *)(&ecmd1->paramN) + 0)= main_h; 
				*((long *)(&ecmd1->paramE))= iMinBase; 
				sStream += sizeof (STREAMCOMMANDEX); 
				if (debug_mode) printf ("compress place marker %d\r\n", 18); 
			} 
			if (debug_mode) printf ("compress place marker %d\r\n", 19); 
			for (i= 0; i + 1 < dmapW; i++){ 
				for (j= 0; j + 1 < dmapH; j++){ 
					if (debug_mode) printf ("compress place marker %d\r\n", 20); 
					for (y= 16 * j; y < 16 * (j + 1); y++){ 
						for (x= 16 * i; x < 16 * (i + 1); x++){ 
							// pLong= (long *)(pBase + main_w * 8 * y + 8 * x + 4); 
							// pLong2= (long *)(frame + main_w * 8 * y + 8 * x + 4); 
							// asm { 
								// mov eax, dword ptr [main_w] 
								// mov ecx, 8 
								// mul ecx 
								// mov ecx, eax 
								// mov eax, dword ptr [y] 
								// mul ecx 
								// mov ebx, eax 
								// mov eax, dword ptr [x] 
								// mov ecx, 8 
								// mul ecx 
								// add ebx, eax 
								
								// mov eax, dword ptr [pBase] 
								// add eax, ebx 
								// add eax, 4 
								// mov dword ptr [pLong], eax 
								
								// mov eax, dword ptr [frame] 
								// add eax, ebx 
								// add eax, 4 
								// mov dword ptr [pLong2], eax 
							// } 
							pLong2= (long *)(main_w * 8 * y); 
							pLong2 += x * 8; 
							pLong= (long *)((int)(pLong2) + pBase + 4); 
							pLong2 += (int)(frame) + 4; 
							tdata1.data[(y - 16 * j) * 16 + x - 16 * i]= *pLong2; 
							tdata2.data[(y - 16 * j) * 16 + x - 16 * i]= *pLong2 - *pLong; 
						} 
					} 
					n= 0; 
					for (k= 0; k < 256; k++) if (tdata2.data[k]) n++; 
					if (n){ 
						cdata1len= 4096; 
						cr1= compress2 ((unsigned char *)(cdata1), &cdata1len, (unsigned char const *)(&tdata1), sizeof (TILE_DATA), 9); 
						cdata2len= 4096; 
						cr2= compress2 ((unsigned char *)(cdata2), &cdata2len, (unsigned char const *)(&tdata2), sizeof (TILE_DATA), 9); 
						PrefilterLongs ((long *)(tdata1.data), 256); 
						PrefilterLongs ((long *)(tdata2.data), 256); 
						cdata3len= 4096; 
						cr3= compress2 ((unsigned char *)(cdata3), &cdata3len, (unsigned char const *)(&tdata1), sizeof (TILE_DATA), 9); 
						cdata4len= 4096; 
						cr4= compress2 ((unsigned char *)(cdata4), &cdata4len, (unsigned char const *)(&tdata2), sizeof (TILE_DATA), 9); 
						cc= ChooseCompressOutput (&cr1, &cdata1len, 4, 1024); 
						// if (cr1 == Z_OK && cr2 == Z_OK){ 
							// if (cdata1len < cdata2len) pcdata= cdata1; 
							// else pcdata= cdata2; 
						// } else if (cr1 == Z_OK){ 
							// pcdata= cdata1; 
						// } else if (cr2 == Z_OK){ 
							// pcdata= cdata2; 
						// } else { 
							// pcdata= 0; 
							// if (debug_mode) printf ("Error:  Could not compress tile data; compression error codes: %d, %d\r\n", cr1, cr2); 
						// } 
						thead.flags= 4; 
						if (cc == -1){ 
							thead.flags= (thead.flags & (255 - 4)) | 2; 
							thead.compressed_size= 1024; 
							pcdata= (unsigned long *)(&tdata1); 
							if (debug_mode) printf ("Compression error. \r\n"); 
						} else if (!cc){ 
							thead.flags= (thead.flags & (255 - 4)) | 2; 
							thead.compressed_size= 1024; 
							pcdata= (unsigned long *)(&tdata1); 
						} else if (cc == 1){ 
							thead.compressed_size= cdata1len; 
							pcdata= cdata1; 
						} else if (cc == 2){ 
							thead.compressed_size= cdata2len; 
							pcdata= cdata2; 
						} else if (cc == 3){ 
							thead.flags= thead.flags | 2; 
							thead.compressed_size= cdata3len; 
							pcdata= cdata3; 
						} else if (cc == 4){ 
							thead.flags= thead.flags | 2; 
							thead.compressed_size= cdata4len; 
							pcdata= cdata4; 
						} 
						n= FindExactTileMatch (pTiles, sTiles, &thead, pcdata); 
						if (n == -1){ 
							memmove (pTiles + sTiles, &thead, sizeof (TILE_HEADER)); 
							sTiles += sizeof (TILE_HEADER); 
							memmove (pTiles + sTiles, pcdata, thead.compressed_size); 
							sTiles += thead.compressed_size; 
							if (cTiles < 32768){ 
								cmd1= (STREAMCOMMAND *)(pStream + sStream); 
								cmd1->flags= 0x03; 
								cmd1->param= (cTiles<<16) + (j<<8) + i; 
								sStream += sizeof (STREAMCOMMAND); 
								cTiles++; 
							} else { 
								ecmd1= (STREAMCOMMANDEX *)(pStream + sStream); 
								ecmd1->flags= 0x06; 
								ecmd1->paramW= ((j * 16)<<16) + (i * 16); 
								ecmd1->paramN= (16<<16) + 16; 
								ecmd1->paramE= cTiles; 
								sStream += sizeof (STREAMCOMMANDEX); 
								cTiles++; 
							} 
						} else { 
							if (n < 32768){ 
								cmd1= (STREAMCOMMAND *)(pStream + sStream); 
								cmd1->flags= 0x03; 
								cmd1->param= (n<<16) + (j<<8) + i; 
								sStream += sizeof (STREAMCOMMAND); 
							} else { 
								ecmd1= (STREAMCOMMANDEX *)(pStream + sStream); 
								ecmd1->flags= 0x06; 
								ecmd1->paramW= ((j * 16)<<16) + (i * 16); 
								ecmd1->paramN= (16<<16) + 16; 
								ecmd1->paramE= n; 
								sStream += sizeof (STREAMCOMMANDEX); 
							} 
						} 
						if (sStream > aStream - 4096){ 
							aStream += 4096; 
							pStream= GlobalReAlloc (pStream, aStream, GMEM_ZEROINIT); 
						} 
						if (sTiles > aTiles - 65536){ 
							aTiles += 65536; 
							pTiles= GlobalReAlloc (pTiles, aTiles, GMEM_ZEROINIT); 
							// pTmp= pTiles; 
							// aTiles += 65536; 
							// pTiles= GlobalAlloc (GMEM_ZEROINIT, aTiles); 
							// memmove (pTiles, pTmp, sTiles); 
							// GlobalFree (pTmp); 
						} 
					} 
				} 
			} 
		} 
		if (debug_mode) printf ("compress place marker %d\r\n", 21); 
		
		if (sStream > aStream - 4096){ 
			aStream += 4096; 
			pStream= GlobalReAlloc (pStream, aStream, GMEM_ZEROINIT); 
			// pTmp= pStream; 
			// pStream= GlobalAlloc (GMEM_ZEROINIT, aStream + 4096); 
			// memmove (pStream, pTmp, sStream); 
			// aStream += 4096; 
			// GlobalFree (pTmp); 
		} 
		; cont00101: ; 
		// if (debug_mode) printf ("compress place marker %d\r\n", 22); 
		GetStdHandle (STD_OUTPUT_HANDLE); 
		f_compress++; 
		fbuf_compress++; 
		if (fbuf_compress >= fbuf_max) fbuf_compress= 0; 
	} 
	// if (debug_mode) printf ("compress place marker %d\r\n", 23); 
	GetStdHandle (STD_OUTPUT_HANDLE); 
	scmd1= (STREAMCOMMANDSMALL *)(pStream + sStream); 
	scmd1->flags= 12; 
	sStream += sizeof (STREAMCOMMANDSMALL); 
	sCompressedBaseMap= 4 * cBaseMap * fbuf_unit_size; 
	pCompressedBaseMap= GlobalAlloc (GMEM_ZEROINIT, sCompressedBaseMap); 
	// if (debug_mode) printf ("compress place marker %d\r\n", 24); 
	GetStdHandle (STD_OUTPUT_HANDLE); 
	if (compress2 ((unsigned char *)(pCompressedBaseMap), &sCompressedBaseMap, (unsigned char const *)(pBaseMap), cBaseMap * fbuf_unit_size, 9) == Z_OK){ 
		
	} else { 
		memmove (pCompressedBaseMap, pBaseMap, cBaseMap * fbuf_unit_size); 
		sCompressedBaseMap /= 4; 
		if (debug_mode) printf ("(Non-Fatal) Error:  Could not compress base map. \r\n"); 
	} 
	// if (debug_mode) printf ("compress place marker %d\r\n", 25); 
	GetStdHandle (STD_OUTPUT_HANDLE); 
	if (strstr (output_mpc, "data\\") != output_mpc && strstr (output_mpc, "data/") != output_mpc){ 
		strcpy (extra_string, "data\\"); 
		strcat (extra_string, output_mpc); 
		strcpy (output_mpc, extra_string); 
	} 
	if (!strstr (output_mpc, ".mps")){ 
		strcat (output_mpc, ".mps"); 
	} 
	// if (debug_mode) printf ("compress place marker %d\r\n", 26); 
	GetStdHandle (STD_OUTPUT_HANDLE); 
	hFile= (HANDLE)(OpenFile (output_mpc, &ofs, 0x1002)); 
	// if (!hFile){ 
		// sprintf (extra_string, "data\\%d.mpc", atoi (output_mpc)); 
		// hFile= (HANDLE)(OpenFile (extra_string, &ofs, 0x1002)); 
	// } 
	if (!hFile){ 
		if (debug_mode) printf ("Error:  Could open neither %s nor %s for reading and writing. \r\n", output_mpc, extra_string); 
	} else { 
		WriteFile (hFile, &file_head, sizeof (MPS_FILE_HEADER), &bWritten, 0); 
		WriteFile (hFile, &sCompressedBaseMap, sizeof (long), &bWritten, 0); 
		WriteFile (hFile, &cBaseMap, sizeof (long), &bWritten, 0); 
		WriteFile (hFile, pCompressedBaseMap, sCompressedBaseMap, &bWritten, 0); 
		WriteFile (hFile, &sTiles, sizeof (long), &bWritten, 0); 
		WriteFile (hFile, &cTiles, sizeof (long), &bWritten, 0); 
		WriteFile (hFile, pTiles, sTiles, &bWritten, 0); 
		extra_long= 0; 
		WriteFile (hFile, &extra_long, sizeof (long), &bWritten, 0); 
		WriteFile (hFile, &extra_long, sizeof (long), &bWritten, 0); 
		strcpy (sdesc.name, "DEPTH"); 
		extra_long= 8; 
		WriteFile (hFile, &extra_long, sizeof (long), &bWritten, 0); 
		extra_long= 1; 
		WriteFile (hFile, &extra_long, sizeof (long), &bWritten, 0); 
		WriteFile (hFile, &sdesc, sizeof (STREAM_DESCRIPTOR), &bWritten, 0); 
		file_head.rel_ptr_stream= SetFilePointer (hFile, 0, 0, 1); 
		WriteFile (hFile, pStream, sStream, &bWritten, 0); 
		file_head.file_size= SetFilePointer (hFile, 0, 0, 1); 
		file_head.px_size= 8; 
		SetFilePointer (hFile, 0, 0, 0); 
		WriteFile (hFile, &file_head, sizeof (MPS_FILE_HEADER), &bWritten, 0); 
		CloseHandle (hFile); 
	} 
	goto comp_cont02; 
	; compress__stop_script_immediately: ; 
	all_stop= 1; 
	; comp_cont02: ; 
	// if (debug_mode) printf ("compress place marker %d\r\n", 27); 
	GetStdHandle (STD_OUTPUT_HANDLE); 
	if (frame) GlobalFree (frame); 
	if (prevFrame) GlobalFree (prevFrame); 
	if (dmap) GlobalFree (dmap); 
	if (pBaseMap) GlobalFree (pBaseMap); 
	if (pBaseArr) GlobalFree (pBaseArr); 
	if (pStream) GlobalFree (pStream); 
	if (pTiles) GlobalFree (pTiles); 
	if (pCompressedBaseMap) GlobalFree (pCompressedBaseMap); 
	if (debug_mode) printf (":compress place marker %d\r\n", 28); 
	// printf ("... "); 
	GetStdHandle (STD_OUTPUT_HANDLE); 
	compress_complete= 1; 
	ExitThread (0); 
} 