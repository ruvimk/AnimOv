// This code requires the BGRA structure type to be defined; 
// uncomment the following code to define the structure 
// here in this file: 
// typedef struct { 
	// unsigned char b; 
	// unsigned char g; 
	// unsigned char r; 
	// unsigned char a; 
// } BGRA; 

// BmpToPngInMemory () 
// Written by Ruvim K. 
// My main reference was the book "Graphics File Formats" , 
// 2nd edition, by David C. Kay and John R. Levine. 
// This function should work with input bitmaps with OS/2 and Windows 3 headers. 
// The function was tested (single trial) with 32-, 24-, 8-, 4-, and 1-bit bitmap files. 
// I couln't find any 8- and 4-bit RLE-compressed files to test this with, 
// so I do not know, at the moment, whether it'll work with those. 
void * BmpToPngInMemory (void * pBitmap, size_t * sizePngFile){ 
	size_t sPng; 
	void * pPng; 
	BGRA * src_data; 
	BGRA * img_data; 
	BGRA * clr; 
	BGRA * px; 
	unsigned char ind; 
	int x; 
	int y; 
	int i; 
	int j; 
	int k; 
	int w; 
	int h; 
	int hsz;       // Header size. 
	int bpp;       // Bits per pixel. 
	int row_size;  // Size of each row of pixels in the bitmap data. 
	int cs;        // Compressed size. 
	int ct;        // Compression type. 
	unsigned char * src_bytes; 
	if ((j= *((long *)((char *)(pBitmap) + 14))) == 40){ 
		w= *((long *)((char *)(pBitmap) + 18)); 
		h= *((long *)((char *)(pBitmap) + 22)); 
		bpp= *((short *)((char *)(pBitmap) + 28)); 
		ct= *((long *)((char *)(pBitmap) + 30)); 
		cs= *((long *)((char *)(pBitmap) + 34)); 
	} else if (j == 12){ 
		w= *((short *)((char *)(pBitmap) + 18)); 
		h= *((short *)((char *)(pBitmap) + 20)); 
		bpp= *((short *)((char *)(pBitmap) + 24)); 
		ct= 0; 
		cs= 0; 
	} else return 0;      // Strange BITMAPINFOHEADER size. 
	hsz= j; 
	img_data= MZ_MALLOC (4 * w * h + 16);      // 16 bytes more just in case. 
	src_data= (BGRA *)((char *)(pBitmap) + *((long *)((char *)(pBitmap) + 10))); 
	if (bpp == 32){ 
		memmove (img_data, src_data, w * h * 4); 
	} else { 
		if (bpp == 24){ 
			x= w * 3 / 4; 
			y= w * 3 % 4; 
			if (y) x++; 
			row_size= x * 4; 
		} else { 
			x= w * bpp / 8; 
			if (w * bpp % 8) x++; 
			y= x / 4; 
			if (x % 4) y++; 
			row_size= y * 4; 
		} 
		if (bpp == 24 || bpp == 1 || !ct){ 
			for (x= 0; x < w; x++){ 
				for (y= 0; y < h; y++){ 
					clr= (BGRA *)((char *)(img_data) + (w * y * 4) + (4 * x)); 
					if (bpp == 24){ 
						px= (BGRA *)((char *)(src_data) + (row_size * y) + (3 * x)); 
						clr->r= px->r; 
						clr->g= px->g; 
						clr->b= px->b; 
						clr->a= 255; 
					} else if (bpp == 8){ 
						ind= *((unsigned char *)((char *)(src_data) + row_size * y + x)); 
						px= (BGRA *)((char *)(pBitmap) + 14 + j + ((j == 40)? 4:3) * ind); 
						clr->r= px->r; 
						clr->g= px->g; 
						clr->b= px->b; 
						clr->a= 255; 
					} else if (bpp == 4){ 
						ind= ((*((unsigned char *)((char *)(src_data) + row_size * y + x / 2)))>>((x % 2)? 0:4)) & 0x0F; 
						px= (BGRA *)((char *)(pBitmap) + 14 + j + ((j == 40)? 4:3) * ind); 
						clr->r= px->r; 
						clr->g= px->g; 
						clr->b= px->b; 
						clr->a= 255; 
					} else if (bpp == 1){ 
						ind= ((*((unsigned char *)((char *)(src_data) + row_size * y + x / 8)))>>(7 - (x % 8))) & 0x01; 
						px= (BGRA *)((char *)(pBitmap) + 14 + j + ((j == 40)? 4:3) * ind); 
						clr->r= px->r; 
						clr->g= px->g; 
						clr->b= px->b; 
						clr->a= 255; 
					} else { 
						// Unknown. 
						clr->r= 0; 
						clr->g= 0; 
						clr->b= 0; 
						clr->a= 0; 
					} 
				} 
			} 
		} else { 
			// Image is compressed; this is only for 4- and 8-bit images. 
			i= 0; 
			x= 0; 
			y= h - 1; 
			src_bytes= (unsigned char *)(src_data); 
			if (bpp == 4){ 
				while (!(src_bytes[i] == 0 && src_bytes[i+1] == 1) && i < cs){ 
					if (src_bytes[i] == 0 && src_bytes[i+1] == 0){ 
						x= 0; 
						y--; 
						i += 2; 
						if (i % 4) i += 4 - i % 4; 
						continue; 
					} else if (src_bytes[i] == 0 && src_bytes[i+1] == 2){ 
						x += src_bytes[i+2]; 
						y += src_bytes[i+3]; 
						i += 4; 
						continue; 
					} 
					if (src_bytes[i]){ 
						// Repeating group. 
						for (j= 0; j < src_bytes[i]; j++){ 
							clr= (BGRA *)((char *)(img_data) + (w * y * 4) + (4 * x)); 
							ind= ((src_bytes[i+1])>>(4 * ((0 - (j % 2)) + 1))) & 0x0F; 
							px= (BGRA *)((char *)(pBitmap) + 14 + hsz + ((hsz == 40)? 4:3) * ind); 
							if (x >= 0 && x < w && y >= 0 && y < h){ 
								// Only do stuff if the pixel to reference is part of the bitmap. 
								clr->r= px->r; 
								clr->g= px->g; 
								clr->b= px->b; 
								clr->a= 255; 
							} 
							x++; 
							if (x >= w){ 
								x= 0; 
								y--; 
							} 
						} 
						i += 2; 
						continue; 
					} else { 
						// Literal group. 
						for (j= 0; j < src_bytes[i+1]; j++){ 
							clr= (BGRA *)((char *)(img_data) + (w * y * 4) + (4 * x)); 
							ind= ((src_bytes[i+2+(j/2)])>>(4 * ((- - (j % 2)) + 1))) & 0x0F; 
							px= (BGRA *)((char *)(pBitmap) + 14 + hsz + ((hsz == 40)? 4:3) * ind); 
							if (x >= 0 && x < w && y >= 0 && y < h){ 
								// Same as for repeating group, make sure not to access memory outside allocated range. 
								clr->r= px->r; 
								clr->g= px->g; 
								clr->b= px->b; 
								clr->a= 255; 
							} 
							x++; 
							if (x >= w){ 
								x= 0; 
								y--; 
							} 
						} 
						j= src_bytes[i+1] / 2 + src_bytes[i+1] % 2; 
						if (j % 2) j++; 
						i += 2 + j; 
						continue; 
					} 
				} 
			} else if (bpp == 8){ 
				while (!(src_bytes[i] == 0 && src_bytes[i+1] == 1) && i < cs){ 
					if (src_bytes[i] == 0 && src_bytes[i+1] == 0){ 
						x= 0; 
						y--; 
						i += 2; 
						if (i % 4) i += 4 - i % 4; 
						continue; 
					} else if (src_bytes[i] == 0 && src_bytes[i+1] == 2){ 
						x += src_bytes[i+2]; 
						y += src_bytes[i+3]; 
						i += 4; 
						continue; 
					} 
					if (src_bytes[i]){ 
						// Repeating group. 
						for (j= 0; j < src_bytes[i]; j++){ 
							clr= (BGRA *)((char *)(img_data) + (w * y * 4) + (4 * x)); 
							ind= src_bytes[i+1]; 
							px= (BGRA *)((char *)(pBitmap) + 14 + hsz + ((hsz == 40)? 4:3) * ind); 
							if (x >= 0 && x < w && y >= 0 && y < h){ 
								clr->r= px->r; 
								clr->g= px->g; 
								clr->b= px->b; 
								clr->a= 255; 
							} 
							x++; 
							if (x >= w){ 
								x= 0; 
								y--; 
							} 
						} 
						i += 2; 
						continue; 
					} else { 
						// Literal group. 
						for (j= 0; j < src_bytes[i+1]; j++){ 
							clr= (BGRA *)((char *)(img_data) + (w * y * 4) + (4 * x)); 
							ind= src_bytes[i+2+j]; 
							px= (BGRA *)((char *)(pBitmap) + 14 + hsz + ((hsz == 40)? 4:3) * ind); 
							if (x >= 0 && x < w && y >= 0 && y < h){ 
								clr->r= px->r; 
								clr->g= px->g; 
								clr->b= px->b; 
								clr->a= 255; 
							} 
							x++; 
							if (x >= w){ 
								x= 0; 
								y--; 
							} 
						} 
						j= src_bytes[i+1]; 
						if (j % 2) j++; 
						i += 2 + j; 
						continue; 
					} 
				} 
			} 
		} 
	} 
	for (i= 0; i < w * h; i++){ 
		clr= &img_data[i]; 
		j= clr->r; 
		clr->r= clr->b; 
		clr->b= j; 
	} 
	for (y= 0; y < h / 2; y++){ 
		for (x= 0; x < w; x++){ 
			px= &img_data[(y * w) + x]; 
			clr= &img_data[((h - y - 1) * w) + x]; 
			j= px->r; 
			px->r= clr->r; 
			clr->r= j; 
			j= px->g; 
			px->g= clr->g; 
			clr->g= j; 
			j= px->b; 
			px->b= clr->b; 
			clr->b= j; 
			j= px->a; 
			px->a= clr->a; 
			clr->a= j; 
		} 
	} 
	pPng= tdefl_write_image_to_png_file_in_memory (img_data, w, h, 4, &sPng); 
	MZ_FREE (img_data); 
	*sizePngFile= sPng; 
	return pPng; 
} 