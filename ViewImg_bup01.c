#include <windows.h> 
#include <string.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <math.h> 

#include "toonhead.h" 

#include "/fseeko1.c" 

#include "/wminiz.c" 

char main_debug[8192]; 

#include "include/cmd.c" 
#include "include/wlib.c" 

#define CHECKJPEGFORMAT                   4119 
#define CHECKPNGFORMAT                    4120 
#define BI_JPEG                           4 
#define BI_PNG                            5 

unsigned long WINAPI htonl (unsigned long number){ 
	char data[4]; 
	long result; 
	register unsigned char a; 
	memmove (data, &number, 4); 
	a= data[0]; 
	data[0]= data[3]; 
	data[3]= a; 
	a= data[1]; 
	data[1]= data[2]; 
	data[2]= a; 
	memmove (&result, data, 4); 
	return result; 
} 

#define IMAGE_WINDOW_LIST_SIZE     4096 
int sImageWindow_list= IMAGE_WINDOW_LIST_SIZE; 
HANDLE ImageWindow_list [IMAGE_WINDOW_LIST_SIZE]= {0}; 
LRESULT CALLBACK ViewImg_WndProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){ 
	int supported= 0; 
	int i; 
	HANDLE img= 0; 
	char ** cImg= (char **)(img); 
	BITMAPINFO bmi; 
	BITMAPINFOHEADER * bh; 
	unsigned long ul; 
	unsigned long ul2; 
	if (uMsg == WM_DESTROY) PostQuitMessage (0); 
	else if (uMsg == WM_PAINT){ 
		for (i= 0; i < sImageWindow_list; i++){ 
			if (*((HANDLE *)(ImageWindow_list[i])) == hWnd){ 
				img= ImageWindow_list[i]; 
				break; 
				// printf ("Found. "); 
			} 
			// if (((char **)(ImageWindow_list[i]))[0] == hWnd){ 
				// img= ImageWindow_list[i]; 
			// } 
		} 
		if (!img){ 
			// MessageBox (0, "Not Found... ", "...", 0); 
			return DefWindowProc (hWnd, uMsg, wParam, lParam); 
		} 
		cImg= (char **)(img); 
		PAINTSTRUCT ps; 
		HDC hdc= BeginPaint (hWnd, &ps); 
		RECT rClient; 
			// MessageBox (0, "Drawing... ", "...", 0); 
			// printf ("WM_PAINT\r\n"); 
			unsigned long comp_type= 0; 
			// printf ("Checking image type... (%d) ", cImg[4]); 
			if (!strcmp (cImg[4], "image/png")){ 
				ul= CHECKPNGFORMAT; 
				comp_type= BI_PNG; 
				// printf ("image/png\r\n"); 
			} else if (!strcmp (cImg[4], "image/jpg")){ 
				ul= CHECKJPEGFORMAT; 
				comp_type= BI_JPEG; 
				// printf ("image/jpg\r\n"); 
			} else //printf ("unknown\r\n"); 
			GetClientRect (hWnd, &rClient); 
			ul2= ul; 
			// printf ("Starting... \r\n"); 
			if (ul){ 
				if ( 
					(ExtEscape (hdc, QUERYESCSUPPORT, sizeof (ul), (char *)(&ul), 0, 0) > 0) 
					&& 
					(ExtEscape (hdc, ul2, (int)(cImg[5]), cImg[1], sizeof (ul), (char *)(&ul)) > 0) 
					&& 
					(ul == 1) 
				){ 
					DrawText (hdc, "Drawing... ", -1, &rClient, 
						DT_WORDBREAK | DT_CENTER | DT_VCENTER); 
					// printf ("Drawing... \r\n"); 
					memset (&bmi, 0, sizeof (bmi)); 
					bh= &(bmi.bmiHeader); 
					bh->biSize= sizeof (BITMAPINFOHEADER); 
					bh->biWidth= (long)(cImg[2]); 
					bh->biHeight= 0 - (long)(cImg[3]); 
					bh->biPlanes= 1; 
					bh->biCompression= comp_type; 
					bh->biSizeImage= (unsigned long)(cImg[5]); 
					
					if (StretchDIBits (hdc, 
						// Destination. 
						0, 0, (int)(cImg[2]), (int)(cImg[3]), 
						// Source. 
						0, 0, (int)(cImg[2]), (int)(cImg[3]), 
						cImg[1], 
						&bmi, 
						DIB_RGB_COLORS, 
						SRCCOPY) == GDI_ERROR){ 
						// Error drawing image. 
						DrawText (hdc, "Error:  Could not draw image. ", -1, &rClient, 
							DT_WORDBREAK | DT_CENTER | DT_VCENTER); 
						// printf ("Could not draw image. \r\n"); 
					} 
				} else { 
					// Problem using image. 
					DrawText (hdc, "Error:  Problem using image. ", -1, &rClient, 
						DT_WORDBREAK | DT_CENTER | DT_VCENTER); 
					// printf ("Problem using image. \r\n"); 
				} 
			} else { 
				// Unknown image type. 
				DrawText (hdc, "Error:  Unknown image type. ", -1, &rClient, 
					DT_WORDBREAK | DT_CENTER | DT_VCENTER); 
				// printf ("Unknown image type. \r\n"); 
			} 
		EndPaint (hWnd, &ps); 
		return 0; 
	} 
	return DefWindowProc (hWnd, uMsg, wParam, lParam); 
} 
HANDLE WINAPI ImageWindow (char * filename, HWND opt_hWndParent, HINSTANCE opt_hInstance){ 
	int sImg= 0; 
	char * pImg= file_get_contents_size (filename, &sImg); 
	char * type= ""; 
	char * pTmp; 
	size_t sTmp; 
	HANDLE self= GlobalAlloc (GMEM_ZEROINIT, 64); 
		// Object format (HANDLE []): 
			// [0] = hWnd 
			// [1] = lpBuffer 
			// [2] = imgW 
			// [3] = imgH 
			// [4] = lpstrType 
			// [5] = iBufferSize 
			// [6] = lpstrClassName 
	char png_signature [8]= { 
		137, 
		80, 
		78, 
		71, 
		13, 
		10, 
		26, 
		10 
	}; 
	char png_img_header_signature [4]= { 
		73, 72, 68, 82 
	}; 
	char jpg_preset_lengths [22]= { 
		0xD8, 0, 
		0xDD, 2, 
		0xD0, 0, 0xD1, 0, 0xD2, 0, 0xD3, 0, 0xD4, 0, 0xD5, 0, 0xD6, 0, 0xD7, 0, 
		0xD9, 0 
	}; 
	int imgW= 0; 
	int imgH= 0; 
	int i; 
	int j; 
	if (pImg){ 
		if (sImg){ 
			if (pImg[0] == 'B' && pImg[1] == 'M'){ 
				// BMP. 
				pTmp= BmpToPngInMemory (pImg, &sTmp); 
				if (pTmp){ 
					GlobalFree (pImg); 
					pImg= pTmp; 
					sImg= sTmp; 
					pTmp= (char *)(sTmp= 0); 
					type= "image/png"; 
				} 
			} 
			if (((sImg > 8 && !memcmp (pImg, png_signature, 8)) || !strcmp (type, "image/png")) && !memcmp (pImg + 8, png_img_header_signature, 4)){ 
				// PNG. 
				imgW= *((long *)(pImg + 12)); 
				imgH= *((long *)(pImg + 16)); 
				type= "image/png"; 
			} else if (((sImg > 8 && !memcmp (pImg, png_signature, 8)) || !strcmp (type, "image/png")) && !memcmp (pImg + 12, png_img_header_signature, 4)){ 
				// Weird PNG? 
				imgW= htonl (*((long *)(pImg + 16))); 
				imgH= htonl (*((long *)(pImg + 20))); 
				type= "image/png"; 
			} else if (pImg[0] == 0xFF && pImg[1] == 0xD8 && pImg[2] == 0xFF){ 
				// JPG. 
				printf ("jpg\r\n"); 
				j= 0; 
				while (j < sImg && pImg[j+1] != 0xD9){ 
					if (pImg[j+1] == 0xC0){ 
						imgH= htonl (*((short *)(&pImg[j+5]))); 
						imgW= htonl (*((short *)(&pImg[j+7]))); 
					} else if (pImg[j+1] == 0xC2){ 
						
					} 
					for (i= 0; i < 22; i += 2){ 
						if (pImg[j+1] == jpg_preset_lengths[i]){ 
							j += 2 + jpg_preset_lengths[i+1]; 
							continue; 
						} 
					} 
					j += 2 + htonl (*((short *)(&pImg[j+2]))); 
				} 
				type= "image/jpg"; 
			} else if (!type || strcmp (type, "image/png")){ 
				// Other. 
				type= "unknown"; 
			} 
			((char **)(self))[1]= pImg; 
			((char **)(self))[2]= (char *)(imgW); 
			((char **)(self))[3]= (char *)(imgH); 
			((char **)(self))[4]= type; 
			((char **)(self))[5]= (char *)(sImg); 
			char string1[512]; 
			sprintf (string1, "Image Type: %s\r\nWidth: %d\r\nHeight: %d\r\n", type, imgW, imgH); 
			MessageBox (0, string1, "Image Info", 0); 
			((char **)(self))[6]= "ViewImg_Window_Class"; 
			WNDCLASSEX wc; 
			wc.cbSize= sizeof (WNDCLASSEX); 
			wc.style= CS_HREDRAW | CS_VREDRAW; 
			wc.lpfnWndProc= ViewImg_WndProc; 
			wc.cbClsExtra= 0; 
			wc.cbWndExtra= 0; 
			wc.hInstance= opt_hInstance? opt_hInstance:GetModuleHandle (0); 
			wc.hIcon= LoadIcon (0, IDI_APPLICATION); 
			wc.hCursor= LoadCursor (0, IDC_ARROW); 
			wc.hbrBackground= (HBRUSH)(COLOR_WINDOW + 1); 
			wc.lpszMenuName= 0; 
			wc.lpszClassName= ((char **)(self))[6]; 
			wc.hIconSm= wc.hIcon; 
			RegisterClassEx (&wc); 
			if (opt_hWndParent){ 
				((char **)(self))[0]= CreateWindowEx (0, ((char **)(self))[6], filename, WS_CHILD | WS_VISIBLE, 
					0, 0, imgW, imgH, 
					opt_hWndParent, 0, wc.hInstance, 0); 
			} else { 
				((char **)(self))[0]= CreateWindowEx (0, ((char **)(self))[6], filename, WS_OVERLAPPEDWINDOW, 
					CW_USEDEFAULT, CW_USEDEFAULT, imgW, imgH, 
					0, 0, wc.hInstance, 0); 
			} 
			ShowWindow (((char **)(self))[0], SW_SHOWDEFAULT); 
			UpdateWindow (((char **)(self))[0]); 
			for (i= 0; i < sImageWindow_list; i++){ 
				if (!ImageWindow_list[i]){ 
					ImageWindow_list[i]= self; 
					break; 
				} 
			} 
			InvalidateRect (((char **)(self))[0], 0, 1); 
		} else { 
			MessageBox (0, "Error:  Image file is blank or does not exist. \r\n", 0, 0); 
		} 
	} else MessageBox (0, "Error:  Could not read from image file. \r\n", 0, 0); 
	return self; 
} 
void WINAPI UpdateImageSize (HANDLE self){ 
	int sImg= (int)(((char **)(self))[5]); 
	// char * pImg= file_get_contents_size (filename, &sImg); 
	char * pImg= ((char **)(self))[1]; 
	char * type= ""; 
	char * pTmp; 
	size_t sTmp; 
	// HANDLE self= GlobalAlloc (GMEM_ZEROINIT, 64); 
		// Object format (HANDLE []): 
			// [0] = hWnd 
			// [1] = lpBuffer 
			// [2] = imgW 
			// [3] = imgH 
			// [4] = lpstrType 
			// [5] = iBufferSize 
			// [6] = lpstrClassName 
	char png_signature [8]= { 
		137, 
		80, 
		78, 
		71, 
		13, 
		10, 
		26, 
		10 
	}; 
	char png_img_header_signature [4]= { 
		73, 72, 68, 82 
	}; 
	char jpg_preset_lengths [22]= { 
		0xD8, 0, 
		0xDD, 2, 
		0xD0, 0, 0xD1, 0, 0xD2, 0, 0xD3, 0, 0xD4, 0, 0xD5, 0, 0xD6, 0, 0xD7, 0, 
		0xD9, 0 
	}; 
	int imgW= 0; 
	int imgH= 0; 
	int i; 
	int j; 
			if (pImg[0] == 'B' && pImg[1] == 'M'){ 
				// BMP. 
				pTmp= BmpToPngInMemory (pImg, &sTmp); 
				if (pTmp){ 
					GlobalFree (pImg); 
					pImg= pTmp; 
					sImg= sTmp; 
					((char **)(self))[1]= pImg; 
					((char **)(self))[5]= (char *)(sImg); 
					pTmp= (char *)(sTmp= 0); 
					type= "image/png"; 
				} 
			} 
			if (((sImg > 8 && !memcmp (pImg, png_signature, 8)) || !strcmp (type, "image/png")) && !memcmp (pImg + 8, png_img_header_signature, 4)){ 
				// PNG. 
				imgW= *((long *)(pImg + 12)); 
				imgH= *((long *)(pImg + 16)); 
				type= "image/png"; 
			} else if (((sImg > 8 && !memcmp (pImg, png_signature, 8)) || !strcmp (type, "image/png")) && !memcmp (pImg + 12, png_img_header_signature, 4)){ 
				// Weird PNG? 
				imgW= htonl (*((long *)(pImg + 16))); 
				imgH= htonl (*((long *)(pImg + 20))); 
				type= "image/png"; 
			} else if (pImg[0] == 0xFF && pImg[1] == 0xD8 && pImg[2] == 0xFF){ 
				// JPG. 
				j= 0; 
				while (j < sImg && pImg[j+1] != 0xD9){ 
					if (pImg[j+1] == 0xC0){ 
						imgH= htonl (*((short *)(&pImg[j+5]))); 
						imgW= htonl (*((short *)(&pImg[j+7]))); 
					} else if (pImg[j+1] == 0xC2){ 
						
					} 
					for (i= 0; i < 22; i += 2){ 
						if (pImg[j+1] == jpg_preset_lengths[i]){ 
							j += 2 + jpg_preset_lengths[i+1]; 
							continue; 
						} 
					} 
					j += 2 + htonl (*((short *)(&pImg[j+2]))); 
				} 
				type= "image/jpg"; 
			} else if (strcmp (type, "image/png")){ 
				// Other. 
				type= "unknown"; 
			} 
			// ((char **)(self))[1]= pImg; 
			((char **)(self))[2]= (char *)(imgW); 
			((char **)(self))[3]= (char *)(imgH); 
			((char **)(self))[4]= type; 
			// ((char **)(self))[5]= (char *)(sImg); 
			// char string1[512]; 
			// sprintf (string1, "Image Type: %s\r\nWidth: %d\r\nHeight: %d\r\n", type, imgW, imgH); 
			// MessageBox (0, string1, "Image Info", 0); 
			// ((char **)(self))[6]= "ViewImg_Window_Class"; 
			// WNDCLASSEX wc; 
			// wc.cbSize= sizeof (WNDCLASSEX); 
			// wc.style= CS_HREDRAW | CS_VREDRAW; 
			// wc.lpfnWndProc= ViewImg_WndProc; 
			// wc.cbClsExtra= 0; 
			// wc.cbWndExtra= 0; 
			// wc.hInstance= opt_hInstance? opt_hInstance:GetModuleHandle (0); 
			// wc.hIcon= LoadIcon (0, IDI_APPLICATION); 
			// wc.hCursor= LoadCursor (0, IDC_ARROW); 
			// wc.hbrBackground= (HBRUSH)(COLOR_WINDOW + 1); 
			// wc.lpszMenuName= 0; 
			// wc.lpszClassName= ((char **)(self))[6]; 
			// wc.hIconSm= wc.hIcon; 
			// RegisterClassEx (&wc); 
			// if (opt_hWndParent){ 
				// ((char **)(self))[0]= CreateWindowEx (0, ((char **)(self))[6], filename, WS_CHILD | WS_VISIBLE, 
					// 0, 0, imgW, imgH, 
					// opt_hWndParent, 0, wc.hInstance, 0); 
			// } else { 
				// ((char **)(self))[0]= CreateWindowEx (0, ((char **)(self))[6], filename, WS_OVERLAPPEDWINDOW, 
					// CW_USEDEFAULT, CW_USEDEFAULT, imgW, imgH, 
					// 0, 0, wc.hInstance, 0); 
			// } 
			// ShowWindow (((char **)(self))[0], SW_SHOWDEFAULT); 
			// UpdateWindow (((char **)(self))[0]); 
			// for (i= 0; i < sImageWindow_list; i++){ 
				// if (!ImageWindow_list[i]){ 
					// ImageWindow_list[i]= self; 
					// break; 
				// } 
			// } 
} 
WPARAM WINAPI NormalMessageLoop (){ 
	MSG msg; 
	while (GetMessage (&msg, 0, 0, 0)){ 
		TranslateMessage (&msg); 
		DispatchMessage (&msg); 
	} 
	return msg.wParam; 
} 

int main (int argc, char * argv []){ 
	HANDLE img; 
	if (argc > 1){ 
		img= ImageWindow (argv[1], 0, 0); 
		ExitProcess (NormalMessageLoop ()); 
	} else { 
		printf ("Usage:  ViewImg  <image filename>\r\n"); 
	} 
} 