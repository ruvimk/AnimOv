void TrimAtSpace (char * the_string){ 
	char * p= the_string; 
	while (*p && *p != 32 && *p != 9) p++; 
	*p= 0; 
} 

char * SwitchStart (LPSTR CmdLine, char * SwitchName){ 
	char string1[512]; 
	char * cmdline= (char *)(CmdLine); 
	char * p= cmdline; 
	int c= strlen (p); 
	int s= 0; 
	int j; 
	char * r= (char *)(0); 
	for (; p < cmdline + c; p++){ 
		if (!*p) break; 
		if (*p == 34){ 
			s= (s * -1) + 1; 
		} else if (!s){ 
			if (*p == 32 || *p == 9){ 
				if (p[1] == 45 || p[1] == 47){ 
					strcpy (string1, p + 2); 
					TrimAtSpace (string1); 
					if (!strcmp (string1, SwitchName)){ 
						j= 2; 
						while (p[j] && p[j] != 32 && p[j] != 9) j++; 
						while (p[j] && (p[j] == 32 || p[j] == 9)) j++; 
						r= (char *)(p + j); 
					} else { 
						p++; 
					} 
				} 
			} 
		} 
	} 
	return r; 
} 

char * SwitchValue (LPSTR CmdLine, char * SwitchName){ 
	if (UseDebug){ 
		//printf ("SwitchValue (); CmdLine = %d \r\n", CmdLine); 
		//printf ("CmdLine: %s\r\n", CmdLine); 
	} 
	char * s1= SwitchStart (CmdLine, SwitchName); 
		if (!s1) return 0; 
	char * s2= GlobalAlloc (GMEM_ZEROINIT, strlen (s1) + 16); 
	char * p; 
	strcpy (s2, s1); 
	if (*s2 == 34){ 
		p= s2 + 1; 
		while (*p && *p != 34){ 
			if (*p == 92) p++; 
			p++; 
		} 
		// while (*p && *p != 34) p++; 
		if (*p) p++; 
		*p= 0; 
	} else { 
		TrimAtSpace (s2); 
	} 
	return s2; 
} 
double SwitchNumber (LPSTR CmdLine, char * SwitchName){ 
	char * string1= SwitchValue (CmdLine, SwitchName); 
	double x= 0; 
	if (!string1) return 0; 
	int n= sscanf (string1, " %lf", &x); 
	GlobalFree (string1); 
	return x; 
} 