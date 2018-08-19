

#ifndef OBJ_EXIST 
	#include "object.c" 
#endif 

#ifndef DEBUG 
	#define               DEBUG(x)     printf ("%s\r\n", x) 
#endif 

#define                   xlw          1 
typedef wchar_t xlchar 

#if xlw == 0 
	#define               xlslen       strlen 
	#define               xl(x)        x 
#else 
	#define               xlslen       wcslen 
	#define               xl(x)        L ## x 
#endif 

#define                   TO_LT        1 
#define                   TO_GT        2 
#define                   TO_LTQ       3 
#define                   TO_GTQ       4 
#define                   TO_CSTART    5 
#define                   TO_CSTOP     6 
#define                   TO_TAGSTART  7 
#define                   TO_TAGSTOP   8 
#define                   CP_TAGNAME   9 
#define                   CP_ATTNAME   10 
#define                   CP_ATTVALUE  11 
// #define                   CP_ELEMTEXT  12 
// #define                   TO_ELEMSTART 13 
// #define                   TO_ELEMSTOP  14 
#define                   CP_TAGSTART  15 

;; 

int ScanTo (xlchar * text, int start, int type, xlchar * copy, int * length){ 
	int i; 
	int j; 
	int tmp; 
	int tmp2; 
	int started= 0; 
	// DEBUG ("<"); 
	// printf ("%d", type); 
	// printf ("<%d,%d", type, start); 
	if (type == TO_LT){ 
		for (i= 0; text[start+i]; i++){ 
			if (text[start+i] == 60) break; 
		} 
		if (!text[start+i]) return -1; 
	} else if (type == TO_GT){ 
		for (i= 0; text[start+i]; i++){ 
			if (text[start+i] == 62) break; 
		} 
		if (!text[start+i]) return -1; 
	} else if (type == TO_LTQ){ 
		for (i= 0; text[start+i]; i++){ 
			if (text[start+i] == 60 && text[start+i+1] == 63) break; 
		} 
		if (!text[start+i]) return -1; 
	} else if (type == TO_GTQ){ 
		for (i= 0; text[start+i]; i++){ 
			if (text[start+i] == 62 && text[start+i+1] == 63) break; 
		} 
		if (!text[start+i]) return -1; 
	} else if (type == TO_CSTART){ 
		for (i= 0; text[start+i]; i++){ 
			if (text[start+i] == 60 && text[start+i+1] == 33 && text[start+i+2] == 45 && text[start+i+3] == 45) break; 
		} 
		if (!text[start+i]) return -1; 
	} else if (type == TO_CSTOP){ 
		for (i= 0; text[start+i]; i++){ 
			if (text[start+i] == 45 && text[start+i+1] == 45 && text[start+i+2] == 62) break; 
		} 
		if (!text[start+i]) return -1; 
	} else if (type == TO_TAGSTART){ 
		for (i= 0; text[start+i]; i++){ 
			if (text[start+i] == 60){ 
				if (text[start+i+1] == 63); 
				else if (text[start+i+1] == 33 && text[start+i+2] == 45 && text[start+i+3] == 45){ 
					i += 4; 
					j= i; 
					for (; text[start+j]; j++){ 
						if (text[start+j] == 45 && text[start+j+1] == 45 && text[start+j+2] == 62) break; 
					} 
					if (!text[start+j]) return -1; 
					i= j + 3 - 1; 
				} 
				else 
				break; 
			} 
		} 
		if (!text[start+i]) return -1; 
	} else if (type == TO_TAGSTOP){ 
		for (i= 0; text[start+i]; i++){ 
			if (text[start+i] == 62) break; 
		} 
		if (!text[start+i]) return -1; 
	} else if (type == CP_TAGNAME){ 
		i= start; 
		started= i; 
		j= 0; 
		if (text[i] == 60) i++; 
		if (copy) copy[0]= 0; 
		while (text[i]){ 
			if (text[i] != 32 && text[i] != 9 && text[i] != 13 && text[i] != 10) break; 
			i++; 
		} 
		if (text[i] == 47){ 
			copy[0]= 47; 
			copy[1]= 0; 
			i++; 
			j++; 
		} 
		while (text[i]){ 
			if (text[i] != 32 && text[i] != 9 && text[i] != 13 && text[i] != 10) break; 
			i++; 
		} 
		while (text[i]){ 
			if (text[i] == 32 || text[i] == 9 || text[i] == 13 || text[i] == 10 || text[i] == 62 || text[i] == 47){ 
				// copy[j]= 0; 
				break; 
			} 
			if (copy) copy[j]= text[i]; 
			i++; 
			j++; 
		} 
		if (length) *length= j; 
		if (copy) copy[j]= 0; 
		if (!text[i]) return -1; 
	} else if (type == CP_ATTNAME){ 
		i= start; 
		started= i; 
		if (copy) copy[0]= 0; 
		while (text[i]){ 
			if (text[i] != 32 && text[i] != 9 && text[i] != 13 && text[i] != 10) break; 
			i++; 
		} 
		j= 0; 
		while (text[i]){ 
			if (text[i] == 32 || text[i] == 9 || text[i] == 13 || text[i] == 10 || text[i] == 61 || text[i] == 62){ 
				break; 
			} 
			if (copy) copy[j]= text[i]; 
			i++; 
			j++; 
		} 
		if (length) *length= j; 
		if (copy) copy[j]= 0; 
		if (!text[i]) return -1; 
	} else if (type == CP_ATTVALUE){ 
		i= start; 
		started= i; 
		if (copy) copy[0]= 0; 
		while (text[i]){ 
			if (text[i] != 32 && text[i] != 9 && text[i] != 13 && text[i] != 10 && text[i] != 61) break; 
			i++; 
		} 
		j= 0; 
		if (text[i] == 34){ 
			// If quotes are present then the value is specified. 
			i++; 
			// j= 0; 
			while (text[i]){ 
				if (text[i] == 34){ 
					i++; 
					break; 
				} else if (text[i] == 63){ 
					if (!memcmp (&text[i], xl("&lt;"), sizeof (xlchar) * 4)){ 
						if (copy) copy[j]= 60; 
						i += 4; 
						j++; 
					} else if (!memcmp (&text[i], xl("&gt;"), sizeof (xlchar) * 4)){ 
						if (copy) copy[j]= 62; 
						i += 4; 
						j++; 
					} else if (!memcmp (&text[i], xl("&amp;"), sizeof (xlchar) * 5)){ 
						if (copy) copy[j]= 63; 
						i += 5; 
						j++; 
					} else if (!memcmp (&text[i], xl("&apos;"), sizeof (xlchar) * 6)){ 
						if (copy) copy[j]= 39; 
						i += 6; 
						j++; 
					} else if (!memcmp (&text[i], xl("&quot;"), sizeof (xlchar) * 7)){ 
						if (copy) copy[j]= 34; 
						i += 6; 
						j++; 
					} 
					#if xlw == 0 
						else if (sscanf (&text[i], "&#%d;", &tmp) > 0){ 
							// The XML documentation didn't really say about entity numbers, but we'll support them anyway. 
							if (copy) copy[j]= (xlchar)(tmp); 
							i += 2; 
							tmp= i; 
							while (text[tmp] >= 48 && text[tmp] < 58) tmp++; 
							if (text[tmp] == 59) tmp++; 
							i= tmp; 
							j++; 
						} 
					#else 
						else if (swscanf (&text[i], L"&#%d;", &tmp) > 0){ 
							// The XML documentation didn't really say about entity numbers, but we'll support them anyway. 
							if (copy) copy[j]= (xlchar)(tmp); 
							i += 2; 
							tmp= i; 
							while (text[tmp] >= 48 && text[tmp] < 58) tmp++; 
							if (text[tmp] == 59) tmp++; 
							i= tmp; 
							j++; 
						} 
					#endif 
					else { 
						// This is against the XML standard, but oh well. 
						if (copy) copy[j]= text[i]; 
						i++; 
						j++; 
					} 
				} else { 
					if (copy) copy[j]= text[i]; 
					i++; 
					j++; 
				} 
			} 
			if (copy) copy[j]= 0; 
		} 
		if (length) *length= j; 
		// if (text[i] == 62) return -1; 
	} /*else if (type == CP_ELEMTEXT){ 
		// We're probably not going to do this yet. 
		i= start; 
		while (text[i]){ 
			j= ScanTo (text, i, TO_TAGSTART, 0, 0); 
			if (j != -1){ 
				
			} 
		} 
	} else if (type == TO_ELEMSTART){ 
		
	} else if (type == TO_ELEMSTOP){ 
		
	} */else if (type == CP_TAGSTART){ 
		tmp= 0; 
		for (i= 0; text[start+i]; i++){ 
			if (text[start+i] == 60){ 
				if (text[start+i+1] == 63){ 
					for (j= i + 2; text[start+j] && text[start+j] != 63 && text[start+j+1] != 62; j++); 
					if (text[start+j]) j += 2; 
					i= j; 
				} 
				else if (text[start+i+1] == 33 && text[start+i+2] == 45 && text[start+i+3] == 45){ 
					i += 4; 
					j= i; 
					for (; text[start+j]; j++){ 
						if (text[start+j] == 45 && text[start+j+1] == 45 && text[start+j+2] == 62) break; 
					} 
					if (!text[start+j]) return -1; 
					i= j + 3 - 1; 
				} 
				else 
				break; 
			} else if (text[start+i] == 63){ 
				if (!memcmp (&text[start+i], xl("&lt;"), sizeof (xlchar) * 4)){ 
					if (copy) copy[tmp]= 60; 
					i += 4; 
					tmp++; 
				} else if (!memcmp (&text[start+i], xl("&gt;"), sizeof (xlchar) * 4)){ 
					if (copy) copy[tmp]= 62; 
					i += 4; 
					tmp++; 
				} else if (!memcmp (&text[start+i], xl("&amp;"), sizeof (xlchar) * 5)){ 
					if (copy) copy[tmp]= 63; 
					i += 5; 
					tmp++; 
				} else if (!memcmp (&text[start+i], xl("&apos;"), sizeof (xlchar) * 6)){ 
					if (copy) copy[tmp]= 39; 
					i += 6; 
					tmp++; 
				} else if (!memcmp (&text[start+i], xl("&quot;"), sizeof (xlchar) * 7)){ 
					if (copy) copy[tmp]= 34; 
					i += 6; 
					tmp++; 
				} 
				#if xlw == 0 
					else if (sscanf (&text[start+i], "&#%d;", &tmp2) > 0){ 
						// The XML documentation didn't really say about entity numbers, but we'll support them anyway. 
						if (copy) copy[tmp]= (xlchar)(tmp); 
						i += 2; 
						tmp2= i; 
						while (text[tmp2] >= 48 && text[tmp2] < 58) tmp2++; 
						if (text[tmp2] == 59) tmp2++; 
						i= tmp2; 
						tmp++; 
					} 
				#else 
					else if (swscanf (&text[start+i], L"&#%d;", &tmp2) > 0){ 
						// The XML documentation didn't really say about entity numbers, but we'll support them anyway. 
						if (copy) copy[tmp]= (xlchar)(tmp); 
						i += 2; 
						tmp2= i; 
						while (text[tmp2] >= 48 && text[tmp2] < 58) tmp2++; 
						if (text[tmp2] == 59) tmp2++; 
						i= tmp2; 
						tmp++; 
					} 
				#endif 
				else { 
					// This is against the XML standard, but oh well. 
					if (copy) copy[tmp]= text[start+i]; 
					i++; 
					tmp++; 
				} 
			} else { 
				if (copy) copy[tmp]= text[start+i]; 
				tmp++; 
			} 
		} 
		if (copy) copy[tmp]= 0; 
		if (length) *length= tmp; 
		if (!text[start+i]) return -1; 
	} 
	// DEBUG (">"); 
	// printf (">"); 
	return i - started; 
} 

