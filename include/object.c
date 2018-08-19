#define                       OBJ_EXIST           1 

HANDLE WINAPI ObjectToString (HANDLE obj); 

typedef struct { 
	char * stuff[32]; 
} OBJ_DUMMY; 

// Object API 
// #define                       OBJ_ALLOC(x)        GlobalAlloc (GMEM_ZEROINIT, x) 
// #define                       OBJ_REALLOC(p,x)    GlobalReAlloc (p, x, GMEM_ZEROINIT) 
// #define                       OBJ_FREE(x)         GlobalFree (x) 
#define                       OBJ_ALLOC(x)        HeapAlloc (GetProcessHeap (), HEAP_ZERO_MEMORY, x) 
#define                       OBJ_REALLOC(p,x)    HeapReAlloc (GetProcessHeap (), HEAP_ZERO_MEMORY, p, x) 
#define                       OBJ_FREE(x)         HeapFree (GetProcessHeap (), 0, x) 
int Object_UniqueNextAssignID= 1; 
int Object_UniqueDeletedCount= 0; 
HANDLE WINAPI Object (){ 
	HANDLE self= OBJ_ALLOC (sizeof (OBJ_DUMMY)); 
		// Format: 
			// [0] = lpstrObjectType 
			// [1] = lplpvoidPropertyIndices 
			// [2] = lplpvoidPropertyValues 
			// [3] = iPropertiesBufferSize 
			// [4] = iParentCount 
			// [5] = lpstrBuffer 
			// [6] = iBufferLength 
			// [7] = iIntValue 
			// [8] = iUniqueObjectID 
	int ThisUniqueID= 0; 
	((char **)(self))[0]= "Object"; 
	((char **)(self))[1]= OBJ_ALLOC (16); 
	((char **)(self))[2]= OBJ_ALLOC (16); 
	((char **)(self))[3]= (char *)(0); 
	((char **)(self))[4]= 0; 
	// This next part is very picky about how it needs to be done, so assembly needs to be used. 
	#if __INTSIZE == 2 
		asm { 
			mov ax, word ptr [Object_UniqueNextAssignID] 
			inc word ptr [Object_UniqueNextAssignID] 
			mov word ptr [ThisUniqueID], ax 
		} 
	#elseif __INTSIZE == 4 
		asm { 
			mov eax, dword ptr [Object_UniqueNextAssignID] 
			inc dword ptr [Object_UniqueNextAssignID] 
			mov dword ptr [ThisUniqueID], eax 
		} 
	#elseif __INTSIZE == 8 
		asm { 
			mov rax, qword ptr [Object_UniqueNextAssignID] 
			inc qword ptr [Object_UniqueNextAssignID] 
			mov qword ptr [ThisUniqueID], rax 
		} 
	#endif 
	// printf ("\tNew Object %d\r\n", ThisUniqueID); 
	((char **)(self))[8]= (char *)(ThisUniqueID); 
	return self; 
} 
void WINAPI ObjectDelete (HANDLE obj){ 
	if (!obj) return; 
	HANDLE a; 
	int c= 0; 
	// printf ("\tDeleting Object %d... \r\n", (int)(((char **)(obj))[8])); 
	// if (((char **)(obj))[2]){ 
		while ((c + 1) * sizeof (char *) <= (int)(((char **)(obj))[3])){ 
			if (((char **)(obj))[1]){ 
				a= (HANDLE)(((char ***)(obj))[1][c]); 
				if (a){ 
					((char **)(a))[4]= (char *)((int)(((char **)(a))[4]) - 1); 
					if (!((char **)(a))[4]) ObjectDelete (a); 
				} 
			} 
			if (((char **)(obj))[2]){ 
				a= (HANDLE)(((char ***)(obj))[2][c]); 
				// if (!a){ 
					// c++; 
					// continue; 
				// } 
				if (a){ 
					((char **)(a))[4]= (char *)((int)(((char **)(a))[4]) - 1); 
					if (!((char **)(a))[4]) ObjectDelete (a); 
				} 
			} 
			c++; 
		} 
	// } 
	if (((char **)(obj))[5]) OBJ_FREE (((char **)(obj))[5]); 
	OBJ_FREE (obj); 
	Object_UniqueDeletedCount++; 
} 
void WINAPI ObjectAddParentCount (HANDLE obj, int count){ 
	((char **)(obj))[4]= (char *)((int)(((char **)(obj))[4]) + count); 
} 
int WINAPI ObjectCompare (HANDLE obj1, HANDLE obj2){ 
	if (obj1 == obj2){ 
		if (obj1 && obj2){ 
			if (((char **)(obj1))[8] == ((char **)(obj2))[8]) return 0; 
		} 
	} 
	if (!obj1) return (1<<31); 
	if (!obj2) return (1<<31); 
	char ** a= (char **)(obj1); 
	char ** b= (char **)(obj2); 
	wchar_t * one= OBJ_ALLOC (((int)(a[6])) * 2 + 16); 
	wchar_t * two= OBJ_ALLOC (((int)(b[6])) * 2 + 16); 
	char string1[512]; 
	int i; 
	int j; 
	int lenOne= 0; 
	int lenTwo= 0; 
	// printf ("obj1=\""); 
	if (!strcmp (a[0], "Buffer")){ 
		j= (int)(a[6]); 
		for (i= 0; i < j; i++){ 
			one[i]= (wchar_t)(((char *)(a[5]))[i]); 
			// printf ("%c", (char)(one[i])); 
		} 
		lenOne= j; 
	} else if (!strcmp (a[0], "String")){ 
		memmove (one, ((char *)(a[5])), (int)(a[6]) * sizeof (wchar_t)); 
		// for (i= 0; i < ((int)(a[6])); i++) printf ("%c", (char)(one[i])); 
		lenOne= (int)(a[6]); 
	} else if (!strcmp (a[0], "Number")){ 
		sprintf (string1, "%f", *((double *)(a[5]))); 
		j= strlen (string1); 
		for (i= 0; i < j; i++){ 
			one[i]= (wchar_t)(((char *)(string1))[i]); 
			// printf ("%c", (char)(one[i])); 
		} 
		lenOne= j; 
	} else { 
		OBJ_FREE (one); 
		OBJ_FREE (two); 
		return (1<<31); 
	} 
	// printf ("\"\r\n"); 
	// printf ("obj2=\""); 
	if (!strcmp (b[0], "Buffer")){ 
		j= (int)(b[6]); 
		for (i= 0; i < j; i++){ 
			two[i]= (wchar_t)(((char *)(b[5]))[i]); 
			// printf ("%c", (char)(two[i])); 
		} 
		lenTwo= j; 
	} else if (!strcmp (b[0], "String")){ 
		memmove (two, ((char *)(b[5])), (int)(b[6]) * sizeof (wchar_t)); 
		// for (i= 0; i < ((int)(b[6])); i++) printf ("%c", (char)(two[i])); 
		lenTwo= (int)(b[6]); 
	} else if (!strcmp (b[0], "Number")){ 
		sprintf (string1, "%f", *((double *)(b[5]))); 
		j= strlen (string1); 
		for (i= 0; i < j; i++){ 
			two[i]= (wchar_t)(((char *)(string1))[i]); 
			// printf ("%c", (char)(two[i])); 
		} 
		lenTwo= j; 
	} else { 
		OBJ_FREE (one); 
		OBJ_FREE (two); 
		return (1<<31); 
	} 
	// printf ("\"\r\n"); 
	if (lenOne < lenTwo){ 
		j= memcmp (one, two, lenOne); 
		if (j) return j; 
		else return (1<<31); 
	} else if (lenOne > lenTwo){ 
		j= memcmp (one, two, lenTwo); 
		if (j) return j; 
		else return (1<<31); 
	} 
	return memcmp (one, two, lenOne); 
} 
HANDLE WINAPI ObjectGetProperty (HANDLE obj, HANDLE ind){ 
	// char * val1; 
	// asm { 
		// mov eax, dword ptr [obj] 
		// mov ebx, eax 
		// add ebx, 8 
		// mov eax, dword ptr [ebx] 
		// mov ebx, eax 
		// mov eax, dword ptr [ebx] 
		// mov dword ptr [val1], eax 
	// } 
	// printf ("(debug (%d))\r\n", (int)val1); 
	// printf ("debug %d\r\n", (int)((((char ***)(obj))[2][0]))); 
	if (!obj){ 
		if (ind) if (!((char **)(ind))[4]) ObjectDelete (ind); 
		return 0; 
	} 
	// printf ("debug %d\r\n", (int)((((char ***)(obj))[2][0]))); 
	if (!ind) return 0; 
	// printf ("debug %d\r\n", (int)((((char ***)(obj))[2][0]))); 
	if (!((char **)(obj))[0] || !((char **)(obj))[1] || !((char **)(obj))[2] || !((char **)(obj))[3]){ 
		if (ind) if (!((char **)(ind))[4]) ObjectDelete (ind); 
		return 0; 
	} 
	// printf ("debug %d\r\n", (int)((((char ***)(obj))[2][0]))); 
	// First scan the property name string to see if the property exists. 
	int c= 0; 
	int found= 0; 
	int not_ind= 0; 
	int ind_type= 0; 
	// printf ("debug %d\r\n", (int)((((char ***)(obj))[2][0]))); 
	if (!strcmp (((char **)(ind))[0], "Buffer")){ 
		ind_type= 4; 
		if (!((char **)(ind))[6]) not_ind= 1; 
	} else if (!strcmp (((char **)(ind))[0], "String")){ 
		ind_type= 5; 
		if (!((char **)(ind))[6]) not_ind= 1; 
	} else if (!strcmp (((char **)(ind))[0], "Number")){ 
		ind_type= 6; 
		if (!((char **)(ind))[5]) not_ind= 1; 
		else if (!*((double *)(((char **)(ind))[5]))) not_ind= 1; 
	} else if (!strcmp (((char **)(ind))[0], "Bool")){ 
		ind_type= 7; 
		if (!((char **)(ind))[7]) not_ind= 1; 
	} else if (!strcmp (((char **)(ind))[0], "NULL")){ 
		ind_type= 8; 
		not_ind= 1; 
	} 
	// printf ("debug %d\r\n", (int)((((char ***)(obj))[2][0]))); 
	while ((c + 1) * sizeof (char *) <= (int)(((char **)(obj))[3])){ 
		// printf ("%d\r\n", (int)(((char ***)(obj))[1][c])); 
		// printf ("debug %d\r\n", (int)((((char ***)(obj))[2][0]))); 
		if (!(int)(((char ***)(obj))[1][c])){ 
			c++; 
			continue; 
		} 
		if (!(int)(((char ****)(obj))[1][c])[0]){ 
			c++; 
			continue; 
		} 
		// printf ("ind_type=%d\r\n", ind_type); 
		// printf ("type=%s;\r\n", (((char ****)(obj))[1][c])[0]); 
		// printf ("comp=%d\r\n", strcmp (((((char ****)(obj))[1][c])[0]), "Buffer")); 
		// printf ("%d, %d\r\n", (int)(((char ***)(obj))[1][c]), (int)((char *)(ind))); 
		if ((((char ***)(obj))[1][c] == (char *)(ind)) && (((char **)(obj))[8] == ((char **)(ind))[8])){ 
			found= 1; 
			break; 
		} else if (!strcmp (((((char ****)(obj))[1][c])[0]), "Buffer")){ 
			// printf ("Buffer\r\n"); 
			if (ind_type >= 4 && ind_type < 7){ 
				if (!ObjectCompare ((HANDLE)(((char ***)(obj))[1][c]), ind)){ 
					// printf ("Found. \r\n"); 
					found= 1; 
					break; 
				} 
			} 
		} else if (!strcmp ((((char ****)(obj))[1][c])[0], "String")){ 
			if (ind_type >= 4 && ind_type < 7){ 
				if (!ObjectCompare ((HANDLE)(((char ***)(obj))[1][c]), ind)){ 
					found= 1; 
					break; 
				} 
			} 
		} else if (!strcmp ((((char ****)(obj))[1][c])[0], "Number")){ 
			if (ind_type >= 4 && ind_type < 7){ 
				if (!ObjectCompare ((HANDLE)(((char ***)(obj))[1][c]), ind)){ 
					found= 1; 
					break; 
				} 
			} 
		} else if (!strcmp ((((char ****)(obj))[1][c])[0], "Bool")){ 
			if (ind_type == 7){ 
				if ((((char ***)(obj))[1][c])[7]){ 
					if (!not_ind){ 
						found= 1; 
						break; 
					} 
				} else { 
					if (not_ind){ 
						found= 1; 
						break; 
					} 
				} 
			} 
		} else if (!strcmp ((((char ****)(obj))[1][c])[0], "NULL")){ 
			if (ind_type == 8){ 
				found= 1; 
				break; 
			} 
		} 
		c++; 
	} 
	// printf ("c = %d; max %d\r\n", c, (int)(((char **)(obj))[3])); 
	// Next, check if the property index points to an accessible location. 
	if ((c + 1) * sizeof (char *) > (int)(((char **)(obj))[3])){ 
		if (ind) if (!((char **)(ind))[4]) ObjectDelete (ind); 
		return 0; 
	} 
	// printf ("Okay. \r\n"); 
	// Now return the property value. 
	// printf ("offset %d\r\n", (int)(&(((char ***)(obj))[2][c]))); 
	// printf ("debug %d\r\n", (int)((((char ***)(obj))[2][0]))); 
	if (ind) if (!((char **)(ind))[4]) ObjectDelete (ind); 
	// else printf ("No-delete; ID=%d\r\n", ((char **)(ind))[8]); 
	return (HANDLE)(((char ***)(obj))[2][c]); 
} 
HANDLE WINAPI ObjectSetProperty (HANDLE obj, HANDLE ind, HANDLE value){ 
	if (!obj){ 
		if (ind) if (!((char **)(ind))[4]) ObjectDelete (ind); 
		return 0; 
	} 
	if (!ind) return 0; 
	if (!((char **)(obj))[0]) ((char **)(obj))[0]= "Object"; 
	if (!((char **)(obj))[1]) ((char **)(obj))[1]= OBJ_ALLOC (16); 
	if (!((char **)(obj))[2]){ 
		((char **)(obj))[2]= OBJ_ALLOC (16); 
		((char **)(obj))[3]= (char *)(16); 
	} 
	// First scan the property name string to see if the property exists. 
	int c= 0; 
	int found= 0; 
	int not_ind= 0; 
	int ind_type= 0; 
	if (!strcmp (((char **)(ind))[0], "Buffer")){ 
		ind_type= 4; 
		if (!((char **)(ind))[6]) not_ind= 1; 
	} else if (!strcmp (((char **)(ind))[0], "String")){ 
		ind_type= 5; 
		if (!((char **)(ind))[6]) not_ind= 1; 
	} else if (!strcmp (((char **)(ind))[0], "Number")){ 
		ind_type= 6; 
		if (!((char **)(ind))[5]) not_ind= 1; 
		else if (!*((double *)(((char **)(ind))[5]))) not_ind= 1; 
	} else if (!strcmp (((char **)(ind))[0], "Bool")){ 
		ind_type= 7; 
		if (!((char **)(ind))[7]) not_ind= 1; 
	} else if (!strcmp (((char **)(ind))[0], "NULL")){ 
		ind_type= 8; 
		not_ind= 1; 
	} 
	// printf ("part 1\r\n"); 
	while ((c + 1) * sizeof (char *) <= (int)(((char **)(obj))[3])){ 
		if (!(int)(((char ***)(obj))[1][c])){ 
			c++; 
			continue; 
		} 
		if (!(int)(((char ****)(obj))[1][c])[0]){ 
			c++; 
			continue; 
		} 
		// printf ("Scanning (%s)... \r\n", (((char ****)(obj))[1][c])[0]); 
		if ((((char ***)(obj))[1][c] == (char *)(ind)) && (((char **)(obj))[8] == ((char **)(ind))[8])){ 
			found= 1; 
			break; 
		} else if (!strcmp ((((char ****)(obj))[1][c])[0], "Buffer")){ 
			if (ind_type >= 4 && ind_type < 7){ 
				if (!ObjectCompare ((HANDLE)(((char ***)(obj))[1][c]), ind)){ 
					found= 1; 
					break; 
				} 
			} 
		} else if (!strcmp ((((char ****)(obj))[1][c])[0], "String")){ 
			if (ind_type >= 4 && ind_type < 7){ 
				if (!ObjectCompare ((HANDLE)(((char ***)(obj))[1][c]), ind)){ 
					found= 1; 
					break; 
				} 
			} 
		} else if (!strcmp ((((char ****)(obj))[1][c])[0], "Number")){ 
			if (ind_type >= 4 && ind_type < 7){ 
				if (!ObjectCompare ((HANDLE)(((char ***)(obj))[1][c]), ind)){ 
					found= 1; 
					break; 
				} 
			} 
		} else if (!strcmp ((((char ****)(obj))[1][c])[0], "Bool")){ 
			if (ind_type == 7){ 
				if ((((char ***)(obj))[1][c])[7]){ 
					if (!not_ind){ 
						found= 1; 
						break; 
					} 
				} else { 
					if (not_ind){ 
						found= 1; 
						break; 
					} 
				} 
			} 
		} else if (!strcmp ((((char ****)(obj))[1][c])[0], "NULL")){ 
			if (ind_type == 8){ 
				found= 1; 
				break; 
			} 
		} 
		c++; 
	} 
	// printf ("part 2\r\n"); 
	// printf ("1\r\n"); 
	// Next, check if the property index points to an accessible location. 
	if ((c + 1) * sizeof (char *) > (int)(((char **)(obj))[3])){ 
		// printf ("\tupdating location... \r\n"); 
		// printf ("Current Size: %d\r\n", (int)(((char **)(obj))[3])); 
		((char **)(obj))[3]= (char *)((c + 1) * sizeof (char *)); 
		// printf ("Resize To: %d\r\n", (c + 1) * sizeof (char *)); 
		((char **)(obj))[1]= OBJ_REALLOC (((char **)(obj))[1], (c + 1) * sizeof (char *) + 16); 
			// printf ("Last Error: %d (c=%d)\r\n", GetLastError (), c); 
		((char **)(obj))[2]= OBJ_REALLOC (((char **)(obj))[2], (c + 1) * sizeof (char *) + 16); 
		// printf ("New Pointers: %d, %d\r\n", (int)((char **)(obj))[1], (int)((char **)(obj))[2]); 
		// printf ("\tsetting new property name... \r\n"); 
		// printf ("%d\r\n", ((int)(((char ***)(obj))[1]))); 
		(((char ***)(obj))[1][c])= (char *)(ind); 
	} //else (((char ***)(obj))[1][c])= (char *)(ind); 
	if (!found){ 
		// If new property, ... 
		(((char ***)(obj))[1][c])= (char *)(ind); 
		((char **)(obj))[4]= (char *)((int)(((char **)(obj))[4]) + 1); 
	} 
	// printf ("2\r\n"); 
	// Set the property value. 
	// printf ("\tobtaining old value... \r\n"); 
	HANDLE old_value= (HANDLE)(((char ***)(obj))[2][c]); 
	// printf ("\tsetting new value... \r\n"); 
	// printf ("set, c = %d\r\n", c); 
	((char ***)(obj))[2][c]= (char *)(value); 
	// Increment the parent count of the new value. 
	// printf ("\tincrementing new value count... \r\n"); 
		if (value) ((char **)(value))[4]= (char *)(((int)(((char **)(value))[4])) + 1); 
	// Take care of the old value. 
	if (old_value){ 
		// Decrement the parent count of the old value. 
		// printf ("\tdecrementing old value count... \r\n"); 
		((char **)(old_value))[4]= (char *)(((int)(((char **)(value))[4])) - 1); 
		// If the new parent count of the old value is 0, destroy the old value. 
		if (!((char **)(old_value))[4]){ 
			// printf ("\tdeleting old value... \r\n"); 
			ObjectDelete (old_value); 
		} 
	} 
	// Return the new value. 
	// printf ("\tind=%d\r\n", (int)((char *)(ind))); 
	// HANDLE one= ObjectToString (ind); 
	// HANDLE two= ObjectToString (value); 
	// wprintf (L"\r\nObject[%s]=%s\r\n", ((wchar_t **)(one))[5], ((wchar_t **)(two))[5]); 
	// ObjectDelete (one); 
	// ObjectDelete (two); 
	if (ind) if (!((char **)(ind))[4]){ 
		ObjectDelete (ind); 
	} 
	return (HANDLE)(((char **)(obj))[2][c]); 
} 
// char * debug1= ""; 
HANDLE WINAPI String (wchar_t * text, int size){ 
	HANDLE a= Object (); 
	int length= size; 
	if (size == -1) length= wcslen (text); 
	// printf ("Debug1: %s\r\n", debug1); 
	// printf ("Offset 1: %d\r\n", &((char **)(a))[5]); 
	((char **)(a))[5]= OBJ_ALLOC (length * sizeof (wchar_t) + 16); 
	// printf ("Debug1: %s\r\n", debug1); 
	// printf ("Offset 2: %d\r\n", &((char **)(a))[6]); 
	((char **)(a))[6]= (char *)(length); 
	// printf ("Debug1: %s\r\n", debug1); 
	// printf ("Offset 3: %d\r\n", text); 
	memmove (((char **)(a))[5], text, length * sizeof (wchar_t)); 
	// printf ("Offset 4: %d\r\n", &((char **)(a))[5]); 
	// printf ("Debug1: %s\r\n", debug1); 
	// strcpy (((char **)(a))[0], "String"); 
	((char **)(a))[0]= "String"; 
	// printf ("Debug1: %s\r\n", debug1); 
	// printf ("&Debug1: %d\r\n", debug1); 
	// printf ("String's Offset: %d\r\n", &((char **)(a))[0]); 
	// printf ("String's Offset: %d\r\n", ((char **)(a))[0]); 
	return a; 
} 
HANDLE WINAPI Buffer (char * text, int size){ 
	HANDLE a= Object (); 
	int length= size; 
	if (size == -1) length= strlen (text); 
	((char **)(a))[5]= OBJ_ALLOC (length + 16); 
	((char **)(a))[6]= (char *)(length); 
	memmove (((char **)(a))[5], text, length); 
	// strcpy (((char **)(a))[0], "Buffer"); 
	((char **)(a))[0]= "Buffer"; 
	return a; 
} 
HANDLE WINAPI Number (double value){ 
	HANDLE a= Object (); 
	double * b= OBJ_ALLOC (sizeof (double) + 16); 
	((char **)(a))[5]= (char *)(b); 
	((char **)(a))[6]= (char *)(sizeof (double)); 
	*b= value; 
	// strcpy (((char **)(a))[0], "Number"); 
	((char **)(a))[0]= "Number"; 
	return a; 
} 
HANDLE WINAPI Bool (int value){ 
	HANDLE a= Object (); 
	int v; 
	if (value) v= 1; 
	else v= 0; 
	((char **)(a))[7]= (char *)(v); 
	// strcpy (((char **)(a))[0], "Bool"); 
	((char **)(a))[0]= "Bool"; 
	return a; 
} 
HANDLE WINAPI Null (){ 
	HANDLE a= Object (); 
	// strcpy (((char **)(a))[0], "NULL"); 
	((char **)(a))[0]= "NULL"; 
	return a; 
} 
HANDLE ArrayPush (HANDLE obj, HANDLE item){ 
	HANDLE length= ObjectGetProperty (obj, Buffer ("length", -1)); 
	int len= 0; 
	if (length){ 
		len= (int)(*((double *)(((char **)(length))[5]))); 
	} else { 
		length= Number (len); 
		ObjectSetProperty (obj, Buffer ("length", -1), length); 
	} 
	ObjectSetProperty (obj, Number ((double)(len)), item); 
	*((double *)(((char **)(length))[5]))= (double)(len + 1); 
	return length; 
} 
HANDLE WINAPI ObjectToString (HANDLE obj){ 
	if (!obj) return String (L"undefined", -1); 
	wchar_t * s1; 
	HANDLE * arr; 
	HANDLE a; 
	HANDLE b; 
	int len= 0; 
	int total= 0; 
	HANDLE s= 0; 
	char * type= ((char **)(obj))[0]; 
	wchar_t * buff; 
	char string1[512]; 
	int i; 
	// printf ("ObjectToString (%d)\r\n", ((char **)(obj))[8]); 
	// printf ("Type: %s\r\n", type); 
	if (!strcmp (type, "String")){ 
		s= String ((wchar_t *)(((char **)(obj))[5]), (int)(((char **)(obj))[6])); 
	} else if (!strcmp (type, "Buffer")){ 
		// printf ("Buffer\r\n"); 
		buff= OBJ_ALLOC ((int)(((char **)(obj))[6]) * sizeof (wchar_t) + 16); 
		for (i= 0; i < (int)(((char **)(obj))[6]); i++){ 
			buff[i]= (wchar_t)((((char **)(obj))[5])[i]); 
			// printf (":%c\r\n", ((((char **)(obj))[5])[i])); 
			// printf ("Type: %s\r\n", type); 
		} 
		// printf ("Type: %s\r\n", type); 
		// debug1= type; 
		s= String (buff, (int)(((char **)(obj))[6])); 
		// debug1= ""; 
		// printf ("Type: %s\r\n", type); 
		// printf ("%d, %d\r\n", (char *)(obj), (char *)(s)); 
		OBJ_FREE (buff); 
	} else if (!strcmp (type, "Number")){ 
		sprintf (string1, "%f", *((double *)(((char **)(obj))[5]))); 
		buff= OBJ_ALLOC (strlen (string1) + 16); 
		for (i= 0; string1[i]; i++) buff[i]= (wchar_t)(string1[i]); 
		s= String (buff, i); 
		OBJ_FREE (buff); 
	} else if (!strcmp (type, "Bool")){ 
		if (((char **)(obj))[7]){ 
			s= String (L"true", -1); 
		} else { 
			s= String (L"false", -1); 
		} 
	} else if (!strcmp (type, "NULL")){ 
		s= String (L"null", -1); 
	} else if (!strcmp (type, "Object")){ 
		return String (L"[object Object]", -1); 
	} else if (!strcmp (type, "Array")){ 
		a= ObjectGetProperty (obj, Buffer ("length", -1)); 
		len= (int)(*((double *)(((char **)(a))[5]))); 
		arr= OBJ_ALLOC (sizeof (HANDLE) * len + 16); 
		total= 0; 
		for (i= 0; i < len; i++){ 
			arr[i]= ObjectToString (ObjectGetProperty (obj, Number ((double)(i)))); 
			total += (int)(((char **)(arr[i]))[6]); 
		} 
		s1= OBJ_ALLOC (sizeof (wchar_t) * total + sizeof (wchar_t) * len + 16); 
		for (i= 0; i < len; i++){ 
			if (i) wcscat (s1, L","); 
			wcscat (s1, (wchar_t *)(((char ***)(arr[i]))[5])); 
			ObjectDelete (arr[i]); 
		} 
		OBJ_FREE (arr); 
		a= String (s1, -1); 
		OBJ_FREE (s1); 
		return a; 
	} else s= String (L"unknown", -1); 
	if (!(((char **)(obj))[4])) ObjectDelete (obj); 
	// printf ("Type: %s\r\n", type); 
	return s; 
} 
HANDLE WINAPI ObjectDump (HANDLE obj, HANDLE ** p_all, int * s_all){ 
	wchar_t * s1; 
	HANDLE * arr; 
	HANDLE a; 
	HANDLE b; 
	HANDLE tmp; 
	HANDLE * all; 
	int alls; 
	if (p_all && s_all){ 
		all= *p_all; 
		alls= *s_all; 
	} else { 
		all= OBJ_ALLOC (4096); 
		alls= 4096; 
	} 
	int len= 0; 
	int total= 0; 
	int i; 
	int j; 
	int k; 
	if (!obj) return String (L"undefined", -1); 
	if (!(int)((((char **)(obj))[0]))) return String (L"undefined", -1); 
	if (!strcmp (((char **)(obj))[0], "Object")){ 
		// ((int)(((char **)(obj))[3])); 
		len= ((int)(((char **)(obj))[3])) / sizeof (char *); 
		arr= OBJ_ALLOC (sizeof (HANDLE) * len * 2 + 16); 
		total= 0; 
		for (i= 0; i < len; i++){ 
			if (!(((char ***)(obj))[1])) continue; 
			if (!(((char ***)(obj))[2])) continue; 
			a= (HANDLE)(((char ***)(obj))[1][i]); 
			b= (HANDLE)(((char ***)(obj))[2][i]); 
			if (!a) continue; 
			tmp= ObjectToString (a); 
			wprintf (L"dump %s = ", ((char **)(tmp))[5]); 
			ObjectDelete (tmp); 
			tmp= ObjectToString (b); 
			wprintf (L"%s\r\n", ((char **)(tmp))[5]); 
			ObjectDelete (tmp); 
			// Some checks. 
				k= 0; 
				for (j= 0; !k && j * sizeof (HANDLE) < alls; j++){ 
					if (a == all[j] || b == all[j]){ 
						k= 1; 
						continue; 
					} else if (!all[j]){ 
						all[j+0]= a; 
						all[j+1]= b; 
						k= 1; 
						continue; 
					} 
				} 
				if (all[j+0] != b) continue; 
				if (alls - j < 2048){ 
					all= OBJ_REALLOC (all, alls + 2048); 
					if (p_all){ 
						*p_all= all; 
					} 
					if (s_all){ 
						*s_all= alls; 
					} 
				} 
			arr[i*2+0]= ObjectDump (a, &all, &alls); 
			arr[i*2+1]= ObjectDump (b, &all, &alls); 
			total += (int)(((char **)(arr[i+0]))[6]); 
			total += (int)(((char **)(arr[i+1]))[6]); 
		} 
		s1= OBJ_ALLOC (sizeof (wchar_t) * total + sizeof (wchar_t) * len * 4 + 16); 
		wcscpy (s1, L"{"); 
		for (i= 0; i < len; i++){ 
			if (i) wcscat (s1, L","); 
			wcscat (s1, (wchar_t *)(((char ***)(arr[i*2+0]))[5])); 
			wcscat (s1, L":"); 
			wcscat (s1, (wchar_t *)(((char ***)(arr[i*2+1]))[5])); 
			ObjectDelete (arr[i*2+0]); 
			ObjectDelete (arr[i*2+1]); 
		} 
		OBJ_FREE (arr); 
		wcscat (s1, L"}"); 
		a= String (s1, -1); 
		OBJ_FREE (s1); 
		if (!p_all){ 
			OBJ_FREE (all); 
		} 
		return a; 
	} else if (!strcmp (((char **)(obj))[0], "Array")){ 
		a= ObjectGetProperty (obj, Buffer ("length", -1)); 
		len= (int)(*((double *)(((char **)(a))[5]))); 
		arr= OBJ_ALLOC (sizeof (HANDLE) * len + 16); 
		total= 0; 
		for (i= 0; i < len; i++){ 
			// Some checks. 
				k= 0; 
				for (j= 0; !k && j * sizeof (HANDLE) < alls; j++){ 
					if (a == all[j] || b == all[j]){ 
						k= 1; 
						continue; 
					} else if (!all[j]){ 
						all[j+0]= a; 
						all[j+1]= b; 
						k= 1; 
						continue; 
					} 
				} 
				if (all[j+0] != b) continue; 
				if (alls - j < 2048){ 
					all= OBJ_REALLOC (all, alls + 2048); 
					if (p_all){ 
						*p_all= all; 
					} 
					if (s_all){ 
						*s_all= alls; 
					} 
				} 
			arr[i]= ObjectDump (ObjectGetProperty (obj, Number ((double)(i))), &all, &alls); 
			total += (int)(((char **)(arr[i]))[6]); 
		} 
		s1= OBJ_ALLOC (sizeof (wchar_t) * total + sizeof (wchar_t) * len + 16); 
		wcscpy (s1, L"["); 
		for (i= 0; i < len; i++){ 
			if (i) wcscat (s1, L","); 
			wcscat (s1, (wchar_t *)(((char ***)(arr[i]))[5])); 
			ObjectDelete (arr[i]); 
		} 
		wcscat (s1, L"]"); 
		OBJ_FREE (arr); 
		a= String (s1, -1); 
		OBJ_FREE (s1); 
		if (!p_all){ 
			OBJ_FREE (all); 
		} 
		return a; 
	} else { 
		if (!p_all){ 
			OBJ_FREE (all); 
		} 
		return ObjectToString (obj); 
	} 
} 
HANDLE WINAPI ObjectCopy (HANDLE obj){ 
	HANDLE clone= OBJ_ALLOC (sizeof (OBJ_DUMMY)); 
	memmove (clone, obj, sizeof (OBJ_DUMMY)); 
	return clone; 
} 