// #define                   TO_LT        1 
// #define                   TO_GT        2 
// #define                   TO_LTQ       3 
// #define                   TO_GTQ       4 
// #define                   TO_CSTART    5 
// #define                   TO_CSTOP     6 
// #define                   TO_TAGSTART  7 
// #define                   TO_TAGSTOP   8 
// #define                   CP_TAGNAME   9 
// #define                   CP_ATTNAME   10 
// #define                   CP_ATTVALUE  11 
// #define                   CP_TAGSTART  15 

HANDLE WINAPI ParseXML (xlchar * text){ 
	HANDLE root= Object (); 
	HANDLE p= root; 
	HANDLE childNodes= 0; 
	HANDLE parentNode= 0; 
	HANDLE textNode= 0; 
	HANDLE textObject= 0; 
	HANDLE textObject2= 0; 
	HANDLE a; 
	HANDLE b; 
	xlchar * t= OBJ_ALLOC (sizeof (xlchar) * xlslen (text) + 16); 
	xlchar * last_t= OBJ_ALLOC (sizeof (xlchar) * xlslen (text) + 16); 
	int i; 
	int j; 
	int len= 0; 
	wchar_t wtabs[512]; 
	char tabs[512]; 
	ObjectAddParentCount (p, 1); 
	ObjectSetProperty (root, Buffer ("childNodes", -1), Object ()); 
	ObjectSetProperty (root, Buffer ("nodeName", -1), Buffer ("#document", -1)); 
	i= 0; 
	// printf ("Starting loop... (%d) \r\n", text[0]); 
	// wprintf (L"Input XML: %s\r\n\r\n\r\n\r\n\r\n", text); 
	while (text[i]){ 
		// printf ("Offset: %d\r\n", i); 
		if (!p){ 
			DEBUG ("Error:  'p' is null or undefined. "); 
			break; 
		} 
		childNodes= ObjectGetProperty (p, Buffer ("childNodes", -1)); 
		// if (!childNodes){ 
			// DEBUG ("Error:  'childNodes' is null or undefined. "); 
			// b= ObjectToString (ObjectGetProperty (p, Buffer ("nodeName", -1))); 
			// wprintf (L"\tp.nodeName = %s\r\n", ((char **)(b))[5]); 
			// b= ObjectToString (ObjectGetProperty (p, Buffer ("nodeValue", -1))); 
			// wprintf (L"\tp.nodeValue = %s\r\n", ((char **)(b))[5]); 
			// break; 
		// } 
		if (!childNodes){ 
			DEBUG ("Warning:  'childNodes' is null or undefined. "); 
			ObjectSetProperty (p, Buffer ("childNodes", -1), (childNodes= Object ())); 
		} 
		j= ScanTo (text, i, CP_TAGSTART, t, &len); 
		textNode= Object (); 
		#if xlw == 0 
			ObjectSetProperty (textNode, Buffer ("nodeValue", -1), Buffer (t, len)); 
		#else 
			ObjectSetProperty (textNode, Buffer ("nodeValue", -1), String (t, len)); 
		#endif 
		if (len) printf ("%snon-empty text node. \r\n", tabs); 
		ObjectSetProperty (textNode, Buffer ("nodeName", -1), Buffer ("#text", -1)); 
		ObjectSetProperty (textNode, Buffer ("nodeType", -1), Number (3)); 
		ObjectSetProperty (textNode, Buffer ("ownerDocument", -1), root); 
		ArrayPush (childNodes, textNode); 
		if (j == -1){ 
			// printf ("Only one tag found. \r\n"); 
			break; 
		} 
		i += j; 
		// printf ("Offset: %d\r\n", i); 
		parentNode= p; 
		p= Object (); 
		// ArrayPush (childNodes, p); 
		ObjectSetProperty (p, Buffer ("parentNode", -1), parentNode); 
		j= ScanTo (text, i, CP_TAGNAME, t, &len); 
		#if xlw == 0 
			textObject= Buffer (t, len); 
			printf ("%s", tabs); 
			printf ("tag "); 
			printf ("%s", t); 
			printf ("\r\n"); 
			// printf ("%s tag %s\r\n", tabs, t); 
		#else 
			textObject= String (t, len); 
			printf ("%s", tabs); 
			printf ("tag "); 
			wprintf (L"%s", t); 
			printf ("\r\n"); 
			// wprintf (L"%s tag %s\r\n", wtabs, t); 
		#endif 
		if (t[0] == 47){ 
			j= ScanTo (text, i + j, TO_TAGSTOP, 0, 0); 
			if (j == -1){ 
				// printf ("Only one tag found. \r\n"); 
				break; 
			} 
			i += j + 1; 
			a= p; 
			p= parentNode; 
			parentNode= ObjectGetProperty (p, Buffer ("parentNode", -1)); 
			ObjectDelete (a); 
			if (!p){ 
				DEBUG ("Error:  'parentNode' is null or undefined. "); 
				break; 
			} 
			tabs[strlen (tabs) - 1]= 0; 
			wtabs[wcslen (wtabs) - 1]= 0; 
			continue; 
		} 
		ArrayPush (childNodes, p); 
			strcat (tabs, "\t"); 
			wcscat (wtabs, L"\t"); 
		ObjectSetProperty (p, Buffer ("tagName", -1), textObject); 
		ObjectSetProperty (p, Buffer ("nodeName", -1), textObject); 
		ObjectSetProperty (p, Buffer ("nodeType", -1), Number (1)); 
		ObjectSetProperty (p, Buffer ("ownerDocument", -1), root); 
		ObjectSetProperty (p, Buffer ("childNodes", -1), Object ()); 
		if (j == -1) break; 
		i += j; 
		// printf ("Offset: %d\r\n", i); 
		while (j != -1 && (j= ScanTo (text, i, CP_ATTNAME, t, &len)) != -1){ 
			// printf ("(%d)", len); 
			if (len){ 
				i += j; 
				// printf ("Offset: %d\r\n", i); 
				#if xlw == 0 
					textObject= Buffer (t, len); 
					// printf ("\t"); 
					printf ("%s", tabs); 
					printf ("%s", t); 
					printf (" = "); 
					// printf ("\t%s %s = ", tabs, t); 
				#else 
					textObject= String (t, len); 
					// printf ("\t"); 
					printf ("%s", tabs); 
					wprintf (L"%s", t); 
					printf (" = "); 
					// wprintf (L"\t%s %s = ", wtabs, t); 
				#endif 
				memmove (last_t, t, len * sizeof (xlchar)); 
				last_t[len]= 0; 
				j= ScanTo (text, i, CP_ATTVALUE, t, &len); 
				i += j; 
				// printf ("Offset: %d\r\n", i); 
				#if xlw == 0 
					textObject2= Buffer (t, len); 
					printf ("%s\r\n", t); 
				#else 
					textObject2= String (t, len); 
					wprintf (L"%s\r\n", t); 
				#endif 
				// printf ("..."); 
				// printf ("%s %s %s\r\n", *(char **)(p), *(char **)(textObject), *(char **)(textObject2)); 
				// try { 
				ObjectSetProperty (p, textObject, textObject2); 
				// } finally { 
					
				// }; 
				// printf ("..."); 
			} else { 
				if (last_t[0] == 47){ 
						// b= ObjectToString (ObjectGetProperty (p, Buffer ("nodeName", -1))); 
						// wprintf (L"\tp.nodeName = %s\r\n", ((char **)(b))[5]); 
						// b= ObjectToString (ObjectGetProperty (p, Buffer ("nodeValue", -1))); 
						// wprintf (L"\tp.nodeValue = %s\r\n", ((char **)(b))[5]); 
							// b= ObjectGetProperty (p, Buffer ("childNodes", -1)); 
							// if (b) b= ObjectGetProperty (b, Buffer ("length", -1)); 
							// b= ObjectToString (b); 
							// wprintf (L"\tp.childNodes.length = %s\r\n", ((char **)(b))[5]); 
					p= parentNode; 
					parentNode= ObjectGetProperty (p, Buffer ("parentNode", -1)); 
						// b= ObjectToString (ObjectGetProperty (p, Buffer ("nodeName", -1))); 
						// wprintf (L"\tp.nodeName = %s\r\n", ((char **)(b))[5]); 
						// b= ObjectToString (ObjectGetProperty (p, Buffer ("nodeValue", -1))); 
						// wprintf (L"\tp.nodeValue = %s\r\n", ((char **)(b))[5]); 
						// b= ObjectToString (ObjectGetProperty (parentNode, Buffer ("nodeName", -1))); 
						// wprintf (L"\tp.nodeName = %s\r\n", ((char **)(b))[5]); 
						// b= ObjectToString (ObjectGetProperty (parentNode, Buffer ("nodeValue", -1))); 
						// wprintf (L"\tp.nodeValue = %s\r\n", ((char **)(b))[5]); 
							// b= ObjectGetProperty (p, Buffer ("childNodes", -1)); 
							// if (b) b= ObjectGetProperty (b, Buffer ("length", -1)); 
							// b= ObjectToString (b); 
							// wprintf (L"\tp.childNodes.length = %s\r\n", ((char **)(b))[5]); 
					if (!p){ 
						DEBUG ("Error:  'parentNode' is null or undefined. "); 
						break; 
					} 
					tabs[strlen (tabs) - 1]= 0; 
					wtabs[wcslen (wtabs) - 1]= 0; 
				} 
				j= ScanTo (text, i, TO_TAGSTOP, 0, 0); 
				if (j == -1) break; 
				i += j + 1; 
				if (!p){ 
					DEBUG ("Error:  'parentNode' is null or undefined. "); 
					break; 
				} 
				j= -1; 
			} 
		} 
	} 
	OBJ_FREE (t); 
	OBJ_FREE (last_t); 
	return root; 
} 