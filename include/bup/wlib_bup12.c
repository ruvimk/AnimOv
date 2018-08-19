double timeAt; 
double max_smart_area; 

// Last known good backup: 7 

double RayEllipsoidEx (double s0, double s1, double s2, double rX, double rY, double rZ, 
	double a, double b, double c, double re0, double re1, double re2, double rd0, double rd1, double rd2, double * result1, double * result2){ 
	*result1= NO_INTERSECT; 
	*result2= NO_INTERSECT; 
	if (!(a * b * c)) return NO_INTERSECT; 
	if (!rd0 && !rd1 && !rd2) return NO_INTERSECT; 
	double cosRX= cos (rX); 
	double sinRX= sin (rX); 
	double cosRY= cos (rY); 
	double sinRY= sin (rY); 
	double cosRZ= cos (rZ); 
	double sinRZ= sin (rZ); 
	double r00= cosRY * cosRZ; 
	double r01= cosRY * sinRZ; 
	double r02= -sinRY; 
	double r10= sinRX * sinRY * cosRZ - cosRX * sinRZ; 
	double r11= sinRX * sinRY * sinRZ + cosRX * cosRZ; 
	double r12= sinRX * cosRY; 
	double r20= cosRX * sinRY * cosRZ + sinRX * sinRZ; 
	double r21= cosRX * sinRY * sinRZ - sinRX * cosRZ; 
	double r22= cosRX * cosRY; 
	double sre0= re0 - s0; 
	double sre1= re1 - s1; 
	double sre2= re2 - s2; 
	double e0= r00 * sre0 + r01 * sre1 + r02 * sre2; 
	double e1= r10 * sre0 + r11 * sre1 + r12 * sre2; 
	double e2= r20 * sre0 + r21 * sre1 + r22 * sre2; 
	double d0= r00 * rd0 + r01 * rd1 + r02 * rd2; 
	double d1= r10 * rd0 + r11 * rd1 + r12 * rd2; 
	double d2= r20 * rd0 + r21 * rd1 + r22 * rd2; 
	double a4= pow (a, 4); 
	double b4= pow (b, 4); 
	double c4= pow (c, 4); 
	double oa4= 1 / a4; 
	double ob4= 1 / b4; 
	double oc4= 1 / c4; 
	double d02= pow (d0, 2); 
	double d12= pow (d1, 2); 
	double d22= pow (d2, 2); 
	double e02= pow (e0, 2); 
	double e12= pow (e1, 2); 
	double e22= pow (e2, 2); 
	double qA= oa4 * d02 + ob4 * d12 + oc4 * d22; 
	double qB= oa4 * 2 * e0 * d0 + ob4 * 2 * e1 * d1 + oc4 * 2 * e2 * d2; 
	double qC= oa4 * e02 + ob4 * e12 + oc4 * e22 - 1; 
	double srParam= qB * qB - 4 * qA * qC; 
	if (srParam < 0) return NO_INTERSECT; 
	double srValue= sqrt (srParam); 
	double qDivisor= 2 * qA; 
	if (srParam == 0){ 
		*result1= (-qB + srValue) / qDivisor; 
		return *result1; 
	} 
	double t0= (-qB + srValue) / qDivisor; 
	double t1= (-qB - srValue) / qDivisor; 
	*result1= t0; 
	*result2= t1; 
	return t0; 
} 
double RayEllipsoid (double a, double b, double c, double e0, double e1, double e2, double d0, double d1, double d2, double * result1, double * result2){ 
	*result1= NO_INTERSECT; 
	*result2= NO_INTERSECT; 
	if (!(a * b * c)) return NO_INTERSECT; 
	if (!d0 && !d1 && !d2) return NO_INTERSECT; 
	double a2= pow (a, 2); 
	double b2= pow (b, 2); 
	double c2= pow (c, 2); 
	double oa2= 1 / a2; 
	double ob2= 1 / b2; 
	double oc2= 1 / c2; 
	double d02= pow (d0, 2); 
	double d12= pow (d1, 2); 
	double d22= pow (d2, 2); 
	double e02= pow (e0, 2); 
	double e12= pow (e1, 2); 
	double e22= pow (e2, 2); 
	double qA= oa2 * d02 + ob2 * d12 + oc2 * d22; 
	double qB= 2 * (oa2 * e0 * d0 + ob2 * e1 * d1 + oc2 * e2 * d2); 
	double qC= oa2 * e02 + ob2 * e12 + oc2 * e22 - 1; 
	double srParam= qB * qB - 4 * qA * qC; 
	if (srParam < 0) return NO_INTERSECT; 
	double srValue= sqrt (srParam); 
	double qDivisor= 2 * qA; 
	if (srValue == 0){ 
		*result1= (-qB + srValue) / qDivisor; 
		return *result1; 
	} 
	double t0= (-qB + srValue) / qDivisor; 
	double t1= (-qB - srValue) / qDivisor; 
	*result1= t0; 
	*result2= t1; 
	return t0; 
} 

void EnsureDigits (char * s, int d){ 
	int c; 
	while ((c= strlen (s)) < d){ 
		memmove (s + 1, s, c + 1); 
		s[0]= 48; 
	} 
} 

char * scan_key (char * buffer, char * output1, int max_output1_size, char * output2, int max_output2_size){ 
	char a; 
	int i= 0; 
	int j; 
	if (!buffer) return 0; 
	if (!(*buffer)) return 0; 
	char * p01; 
	char * p02; 
	p01= strchr (buffer, ':'); 
	p02= strchr (buffer, ';'); 
	if ((p01 && p02 && p01 > p02) || (!p01)){ 
		strcpy (output1, "0"); 
		while (1){ 
			a= buffer[i]; 
			if (a == 59) break; 
			if (i + 1 < max_output2_size) output2[i]= a; 
			else if (i < max_output2_size) output2[i]= 0; 
			if (!a) break; 
			i++; 
		} 
		if (i < max_output2_size) output2[i]= 0; 
		if (!buffer[i]) return buffer + i; 
		i++; 
		return buffer + i; 
	} 
	*output1= 0; 
	*output2= 0; 
	while (1){ 
		a= buffer[i]; 
		if (a == 58) break; 
		if (a == 59) break; 
		if (i + 1 < max_output1_size) output1[i]= a; 
		else if (i < max_output1_size) output1[i]= 0; 
		if (!a) break; 
		i++; 
	} 
	if (i < max_output1_size) output1[i]= 0; 
	i++; 
	if (a == 59){ 
		return buffer + i; 
	} 
	j= i; 
	while (1){ 
		a= buffer[i]; 
		if (a == 59) break; 
		if (i + 1 < max_output2_size) output2[i-j]= a; 
		else if (i < max_output2_size) output2[i-j]= 0; 
		if (!a) break; 
		i++; 
	} 
	if (i < max_output2_size) output2[i-j]= 0; 
	i++; 
	//printf ("%s => %s\r\n", output1, output2); 
	//while ((buffer[i] == 58) | (buffer[i] == 59)) i++; 
	return buffer + i; 
} 

void file_put_contents (char * filename, char * text, int the_size){ 
	HANDLE hFile; 
	OFSTRUCT ofs; 
	unsigned long bWritten; 
	// hFile= (HANDLE)(OpenFile (filename, &ofs, 0x1001)); 
	hFile= CreateFile (filename, GENERIC_WRITE, 7, 0, 2, 128, 0); 
	SetEndOfFile (hFile); 
	WriteFile (hFile, text, the_size, &bWritten, 0); 
		// printf (":Last Error: %d\r\n\r\n", GetLastError ()); 
	CloseHandle (hFile); 
} 

double LookUpNumber (char * definitions, char * name){ 
	char string1[1024]; 
	double result= 0; 
	LookUpProperty (definitions, name, string1); 
	sscanf (string1, "%lf", &result); 
	return result; 
} 
double LookUpNumber2 (char * definitions, char * name){ 
	char string1[1024]; 
	double result= 1; 
	LookUpProperty (definitions, name, string1); 
	sscanf (string1, "%lf", &result); 
	return result; 
} 
void LookUpProperty (char * definitions, char * name, char * output){ 
	char string1[1024]; 
	char result1[1024]; 
	char * p= definitions; 
	int i; 
	int c; 
	*output= 0; 
	while ((p= scan_line (p, string1, 1024)) && string1[0]){ 
		if (!strcmp (string1, "STOP")) break; 
		result1[0]= 0; 
		for (i= 0; string1[i]; i++) if (string1[i] == 61) break; 
		if (string1[i]) strcpy (result1, string1 + i + 1); 
		string1[i]= 0; 
		if (!strcmp (name, string1)){ 
			strcpy (output, result1); 
			break; 
		} 
	} 
} 
void LookUpPropertyExBin (char ** buff, char * path, double at, char * output){ 
	char ** buf; 
	int i; 
	int j; 
	double kMax= -1; 
	double number; 
	*output= 0; 
	for (i= 0; buff[i]; i += 2){ 
		if (strcmp (buff[i], path)) continue; 
		if (!(buf= (char **)(buff[i+1]))) continue; 
		kMax= -1; 
		for (j= 0; buf[j]; j += 2){ 
			number= *((double *)(buf[j])); 
			// if (buf[j+1]) printf (":%s\r\n", buf[j+1] + sizeof (double)); 
			if (number == at){ 
				if (buf[j+1]){ 
					strcpy (output, buf[j+1] + sizeof (double)); 
					break; 
				} 
			} 
			if (number < at && (kMax == -1 || kMax < number)){ 
				// if (kMax == -1 || (kMax < number && number < at)){ 
				kMax= number; 
				if (buf[j+1]) strcpy (output, buf[j+1] + sizeof (double)); 
				else *output= 0; 
			} 
		} 
		break; 
	} 
	// printf (":<done>;\r\n"); 
} 
void LookUpPropertyEx (char * buffer, char * path, double at, char * output){ 
	char string1[4096]; 
	char string2[4096]; 
	char key[512]; 
	char value[512]; 
	char vMaxLower[512]; 
	char * p; 
	double a; 
	double b; 
	double maxLower= -1; 
	double rAt= floor (at * 100) / 100; 
	LookUpProperty (buffer, path, string1); 
	*output= 0; 
	if (!string1[0]) return; 
	if (at){ 
		vMaxLower[0]= 0; 
		p= string1; 
		while (p= scan_key (p, key, 512, value, 512)){ 
			a= -1; 
			sscanf (key, " %lf", &a); 
			if (a != -1){ 
				b= floor (a * 100) / 100; 
				// printf ("at= %f\r\n\trAt= %f\r\n", at, rAt); 
				// printf ("\ta= %f\r\n\tb= %f\r\n", a, b); 
				if (b == rAt){ 
					// printf ("equal; value= %s\r\n", value); 
					strcpy (output, value); 
					return; 
				} 
				if (a < at && (a > maxLower || maxLower == -1)){ 
					maxLower= a; 
					strcpy (vMaxLower, value); 
					if (!vMaxLower[0]) vMaxLower[0]= 32; 
				} 
			} 
		} 
		strcpy (output, vMaxLower); 
		if (maxLower == -1 || !vMaxLower[0]){ 
			p= strchr (string1, ';'); 
			if (p) *p= 0; 
			strcpy (output, string1); 
		} 
	} else { 
		p= strchr (string1, ';'); 
		if (p) *p= 0; 
		strcpy (output, string1); 
	} 
	if (!strcmp (output, " ")) *output= 0; 
} 
double LookUpNumberExBin (char ** buff, char * path, double at){ 
	char ** buf; 
	int i; 
	int j; 
	double kMax= -1; 
	double kMin= -1; 
	double vMax= 0; 
	double vMin= 0; 
	double number; 
	double value; 
	// double difference; 
	register int i_difference; 
	double rAt= (double)(floor (at * 100)); 
	register char ** p1; 
	// char * pA; 
	// char * pB; 
	for (i= 0; buff[i]; i += 2){ 
		if (strcmp (buff[i], path)) continue; 
		if (!(buf= (char **)(buff[i+1]))) continue; 
		// kMax= -1; 
		// kMin= -1; 
		for (j= 0; buf[j]; j += 2){ 
			if (!((p1= &buf[j])[1])) continue; 
			number= *((double *)(p1[0])); 
			value= *((double *)(p1[1])); 
			// printf ("\t%f:%f\r\n", number, value); 
			// difference= (double)(floor (number * 100)) - rAt; 
			i_difference= (int)(floor (number * 100) - rAt); 
			// printf ("f %f:%f\r\n", number, value); 
			// printf ("Difference: %f\r\n", difference); 
			// if (!((int)(difference))){ 
				// printf ("Value: %f\r\n", value); 
				if (!i_difference) return value; 
			// } 
			if (number > at){ 
				if (kMin > number) goto lbl01_001_01000001a; 
				else if (kMin == -1) goto lbl01_001_01000001a; 
				// if (kMin > number){ 
					// kMin= number; 
					// vMin= value; 
				// } else if (kMin == -1){ 
					// kMin= number; 
					// vMin= value; 
				// } 
			} else if (number < at){ 
				if (kMax < number) goto lbl01_001_01000001b; 
				else if (kMax == -1) goto lbl01_001_01000001b; 
				// if (kMax < number){ 
					// kMax= number; 
					// vMax= value; 
				// } else if (kMax == -1){ 
					// kMax= number; 
					// vMax= value; 
				// } 
			} 
			continue; 
			; lbl01_001_01000001a: ; 
			kMin= number; 
			vMin= value; 
			continue; 
			; lbl01_001_01000001b: ; 
			kMax= number; 
			vMax= value; 
			continue; 
			// if (number > at && (kMin > number || kMin == -1 /*|| kMin > number*/)){ 
				// kMin= number; 
				// vMin= value; 
			// } 
			// if (number < at && (kMax < number || kMax == -1 /*|| kMax < number*/)){ 
				// kMax= number; 
				// vMax= value; 
			// } 
		} 
		break; 
	} 
	// printf ("max %f:%f\r\n", kMax, vMax); 
	// printf ("min %f:%f\r\n", kMin, vMin); 
	if (kMax == -1 && kMin == -1) return 0; 
	if (kMax == -1) return at * vMin / kMin; 
	if (kMin == -1) return vMax; 
	return (at - kMin) * (vMax - vMin) / (kMax - kMin) + vMin; 
} 
double LookUpNumberEx (char * buffer, char * path, double at){ 
	char string1[4096]; 
	char key[512]; 
	char value[512]; 
	char * p; 
	double a; 
	double b; 
	double c; 
	double d; 
	double rAt= floor (at * 100) / 100; 
	double vMaxLower= -1; 
	double vMinHigher= -1; 
	double maxLower= -1; 
	double minHigher= -1; 
	double result= 0; 
	double at0= 0; 
	LookUpProperty (buffer, path, string1); 
	sscanf (string1, " %lf", &at0); 
	if (at){ 
		p= string1; 
		while (p= scan_key (p, key, 512, value, 512)){ 
			// printf ("Scanning key (%s => %s)... \r\n", key, value); 
			// printf ("\tminHigher: %f\r\n\tmaxLower: %f\r\n\tvMinHigher: %f\r\n\tvMaxLower: %f\r\n", 
				// minHigher, maxLower, vMinHigher, vMaxLower); 
			a= -1; 
			sscanf (key, " %lf", &a); 
			if (a != -1){ 
				c= floor (a * 100) / 100; 
				b= -1; 
				sscanf (value, " %lf", &b); 
				if (b != -1){ 
					d= floor (b * 100) / 100; 
					if (c == rAt){ 
						return b; 
					} 
					if (a > at && (a < minHigher || minHigher == -1)){ 
						minHigher= a; 
						vMinHigher= b; 
					} 
					if (a < at && (a > maxLower || maxLower == -1)){ 
						maxLower= a; 
						vMaxLower= b; 
					} 
				} 
			} 
		} 
		if (minHigher != -1 && maxLower != -1){ 
			result= (at - maxLower) * (vMinHigher - vMaxLower) / (minHigher - maxLower) + vMaxLower; 
		} else if (minHigher != -1){ 
			result= (at) * (vMinHigher - at0) / (minHigher) + at0; 
		} else if (maxLower != -1){ 
			result= vMaxLower; 
		} else result= at0; 
	} else { 
		// sscanf (string1, " %lf", &result); 
		result= at0; 
	} 
	return result; 
} 
double LookUpNumberExBin2 (char ** buff, char * path, double at){ 
	char ** buf; 
	int i; 
	int j; 
	double kMax= -1; 
	double kMin= -1; 
	double vMax; 
	double vMin; 
	double number; 
	double value; 
	double difference; 
	double rAt= (double)(floor (at * 100)); 
	for (i= 0; buff[i]; i += 2){ 
		if (strcmp (buff[i], path)) continue; 
		if (!(buf= (char **)(buff[i+1]))) continue; 
		kMax= -1; 
		kMin= -1; 
		for (j= 0; buf[j]; j += 2){ 
			if (!buf[j+1]) continue; 
			number= *((double *)(buf[j])); 
			value= *((double *)(buf[j+1])); 
			difference= (double)(floor (number * 100)) - rAt; 
			if (!((int)(difference))) return value; 
			// printf ("f %f:%f\r\n", number, value); 
			if (number > at && (kMin == -1 || kMin > number)){ 
				kMin= number; 
				vMin= value; 
			} 
			if (number < at && (kMax == -1 || kMax < number)){ 
				kMax= number; 
				vMax= value; 
			} 
		} 
		break; 
	} 
	// printf ("max %f:%f\r\n", kMax, vMax); 
	// printf ("min %f:%f\r\n", kMin, vMin); 
	if (kMax == -1 && kMin == -1) return 1; 
	if (kMax == -1) return at * (vMin - 1) / kMin; 
	if (kMin == -1) return vMax; 
	return (at - kMin) * (vMax - vMin) / (kMax - kMin) + vMin; 
} 
double LookUpNumberEx2 (char * buffer, char * path, double at){ 
	char string1[4096]; 
	char key[512]; 
	char value[512]; 
	char * p; 
	double a; 
	double b; 
	double c; 
	double d; 
	double rAt= floor (at * 100) / 100; 
	double vMaxLower= -1; 
	double vMinHigher= -1; 
	double maxLower= -1; 
	double minHigher= -1; 
	double result= 1; 
	double at0= 1; 
	LookUpProperty (buffer, path, string1); 
	sscanf (string1, " %lf", &at0); 
	if (at){ 
		p= string1; 
		while (p= scan_key (p, key, 512, value, 512)){ 
			// printf ("Scanning key (%s => %s)... \r\n", key, value); 
			// printf ("\tminHigher: %f\r\n\tmaxLower: %f\r\n\tvMinHigher: %f\r\n\tvMaxLower: %f\r\n", 
				// minHigher, maxLower, vMinHigher, vMaxLower); 
			a= -1; 
			sscanf (key, " %lf", &a); 
			if (a != -1){ 
				c= floor (a * 100) / 100; 
				b= -1; 
				sscanf (value, " %lf", &b); 
				if (b != -1){ 
					d= floor (b * 100) / 100; 
					if (c == rAt){ 
						return b; 
					} 
					if (a > at && (a < minHigher || minHigher == -1)){ 
						minHigher= a; 
						vMinHigher= b; 
					} 
					if (a < at && (a > maxLower || maxLower == -1)){ 
						maxLower= a; 
						vMaxLower= b; 
					} 
				} 
			} 
		} 
		if (minHigher != -1 && maxLower != -1){ 
			result= (at - maxLower) * (vMinHigher - vMaxLower) / (minHigher - maxLower) + vMaxLower; 
		} else if (minHigher != -1){ 
			result= (at) * (vMinHigher - at0) / (minHigher) + at0; 
		} else if (maxLower != -1){ 
			result= vMaxLower; 
		} else result= at0; 
	} else { 
		// sscanf (string1, " %lf", &result); 
		result= at0; 
	} 
	return result; 
} 
int IsPropertyKey (char ** buff, char * path, double at){ 
	char ** buf; 
	int i; 
	int j; 
	double kMax= -1; 
	double kMin= -1; 
	double vMax= 0; 
	double vMin= 0; 
	double number; 
	double value; 
	// double difference; 
	register int i_difference; 
	double rAt= (double)(floor (at * 100)); 
	register char ** p1; 
	// char * pA; 
	// char * pB; 
	for (i= 0; buff[i]; i += 2){ 
		if (strcmp (buff[i], path)) continue; 
		if (!(buf= (char **)(buff[i+1]))) continue; 
		// kMax= -1; 
		// kMin= -1; 
		for (j= 0; buf[j]; j += 2){ 
			if (!((p1= &buf[j])[1])) continue; 
			number= *((double *)(p1[0])); 
			// value= *((double *)(p1[1])); 
			// printf ("\t%f:%f\r\n", number, value); 
			// difference= (double)(floor (number * 100)) - rAt; 
			i_difference= (int)(floor (number * 100) - rAt); 
			// printf ("f %f:%f\r\n", number, value); 
			// printf ("Difference: %f\r\n", difference); 
			// if (!((int)(difference))){ 
				// printf ("Value: %f\r\n", value); 
				if (!i_difference) return 1; 
			// } 
			if (number > at){ 
				if (kMin > number) goto lbl01_001_01000001a; 
				else if (kMin == -1) goto lbl01_001_01000001a; 
				// if (kMin > number){ 
					// kMin= number; 
					// vMin= value; 
				// } else if (kMin == -1){ 
					// kMin= number; 
					// vMin= value; 
				// } 
			} else if (number < at){ 
				if (kMax < number) goto lbl01_001_01000001b; 
				else if (kMax == -1) goto lbl01_001_01000001b; 
				// if (kMax < number){ 
					// kMax= number; 
					// vMax= value; 
				// } else if (kMax == -1){ 
					// kMax= number; 
					// vMax= value; 
				// } 
			} 
			continue; 
			; lbl01_001_01000001a: ; 
			// kMin= number; 
			// vMin= value; 
			continue; 
			; lbl01_001_01000001b: ; 
			// kMax= number; 
			// vMax= value; 
			continue; 
			// if (number > at && (kMin > number || kMin == -1 /*|| kMin > number*/)){ 
				// kMin= number; 
				// vMin= value; 
			// } 
			// if (number < at && (kMax < number || kMax == -1 /*|| kMax < number*/)){ 
				// kMax= number; 
				// vMax= value; 
			// } 
		} 
		break; 
	} 
	// // printf ("max %f:%f\r\n", kMax, vMax); 
	// // printf ("min %f:%f\r\n", kMin, vMin); 
	// if (kMax == -1 && kMin == -1) return 0; 
	// if (kMax == -1) return at * vMin / kMin; 
	// if (kMin == -1) return vMax; 
	// return (at - kMin) * (vMax - vMin) / (kMax - kMin) + vMin; 
	return 0; 
} 
double FindNextKey (char ** buff, char * path, double at, int direction){ 
	char ** buf; 
	int i; 
	int j; 
	double kMax= -1; 
	double kMin= -1; 
	double vMax= 0; 
	double vMin= 0; 
	double number; 
	double value; 
	// double difference; 
	register int i_difference; 
	double rAt= (double)(floor (at * 100)); 
	register char ** p1; 
	// char * pA; 
	// char * pB; 
	for (i= 0; buff[i]; i += 2){ 
		if (strcmp (buff[i], path)) continue; 
		if (!(buf= (char **)(buff[i+1]))) continue; 
		// kMax= -1; 
		// kMin= -1; 
		for (j= 0; buf[j]; j += 2){ 
			if (!((p1= &buf[j])[1])) continue; 
			number= *((double *)(p1[0])); 
			// value= *((double *)(p1[1])); 
			// printf ("\t%f:%f\r\n", number, value); 
			// difference= (double)(floor (number * 100)) - rAt; 
			i_difference= (int)(floor (number * 100) - rAt); 
			// printf ("f %f:%f\r\n", number, value); 
			// printf ("Difference: %f\r\n", difference); 
			// if (!((int)(difference))){ 
				// printf ("Value: %f\r\n", value); 
				if (!i_difference) continue; //return value; 
			// } 
			if (number > at){ 
				if (kMin > number) goto lbl01_001_01000001a; 
				else if (kMin == -1) goto lbl01_001_01000001a; 
				// if (kMin > number){ 
					// kMin= number; 
					// vMin= value; 
				// } else if (kMin == -1){ 
					// kMin= number; 
					// vMin= value; 
				// } 
			} else if (number < at){ 
				if (kMax < number) goto lbl01_001_01000001b; 
				else if (kMax == -1) goto lbl01_001_01000001b; 
				// if (kMax < number){ 
					// kMax= number; 
					// vMax= value; 
				// } else if (kMax == -1){ 
					// kMax= number; 
					// vMax= value; 
				// } 
			} 
			continue; 
			; lbl01_001_01000001a: ; 
			kMin= number; 
			// vMin= value; 
			continue; 
			; lbl01_001_01000001b: ; 
			kMax= number; 
			// vMax= value; 
			continue; 
			// if (number > at && (kMin > number || kMin == -1 /*|| kMin > number*/)){ 
				// kMin= number; 
				// vMin= value; 
			// } 
			// if (number < at && (kMax < number || kMax == -1 /*|| kMax < number*/)){ 
				// kMax= number; 
				// vMax= value; 
			// } 
		} 
		break; 
	} 
	if (direction > 0 && kMin != -1) return kMin; 
	if (direction < 0 && kMax != -1) return kMax; 
	return at; 
} 

int FindParentObject (char * name, char * output){ 
	int i; 
	strcpy (output, name); 
	for (i= strlen (output) - 1; i + 1; i--) if (output[i] == 46) break; 
	if (i + 1) output[i]= 0; 
	else return 0; 
	return 1; 
} 

void Rel2AbsOpc (double * opc, char * fg, char * fig){ 
	char start_vct[1024]; 
	char vct[1024]; 
	char obj[1024]; 
	double x; 
	strcpy (start_vct, fg); 
	*opc= 1; 
	do { 
		strcpy (vct, start_vct); 
		sprintf (obj, "%s.%s", vct, "opc"); 
		x= LookUpNumberEx2 (fig, obj, timeAt); 
		*opc *= x; 
	} while (FindParentObject (vct, start_vct)); 
} 
void Rel2Abs (double * pt, char * fg, char * fig){ 
	char start_vct[1024]; 
	char vct[1024]; 
	char obj[1024]; 
	double x, y, z, rX, rY, rZ; 
	strcpy (start_vct, fg); 
	do { 
		strcpy (vct, start_vct); 
		x= y= z= rX= rY= rZ= 0; 
		sprintf (obj, "%s.%s", vct, "x"); 
		x= LookUpNumberEx (fig, obj, timeAt); 
		sprintf (obj, "%s.%s", vct, "y"); 
		y= LookUpNumberEx (fig, obj, timeAt); 
		sprintf (obj, "%s.%s", vct, "z"); 
		z= LookUpNumberEx (fig, obj, timeAt); 
		sprintf (obj, "%s.%s", vct, "aboutX"); 
		rX= LookUpNumberEx (fig, obj, timeAt); 
		sprintf (obj, "%s.%s", vct, "aboutY"); 
		rY= LookUpNumberEx (fig, obj, timeAt); 
		sprintf (obj, "%s.%s", vct, "aboutZ"); 
		rZ= LookUpNumberEx (fig, obj, timeAt); 
		RotateAboutOrigin (pt, &rX); 
		pt[0] += x; 
		pt[1] += y; 
		pt[2] += z; 
	} while (FindParentObject (vct, start_vct)); 
} 
// void Rel2AbsBin (double * pt, char * fg, char ** fig){ 
	// char start_vct[1024]; 
	// char vct[1024]; 
	// char obj[1024]; 
	// double x, y, z, rX, rY, rZ; 
	// strcpy (start_vct, fg); 
	// do { 
		// strcpy (vct, start_vct); 
		// x= y= z= rX= rY= rZ= 0; 
		// sprintf (obj, "%s.%s", vct, "x"); 
		// x= LookUpNumberExBin (fig, obj, timeAt); 
		// sprintf (obj, "%s.%s", vct, "y"); 
		// y= LookUpNumberExBin (fig, obj, timeAt); 
		// sprintf (obj, "%s.%s", vct, "z"); 
		// z= LookUpNumberExBin (fig, obj, timeAt); 
		// sprintf (obj, "%s.%s", vct, "aboutX"); 
		// rX= LookUpNumberExBin (fig, obj, timeAt); 
		// sprintf (obj, "%s.%s", vct, "aboutY"); 
		// rY= LookUpNumberExBin (fig, obj, timeAt); 
		// sprintf (obj, "%s.%s", vct, "aboutZ"); 
		// rZ= LookUpNumberExBin (fig, obj, timeAt); 
		// RotateAboutOrigin (pt, &rX); 
		// pt[0] += x; 
		// pt[1] += y; 
		// pt[2] += z; 
	// } while (FindParentObject (vct, start_vct)); 
// } 
void Rel2AbsOpcBin (double * opc, char * fg, char ** fig){ 
	char start_vct[1024]; 
	char vct[1024]; 
	char obj[1024]; 
	double x; 
	strcpy (start_vct, fg); 
	*opc= 1; 
	do { 
		strcpy (vct, start_vct); 
		sprintf (obj, "%s.%s", vct, "opc"); 
		x= LookUpNumberExBin2 (fig, obj, timeAt); 
		*opc *= x; 
	} while (FindParentObject (vct, start_vct)); 
	// if (debug_mode) printf ("Rel2AbsBinOpc place marker %d\r\n", 5); 
} 
void Rel2AbsBin (double * pt, char * fg, char ** fig){ 
	char start_vct[1024]; 
	char vct[1024]; 
	char obj[1024]; 
	double x, y, z, rX, rY, rZ; 
	strcpy (start_vct, fg); 
	do { 
		// if (debug_mode) printf ("Rel2AbsBin place marker %d\r\n", 0); 
		strcpy (vct, start_vct); 
		x= y= z= rX= rY= rZ= 0; 
		sprintf (obj, "%s.%s", vct, "x"); 
		// if (debug_mode) printf ("Rel2AbsBin place marker %d\r\n", 1); 
		// if (debug_mode) printf ("\tRel2AbsBin obj<%s>, t=%f\r\n", obj, timeAt); 
		x= LookUpNumberExBin (fig, obj, timeAt); 
		// if (debug_mode) printf ("Rel2AbsBin place marker %d\r\n", 2); 
		sprintf (obj, "%s.%s", vct, "y"); 
		// if (debug_mode) printf ("Rel2AbsBin place marker %d\r\n", 4); 
		// if (debug_mode) printf ("\tRel2AbsBin obj<%s>, t=%f\r\n", obj, timeAt); 
		y= LookUpNumberExBin (fig, obj, timeAt); 
		// if (debug_mode) printf ("Rel2AbsBin place marker %d\r\n", 5); 
		sprintf (obj, "%s.%s", vct, "z"); 
		// if (debug_mode) printf ("Rel2AbsBin place marker %d\r\n", 6); 
		z= LookUpNumberExBin (fig, obj, timeAt); 
		// if (debug_mode) printf ("Rel2AbsBin place marker %d\r\n", 7); 
		sprintf (obj, "%s.%s", vct, "aboutX"); 
		// if (debug_mode) printf ("Rel2AbsBin place marker %d\r\n", 8); 
		rX= LookUpNumberExBin (fig, obj, timeAt); 
		// if (debug_mode) printf ("Rel2AbsBin place marker %d\r\n", 9); 
		sprintf (obj, "%s.%s", vct, "aboutY"); 
		// if (debug_mode) printf ("Rel2AbsBin place marker %d\r\n", 10); 
		rY= LookUpNumberExBin (fig, obj, timeAt); 
		// if (debug_mode) printf ("Rel2AbsBin place marker %d\r\n", 11); 
		sprintf (obj, "%s.%s", vct, "aboutZ"); 
		// if (debug_mode) printf ("Rel2AbsBin place marker %d\r\n", 12); 
		rZ= LookUpNumberExBin (fig, obj, timeAt); 
		// if (debug_mode) printf ("Rel2AbsBin place marker %d\r\n", 13); 
		RotateAboutOrigin (pt, &rX); 
		// if (debug_mode) printf ("Rel2AbsBin place marker %d\r\n", 14); 
		pt[0] += x; 
		pt[1] += y; 
		pt[2] += z; 
	} while (FindParentObject (vct, start_vct)); 
	// if (debug_mode) printf ("Rel2AbsBin place marker %d\r\n", 15); 
} 
void RotateAboutOrigin (double * pt, double * angles){ 
	double cosRX= cos (angles[0]); 
	double sinRX= sin (angles[0]); 
	double cosRY= cos (angles[1]); 
	double sinRY= sin (angles[1]); 
	double cosRZ= cos (angles[2]); 
	double sinRZ= sin (angles[2]); 
	double prevX= pt[0]; 
	double prevY= pt[1]; 
	double prevZ= pt[2]; 
	double primeX= prevX; 
	double primeY= prevY * cosRX + prevZ * sinRX; 
	double primeZ= -prevY * sinRX + prevZ * cosRX; 
	prevX= primeX * cosRY - primeZ * sinRY; 
	prevY= primeY; 
	prevZ= primeX * sinRY + primeZ * cosRY; 
	primeX= prevX * cosRZ + prevY * sinRZ; 
	primeY= -prevX * sinRZ + prevY * cosRZ; 
	primeZ= prevZ; 
	pt[0]= primeX; 
	pt[1]= primeY; 
	pt[2]= primeZ; 
} 
void CameraPositionTransformPt (POINT3D * pt, double * cam){ 
	POINT3D r; 
	pt->x -= cam[0]; 
	pt->y -= cam[1]; 
	pt->z -= cam[2]; 
	r.x= -cam[3]; 
	r.y= -cam[4]; 
	r.z= -cam[5]; 
	RotateAboutOrigin ((double *)pt, (double *)(&r)); 
} 
void OpticTangentTransform (POINT3D * pts, int len, int w, int h, double rh, double rv, double zoom){ 
	if (UseVerbose && UseDebug) printf ("\t\tOTT (pts, len=%d, %d,%d, %f,%f, %f)\r\n", len, 
		w, h, rh, rv, zoom); 
	int i; 
	POINT3D * pt; 
	POINT3D old; 
	double zm= pow (0.9, zoom); 
	double angle1; 
	double angle2; 
	for (i= 0; i < len; i++){ 
		pt= &pts[i]; 
		old.x= pt->x; 
		old.y= pt->y; 
		old.z= pt->z; 
		if (!old.z){ 
			if (!old.x || !old.y){ 
				pt->z= PX_UNDEFINED; 
			} 
		} 
		pt->z= sqrt (old.x * old.x + old.y * old.y + old.z * old.z); 
		if (old.z < 0) pt->z *= -1; 
		angle1= atan2 (-old.z, old.x); 
		pt->x= -w * (2 * angle1 - 2 * rh * zm - PI) / (4 * rh * zm); 
		angle2= atan2 (old.y, -old.z); 
		pt->y= -h * (angle2 - rv * zm) / (2 * rv * zm); 
	} 
} 
void SwapYZ (POINT3D * pts, int len){ 
	int i; 
	double a; 
	POINT3D * pt; 
	for (i= 0; i < len; i++){ 
		pt= &pts[i]; 
		a= pt->y; 
		pt->y= pt->z; 
		pt->z= a; 
	} 
} 

int RREF (double * matrix, int h, int w){ 
	double c; 
	int i; 
	int j; 
	int k; 
	int n= h; 
	if (w < h) return 0; 
	// matrix access: 
	// matrix[row * w + column] 
	for (i= 0; i < n; i++){ 
		c= matrix[i * w + i]; 
		if (!c) return 0; 
		for (j= 0; j < w; j++){ 
			matrix[i * w + j] /= c; 
		} 
		for (j= 0; j < h; j++){ 
			if (i == j) continue; 
			c= matrix[j * w + i]; 
			for (k= 0; k < w; k++){ 
				matrix[j * w + k] -= c * matrix[i * w + k]; 
			} 
		} 
	} 
	return 1; 
} 
int MatMult (double * matA, int hA, int wA, double * matB, int hB, int wB, double * result){ 
	int i; 
	int j; 
	int k; 
	double * p; 
	double sum; 
	if (wA != hB) return 0; 
	for (i= 0; i < hA; i++){ 
		for (j= 0; j < wB; j++){ 
			p= &(result[i * wB + j]); 
			sum= 0; 
			for (k= 0; k < wA; k++){ 
				sum += matA[i * wA + k] * matB[k * wB + j]; 
			} 
			*p= sum; 
		} 
	} 
	return 1; 
} 
int AugmentMatrices (double * matA, int hA, int wA, double * matB, int hB, int wB, double * result){ 
	int i; 
	int j; 
	int W= wA + wB; 
	if (hA != hB) return 0; 
	for (i= 0; i < W; i++){ 
		for (j= 0; j < hA; j++){ 
			if (i < wA){ 
				result[j * W + i]= matA[j * wA + i]; 
			} else { 
				result[j * W + i]= matB[j * wB + i - wA]; 
			} 
		} 
	} 
	return 1; 
} 
void OutputMatrix (double * matrix, int h, int w){ 
	int i; 
	int j; 
	printf ("[ \r\n"); 
	for (i= 0; i < h; i++){ 
		printf ("\t["); 
		for (j= 0; j < w; j++){ 
			if (j) printf ("\t"); 
			printf ("%f", matrix[i * w + j]); 
		} 
		printf ("] \r\n"); 
	} 
	printf ("] \r\n"); 
} 
void sOutputMatrix (char * s, double * matrix, int h, int w){ 
	char string1[4096]; 
	int i; 
	int j; 
	sprintf (string1, "[ \r\n"); 
	strcpy (s, string1); 
	for (i= 0; i < h; i++){ 
		sprintf (string1, "\t["); 
		strcat (s, string1); 
		for (j= 0; j < w; j++){ 
			if (j) strcat (s, "\t"); 
			sprintf (string1, "%f", matrix[i * w + j]); 
			strcat (s, string1); 
		} 
		sprintf (string1, "] \r\n"); 
		strcat (s, string1); 
	} 
	strcat (s, "] \r\n"); 
} 

void ScalePoint (POINT3D * pt, double scalar){ 
	pt->x *= scalar; 
	pt->y *= scalar; 
	pt->z *= scalar; 
} 
void AddPoint (POINT3D * pt1, POINT3D * pt2){ 
	pt1->x += pt2->x; 
	pt1->y += pt2->y; 
	pt1->z += pt2->z; 
} 
void SubPoint (POINT3D * pt1, POINT3D * pt2){ 
	pt1->x -= pt2->x; 
	pt1->y -= pt2->y; 
	pt1->z -= pt2->z; 
} 

long CountCurvePoints (POINT_LINK * first){ 
	if (!first) return 0; 
	long c= 1; 
	POINT_LINK * p= first; 
	while (p= (POINT_LINK *)(p->pNext)) c++; 
	return c; 
} 

void RenderEllipsoid (){ 
	unsigned long time1= GetTickCount (); 
	unsigned long time2; 
	unsigned long time3; 
	POINT3D primeP; 
	POINT3D primeI; 
	POINT3D primeJ; 
	POINT3D primeK; 
	POINT3D corners[8]; 
	double mat3x2[3 * 2]; 
	double mat3x3[3 * 3]; 
	double mat3x6[3 * 6]; 
	double matIdentity3[3 * 3]= { 
		1, 0, 0, 
		0, 1, 0, 
		0, 0, 1 
	}; 
	double matRinv[3 * 3]; 
	PT_ANGLE buf; 
	PT_ANGLE * ith; 
	PT_ANGLE arr[8]; 
	int arr_cnt= 8; 
	int i; 
	int j; 
	double ctrX= 0; 
	double ctrY= 0; 
	double ctrZ= 0; 
	double ctrCnt= 0; 
	double nX; 
	double nY; 
	double nZ; 
	POINT2D a; 
	POINT2D b; 
	POINT2D p; 
	PT_ANGLE * ptLast; 
	PT_ANGLE * ptNext; 
	double c; 
	int d[32]; 
	int d_cnt= 0; 
	int minD; 
	int maxD; 
	int minC; 
	int maxC; 
	double t; 
	int xt; 
	int x; 
	int y; 
	DEPTH_PIXEL * px; 
	double t1; 
	double t2; 
	double matE[3]; 
	double matD[3]; 
	double matE1[3]; 
	double matD1[3]; 
	POINT3D angles01; 
	double opc; 
	double zm= pow (0.9, cam_zoom); 
	char * cp00101; 
	int override_full_acc= 0; 
	int center; 
	int run_loop_1; 
	#define ptPrev ptLast 
	primeP.x= primeP.y= primeP.z= 0; 
	primeI.x= primeI.y= primeI.z= 0; 
	primeJ.x= primeJ.y= primeJ.z= 0; 
	primeK.x= primeK.y= primeK.z= 0; 
	primeI.x= primeJ.y= primeK.z= 1; 
	RotateAboutOrigin ((double *)(&primeI), &aboutX); 
	RotateAboutOrigin ((double *)(&primeJ), &aboutX); 
	RotateAboutOrigin ((double *)(&primeK), &aboutX); 
	AddPoint (&primeI, (POINT3D *)(&atX)); 
	AddPoint (&primeJ, (POINT3D *)(&atX)); 
	AddPoint (&primeK, (POINT3D *)(&atX)); 
	AddPoint (&primeP, (POINT3D *)(&atX)); 
	// if (UseVerbose && UseDebug) OutputMatrix ((double *)(&primeP), 3, 1); 
	Rel2AbsBin ((double *)(&primeP), objVector, (char **)(InputBuffer)); 
	Rel2AbsBin ((double *)(&primeI), objVector, (char **)(InputBuffer)); 
	Rel2AbsBin ((double *)(&primeJ), objVector, (char **)(InputBuffer)); 
	Rel2AbsBin ((double *)(&primeK), objVector, (char **)(InputBuffer)); 
	Rel2AbsOpcBin (&opc, objVector, (char **)(InputBuffer)); 
	if (!opc){ 
		if (UseDebug) printf ("\tEllipsoid not opaque; returning... \r\n"); 
		return; 
	} 
	// if (UseVerbose && UseDebug) OutputMatrix ((double *)(&primeP), 3, 1); 
	CameraPositionTransformPt (&primeP, &cam_x); 
	CameraPositionTransformPt (&primeI, &cam_x); 
	CameraPositionTransformPt (&primeJ, &cam_x); 
	CameraPositionTransformPt (&primeK, &cam_x); 
	// if (UseVerbose && UseDebug) OutputMatrix ((double *)(&primeP), 3, 1); 
	primeI.x -= primeP.x; 
	primeJ.x -= primeP.x; 
	primeK.x -= primeP.x; 
	primeI.y -= primeP.y; 
	primeJ.y -= primeP.y; 
	primeK.y -= primeP.y; 
	primeI.z -= primeP.z; 
	primeJ.z -= primeP.z; 
	primeK.z -= primeP.z; 
	// if (UseVerbose && UseDebug) OutputMatrix ((double *)(&primeJ), 3, 1); 
	AugmentMatrices ((double *)(&primeI), 3, 1, (double *)(&primeJ), 3, 1, (double *)(mat3x2)); 
	AugmentMatrices ((double *)(mat3x2), 3, 2, (double *)(&primeK), 3, 1, (double *)(mat3x3)); 
	AugmentMatrices ((double *)(mat3x3), 3, 3, (double *)(matIdentity3), 3, 3, (double *)(mat3x6)); 
	cp00101= GlobalAlloc (GMEM_ZEROINIT, 512); 
	// sOutputMatrix (cp00101, mat3x6, 3, 6); 
	// file_put_contents ("\\rrefstat.txt", cp00101, strlen (cp00101)); 
	// file_put_contents ("C:\\WBord\\wwwroot\\AnimOv\\rrefstat.txt", cp00101, strlen (cp00101)); 
	GlobalFree (cp00101); 
	RREF ((double *)(mat3x6), 3, 6); 
	if (UseDebug) printf ("step %d\r\n", 1); 
	cp00101= GlobalAlloc (GMEM_ZEROINIT, 512); 
	// sOutputMatrix (cp00101, mat3x6, 3, 6); 
	// file_put_contents ("\\rrefstat.txt", cp00101, strlen (cp00101)); 
	// file_put_contents ("C:\\WBord\\wwwroot\\AnimOv\\rrefstat2.txt", cp00101, strlen (cp00101)); 
	GlobalFree (cp00101); 
	if (UseDebug) printf ("step %d\r\n", 2); 
	matRinv[0]= mat3x6[3]; 
		matRinv[1]= mat3x6[4]; 
		matRinv[2]= mat3x6[5]; 
	matRinv[3]= mat3x6[9]; 
		matRinv[4]= mat3x6[10]; 
		matRinv[5]= mat3x6[11]; 
	matRinv[6]= mat3x6[15]; 
		matRinv[7]= mat3x6[16]; 
		matRinv[8]= mat3x6[17]; 
	// if (UseVerbose && UseDebug) OutputMatrix ((double *)(&primeJ), 3, 1); 
	ScalePoint (&primeI, axisX); 
	ScalePoint (&primeJ, axisY); 
	ScalePoint (&primeK, axisZ); 
	// if (UseVerbose && UseDebug) OutputMatrix ((double *)(&primeJ), 3, 1); 
	// ScalePoint (&corners[0], 0); 
	// ScalePoint (&corners[1], 0); 
	// ScalePoint (&corners[2], 0); 
	// ScalePoint (&corners[3], 0); 
	// ScalePoint (&corners[4], 0); 
	// ScalePoint (&corners[5], 0); 
	// ScalePoint (&corners[6], 0); 
	// ScalePoint (&corners[7], 0); 
	if (axisX * axisY * axisZ / pow (sqrt (pow (primeP.x, 2) + pow (primeP.y, 2) + pow (primeP.z, 2)), 3) > max_smart_area) override_full_acc= 1; 
	if (!override_full_acc){ 
		for (i= 0; i < 8; i++){ 
			// printf ("before %d,%d; ", (int)corners[i].x, (int)corners[i].y); 
			// ctrX += corners[i].x; 
			// ctrY += corners[i].y; 
			// ctrZ += corners[i].z; 
			// ctrCnt++; 
			corners[i].x= corners[i].y= corners[i].z= 0; 
		} 
	} 
	// printf ("\r\n"); 
	if (!override_full_acc){ 
		AddPoint (&corners[0], &primeI); 
			AddPoint (&corners[0], &primeJ); 
			AddPoint (&corners[0], &primeK); 
		AddPoint (&corners[1], &primeI); 
			AddPoint (&corners[1], &primeJ); 
			SubPoint (&corners[1], &primeK); 
		AddPoint (&corners[2], &primeI); 
			SubPoint (&corners[2], &primeJ); 
			AddPoint (&corners[2], &primeK); 
		AddPoint (&corners[3], &primeI); 
			SubPoint (&corners[3], &primeJ); 
			SubPoint (&corners[3], &primeK); 
		SubPoint (&corners[4], &primeI); 
			AddPoint (&corners[4], &primeJ); 
			AddPoint (&corners[4], &primeK); 
		SubPoint (&corners[5], &primeI); 
			AddPoint (&corners[5], &primeJ); 
			SubPoint (&corners[5], &primeK); 
		SubPoint (&corners[6], &primeI); 
			SubPoint (&corners[6], &primeJ); 
			AddPoint (&corners[6], &primeK); 
		SubPoint (&corners[7], &primeI); 
			SubPoint (&corners[7], &primeJ); 
			SubPoint (&corners[7], &primeK); 
		AddPoint (&corners[0], &primeP); 
		AddPoint (&corners[1], &primeP); 
		AddPoint (&corners[2], &primeP); 
		AddPoint (&corners[3], &primeP); 
		AddPoint (&corners[4], &primeP); 
		AddPoint (&corners[5], &primeP); 
		AddPoint (&corners[6], &primeP); 
		AddPoint (&corners[7], &primeP); 
	} 
	// if (UseVerbose && UseDebug) printf ("\r\nCorners: \r\n"); 
	// if (UseVerbose && UseDebug) OutputMatrix ((double *)(&corners[0]), 3, 1); 
	// if (UseVerbose && UseDebug) OutputMatrix ((double *)(&corners[1]), 3, 1); 
	// if (UseVerbose && UseDebug) OutputMatrix ((double *)(&corners[2]), 3, 1); 
	// if (UseVerbose && UseDebug) OutputMatrix ((double *)(&corners[3]), 3, 1); 
	// if (UseVerbose && UseDebug) OutputMatrix ((double *)(&corners[4]), 3, 1); 
	// if (UseVerbose && UseDebug) OutputMatrix ((double *)(&corners[5]), 3, 1); 
	// if (UseVerbose && UseDebug) OutputMatrix ((double *)(&corners[6]), 3, 1); 
	// if (UseVerbose && UseDebug) OutputMatrix ((double *)(&corners[7]), 3, 1); 
	// if (UseVerbose) printf ("\tOpticTangentTransform (); \r\n"); 
	if (!override_full_acc) SwapYZ (corners, 8); 
	// for (i= 0; i < 8; i++){ 
		// printf ("corner %d,%d; ", (int)corners[i].x, (int)corners[i].y); 
		// // ctrX += corners[i].x; 
		// // ctrY += corners[i].y; 
		// // ctrZ += corners[i].z; 
		// // ctrCnt++; 
	// } 
	// printf ("\r\n"); 
	if (!override_full_acc) OpticTangentTransform (corners, 8, main_w, main_h, cam_rh, cam_rv, cam_zoom); 
	// if (UseVerbose) printf ("\tReturned. \r\n"); 
	// if (UseVerbose && UseDebug) printf ("\r\nCorners: \r\n"); 
	// if (UseVerbose && UseDebug) OutputMatrix ((double *)(&corners[0]), 3, 1); 
	// if (UseVerbose && UseDebug) OutputMatrix ((double *)(&corners[1]), 3, 1); 
	// if (UseVerbose && UseDebug) OutputMatrix ((double *)(&corners[2]), 3, 1); 
	// if (UseVerbose && UseDebug) OutputMatrix ((double *)(&corners[3]), 3, 1); 
	// if (UseVerbose && UseDebug) OutputMatrix ((double *)(&corners[4]), 3, 1); 
	// if (UseVerbose && UseDebug) OutputMatrix ((double *)(&corners[5]), 3, 1); 
	// if (UseVerbose && UseDebug) OutputMatrix ((double *)(&corners[6]), 3, 1); 
	// if (UseVerbose && UseDebug) OutputMatrix ((double *)(&corners[7]), 3, 1); 
	if (!override_full_acc){ 
		for (i= 0; i < 8; i++){ 
			// printf ("corner %d,%d; ", (int)corners[i].x, (int)corners[i].y); 
			ctrX += corners[i].x; 
			ctrY += corners[i].y; 
			ctrZ += corners[i].z; 
			ctrCnt++; 
		} 
		// printf ("\r\n"); 
		if (ctrCnt){ 
			ctrX /= ctrCnt; 
			ctrY /= ctrCnt; 
			ctrZ /= ctrCnt; 
		} 
		for (i= 0; i < 8; i++){ 
			ith= &arr[i]; 
			// printf ("corner %d,%d; ", (int)corners[i].x, (int)corners[i].y); 
			ith->x= corners[i].x; 
			ith->y= corners[i].y; 
			ith->z= corners[i].z; 
			ith->angle= atan2 (ctrY - (ith->y), (ith->x) - ctrX); 
		} 
	} 
	// printf ("\r\n"); 
	// arr_cnt= 8; 
	// for (i= 0; i < arr_cnt; i++){ 
		// for (j= 0; j + 1 < arr_cnt; j++){ 
			// if (sqrt (pow (arr[j+0].x - ctrX, 2) + pow (arr[j+0].y - ctrY, 2)) < sqrt (pow (arr[j+1].x - ctrX, 2) + pow (arr[j+1].y - ctrY, 2))){ 
				// memmove (&buf, &arr[j+0], sizeof (PT_ANGLE)); 
				// memmove (&arr[j+0], &arr[j+1], sizeof (PT_ANGLE)); 
				// memmove (&arr[j+1], &buf, sizeof (PT_ANGLE)); 
			// } 
		// } 
	// } 
	// memmove (&arr[0], &arr[1], (arr_cnt - 1) * sizeof (PT_ANGLE)); 
	// memmove (&arr[0], &arr[1], (arr_cnt - 1) * sizeof (PT_ANGLE)); 
	if (!override_full_acc){ 
		arr_cnt= 8; 
		for (i= 0; i < arr_cnt; i++){ 
			for (j= 0; j + 1 < arr_cnt; j++){ 
				if ((arr[j+0].angle) > (arr[j+1].angle)){ 
					memmove (&buf, &arr[j+0], sizeof (PT_ANGLE)); 
					memmove (&arr[j+0], &arr[j+1], sizeof (PT_ANGLE)); 
					memmove (&arr[j+1], &buf, sizeof (PT_ANGLE)); 
				} 
			} 
		} 
	} 
	if (UseDebug) printf ("step %d\r\n", 3); 
	// printf ("arr_cnt: %d; ", arr_cnt); 
	// for (i= 0; i < arr_cnt; i++){ 
		// if ((int)(arr[i].x) == NaN || (int)(arr[i].y) == NaN){ 
			// if (i + 1 < arr_cnt) memmove (&arr[i+0], &arr[i+1], (arr_cnt - i - 1) * sizeof (PT_ANGLE)); 
			// arr_cnt--; 
			// i--; 
		// } 
	// } 
	// printf ("arr_cnt now: %d; ", arr_cnt); 
	if (!override_full_acc){ 
		for (i= 0; i < arr_cnt; i++){ 
			if (!i){ 
				ptLast= &arr[arr_cnt - 1]; 
				ptNext= &arr[i + 1]; 
			} else if (i + 1 == arr_cnt){ 
				ptLast= &arr[i - 1]; 
				ptNext= &arr[0]; 
			} else { 
				ptLast= &arr[i - 1]; 
				ptNext= &arr[i + 1]; 
			} 
			p.x= arr[i].x; 
			p.y= arr[i].y; 
			a.x= p.x - ptLast->x; 
			a.y= p.y - ptLast->y; 
			b.x= ptNext->x - p.x; 
			b.y= ptNext->y - p.y; 
			c= atan2 (b.y, b.x) - atan2 (a.y, a.x); 
			while (c > +PI) c -= 2 * PI; 
			while (c < -PI) c += 2 * PI; 
			// px= (DEPTH_PIXEL *)(((char *)(BitmapData)) + (main_w * 8 * (int)p.y) + (8 * (int)p.x)); 
			// if ((int)(px->r)){ 
				// px->r *= 2; 
				// px->b *= 2; 
			// } else { 
				// px->r= 63; 
				// px->b= 63; 
			// } 
			// // printf ("pt %d,%d; ", (int)p.x, (int)p.y); 
			// px->depth= 0; 
			// printf ("p:(%f,%f); last:(%f,%f); next:(%f,%f); %f ", p.x, p.y, ptLast->x, ptLast->y, ptNext->x, ptNext->y, c); 
			if (c >= 0 || (int)(c) == NaN){ 
				if (i + 1 < arr_cnt) memmove (&arr[i+0], &arr[i+1], (arr_cnt - i - 1) * sizeof (PT_ANGLE)); 
				arr_cnt--; 
				i--; 
				// px->b= 0; 
				// printf ("dropping... \r\n"); 
				// if (arr_cnt < 6) printf ("arr_cnt: %d; \r\n", arr_cnt); 
				continue; 
			} 
			// printf ("\r\n"); 
		} 
	} 
	// printf ("now arr_cnt: %d; \r\n", arr_cnt); 
	if (UseDebug) printf ("step %d\r\n", 4); 
	// minC= main_h; 
	// maxC= 0; 
	// for (i= 0; i < arr_cnt; i++){ 
		// if ((int)(arr[i].y) < minC) minC= (int)(arr[i].y); 
		// if ((int)(arr[i].y) > maxC) maxC= (int)(arr[i].y); 
	// } 
	if (override_full_acc){ 
		minC= 0; 
		maxC= main_h - 1; 
	} else { 
		minC= main_h; 
		maxC= 0; 
		for (i= 0; i < arr_cnt; i++){ 
			if ((int)(arr[i].y) < minC) minC= (int)(arr[i].y); 
			if ((int)(arr[i].y) > maxC) maxC= (int)(arr[i].y); 
		} 
	} 
	if (UseVerbose && UseDebug){ 
		printf ("Object Color:  RGB(%d,%d,%d)\r\n", (int)(objColor[0]), (int)(objColor[1]), (int)(objColor[2])); 
	} 
	// if (UseVerbose && UseDebug) OutputMatrix ((double *)(matRinv), 3, 3); 
	// if (UseVerbose && UseDebug) printf ("minC: %d\r\nmaxC: %d\r\n", minC, maxC); 
	//if (minC < 0 && !(maxC > 0)) return; 
	if (minC < 0) minC= 0; 
	if (maxC >= main_w) maxC= main_w - 1; 
	if (minC > maxC){ 
		printf ("\t(Ellipsoid) minC > maxC; returning... \r\n"); 
		return; 
	} 
	time2= GetTickCount (); 
	// printf ("\tentering loop... (minC, maxC) = (%d, %d)\r\n", minC, maxC); 
	for (i= minC; i < maxC; i++){ 
		if (UseVerbose) if (!(i % 25)) printf ("\ti= %d (%d)\r\n", i, arr_cnt); 
		if (override_full_acc){ 
			minD= 0; 
			maxD= main_w - 1; 
		} else { 
			d_cnt= 0; 
			for (j= 0; j < arr_cnt; j++){ 
				ptLast= &arr[j]; 
				if (j + 1 < arr_cnt) ptNext= &arr[j+1]; 
				else ptNext= &arr[0]; 
				if (ptLast->y == ptNext->y) continue; 
				t= ((double)(i) - ptLast->y) / (ptNext->y - ptLast->y); 
				if (t < 0) continue; 
				if (t > 1) continue; 
				//if (UseVerbose && UseDebug) printf ("\tt: %f\r\n", t); 
				xt= (int)(ptPrev->x + (ptNext->x - ptPrev->x) * t); 
				d[d_cnt]= xt; 
				d_cnt++; 
				if (d_cnt >= 32){ 
					j= arr_cnt; 
					continue; 
				} 
			} 
			minD= main_w; 
			maxD= 0; 
			//if (UseVerbose && UseDebug) 
			//printf ("Iterating d array... \r\n"); 
			for (j= 0; j < d_cnt; j++){ 
				// if (UseVerbose && UseDebug) printf ("d[%d]=%d\r\n", j, d[j]); 
				if (d[j] == NaN) continue; 
				if (d[j] < minD) minD= d[j]; 
				if (d[j] > maxD) maxD= d[j]; 
			} 
			// printf ("\t(minD, maxD, d_cnt) = (%d, %d, %d)\r\n", minD, maxD, d_cnt); 
			// if (UseVerbose && UseDebug){ 
				// //printf ("\tminD: %d\r\n\tmaxD: %d\r\n", minD, maxD); 
			// } 
			// y= i; 
			if (minD > maxD) continue; 
			if (minD < 0) minD= 0; 
			if (minD >= main_w) minD= main_w - 1; 
		} 
		y= i; 
		center= (maxD - minD) / 2 + minD; 
		j= center + 1; 
		run_loop_1= 1; 
		while (run_loop_1 && j < maxD){ 
			x= j; 
			px= (DEPTH_PIXEL *)(((char *)(BitmapData)) + (main_w * 8 * y) + (8 * x)); 
			if ((int)(px) >= (int)(BitmapData) && (int)(BitmapData) + (main_w * main_h * 8) > (int)(px)){ 
				matE1[0]= matE1[1]= matE1[2]= 0; 
				matD1[0]= matD1[2]= 0; 
				matD1[1]= -1; 
				angles01.x= -(y - (main_h / 2)) * 2 * (cam_rv * zm) / main_h; 
				angles01.y= 0; 
				angles01.z= -(x - (main_w / 2)) * 2 * (cam_rh * zm) / main_w; 
				RotateAboutOrigin ((double *)(matD1), (double *)(&angles01)); 
				SubPoint ((POINT3D *)(matE1), (POINT3D *)(&primeP)); 
				MatMult (matRinv, 3, 3, matE1, 3, 1, matE); 
				MatMult (matRinv, 3, 3, matD1, 3, 1, matD); 
				// RotateAboutOrigin ((double *)(matD), (double *)(&angles01)); 
				// SubPoint ((POINT3D *)(matE), (POINT3D *)(&primeP)); 
				// if (rand () < 64){ 
					// printf ("\r\n\r\nRay-Casting... \r\n"); 
					// OutputMatrix ((double *)(&matE), 2, 3); 
					// OutputMatrix (&axisX, 1, 3); 
				// } 
				RenderEllipsoid_TotalCasts++; 
				t= NO_INTERSECT; 
				if (RayEllipsoid (axisX, axisY, axisZ, 
					matE[0], matE[1], matE[2], 
					matD[0], matD[1], matD[2], 
					&t1, &t2) != NO_INTERSECT){ 
						RenderEllipsoid_UsefulCasts++; 
						if (t2 == NO_INTERSECT || t2 < maxY){ 
							t= t1; 
						} else { 
							if (t1 < maxY) t= t2; 
							else { 
								if (t1 < t2) t= t1; 
								else t= t2; 
							} 
						} 
						if (-t > maxY){ 
							j++; 
							continue; 
						} 
					if (px->depth == PX_UNDEFINED || ((px->depth) > (long)(t * DepthScalar))){ 
						px->r= objColor[0]; 
						px->g= objColor[1]; 
						px->b= objColor[2]; 
						px->depth= (long)(t * DepthScalar); 
					} 
				} else if (UseDebugBox){ 
					if (px->depth == PX_UNDEFINED){ 
						px->depth= 2000; 
						px->g= 200; 
					} 
				} else { 
					run_loop_1= 0; 
				} 
			} else run_loop_1= 0; 
			j++; 
		} 
		j= center; 
		run_loop_1= 1; 
		while (run_loop_1 && j > minD){ 
			x= j; 
			px= (DEPTH_PIXEL *)(((char *)(BitmapData)) + (main_w * 8 * y) + (8 * x)); 
			if ((int)(px) >= (int)(BitmapData) && (int)(BitmapData) + (main_w * main_h * 8) > (int)(px)){ 
				matE1[0]= matE1[1]= matE1[2]= 0; 
				matD1[0]= matD1[2]= 0; 
				matD1[1]= -1; 
				angles01.x= -(y - (main_h / 2)) * 2 * (cam_rv * zm) / main_h; 
				angles01.y= 0; 
				angles01.z= -(x - (main_w / 2)) * 2 * (cam_rh * zm) / main_w; 
				RotateAboutOrigin ((double *)(matD1), (double *)(&angles01)); 
				SubPoint ((POINT3D *)(matE1), (POINT3D *)(&primeP)); 
				MatMult (matRinv, 3, 3, matE1, 3, 1, matE); 
				MatMult (matRinv, 3, 3, matD1, 3, 1, matD); 
				// RotateAboutOrigin ((double *)(matD), (double *)(&angles01)); 
				// SubPoint ((POINT3D *)(matE), (POINT3D *)(&primeP)); 
				// if (rand () < 64){ 
					// printf ("\r\n\r\nRay-Casting... \r\n"); 
					// OutputMatrix ((double *)(&matE), 2, 3); 
					// OutputMatrix (&axisX, 1, 3); 
				// } 
				RenderEllipsoid_TotalCasts++; 
				t= NO_INTERSECT; 
				if (RayEllipsoid (axisX, axisY, axisZ, 
					matE[0], matE[1], matE[2], 
					matD[0], matD[1], matD[2], 
					&t1, &t2) != NO_INTERSECT){ 
						RenderEllipsoid_UsefulCasts++; 
						if (t2 == NO_INTERSECT || t2 < maxY){ 
							t= t1; 
						} else { 
							if (t1 < maxY) t= t2; 
							else { 
								if (t1 < t2) t= t1; 
								else t= t2; 
							} 
						} 
						if (-t > maxY){ 
							j--; 
							continue; 
						} 
					if (px->depth == PX_UNDEFINED || ((px->depth) > (long)(t * DepthScalar))){ 
						px->r= objColor[0]; 
						px->g= objColor[1]; 
						px->b= objColor[2]; 
						px->depth= (long)(t * DepthScalar); 
					} 
				} else if (UseDebugBox){ 
					if (px->depth == PX_UNDEFINED){ 
						px->depth= 2000; 
						px->g= 200; 
					} 
				} else { 
					run_loop_1= 0; 
				} 
			} else run_loop_1= 0; 
			j--; 
		} 
		// for (j= center + 1; j != -1; (j > center)? j++:((j == center)? 0:j--)){ 
			// x= j; 
			// // if (x < 0) continue; 
			// if (y < 0){ 
				// j= 0; 
				// continue; 
			// } 
			// if (x >= main_w){ 
				// j= 0; 
				// continue; 
			// } 
			// if (y >= main_h){ 
				// continue; 
			// } 
			// px= (DEPTH_PIXEL *)(((char *)(BitmapData)) + (main_w * 8 * y) + (8 * x)); 
			// if ((int)(px) >= (int)(BitmapData) && (int)(BitmapData) + (main_w * main_h * 8) > (int)(px)){ 
				// matE1[0]= matE1[1]= matE1[2]= 0; 
				// matD1[0]= matD1[2]= 0; 
				// matD1[1]= -1; 
				// angles01.x= -(y - (main_h / 2)) * 2 * (cam_rv * zm) / main_h; 
				// angles01.y= 0; 
				// angles01.z= -(x - (main_w / 2)) * 2 * (cam_rh * zm) / main_w; 
				// RotateAboutOrigin ((double *)(matD1), (double *)(&angles01)); 
				// SubPoint ((POINT3D *)(matE1), (POINT3D *)(&primeP)); 
				// MatMult (matRinv, 3, 3, matE1, 3, 1, matE); 
				// MatMult (matRinv, 3, 3, matD1, 3, 1, matD); 
				// // RotateAboutOrigin ((double *)(matD), (double *)(&angles01)); 
				// // SubPoint ((POINT3D *)(matE), (POINT3D *)(&primeP)); 
				// // if (rand () < 64){ 
					// // printf ("\r\n\r\nRay-Casting... \r\n"); 
					// // OutputMatrix ((double *)(&matE), 2, 3); 
					// // OutputMatrix (&axisX, 1, 3); 
				// // } 
				// RenderEllipsoid_TotalCasts++; 
				// t= NO_INTERSECT; 
				// if (RayEllipsoid (axisX, axisY, axisZ, 
					// matE[0], matE[1], matE[2], 
					// matD[0], matD[1], matD[2], 
					// &t1, &t2) != NO_INTERSECT){ 
						// RenderEllipsoid_UsefulCasts++; 
						// if (t2 == NO_INTERSECT || t2 < maxY){ 
							// t= t1; 
						// } else { 
							// if (t1 < maxY) t= t2; 
							// else { 
								// if (t1 < t2) t= t1; 
								// else t= t2; 
							// } 
						// } 
						// if (-t > maxY) continue; 
					// if (px->depth == PX_UNDEFINED || ((px->depth) > (long)(t * DepthScalar))){ 
						// px->r= objColor[0]; 
						// px->g= objColor[1]; 
						// px->b= objColor[2]; 
						// px->depth= (long)(t * DepthScalar); 
					// } 
				// } else if (UseDebugBox){ 
					// if (px->depth == PX_UNDEFINED){ 
						// px->depth= 2000; 
						// px->g= 200; 
					// } 
				// } else { 
					// if (j > center) j= center; 
					// else j= 0; 
				// } 
				// if (UseDebug && rand () < 128){ 
					// printf ("\r\n\r\nRay-Casting... \r\n"); 
					// OutputMatrix ((double *)(&matE), 2, 3); 
					// OutputMatrix (&axisX, 1, 3); 
					// printf ("(t1, t2) = (%f, %f)\r\n", t1, t2); 
					// printf ("Depth: %d (undefined=%d)\r\n", px->depth, (int)(px->depth == PX_UNDEFINED)); 
					// printf ("Color (R, G, B): %d, %d, %d\r\n", px->r, px->g, px->b); 
				// } 
				// // if (rand () < 64){ 
					// // printf ("Current eyepoint and direction: \r\n"); 
					// // OutputMatrix ((double *)(matE), 1, 3); 
					// // OutputMatrix ((double *)(matD), 1, 3); 
					// // // printf ("Current t: %f\r\n", t); 
					// // // OutputMatrix ((double *)(&angles01), 3, 1); 
				// // } 
			// } else j= 0; 
		// } 
	} 
	time3= GetTickCount (); 
	#ifdef PROFILE_ON 
		profile_raycasting_before += (time2 - time1); 
		profile_raycasting_during += (time3 - time2); 
	#endif 
} 

int NewtonsMethodCubicZerosInteger (double a, double b, double c, double d, double start){ 
	int r1; 
	int r2= (int)(start); 
	double x0; 
	double x1= start; 
	// r1 = 46 
	// r2 = 30 
	// x0 = 46.4393 
	// x1 = 30.729 
	do { 
		x0= x1; 
		x1= x0 - (a * pow (x0, 3) + b * pow (x0, 2) + c * x0 + d) / (3 * a * pow (x0, 2) + 2 * b * x0 + c); 
		r1= r2; 
		r2= (int)(x1); 
	} while ((int)(abs (x0 - x1) * 2)); 
	return r2; 
} 
double NewtonsMethodCubicZerosIntegerDouble (double a, double b, double c, double d, double start){ 
	double x0; 
	double x1= start; 
	int count= 0; 
	if (!a && !b && !c) return start; 
	do { 
		if (count > 1024) break; 
		count++; 
		x0= x1; 
		x1= x0 - (a * pow (x0, 3) + b * pow (x0, 2) + c * x0 + d) / (3 * a * pow (x0, 2) + 2 * b * x0 + c); 
	// while (abs ((x0 - x1) * 8000)); 
	} while (floor ((a * pow (x1, 3) + b * pow (x1, 2) + c * x1 + d) * 8000)); 
	return x1; 
} 

int IsPointInPath (int x, int y, POINT_LINK * path, int len){ 
	
} 
	int CheckRange4 (int value, int a, int b, int c, int d){ 
		int min; 
		int max; 
		if (value == NaN) return value; 
		min= a; 
		if (b < min) min= b; 
		if (c < min) min= c; 
		if (d < min) min= d; 
		max= a; 
		if (b > max) max= b; 
		if (c > max) max= c; 
		if (d > max) max= d; 
		if (value < min) return NaN; 
		if (value > max) return NaN; 
		return value; 
	} 
int * GetPathIntersects (int y, int * count, POINT_LINK * path, int len){ 
	int * isects; 
	double a; 
	double b; 
	double c; 
	double infl; 
	double number; 
	double inflB; 
	double numberB; 
	double ans1, ans2, ans3; 
	int x0, x1, x2; 
	int i; 
	int j= 0; 
	int k; 
	int isects_len= j; 
	POINT_LINK * p0; 
	POINT_LINK * p1; 
	POINT_LINK * p2; 
	POINT_LINK * p3; 
	DEPTH_PIXEL * px; 
	double t; 
	int DivValue= 4; 
	*count= 0; 
	isects= GlobalAlloc (GMEM_ZEROINIT, (len + 4) * sizeof (int)); 
	if (len < 2){ 
		if (len == 1){ 
			p0= &path[0]; 
			if (p0->y == y){ 
				isects[0]= p0->x; 
			} else return isects; 
			*count= 1; 
			return isects; 
		} 
		return isects; 
	} 
	for (i= 1; i + 2 < len; i += 3){ 
		p0= &path[i-1]; 
		p1= &path[i-0]; 
		p2= &path[i+1]; 
		p3= &path[i+2]; 
		if (UseDebug) 
		if (0 && p0->y == p1->y) 
		printf ("bezier (%f, %f), (%f, %f), (%f, %f), (%f, %f)\r\n", 
			p0->x, p0->y, p1->x, p1->y, p2->x, p2->y, p3->x, p3->y); 
		c= 3 * (int)(p1->y - p0->y); 
		b= 3 * (int)(p2->y - p1->y) - c; 
		a= (int)(p3->y - p0->y) - c - b; 
		number= sqrt (4 * b * b - 12 * a * c) / (3 * a) * 2; 
		infl= -b / (3 * a); 
		ans1= NewtonsMethodCubicZerosIntegerDouble ((int)a, (int)b, (int)c, (int)(p0->y) - y, infl - number); 
		ans2= NewtonsMethodCubicZerosIntegerDouble ((int)a, (int)b, (int)c, (int)(p0->y) - y, infl); 
		ans3= NewtonsMethodCubicZerosIntegerDouble ((int)a, (int)b, (int)c, (int)(p0->y) - y, infl + number); 
		// if (!floor (100 * (p0->y - p1->y)) && !floor (100 * (p1->y - p2->y)) && !floor (100 * (p2->y - p3->y))){ 
			// ans1= 0; 
			// ans2= F_NaN; 
			// ans3= 1; 
		// } 
		// inflB= floor (infl * 1000); 
		// numberB= floor (number * 1000); 
		// if (0 && p0->y == p1->y){ 
			// printf ("\t(a, b, c) = (%f, %f, %f), d = %f\r\n", a, b, c, p0->y - y); 
			// printf ("\tans1= %f,  ans2= %f,  ans3= %f \r\n", ans1, ans2, ans3); 
		// } 
		// if (ans1 == infl - number) ans1= 0; 
		// if (ans3 == infl + number) ans3= 1; 
		// if (floor (ans1 * 1000) == inflB - numberB) ans1= 0; 
		// if (floor (ans2 * 1000) == inflB) ans2= F_NaN; 
		// if (floor (ans3 * 1000) == inflB + numberB) ans3= 1; 
		/*ans1= NewtonsMethodCubicZerosIntegerDouble (a, b, c, p0->y - y, 0); 
		ans2= NewtonsMethodCubicZerosIntegerDouble (a, b, c, p0->y - y, 0.5); 
		ans3= NewtonsMethodCubicZerosIntegerDouble (a, b, c, p0->y - y, 1); */
		if (ans1 < 0 || ans1 >= 1) ans1= F_NaN; 
		if (ans2 < 0 || ans2 >= 1) ans2= F_NaN; 
		if (ans3 < 0 || ans3 >= 1) ans3= F_NaN; 
		if (ans1 != F_NaN){ 
			if (floor ((a * pow (ans1, 3) + b * pow (ans1, 2) + c * ans1 + p0->y) / DivValue) != y / DivValue) ans1= F_NaN; 
		} 
		if (ans2 != F_NaN){ 
			if (floor ((a * pow (ans2, 3) + b * pow (ans2, 2) + c * ans2 + p0->y) / DivValue) != y / DivValue) ans2= F_NaN; 
		} 
		if (ans3 != F_NaN){ 
			if (floor ((a * pow (ans3, 3) + b * pow (ans3, 2) + c * ans3 + p0->y) / DivValue) != y / DivValue) ans3= F_NaN; 
		} 
		c= 3 * (p1->x - p0->x); 
		b= 3 * (p2->x - p1->x) - c; 
		a= p3->x - p0->x - c - b; 
		x0= (int)(a * pow (ans1, 3) + b * pow (ans1, 2) + c * ans1 + p0->x); 
		x1= (int)(a * pow (ans2, 3) + b * pow (ans2, 2) + c * ans2 + p0->x); 
		x2= (int)(a * pow (ans3, 3) + b * pow (ans3, 2) + c * ans3 + p0->x); 
		if (x0 == x2 && x0 == x1 && x1 != NaN){ 
			x0= x2= NaN; 
			// x1= NaN; 
		} else if (x1 == NaN && x0 != NaN){ 
			// x2= NaN; 
		} 
		// x0= CheckRange4 (x0, p0->x, p1->x, p2->x, p3->x); 
		// x1= CheckRange4 (x1, p0->x, p1->x, p2->x, p3->x); 
		// x2= CheckRange4 (x2, p0->x, p1->x, p2->x, p3->x); 
		if (x0 != NaN){ 
			isects[j]= x0; 
			if (x0 >= 0 && x1 < main_w && use_debug_dots){ 
				px= (DEPTH_PIXEL *)(BitmapData + main_w * 8 * y + 8 * x0); 
				if ((int)(px) >= (int)(BitmapData) && (int)(BitmapData) + (main_w * main_h * 8) > (int)(px)){ 
					px->r= 0; 
					px->g= 0; 
					px->b= 0; 
					px->depth= 0; 
				} 
			} 
			j++; 
		} 
		if (x1 != NaN){ 
			isects[j]= x1; 
			if (x1 >= 0 && x1 < main_w && use_debug_dots){ 
				px= (DEPTH_PIXEL *)(BitmapData + main_w * 8 * y + 8 * x1); 
				if ((int)(px) >= (int)(BitmapData) && (int)(BitmapData) + (main_w * main_h * 8) > (int)(px)){ 
					px->r= 0; 
					px->g= 0; 
					px->b= 0; 
					px->depth= 0; 
				} 
			} 
			j++; 
		} 
		if (x2 != NaN){ 
			isects[j]= x2; 
			if (x2 >= 0 && x2 < main_w && use_debug_dots){ 
				px= (DEPTH_PIXEL *)(BitmapData + main_w * 8 * y + 8 * x2); 
				if ((int)(px) >= (int)(BitmapData) && (int)(BitmapData) + (main_w * main_h * 8) > (int)(px)){ 
					px->r= 0; 
					px->g= 0; 
					px->b= 0; 
					px->depth= 0; 
				} 
			} 
			j++; 
		} 
	} 
	for (; i < len; i++){ 
		p0= &path[i-1]; 
		p1= &path[i]; 
		t= ((double)(y) - p0->y) / (p1->y - p0->y); 
		if (t < 0) continue; 
		if (t >= 1) continue; 
		x0= (p1->x - p0->x) * t + p0->x; 
		isects[j]= x0; 
		if (x0 >= 0 && x0 < main_w && use_debug_dots){ 
			px= (DEPTH_PIXEL *)(BitmapData + main_w * 8 * y + 8 * x0); 
			if ((int)(px) >= (int)(BitmapData) && (int)(BitmapData) + (main_w * main_h * 8) > (int)(px)){ 
				px->r= 0; 
				px->g= 0; 
				px->b= 0; 
				px->depth= 0; 
			} 
		} 
		j++; 
	} 
	// p0= &path[0]; 
	// p1= &path[len-1]; 
		// t= ((double)(y) - p0->y) / (p1->y - p0->y); 
		// if (t >= 0 && t <= 1){ 
			// x0= (p1->x - p0->x) * t + p0->x; 
			// isects[j]= x0; 
			// if (x0 >= 0 && x0 < main_w){ 
				// px= (DEPTH_PIXEL *)(BitmapData + main_w * 8 * y + 8 * x0); 
				// if ((int)(px) >= (int)(BitmapData) && (int)(BitmapData) + (main_w * main_h * 8) > (int)(px)){ 
					// px->r= 0; 
					// px->g= 0; 
					// px->b= 0; 
					// px->depth= 0; 
				// } 
			// } 
			// j++; 
		// } 
	*count= j; 
	return isects; 
} 
int GetPathRangeX (int y, POINT_RANGE * range, int max_range_count, POINT_LINK * path, int len){ 
	char string1[8192]; 
	char string2[512]; 
	int * isects; 
	POINT_RANGE * r; 
	double a; 
	double b; 
	double c; 
	double infl; 
	double number; 
	double ans1, ans2, ans3; 
	int x0, x1, x2; 
	int i; 
	int j= 0; 
	int k; 
	int isects_len= j; 
	POINT_LINK * p0; 
	POINT_LINK * p1; 
	POINT_LINK * p2; 
	POINT_LINK * p3; 
	double t; 
	string1[0]= 0; 
	if (UseDebug) printf ("***************************************\r\n"); 
	if (UseDebug) printf ("GetPathRangeX (); len=%d\r\n", len); 
	if (len < 2){ 
		if (len == 1){ 
			p0= &path[0]; 
			if (p0->y == y){ 
				r= &range[0]; 
				r->startY= y; 
				r->stopY= y; 
				r->startX= p0->x; 
				r->stopX= p0->x; 
				if (UseDebug) printf ("return; "); 
			} else return 0; 
			if (UseDebug) printf ("return; "); 
			return 1; 
		} 
		if (UseDebug) printf ("return; "); 
		return 0; 
	} 
	strcat (string1, "Working... \r\n"); 
	isects= GetPathIntersects (y, &isects_len, path, len); 
	// isects= GlobalAlloc (GMEM_ZEROINIT, (len + 4) * sizeof (int)); 
	// for (i= 1; i + 2 < len; i += 3){ 
		// p0= &path[i-1]; 
		// p1= &path[i-0]; 
		// p2= &path[i+1]; 
		// p3= &path[i+2]; 
		// if (UseDebug){ 
			// sprintf (string2, "bezier (%d, %d), (%d, %d), (%d, %d), (%d, %d); \r\n", (int)(p0->x), (int)(p0->y), (int)(p1->x), (int)(p1->y), 
				// (int)(p2->x), (int)(p2->y), (int)(p3->x), (int)(p3->y)); 
			// strcat (string1, string2); 
		// } 
		// c= 3 * (p1->y - p0->y); 
		// b= 3 * (p2->y - p1->y) - c; 
		// a= p3->y - p0->y - c - b; 
		// number= sqrt (4 * b * b - 12 * a * c) / (3 * a); 
		// infl= -b / (3 * a); 
		// ans1= NewtonsMethodCubicZerosIntegerDouble (a, b, c, p0->y - y, infl - number); 
		// ans2= NewtonsMethodCubicZerosIntegerDouble (a, b, c, p0->y - y, infl); 
		// ans3= NewtonsMethodCubicZerosIntegerDouble (a, b, c, p0->y - y, infl + number); 
		// if (UseDebug){ 
			// sprintf (string2, "\tY part. \r\n\t\tCubic Coefficients (a, b, c, d): %f, %f, %f, %f; \r\n\t\tFor y = %d\r\n\t\tRoots: %f, %f, %f\r\n", 
				// a, b, c, p0->y, 
				// y, 
				// ans1, ans2, ans3); 
			// strcat (string1, string2); 
		// } 
		// if (ans1 < 0 || ans1 > 1) ans1= F_NaN; 
		// if (ans2 < 0 || ans2 > 1) ans2= F_NaN; 
		// if (ans3 < 0 || ans3 > 1) ans3= F_NaN; 
		// c= 3 * (p1->x - p0->x); 
		// b= 3 * (p2->x - p1->x) - c; 
		// a= p3->x - p0->x - c - b; 
		// x0= (int)(a * pow (ans1, 3) + b * pow (ans1, 2) + c * ans1 + p0->x); 
		// x1= (int)(a * pow (ans2, 3) + b * pow (ans2, 2) + c * ans2 + p0->x); 
		// x2= (int)(a * pow (ans3, 3) + b * pow (ans3, 2) + c * ans3 + p0->x); 
		// if (UseDebug){ 
			// sprintf (string2, "\tX part. \r\n\t\tCubic Coefficients (a, b, c, d): %f, %f, %f, %f; \r\n\t\tRoots: %f, %f, %f\r\n", 
				// a, b, c, p0->y, 
				// (a * pow (ans1, 3) + b * pow (ans1, 2) + c * ans1 + p0->x), 
				// (a * pow (ans2, 3) + b * pow (ans2, 2) + c * ans2 + p0->x), 
				// (a * pow (ans3, 3) + b * pow (ans3, 2) + c * ans3 + p0->x)); 
			// strcat (string1, string2); 
		// } 
		// if (x0 == x2 && x1 != NaN){ 
			// x0= x2= NaN; 
		// } else if (x1 == NaN && x0 != NaN){ 
			// x2= NaN; 
		// } 
		// if (x0 != NaN){ 
			// if (UseDebug) strcat (string1, "\t\tRoot 0 exists. \r\n"); 
			// isects[j]= x0; 
			// j++; 
		// } 
		// if (x1 != NaN){ 
			// if (UseDebug) strcat (string1, "\t\tRoot 1 exists. \r\n"); 
			// isects[j]= x1; 
			// j++; 
		// } 
		// if (x2 != NaN){ 
			// if (UseDebug) strcat (string1, "\t\tRoot 2 exists. \r\n"); 
			// isects[j]= x2; 
			// j++; 
		// } 
	// } 
	// for (; i < len; i++){ 
		// p0= &path[i-1]; 
		// p1= &path[i]; 
		// t= ((double)(y) - p0->y) / (p1->y - p0->y); 
		// if (t < 0) continue; 
		// if (t > 1) continue; 
		// x0= (p1->x - p0->x) * t + p0->x; 
		// isects[j]= x0; 
		// j++; 
	// } 
	j= isects_len; 
	p0= &path[0]; 
	p1= &path[len - 1]; 
	t= ((double)(y) - p0->y) / (p1->y - p0->y); 
	if (!(t < 0 || t > 1)){ 
		x0= (p1->x - p0->x) * t + p0->x; 
		isects[j]= x0; 
		j++; 
	} 
	isects_len= j; 
	for (i= 0; i + 1 < isects_len; i++){ 
		for (j= 0; j + 1 < isects_len; j++){ 
			if (isects[j+0] > isects[j+1]){ 
				k= isects[j+0]; 
				isects[j+0]= isects[j+1]; 
				isects[j+1]= k; 
			} 
		} 
	} 
	for (i= 0; i < isects_len; i += 2){ 
		if (i / 2 + 1 > max_range_count){ 
			i= isects_len; 
			continue; 
		} 
		r= &range[i / 2]; 
		r->startY= y; 
		r->stopY= y; 
		r->startX= isects[i+0]; 
		r->stopX= isects[i+1]; 
	} 
	sprintf (string2, "C:\\WBord\\wwwroot\\AnimOv\\debug2_y%d.txt", y); 
	if (UseDebug) file_put_contents (string2, string1, strlen (string1)); 
	GlobalFree (isects); 
	if (UseDebug) printf ("return; "); 
	return i / 2; 
} 

// int GetPathRangeX (int y, POINT_RANGE * range, int max_range_count, POINT_LINK * path, int len) 

void RenderCurve (){ 
	char string1[8192]; 
	char string2[512]; 
	long len= CountCurvePoints (pFirst); 
	if (!len) return; 
	POINT_LINK * pts= GlobalAlloc (GMEM_ZEROINIT, (len + 1) * sizeof (POINT_LINK)); 
	int i; 
	int j; 
	int k; 
	int l; 
	POINT_LINK * p= pFirst; 
	POINT_RANGE * range= GlobalAlloc (GMEM_ZEROINIT, 512 * sizeof (POINT_RANGE)); 
	int range_len; 
	int minY= main_h; 
	int maxY= -1; 
	int minX; 
	int maxX; 
	int x; 
	int y; 
	double z; 
	double g; 
	double gdv; 
	double sum; 
	double bottom; 
	double opc= 1; 
	POINT_LINK * pt; 
	DEPTH_PIXEL * px; 
	string1[0]= 0; 
	if (UseDebug){ 
		strcat (string1, "Working... \r\n"); 
	} 
	for (i= 0; i < len; i++){ 
		pt= &pts[i]; 
		memmove (pt, p, sizeof (POINT_LINK)); 
		if (UseDebug){ 
			sprintf (string2, "Original Point: (%f, %f, %f); \r\n", pts[i].x, pts[i].y, pts[i].z); 
			strcat (string1, string2); 
		} 
		// if (debug_mode) printf ("fill place marker %d\r\n", 1); 
		GetStdHandle (STD_OUTPUT_HANDLE); 
		Rel2AbsBin (&pt->x, objVector, (char **)(InputBuffer)); 
		Rel2AbsOpcBin (&opc, objVector, (char **)(InputBuffer)); 
		// if (debug_mode) printf ("fill place marker %d\r\n", 2); 
		GetStdHandle (STD_OUTPUT_HANDLE); 
		CameraPositionTransformPt ((POINT3D *)(&pt->x), &cam_x); 
		z= pt->z; 
		pt->z= pt->y; 
		pt->y= z; 
		if (UseDebug){ 
			sprintf (string2, "Absolute (w/ cam): (%f, %f, %f); \r\n", pts[i].x, pts[i].y, pts[i].z); 
			strcat (string1, string2); 
		} 
		// if (debug_mode) printf ("fill place marker %d\r\n", 3); 
		GetStdHandle (STD_OUTPUT_HANDLE); 
		OpticTangentTransform ((POINT3D *)(&pt->x), 1, main_w, main_h, cam_rh, cam_rv, cam_zoom); 
		if (UseDebug){ 
			sprintf (string2, "Absolute (after OTT): (%f, %f, %f); \r\n", pts[i].x, pts[i].y, pts[i].z); 
			strcat (string1, string2); 
		} 
		// if (debug_mode) printf ("fill place marker %d\r\n", 4); 
		pt->y= floor (pt->y); 
		if (pt->y < minY) minY= pt->y; 
		if (pt->y > maxY) maxY= pt->y; 
		p= (POINT_LINK *)(p->pNext); 
		// if (debug_mode) printf ("fill place marker %d\r\n", 5); 
		GetStdHandle (STD_OUTPUT_HANDLE); 
	} 
	if (UseDebug) printf ("******************** opc=%f\r\n", opc); 
	if (UseDebug) printf ("minY, maxY = %d, %d\r\n", minY, maxY); 
	if (!opc) return; 
	// if (debug_mode) printf ("fill place marker %d\r\n", 6); 
	if (minY <= maxY && ((minY >= 0 && minY < main_h) || (maxY >= 0 && maxY < main_h))){ 
		if (minY != NaN && maxY != NaN){ 
			for (i= minY; i < maxY; i++){ 
				// if (debug_mode) printf ("fill place marker %d\r\n", 7); 
				range_len= GetPathRangeX (i, range, 512, pts, len); 
				for (k= 0; k < range_len; k++){ 
					// if (debug_mode) printf ("fill place marker %d\r\n", 8); 
					minX= range[k].startX; 
					maxX= range[k].stopX; 
					// if (debug_mode) printf ("(minX, maxX)= (%d, %d); ", minX, maxX); 
					if (minX == NaN || maxX == NaN) continue; 
					for (j= minX; j < maxX; j++){ 
						x= j; 
						y= i; 
						if (x < 0) continue; 
						if (y < 0) continue; 
						if (x > main_w) continue; 
						if (y > main_h) continue; 
						sum= 0; 
						bottom= 0; 
						if (UseDebug){ 
							sprintf (string2, "sum=0, bottom=0; so far: \r\n"); 
							strcat (string1, string2); 
						} 
						for (l= 0; l < len; l++){ 
							gdv= sqrt ((x - pts[l].x) * (x - pts[l].x) + (y - pts[l].y) * (y - pts[l].y)); 
							if (!gdv){ 
								sum= -pts[l].z; 
								bottom= 1; 
								l= len; 
								continue; 
							} 
							g= 1 / (gdv * gdv); 
							sum += -pts[l].z * g; 
							bottom += g; 
							if (UseDebug){ 
								sprintf (string2, "\tgdv= %f\r\n", gdv); 
								strcat (string1, string2); 
								sprintf (string2, "\tpt z = %f\r\n", pts[l].z); 
								strcat (string1, string2); 
								sprintf (string2, "\t\tsum += %f\r\n\t\tbottom += %f\r\n\t\tsum: %f\r\n\t\tbottom: %f\r\n", 
									pts[l].z * g, g, 
									sum, bottom); 
								strcat (string1, string2); 
							} 
						} 
						if (!bottom) continue; 
						z= sum / bottom; 
						//if (UseDebug) z= 200; 
						px= (DEPTH_PIXEL *)(BitmapData + main_w * 8 * y + 8 * x); 
						if ((int)(px) >= (int)(BitmapData) && (int)(BitmapData) + (main_w * main_h * 8) > (int)(px)){ 
							if (px->depth == PX_UNDEFINED || ((px->depth) > (long)(z * DepthScalar))){ 
								px->r= objColor[0]; 
								px->g= objColor[1]; 
								px->b= objColor[2]; 
								px->depth= (long)(z * DepthScalar); 
							} 
						} 
					} 
				} 
			} 
		} 
	} 
	// if (debug_mode) printf ("fill place marker %d\r\n", 10); 
	if (UseDebug) file_put_contents ("C:\\WBord\\wwwroot\\AnimOv\\debug1.txt", string1, strlen (string1)); 
	GlobalFree (pts); 
	GlobalFree (range); 
	// if (debug_mode) printf ("fill place marker %d\r\n", 11); 
} 

// int * GetPathIntersects (int y, int * count, POINT_LINK * path, int len) 

void StrokeCurve01 (){ 
	long len= CountCurvePoints (pFirst); 
	if (!len) return; 
	POINT_LINK * pts= GlobalAlloc (GMEM_ZEROINIT, (len + 1) * sizeof (POINT_LINK)); 
	int i; 
	int j; 
	int k; 
	int l; 
	POINT_LINK * p= pFirst; 
	POINT_LINK * pt; 
	double minY= main_h; 
	double maxY= -1; 
	int * xs; 
	int cnt; 
	double z; 
	int x; 
	int y; 
	DEPTH_PIXEL * px; 
	double opc= 1; 
	double sum; 
	double bottom; 
	double gdv; 
	double g; 
	for (i= 0; i < len; i++){ 
		pt= &pts[i]; 
		memmove (pt, p, sizeof (POINT_LINK)); 
		Rel2AbsBin (&pt->x, objVector, (char **)(InputBuffer)); 
		Rel2AbsOpcBin (&opc, objVector, (char **)(InputBuffer)); 
		CameraPositionTransformPt ((POINT3D *)(&pt->x), &cam_x); 
		z= pt->z; 
		pt->z= pt->y; 
		pt->y= z; 
		OpticTangentTransform ((POINT3D *)(&pt->x), 1, main_w, main_h, cam_rh, cam_rv, cam_zoom); 
		pt->y= floor (pt->y); 
		if (pt->y < minY) minY= pt->y; 
		if (pt->y > maxY) maxY= pt->y; 
		p= (POINT_LINK *)(p->pNext); 
	} 
	if (!opc){ 
		if (UseVerbose && UseDebug) printf ("Curve Opacity = 0; returning... \r\n"); 
		return; 
	} 
	if (minY <= maxY && ((minY >= 0 && minY < main_h) || (maxY >= 0 && maxY < main_h))){ 
		for (i= minY; i < maxY; i++){ 
			xs= GetPathIntersects (i, &cnt, pts, len); 
			for (j= 0; j < cnt; j++){ 
				x= xs[j]; 
				y= i; 
				if (x < 0) continue; 
				if (y < 0) continue; 
				if (x > main_w) continue; 
				if (y > main_h) continue; 
				if (UseDebug) printf ("\t\tstroke (%d, %d); \r\n", x, y); 
				sum= 0; 
				bottom= 0; 
				for (l= 0; l < len; l++){ 
					gdv= sqrt (pow (x - pts[l].x, 2) + pow (y - pts[l].y, 2)); 
					if (!gdv){ 
						sum= -pts[l].z; 
						bottom= 1; 
						l= len; 
						continue; 
					} 
					g= 1 / (gdv * gdv); 
					sum += -pts[l].z * g; 
					bottom += g; 
				} 
				if (!bottom) continue; 
				z= sum / bottom; 
				//if (UseDebug) z= 900; 
				px= (DEPTH_PIXEL *)(BitmapData + main_w * 8 * y + 8 * x); 
				if ((int)(px) >= (int)(BitmapData) && (int)(BitmapData) + (main_w * main_h * 8) > (int)(px)){ 
					if (px->depth == PX_UNDEFINED || ((px->depth) > (long)(z * DepthScalar))){ 
						px->r= objOutline[0]; 
						px->g= objOutline[1]; 
						px->b= objOutline[2]; 
						px->depth= (long)(z * DepthScalar); 
					} 
				} 
			} 
			GlobalFree (xs); 
		} 
	} 
	GlobalFree (pts); 
	// if (debug_mode) printf ("stroke place marker %d\r\n", 17); 
} 
void StrokeCurve (){ 
	long len= CountCurvePoints (pFirst); 
	if (!len) return; 
	POINT_LINK * pts= GlobalAlloc (GMEM_ZEROINIT, (len + 1) * sizeof (POINT_LINK)); 
	int i; 
	int j; 
	int k; 
	int l; 
	POINT_LINK * p= pFirst; 
	POINT_LINK * pt; 
	double minY= main_h; 
	double maxY= -1; 
	int * xs; 
	int cnt; 
	double z; 
	int x; 
	int y; 
	DEPTH_PIXEL * px; 
	POINT_LINK * a; 
	POINT_LINK * b; 
	POINT_LINK * c; 
	POINT_LINK * d; 
	double a_x, b_x, c_x; 
	double a_y, b_y, c_y; 
	double a_z, b_z, c_z; 
	double opc= 1; 
	double sum; 
	double bottom; 
	double gdv; 
	double g; 
	int acc= 256; 
	double t; 
	for (i= 0; i < len; i++){ 
		pt= &pts[i]; 
		memmove (pt, p, sizeof (POINT_LINK)); 
		Rel2AbsBin (&pt->x, objVector, (char **)(InputBuffer)); 
		Rel2AbsOpcBin (&opc, objVector, (char **)(InputBuffer)); 
		CameraPositionTransformPt ((POINT3D *)(&pt->x), &cam_x); 
		z= pt->z; 
		pt->z= pt->y; 
		pt->y= z; 
		OpticTangentTransform ((POINT3D *)(&pt->x), 1, main_w, main_h, cam_rh, cam_rv, cam_zoom); 
		// pt->y= floor (pt->y); 
		// if (pt->y < minY) minY= pt->y; 
		// if (pt->y > maxY) maxY= pt->y; 
		p= (POINT_LINK *)(p->pNext); 
	} 
	if (!opc){ 
		if (UseVerbose && UseDebug) printf ("Curve Opacity = 0; returning... \r\n"); 
		return; 
	} 
	for (i= 0; (i + 1) * 3 < len; i += 4){ 
		a= &pts[i+0]; 
		b= &pts[i+1]; 
		c= &pts[i+2]; 
		d= &pts[i+3]; 
		c_x= 3 * ((b->x) - (a->x)); 
		b_x= 3 * ((c->x) - (b->x)) - c_x; 
		a_x= (d->x) - (a->x) - c_x - b_x; 
		c_y= 3 * ((b->y) - (a->y)); 
		b_y= 3 * ((c->y) - (b->y)) - c_y; 
		a_y= (d->y) - (a->y) - c_y - b_y; 
		c_z= 3 * ((b->z) - (a->z)); 
		b_z= 3 * ((c->z) - (b->z)) - c_z; 
		a_z= (d->z) - (a->z) - c_z - b_z; 
		for (j= 0; j < acc; j++){ 
			t= (double)(j) / (double)(acc); 
			x= a_x * pow (t, 3) + b_x * pow (t, 2) + c_x * t + (a->x); 
			y= a_y * pow (t, 3) + b_y * pow (t, 2) + c_y * t + (a->y); 
			z= a_z * pow (t, 3) + b_z * pow (t, 2) + c_z * t + (a->z); 
			// sum= 0; 
			// bottom= 0; 
			// for (l= 0; l < len; l++){ 
				// gdv= sqrt (pow (x - pts[l].x, 2) + pow (y - pts[l].y, 2)); 
				// if (!gdv){ 
					// sum= -pts[l].z; 
					// bottom= 1; 
					// l= len; 
					// continue; 
				// } 
				// g= 1 / (gdv * gdv); 
				// sum += -pts[l].z * g; 
				// bottom += g; 
			// } 
			// if (!bottom) continue; 
			// z= sum / bottom; 
			//if (UseDebug) z= 900; 
			px= (DEPTH_PIXEL *)(BitmapData + main_w * 8 * y + 8 * x); 
			if ((int)(px) >= (int)(BitmapData) && (int)(BitmapData) + (main_w * main_h * 8) > (int)(px)){ 
				if (px->depth == PX_UNDEFINED || ((px->depth) > (long)(z * DepthScalar))){ 
					px->r= objOutline[0]; 
					px->g= objOutline[1]; 
					px->b= objOutline[2]; 
					px->depth= (long)(z * DepthScalar); 
				} 
			} 
		} 
	} 
	for (; i < len; i++){ 
		a= &pts[i-1]; 
		b= &pts[i+0]; 
		for (j= 0; j < acc; j++){ 
			t= (double)(j) / (double)(acc); 
			x= ((b->x) - (a->x)) * t; 
			y= ((b->y) - (a->y)) * t; 
			z= ((b->z) - (a->z)) * t; 
			// sum= 0; 
			// bottom= 0; 
			// for (l= 0; l < len; l++){ 
				// gdv= sqrt (pow (x - pts[l].x, 2) + pow (y - pts[l].y, 2)); 
				// if (!gdv){ 
					// sum= -pts[l].z; 
					// bottom= 1; 
					// l= len; 
					// continue; 
				// } 
				// g= 1 / (gdv * gdv); 
				// sum += -pts[l].z * g; 
				// bottom += g; 
			// } 
			// if (!bottom) continue; 
			// z= sum / bottom; 
			//if (UseDebug) z= 900; 
			px= (DEPTH_PIXEL *)(BitmapData + main_w * 8 * y + 8 * x); 
			if ((int)(px) >= (int)(BitmapData) && (int)(BitmapData) + (main_w * main_h * 8) > (int)(px)){ 
				if (px->depth == PX_UNDEFINED || ((px->depth) > (long)(z * DepthScalar))){ 
					px->r= objOutline[0]; 
					px->g= objOutline[1]; 
					px->b= objOutline[2]; 
					px->depth= (long)(z * DepthScalar); 
				} 
			} 
		} 
	} 
} 
	void PokeSphere (double atX, double atY, double atDepth, double radius, long color); 
		void DrawCurveSegment (POINT_LINK * a, POINT_LINK * b, POINT_LINK * c, POINT_LINK * d){ 
			int i; 
			int j; 
			double z; 
			int x; 
			int y; 
			DEPTH_PIXEL * px; 
			double a_x, b_x, c_x; 
			double a_y, b_y, c_y; 
			double a_z, b_z, c_z; 
			double t; 
			int acc= 16; 
			c_x= 3 * ((b->x) - (a->x)); 
			b_x= 3 * ((c->x) - (b->x)) - c_x; 
			a_x= (d->x) - (a->x) - c_x - b_x; 
			c_y= 3 * ((b->y) - (a->y)); 
			b_y= 3 * ((c->y) - (b->y)) - c_y; 
			a_y= (d->y) - (a->y) - c_y - b_y; 
			c_z= 3 * ((b->z) - (a->z)); 
			b_z= 3 * ((c->z) - (b->z)) - c_z; 
			a_z= (d->z) - (a->z) - c_z - b_z; 
			for (j= 0; j < acc; j++){ 
				t= (double)(j) / (double)(acc); 
				x= a_x * pow (t, 3) + b_x * pow (t, 2) + c_x * t + (a->x); 
				y= a_y * pow (t, 3) + b_y * pow (t, 2) + c_y * t + (a->y); 
				z= a_z * pow (t, 3) + b_z * pow (t, 2) + c_z * t + (a->z); 
				px= (DEPTH_PIXEL *)(BitmapData + main_w * 8 * y + 8 * x); 
				if ((int)(px) >= (int)(BitmapData) && (int)(BitmapData) + (main_w * main_h * 8) > (int)(px)){ 
					if (px->depth == PX_UNDEFINED || ((px->depth) > (long)(z * DepthScalar))){ 
						px->r= objOutline[0]; 
						px->g= objOutline[1]; 
						px->b= objOutline[2]; 
						px->depth= (long)(z * DepthScalar); 
					} 
				} 
			} 
		} 
	void DrawCurveOutline (POINT_LINK * pts, int len, int extraLineWidth){ 
		int i; 
		int j; 
		int k; 
		int l; 
		double z; 
		int x; 
		int y; 
		int f_x; 
		int f_y; 
		double f_z; 
		DEPTH_PIXEL * px; 
		POINT_LINK * a; 
		POINT_LINK * b; 
		POINT_LINK * c; 
		POINT_LINK * d; 
		POINT_LINK a2; 
		POINT_LINK b2; 
		POINT_LINK c2; 
		POINT_LINK d2; 
		double p_x; 
		double p_y; 
		double p_x2; 
		double p_y2; 
		double a_x, b_x, c_x; 
		double a_y, b_y, c_y; 
		double a_z, b_z, c_z; 
		double opc= 1; 
		int acc= 8; 
		double t; 
		double angle; 
		double angle2; 
		double extra01; 
		int ottX, ottY, ottDepth; 
		double ottRadius; 
		double theD; 
		for (i= 0; (i + 1) * 3 < len; i += 4){ 
			a= &pts[i+0]; 
			b= &pts[i+1]; 
			c= &pts[i+2]; 
			d= &pts[i+3]; 
			theD= z; 
			t= 0 - 2 * atan ((theD - sqrt (pow (theD, 2) + pow (extraLineWidth / 2, 2))) / extraLineWidth * 2); 
			ottRadius= sqrt (pow (extraLineWidth / 2 * cos (t), 2) + pow (theD + extraLineWidth / 2 * sin (t), 2)); 
			angle= atan2  ((b->y) - (a->y), (b->x) - (a->x)); 
			angle2= atan2 ((d->y) - (c->y), (d->x) - (c->x)); 
			DrawCurveSegment (a, b, c, d); 
			for (j= 0; j < extraLineWidth / 2; j++){ 
				extra01= sin (j / extraLineWidth * PI); 
				p_x=  j * cos (angle  - PI / 2); 
				p_y=  j * sin (angle  - PI / 2); 
				p_x2= j * cos (angle2 - PI / 2); 
				p_y2= j * cos (angle2 - PI / 2); 
					a2.x= a->x + p_x; 
					a2.y= a->y + p_y; 
						a2.z= a->z - extra01; 
					b2.x= b->x + p_x; 
					b2.y= b->y + p_y; 
						b2.z= b->z - extra01; 
					c2.x= c->x + p_x2; 
					c2.y= c->y + p_y2; 
						c2.z= c->z - extra01; 
					d2.x= d->x + p_x2; 
					d2.y= d->y + p_y2; 
						d2.z= d->z - extra01; 
				DrawCurveSegment (&a2, &b2, &c2, &d2); 
					a2.x= a->x - p_x; 
					a2.y= a->y - p_y; 
					b2.x= b->x - p_x; 
					b2.y= b->y - p_y; 
					c2.x= c->x - p_x2; 
					c2.y= c->y - p_y2; 
					d2.x= d->x - p_x2; 
					d2.y= d->y - p_y2; 
				DrawCurveSegment (&a2, &b2, &c2, &d2); 
			} 
			// // c_x= 3 * ((b->x) - (a->x)); 
			// // b_x= 3 * ((c->x) - (b->x)) - c_x; 
			// // a_x= (d->x) - (a->x) - c_x - b_x; 
			// // c_y= 3 * ((b->y) - (a->y)); 
			// // b_y= 3 * ((c->y) - (b->y)) - c_y; 
			// // a_y= (d->y) - (a->y) - c_y - b_y; 
			// // c_z= 3 * ((b->z) - (a->z)); 
			// // b_z= 3 * ((c->z) - (b->z)) - c_z; 
			// // a_z= (d->z) - (a->z) - c_z - b_z; 
			// // for (j= 0; j < acc; j++){ 
				// // t= (double)(j) / (double)(acc); 
				// // x= a_x * pow (t, 3) + b_x * pow (t, 2) + c_x * t + (a->x); 
				// // y= a_y * pow (t, 3) + b_y * pow (t, 2) + c_y * t + (a->y); 
				// // z= a_z * pow (t, 3) + b_z * pow (t, 2) + c_z * t + (a->z); 
				// sum= 0; 
				// bottom= 0; 
				// for (l= 0; l < len; l++){ 
					// gdv= sqrt (pow (x - pts[l].x, 2) + pow (y - pts[l].y, 2)); 
					// if (!gdv){ 
						// sum= -pts[l].z; 
						// bottom= 1; 
						// l= len; 
						// continue; 
					// } 
					// g= 1 / (gdv * gdv); 
					// sum += -pts[l].z * g; 
					// bottom += g; 
				// } 
				// if (!bottom) continue; 
				// z= sum / bottom; 
				//if (UseDebug) z= 900; 
				// theD= z; 
				// // theD= sqrt (pow (x, 2) + pow (y, 2) + pow (z, 2)); 
				// t= 0 - 2 * atan ((theD - sqrt (pow (theD, 2) + pow (extraLineWidth / 2, 2))) / extraLineWidth * 2); 
				// ottRadius= sqrt (pow (extraLineWidth / 2 * cos (t), 2) + pow (theD + extraLineWidth / 2 * sin (t), 2)); 
				// // PokeSphere (x, y, z, ottRadius, *((long *)(objOutline))); 
				// angle= 
				// for (k= 0; k < (int)(ottRadius + 1); k++){ 
					// f_z= z + ottRadius * cos (k / (ottRadius + 1)); 
					// for (l= 0; l < k * k * 3 + 1; l++){ 
						// angle= l / (k * k) * 2; 
						// f_x= x + ottRadius * cos (angle); 
						// f_y= y + ottRadius * sin (angle); 
						// px= (DEPTH_PIXEL *)(BitmapData + main_w * 8 * f_y + 8 * f_x); 
						// if ((int)(px) >= (int)(BitmapData) && (int)(BitmapData) + (main_w * main_h * 8) > (int)(px)){ 
							// if (px->depth == PX_UNDEFINED || ((px->depth) > (long)(f_z * DepthScalar))){ 
								// px->r= objOutline[0]; 
								// px->g= objOutline[1]; 
								// px->b= objOutline[2]; 
								// px->depth= (long)(f_z * DepthScalar); 
							// } 
						// } 
					// } 
				// } 
		} 
	} 
		// for (; i + 1 < len; i++){ 
			// a= &pts[i-1]; 
			// b= &pts[i+0]; 
			// for (j= 0; j < acc; j++){ 
				// t= (double)(j) / (double)(acc); 
				// x= ((b->x) - (a->x)) * t; 
				// y= ((b->y) - (a->y)) * t; 
				// z= ((b->z) - (a->z)) * t; 
				// // sum= 0; 
				// // bottom= 0; 
				// // for (l= 0; l < len; l++){ 
					// // gdv= sqrt (pow (x - pts[l].x, 2) + pow (y - pts[l].y, 2)); 
					// // if (!gdv){ 
						// // sum= -pts[l].z; 
						// // bottom= 1; 
						// // l= len; 
						// // continue; 
					// // } 
					// // g= 1 / (gdv * gdv); 
					// // sum += -pts[l].z * g; 
					// // bottom += g; 
				// // } 
				// // if (!bottom) continue; 
				// // z= sum / bottom; 
				// //if (UseDebug) z= 900; 
				// // theD= sqrt (pow (atX, 2) + pow (atY, 2) + pow (atDepth, 2)); 
				// // t= 0 - 2 * atan ((theD - sqrt (pow (theD, 2) + pow (radius, 2))) / radius); 
				// // ottRadius= sqrt (pow (radius * cos (t), 2) + pow (theD + radius * sin (t), 2)); 
				// px= (DEPTH_PIXEL *)(BitmapData + main_w * 8 * y + 8 * x); 
				// if ((int)(px) >= (int)(BitmapData) && (int)(BitmapData) + (main_w * main_h * 8) > (int)(px)){ 
					// if (px->depth == PX_UNDEFINED || ((px->depth) > (long)(z * DepthScalar))){ 
						// px->r= objOutline[0]; 
						// px->g= objOutline[1]; 
						// px->b= objOutline[2]; 
						// px->depth= (long)(z * DepthScalar); 
					// } 
				// } 
			// } 
		// } 
	// } 
void TrickyCurve (){ 
	long len= CountCurvePoints (pFirst); 
	if (!len) return; 
	POINT_LINK * pts= GlobalAlloc (GMEM_ZEROINIT, (len + 1) * sizeof (POINT_LINK)); 
	int i; 
	int j; 
	int k; 
	int l; 
	POINT_LINK * p= pFirst; 
	POINT_LINK * pt; 
	double minY= main_h; 
	double maxY= -1; 
	int * xs; 
	int cnt; 
	double z; 
	int x; 
	int y; 
	DEPTH_PIXEL * px; 
	POINT_LINK * a; 
	POINT_LINK * b; 
	POINT_LINK * c; 
	POINT_LINK * d; 
	double a_x, b_x, c_x; 
	double a_y, b_y, c_y; 
	double a_z, b_z, c_z; 
	double opc= 1; 
	double sum; 
	double bottom; 
	double gdv; 
	double g; 
	int acc= 256; 
	double t; 
	for (i= 0; i < len; i++){ 
		pt= &pts[i]; 
		memmove (pt, p, sizeof (POINT_LINK)); 
		Rel2AbsBin (&pt->x, objVector, (char **)(InputBuffer)); 
		Rel2AbsOpcBin (&opc, objVector, (char **)(InputBuffer)); 
		CameraPositionTransformPt ((POINT3D *)(&pt->x), &cam_x); 
		z= pt->z; 
		pt->z= pt->y; 
		pt->y= z; 
		OpticTangentTransform ((POINT3D *)(&pt->x), 1, main_w, main_h, cam_rh, cam_rv, cam_zoom); 
		// pt->y= floor (pt->y); 
		// if (pt->y < minY) minY= pt->y; 
		// if (pt->y > maxY) maxY= pt->y; 
		p= (POINT_LINK *)(p->pNext); 
	} 
	if (!opc){ 
		if (UseVerbose && UseDebug) printf ("Curve Opacity = 0; returning... \r\n"); 
		return; 
	} 
	DrawCurveOutline (pts, len, floor (objLine / 2)); 
} 

void PokeSphere (double atX, double atY, double atDepth, double radius, long color){ 
	DEPTH_PIXEL * px; 
	int i; 
	int j; 
	int startI= floor (atX - radius); 
	int stopI= ceil (atX + radius); 
	int startJ; 
	int stopJ; 
	int startY= floor (atY); 
	int i_radius= ceil (radius); 
	int x; 
	int y; 
	double dist; 
	double ht; 
	double dp; 
	for (i= 0; i < i_radius * 2; i++){ 
		x= i + startI; 
		startJ= i_radius - sqrt (pow (radius, 2) - pow (i - i_radius, 2)); 
		stopJ= 2 * i_radius - startJ; 
		for (j= startJ; j < stopJ; j++){ 
			y= j + startY; 
			dist= sqrt (pow (atX - x, 2) + pow (atY - y, 2)); 
			ht= sqrt (pow (radius, 2) - pow (dist, 2)); 
			if ((long)(ht) != (1<<31)){ 
				dp= (ht + atDepth) * DepthScalar; 
				if ((int)(px) >= (int)(BitmapData) && (int)(BitmapData) + (main_w * main_h * 8) > (int)(px)){ 
					px= (DEPTH_PIXEL *)(BitmapData + main_w * 8 * y + 8 * x); 
					if ((int)(px) >= (int)(BitmapData) && (int)(BitmapData) + (main_w * main_h * 8) > (int)(px)){ 
						if (px->depth == PX_UNDEFINED || ((px->depth) > (long)(dp))){ 
							px->r= (unsigned char)(color); 
							px->g= (unsigned char)(color>>8); 
							px->b= (unsigned char)(color>>16); 
							px->depth= (long)(dp); 
						} 
					} 
				} 
			} 
		} 
	} 
} 

void PokeSphereOTT (double atX, double atY, double atDepth, double radius, long color){ 
	int ottX, ottY, ottDepth; 
	double ottRadius; 
	double t; 
	double theD= sqrt (pow (atX, 2) + pow (atY, 2) + pow (atDepth, 2)); 
		t= 0 - 2 * atan ((theD - sqrt (pow (theD, 2) + pow (radius, 2))) / radius); 
		ottRadius= sqrt (pow (radius * cos (t), 2) + pow (theD + radius * sin (t), 2)); 
	double theX= atX; 
	double theY= -atDepth; 
	double theZ= atY; 
		OpticTangentTransform ((POINT3D *)(&theX), 1, main_w, main_h, cam_rh, cam_rv, cam_zoom); 
		if (theZ < 0) theZ= -theZ; 
	char string1[512]; 
		sprintf (string1, "(x,y,z)=(%f,%f,%f)\r\ntheD=%f\r\nottRadius=%f\r\n", theX, theY, theZ, theD, ottRadius); 
		file_put_contents ("animov_debug_sphere.txt", string1, strlen (string1)); 
	DEPTH_PIXEL * px; 
	int i; 
	int j; 
	int startI= floor (theX - ottRadius); 
	int stopI= ceil (theX + ottRadius); 
	int startJ; 
	int stopJ; 
	int startY= floor (theY); 
	int i_radius= ceil (ottRadius); 
	int x; 
	int y; 
	double dist; 
	double ht; 
	double dp; 
	for (i= 0; i < i_radius * 2; i++){ 
		x= i + startI; 
		startJ= i_radius - sqrt (pow (ottRadius, 2) - pow (i - i_radius, 2)); 
		stopJ= 2 * i_radius - startJ; 
		for (j= startJ; j < stopJ; j++){ 
			y= j + startY; 
			dist= sqrt (pow (theX - x, 2) + pow (theY - y, 2)); 
			ht= sqrt (pow (ottRadius, 2) - pow (dist, 2)); 
			if ((long)(ht) != (1<<31)){ 
				dp= (ht + theZ) * DepthScalar; 
				px= (DEPTH_PIXEL *)(BitmapData + main_w * 8 * y + 8 * x); 
				if ((int)(px) >= (int)(BitmapData) && (int)(BitmapData) + (main_w * main_h * 8) > (int)(px)){ 
					if (px->depth == PX_UNDEFINED || ((px->depth) > (long)(dp))){ 
						px->r= (unsigned char)(color); 
						px->g= (unsigned char)(color>>8); 
						px->b= (unsigned char)(color>>16); 
						px->depth= (long)(dp); 
					} 
				} 
			} 
		} 
	} 
} 

void StrandCurve (){ 
	long len= CountCurvePoints (pFirst); 
	if (!len) return; 
	POINT_LINK * pts= GlobalAlloc (GMEM_ZEROINIT, (len + 1) * sizeof (POINT_LINK)); 
	int i; 
	int j; 
	int k; 
	int l; 
	POINT_LINK * p= pFirst; 
	POINT_LINK * pt; 
	double minY= main_h; 
	double maxY= -1; 
	int * xs; 
	int cnt; 
	double z; 
	int x; 
	int y; 
	DEPTH_PIXEL * px; 
	POINT_LINK * a; 
	POINT_LINK * b; 
	POINT_LINK * c; 
	POINT_LINK * d; 
	double a_x, b_x, c_x; 
	double a_y, b_y, c_y; 
	double a_z, b_z, c_z; 
	double opc= 1; 
	double sum; 
	double bottom; 
	double gdv; 
	double g; 
	int acc= 1; 
	int acc_number; 
	double t; 
	char string1[512]; 
	for (i= 0; i < len; i++){ 
		pt= &pts[i]; 
		memmove (pt, p, sizeof (POINT_LINK)); 
		Rel2AbsBin (&pt->x, objVector, (char **)(InputBuffer)); 
		Rel2AbsOpcBin (&opc, objVector, (char **)(InputBuffer)); 
		CameraPositionTransformPt ((POINT3D *)(&pt->x), &cam_x); 
		z= pt->z; 
		pt->z= pt->y; 
		pt->y= z; 
		OpticTangentTransform ((POINT3D *)(&pt->x), 1, main_w, main_h, cam_rh, cam_rv, cam_zoom); 
		// pt->y= floor (pt->y); 
		// if (floor (pt->y) < minY) minY= floor (pt->y); 
		// if (floor (pt->y) > maxY) maxY= floor (pt->y); 
		p= (POINT_LINK *)(p->pNext); 
	} 
	if (!opc){ 
		if (UseVerbose && UseDebug) printf ("Curve Opacity = 0; returning... \r\n"); 
		return; 
	} 
	for (i= 0; (i + 1) * 3 < len; i += 4){ 
		a= &pts[i+0]; 
		b= &pts[i+1]; 
		c= &pts[i+2]; 
		d= &pts[i+3]; 
		c_x= 3 * ((b->x) - (a->x)); 
		b_x= 3 * ((c->x) - (b->x)) - c_x; 
		a_x= (d->x) - (a->x) - c_x - b_x; 
		c_y= 3 * ((b->y) - (a->y)); 
		b_y= 3 * ((c->y) - (b->y)) - c_y; 
		a_y= (d->y) - (a->y) - c_y - b_y; 
		c_z= 3 * ((b->z) - (a->z)); 
		b_z= 3 * ((c->z) - (b->z)) - c_z; 
		a_z= (d->z) - (a->z) - c_z - b_z; 
		acc_number= 2 * acc * 
			(int)((sqrt (pow ((b->x) - (a->x), 2) + pow ((b->y) - (a->y), 2)) / ((a->reserved1) + (b->reserved1))) + 
			(sqrt (pow ((c->x) - (b->x), 2) + pow ((c->y) - (b->y), 2)) / ((b->reserved1) + (c->reserved1))) + 
			(sqrt (pow ((d->x) - (c->x), 2) + pow ((d->y) - (c->y), 2)) / ((c->reserved1) + (d->reserved1)))); 
		// acc_number= acc_number * 3 / 4; 
		file_put_contents ("anim_debug_acc_number.txt", (char *)(&acc), sizeof (int)); 
		sprintf (string1, "a=(%f,%f,%f,%f)\r\nb=(%f,%f,%f,%f)\r\nc=(%f,%f,%f,%f)\r\nd=(%f,%f,%f,%f)\r\n", 
			a->x, a->y, a->z, a->reserved1, 
			b->x, b->y, b->z, b->reserved1, 
			c->x, c->y, c->z, c->reserved1, 
			d->x, d->y, d->z, d->reserved1); 
		file_put_contents ("anim_debug_abcd.txt", string1, strlen (string1)); 
		// acc_number= 16; 
		for (j= 0; j < acc_number; j++){ 
			t= (double)(j) / (double)(acc_number); 
			x= a_x * pow (t, 3) + b_x * pow (t, 2) + c_x * t + (a->x); 
			y= a_y * pow (t, 3) + b_y * pow (t, 2) + c_y * t + (a->y); 
			z= a_z * pow (t, 3) + b_z * pow (t, 2) + c_z * t + (a->z); 
			// sum= 0; 
			// bottom= 0; 
			// for (l= 0; l < len; l++){ 
				// gdv= sqrt (pow (x - pts[l].x, 2) + pow (y - pts[l].y, 2)); 
				// if (!gdv){ 
					// sum= -pts[l].z; 
					// bottom= 1; 
					// l= len; 
					// continue; 
				// } 
				// g= 1 / (gdv * gdv); 
				// sum += -pts[l].z * g; 
				// bottom += g; 
			// } 
			// if (!bottom) continue; 
			// z= sum / bottom; 
			//if (UseDebug) z= 900; 
			PokeSphere (x, y, z, ((d->reserved1) - (a->reserved1)) * t + (a->reserved1), *((long *)(objColor))); 
			// px= (DEPTH_PIXEL *)(BitmapData + main_w * 8 * y + 8 * x); 
			//if (px->depth == PX_UNDEFINED || ((px->depth) > (long)(z * DepthScalar))){ 
			// if ((int)(px) >= (int)(BitmapData) && (int)(px) < (int)(BitmapData) + main_w * main_h * 8){ 
				// px->r= objOutline[0]; 
				// px->g= objOutline[1]; 
				// px->b= 255; //objOutline[2]; 
				// px->depth= -1; //(long)(z * DepthScalar); 
			// } 
		} 
	} 
	for (; i < len; i++){ 
		a= &pts[i-1]; 
		b= &pts[i+0]; 
		acc_number= 2 * acc * (int)(sqrt (pow ((b->x) - (a->x), 2) + pow ((b->y) - (a->y), 2)) / ((a->reserved1) + (b->reserved1))); 
		acc_number= 16; 
		for (j= 0; j < acc_number; j++){ 
			t= (double)(j) / (double)(acc_number); 
			x= ((b->x) - (a->x)) * t; 
			y= ((b->y) - (a->y)) * t; 
			z= ((b->z) - (a->z)) * t; 
			// sum= 0; 
			// bottom= 0; 
			// for (l= 0; l < len; l++){ 
				// gdv= sqrt (pow (x - pts[l].x, 2) + pow (y - pts[l].y, 2)); 
				// if (!gdv){ 
					// sum= -pts[l].z; 
					// bottom= 1; 
					// l= len; 
					// continue; 
				// } 
				// g= 1 / (gdv * gdv); 
				// sum += -pts[l].z * g; 
				// bottom += g; 
			// } 
			// if (!bottom) continue; 
			// z= sum / bottom; 
			//if (UseDebug) z= 900; 
			PokeSphere (x, y, z, ((b->reserved1) - (a->reserved1)) * t + (a->reserved1), *((long *)(objColor))); 
			// px= (DEPTH_PIXEL *)(BitmapData + main_w * 8 * y + 8 * x); 
			// if (px->depth == PX_UNDEFINED || ((px->depth) > (long)(z * DepthScalar))){ 
				// px->r= objOutline[0]; 
				// px->g= objOutline[1]; 
				// px->b= objOutline[2]; 
				// px->depth= (long)(z * DepthScalar); 
			// } 
		} 
	} 
} 

void * ExportDepthPixelsToBitmap (int mode, int w, int h, DEPTH_PIXEL * map){ 
	// mode:  0 = color , 1 = depth 
	/*if (UseVerbose && UseDebug) 
		printf ("export mode=%d, (%d, %d)\r\n", mode, w, h); */
	int sBmp= 14 + 40 + 4 * w * h; 
	void * bmp= GlobalAlloc (GMEM_ZEROINIT, sBmp + 32); 
	char * t1= bmp; 
	int i; 
	int x; 
	int y; 
	long a1; 
	long a1b; 
	long a2= 5; 
	long a2b= 2; 
	double a3; 
	double a4; 
	double a5= 0.1; 
	double a6; 
	double a7= 3; 
	long minDepth= -1; 
	long maxDepth= 0; 
	double eB; 
	double eC; 
	unsigned char val; 
	BGRA * clr; 
	DEPTH_PIXEL * px; 
	double scalar; 
	t1[0]= 'B'; 
	t1[1]= 'M'; 
	*((long *)((char *)bmp + 02))= sBmp; 
	*((long *)((char *)bmp + 10))= 54; 
	*((long *)((char *)bmp + 14))= 40; 
	*((long *)((char *)bmp + 18))= (long)(w); 
	*((long *)((char *)bmp + 22))= (long)(h); 
	*((short *)((char *)bmp + 26))= 1; 
	*((short *)((char *)bmp + 28))= 32; 
	*((long *)((char *)bmp + 30))= 0; 
	*((long *)((char *)bmp + 34))= 0; 
	*((long *)((char *)bmp + 38))= 0; 
	*((long *)((char *)bmp + 42))= 0; 
	*((long *)((char *)bmp + 46))= 0; 
	*((long *)((char *)bmp + 50))= 0; 
	if (mode){ 
		for (x= 0; x < w * h; x++){ 
			px= &map[x]; 
			if (px->depth != PX_UNDEFINED){ 
				if ((px->depth) - (maxDepth) > 0) maxDepth= px->depth; 
				if (px->depth < minDepth || minDepth == -1) minDepth= px->depth; 
			} 
		} 
		// for (y= 0; y < h; y++){ 
			// for (x= 0; x < w; x++){ 
				// px= &map[w * y + x]; 
				// if (px->depth != PX_UNDEFINED){ 
					// if ((px->depth) - (maxDepth) > 0) maxDepth= px->depth; 
					// if (px->depth < minDepth || minDepth == -1) minDepth= px->depth; 
				// } 
			// } 
		// } 
	} else minDepth= 0; 
	eC= (double)(-minDepth); 
	if (maxDepth) eB = pow (0.1, 1 / ((double)(maxDepth - minDepth))); 
	else eB= 0; 
	// for (y= 0; y < h; y++){ 
		// for (x= 0; x < w; x++){ 
	for (i= 0; i < w * h; i++){ 
		y= i / w; 
		x= i % w; 
			// px= &map[w * y + x]; 
			px= &map[i]; 
			// if (rand () < 64){ 
				// printf ("export (%d,%d,%d)[%d]\r\n", px->r, px->g, px->b, px->depth); 
			// } 
			clr= (BGRA *)((char *)bmp + 14 + 40 + (4 * w * (h - y - 1)) + (4 * x)); 
			if (px->depth == PX_UNDEFINED) continue; 
			clr->a= 255; 
			if (mode){ 
				if (mode == 1){ 
					val= (unsigned char)(255 * pow (eB, px->depth + eC)); 
					//val= (unsigned char)(255 - (px->depth) * 255 / 1000); 
					clr->r= val; 
					clr->g= val; 
					clr->b= val; 
				} else if (mode == 2){ 
					scalar= pow (eB, px->depth + eC); 
					clr->r= (unsigned char)((double)(px->r) * scalar); 
					clr->g= (unsigned char)((double)(px->g) * scalar); 
					clr->b= (unsigned char)((double)(px->b) * scalar); 
				} else if (mode == 3){ 
					scalar= (maxDepth - px->depth) / (maxDepth - minDepth); 
					clr->r= (unsigned char)((double)(px->r) * scalar); 
					clr->g= (unsigned char)((double)(px->g) * scalar); 
					clr->b= (unsigned char)((double)(px->b) * scalar); 
				} else if (mode == 4){ 
					clr->r= (unsigned char)(px->r); 
					clr->g= (unsigned char)(px->g); 
					clr->b= (unsigned char)(px->b); 
					if (x > 0 && x + 1 < w && ((a1= map[i-1].depth) == PX_UNDEFINED || (a1b= map[i+1].depth) == PX_UNDEFINED || 
						abs (abs (px->depth - a1) - abs (px->depth - a1b)) > DepthScalar * a2b)){ 
						clr->r= 0; 
						clr->g= 0; 
						clr->b= 0; 
					} 
					// if (x < w){ 
						// a1= map[i+1].depth; 
						// if ((a1 == PX_UNDEFINED) || abs (px->depth - a1) > DepthScalar * a2){ 
							// clr->r= 0; 
							// clr->g= 0; 
							// clr->b= 0; 
						// } 
					// } 
					if (y > 0 && y + 1 < h){ 
						a1= map[i-w].depth; 
						a1b= map[i+w].depth; 
						if ((a1 == PX_UNDEFINED) || (a1b == PX_UNDEFINED) || abs (abs (px->depth - a1) - abs (px->depth - a1b)) > DepthScalar * a2b){ 
							clr->r= 0; 
							clr->g= 0; 
							clr->b= 0; 
						} 
					} 
					// if (y + 1 < h){ 
						// a1= map[i+w].depth; 
						// if ((a1 == PX_UNDEFINED) || abs (px->depth - a1) > DepthScalar * a2){ 
							// clr->r= 0; 
							// clr->g= 0; 
							// clr->b= 0; 
						// } 
					// } 
				} else if (mode == 6){ 
					clr->r= (unsigned char)(px->r); 
					clr->g= (unsigned char)(px->g); 
					clr->b= (unsigned char)(px->b); 
					a6= 0; 
					if (x > 0 && ((a1= map[i-1].depth) != PX_UNDEFINED && (a3= (double)(abs (px->depth - a1)) / (DepthScalar)))){ 
						if (a3 < a2) a6= a3; 
					} else { 
						a1= map[i+1].depth; 
						a3= (double)(abs (px->depth - a1)) / (DepthScalar); 
						if (a3 < a2) a6= a3; 
					} 
					if (y > 0){ 
						a1= map[i-w].depth; 
						if ((a1 != PX_UNDEFINED) && (a3= (double)(abs (px->depth - a1)) / (DepthScalar))){ 
							if (a3 < a2){ 
								// a4= pow (a5, a3); 
								// clr->r= (unsigned char)((int)((double)(clr->r) * a4)); 
								// clr->g= (unsigned char)((int)((double)(clr->g) * a4)); 
								// clr->b= (unsigned char)((int)((double)(clr->b) * a4)); 
								if (a3 < a2){ 
									if (a6) a6= (a6 + a3) / 2; 
									else a6= a3; 
								} 
							} 
						} 
					} else { 
						a1= map[i+w].depth; 
						a3= (double)(abs (px->depth - a1)) / (DepthScalar); 
						if (a3 < a2){ 
							if (a6) a6= (a6 + a3) / 2; 
							else a6= a3; 
						} 
					} 
					if (a6 < a2){ 
						a4= pow (a5, a7 * atan (a6 / a2)); 
						clr->r= (unsigned char)((int)((double)(clr->r) * a4)); 
						clr->g= (unsigned char)((int)((double)(clr->g) * a4)); 
						clr->b= (unsigned char)((int)((double)(clr->b) * a4)); 
					} 
				} else if (mode == 7){ 
					clr->r= (unsigned char)(px->r); 
					clr->g= (unsigned char)(px->g); 
					clr->b= (unsigned char)(px->b); 
					a6= 0; 
					if (x > 0 && ((a1= map[i-1].depth) != PX_UNDEFINED && (a3= (double)(abs (px->depth - a1)) / (DepthScalar)))){ 
						a6= a3; 
					} else { 
						a1= map[i+1].depth; 
						a3= (double)(abs (px->depth - a1)) / (DepthScalar); 
						a6= a3; 
					} 
					if (y > 0){ 
						a1= map[i-w].depth; 
						if ((a1 != PX_UNDEFINED) && (a3= (double)(abs (px->depth - a1)) / (DepthScalar))){ 
							if (a3 < a2){ 
								// a4= pow (a5, a3); 
								// clr->r= (unsigned char)((int)((double)(clr->r) * a4)); 
								// clr->g= (unsigned char)((int)((double)(clr->g) * a4)); 
								// clr->b= (unsigned char)((int)((double)(clr->b) * a4)); 
								if (a6) a6= (a6 + a3) / 2; 
								else a6= a3; 
							} 
						} 
					} else { 
						a1= map[i+w].depth; 
						a3= (double)(abs (px->depth - a1)) / (DepthScalar); 
						if (a6) a6= (a6 + a3) / 2; 
						else a6= a3; 
					} 
					a4= pow (a5, a7 * atan (a6 / a2)); 
					clr->r= (unsigned char)((int)((double)(clr->r) * a4)); 
					clr->g= (unsigned char)((int)((double)(clr->g) * a4)); 
					clr->b= (unsigned char)((int)((double)(clr->b) * a4)); 
				} else { 
					clr->r= (unsigned char)(px->r); 
					clr->g= (unsigned char)(px->g); 
					clr->b= (unsigned char)(px->b); 
					if (i && map[i-1].depth == PX_UNDEFINED){ 
						clr->r= 0; 
						clr->g= 0; 
						clr->b= 0; 
					} else if (i + 1 < w * h && map[i+1].depth == PX_UNDEFINED){ 
						clr->r= 0; 
						clr->g= 0; 
						clr->b= 0; 
					} else if (y && map[i-w].depth == PX_UNDEFINED){ 
						clr->r= 0; 
						clr->g= 0; 
						clr->b= 0; 
					} else if (y + 1 < w * h && map[i+w].depth == PX_UNDEFINED){ 
						clr->r= 0; 
						clr->g= 0; 
						clr->b= 0; 
					} 
				} 
			} else { 
				clr->r= px->r; 
				clr->g= px->g; 
				clr->b= px->b; 
			} 
	} 
		// } 
	// } 
	return bmp; 
} 
void * ExportDepthPixelsToBitmapEx (int mode, int w, int h, DEPTH_PIXEL * map, int depth_scalar){ 
	// mode:  0 = color , 1 = depth 
	/*if (UseVerbose && UseDebug) 
		printf ("export mode=%d, (%d, %d)\r\n", mode, w, h); */
	int sBmp= 14 + 40 + 4 * w * h; 
	void * bmp= GlobalAlloc (GMEM_ZEROINIT, sBmp + 32); 
	char * t1= bmp; 
	int x; 
	int y; 
	long minDepth= -1; 
	long maxDepth= 0; 
	double eB; 
	double eC; 
	unsigned char val; 
	BGRA * clr; 
	DEPTH_PIXEL * px; 
	double scalar; 
	t1[0]= 'B'; 
	t1[1]= 'M'; 
	*((long *)((char *)bmp + 02))= sBmp; 
	*((long *)((char *)bmp + 10))= 54; 
	*((long *)((char *)bmp + 14))= 40; 
	*((long *)((char *)bmp + 18))= (long)(w); 
	*((long *)((char *)bmp + 22))= (long)(h); 
	*((short *)((char *)bmp + 26))= 1; 
	*((short *)((char *)bmp + 28))= 32; 
	*((long *)((char *)bmp + 30))= 0; 
	*((long *)((char *)bmp + 34))= 0; 
	*((long *)((char *)bmp + 38))= 0; 
	*((long *)((char *)bmp + 42))= 0; 
	*((long *)((char *)bmp + 46))= 0; 
	*((long *)((char *)bmp + 50))= 0; 
	if (mode){ 
		for (y= 0; y < h; y++){ 
			for (x= 0; x < w; x++){ 
				px= &map[w * y + x]; 
				if (px->depth != PX_UNDEFINED){ 
					if ((px->depth) - (maxDepth) > 0) maxDepth= px->depth; 
					if (px->depth < minDepth || minDepth == -1) minDepth= px->depth; 
				} 
			} 
		} 
	} else minDepth= 0; 
	eC= (double)(-minDepth); 
	if (maxDepth) eB = pow (0.1, 1 / ((double)(maxDepth - minDepth))); 
	else eB= 0; 
	for (y= 0; y < h; y++){ 
		for (x= 0; x < w; x++){ 
			px= &map[w * y + x]; 
			clr= (BGRA *)((char *)bmp + 14 + 40 + 4 * w * (h - y - 1) + 4 * x); 
			if (px->depth == PX_UNDEFINED) continue; 
			clr->a= 255; 
			if (mode){ 
				if (mode == 1){ 
					val= (unsigned char)(255 * pow (eB, px->depth + eC)); 
					//val= (unsigned char)(255 - (px->depth) * 255 / 1000); 
					clr->r= val; 
					clr->g= val; 
					clr->b= val; 
				} else if (mode == 2){ 
					scalar= pow (eB, px->depth + eC); 
					clr->r= (unsigned char)((double)(px->r) * scalar); 
					clr->g= (unsigned char)((double)(px->g) * scalar); 
					clr->b= (unsigned char)((double)(px->b) * scalar); 
				} else if (mode == 3){ 
					scalar= (px->depth - minDepth) / (maxDepth - minDepth); 
					clr->r= (unsigned char)((double)(px->r) * scalar); 
					clr->g= (unsigned char)((double)(px->g) * scalar); 
					clr->b= (unsigned char)((double)(px->b) * scalar); 
				} else if (mode == 4){ 
					
				} 
			} else { 
				clr->r= px->r; 
				clr->g= px->g; 
				clr->b= px->b; 
			} 
		} 
	} 
	return bmp; 
} 

#if 0 
	typedef struct { 
		unsigned char b; 
		unsigned char g; 
		unsigned char r; 
		unsigned char a; 
	} BGRA; 
#endif 

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
	img_data= (BGRA *)(MZ_MALLOC (4 * w * h + 16));      // 16 bytes more just in case. 
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
	pPng= (void *)(tdefl_write_image_to_png_file_in_memory (img_data, w, h, 4, &sPng)); 
	MZ_FREE (img_data); 
	*sizePngFile= sPng; 
	return pPng; 
} 

unsigned char * ExpandBMP (void * pBitmap){ 
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
	unsigned char * pBmp; 
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
	img_data= (BGRA *)(MZ_MALLOC (4 * w * h + 16));      // 16 bytes more just in case. 
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
	// for (i= 0; i < w * h; i++){ 
		// clr= &img_data[i]; 
		// j= clr->r; 
		// clr->r= clr->b; 
		// clr->b= j; 
	// } 
	// for (y= 0; y < h / 2; y++){ 
		// for (x= 0; x < w; x++){ 
			// px= &img_data[(y * w) + x]; 
			// clr= &img_data[((h - y - 1) * w) + x]; 
			// j= px->r; 
			// px->r= clr->r; 
			// clr->r= j; 
			// j= px->g; 
			// px->g= clr->g; 
			// clr->g= j; 
			// j= px->b; 
			// px->b= clr->b; 
			// clr->b= j; 
			// j= px->a; 
			// px->a= clr->a; 
			// clr->a= j; 
		// } 
	// } 
	// pPng= (void *)(tdefl_write_image_to_png_file_in_memory (img_data, w, h, 4, &sPng)); 
	pBmp= (unsigned char *)(MZ_MALLOC (14 + 40 + 4 * w * h + 16)); 
		memmove (pBmp + 14 + 40, img_data, 4 * w * h); 
	pBmp[0]= 'B'; 
	pBmp[1]= 'M'; 
	*((long *)(pBmp + 2))= 14 + 40 + 4 * w * h; 
	*((long *)(pBmp + 10))= 14 + 40; 
	*((long *)(pBmp + 14))= 40; 
	*((long *)(pBmp + 18))= w; 
	*((long *)(pBmp + 22))= h; 
	*((short *)(pBmp + 26))= 1; 
	*((short *)(pBmp + 28))= 32; 
	*((long *)(pBmp + 30))= 0; 
	*((long *)(pBmp + 34))= 0; 
	*((long *)(pBmp + 38))= 1000; 
	*((long *)(pBmp + 42))= 1000; 
	MZ_FREE (img_data); 
	// *sizePngFile= sPng; 
	return pBmp; 
} 

int file_size (char * filename){ 
	HANDLE hFile; 
	int sFile; 
	hFile= CreateFile (filename, GENERIC_READ, 7, 0, 3, 128, 0); 
	sFile= GetFileSize (hFile, 0); 
	CloseHandle (hFile); 
	return sFile; 
} 
long cprint (char *); 
char * file_get_contents (char * filename){ 
	HANDLE hFile; 
	OFSTRUCT ofs; 
	int sFile; 
	char * pFile; 
	unsigned long bRead; 
	int err; 
	char string1[512]; 
	// hFile= (HANDLE)(OpenFile (filename, &ofs, 0)); 
	hFile= CreateFile (filename, GENERIC_READ, 7, 0, 3, 128, 0); 
		err= GetLastError (); 
	sFile= GetFileSize (hFile, 0); 
		if (!err) err= GetLastError (); 
		// if (sFile && sFile != -1){ 
			// sprintf (string1, "\r\n\r\ndata: debug:sFile:%d\r\n\r\n", sFile); 
			// cprint (string1); 
		// } 
	pFile= GlobalAlloc (GMEM_ZEROINIT, sFile + 16); 
		if (!err) err= GetLastError (); 
	ReadFile (hFile, pFile, sFile, &bRead, 0); 
		if (!err) err= GetLastError (); 
	CloseHandle (hFile); 
	if (err){ 
		printf (string1, "data: alert:Error: %d\r\n\r\n", err); 
		cprint (string1); 
	} 
	return pFile; 
} 
char * file_get_contents_modified (char * filename, FILETIME * modstore){ 
	HANDLE hFile; 
	OFSTRUCT ofs; 
	int sFile; 
	char * pFile; 
	unsigned long bRead; 
	// hFile= (HANDLE)(OpenFile (filename, &ofs, 0)); 
	hFile= CreateFile (filename, GENERIC_READ, 7, 0, 3, 128, 0); 
	sFile= GetFileSize (hFile, 0); 
	pFile= GlobalAlloc (GMEM_ZEROINIT, sFile + 16); 
	GetFileTime (hFile, 0, 0, modstore); 
	ReadFile (hFile, pFile, sFile, &bRead, 0); 
	CloseHandle (hFile); 
	return pFile; 
} 
char * file_get_contents_size (char * filename, int * filesize){ 
	HANDLE hFile; 
	OFSTRUCT ofs; 
	int sFile; 
	char * pFile; 
	unsigned long bRead; 
	int err; 
	int err1; 
	// hFile= (HANDLE)(OpenFile (filename, &ofs, 0)); 
	hFile= CreateFile (filename, GENERIC_READ, 7, 0, 3, 128, 0); 
		err1= GetLastError (); 
	sFile= GetFileSize (hFile, 0); 
		err= GetLastError (); 
	pFile= GlobalAlloc (GMEM_ZEROINIT, sFile + 16); 
	ReadFile (hFile, pFile, sFile, &bRead, 0); 
	CloseHandle (hFile); 
	*filesize= sFile; 
	if (err1) printf ("Error: %d\r\n", err1); 
	if (err) printf ("Error: %d\r\n", err); 
	return pFile; 
} 

HANDLE file_open_read (char * filename){ 
	OFSTRUCT ofs; 
	HANDLE hFile; 
	// hFile= (HANDLE)(OpenFile (filename, &ofs, 0)); 
	hFile= CreateFile (filename, GENERIC_READ, 7, 0, 3, 128, 0); 
	return hFile; 
} 
void file_close (HANDLE hFile){ 
	CloseHandle (hFile); 
} 
int ModifiedSince (HANDLE hFile, FILETIME * modstore){ 
	FILETIME modlast; 
	debug_file (__FILE__); debug_line (__LINE__); 
	unsigned long * m= (unsigned long *)(&modlast); 
	unsigned long * n= (unsigned long *)(modstore); 
	m[0]= n[0]; 
	m[1]= n[1]; 
	debug_file (__FILE__); debug_line (__LINE__); 
	GetFileTime (hFile, 0, 0, modstore); 
	debug_file (__FILE__); debug_line (__LINE__); 
	if (m[0] == n[0] && m[1] == n[1]) return 0; 
	else return 1; 
} 

void CheckValue (char * string1){ 
	int i; 
	for (i= 0; string1[i]; i++){ 
		if (string1[i] == ':' || string1[i] == ';'){ 
			strcpy (&string1[i], &string1[i+1]); 
			i--; 
			continue; 
		} 
	} 
} 

void RenameProperties (char * buff, char * path, char * new_path){ 
	int i; 
	int j; 
	int k; 
	int c= strlen (path); 
	int d= strlen (new_path); 
	char * buffer= buff; 
	i= 0; 
	while (buffer[i]){ 
		k= 1; 
		for (j= 0; buffer[i+j] && buffer[i+j] != 61 && buffer[i+j] != 10 && k; j++){ 
			if (!path[j]){ 
				k= 0; 
				j--; 
			} else if (buffer[i+j] - path[j]){ 
				k= 0; 
				j--; 
			} 
		} 
		if (!path[j]){ 
			if (buffer[i+c] == '.' || buffer[i+c] == '=' || buffer[i+c] == 32 || buffer[i+c] == 9){ 
				memmove (&buffer[i+d], &buffer[i+c], strlen (&buffer[i+c]) + 1); 
				memmove (&buffer[i], new_path, d); 
			} 
		} 
		for (j= i; buffer[j] && buffer[j] != 10; j++); 
		if (!buffer[j]) break; 
		i= j + 1; 
	} 
} 
void RenamePropertiesBin (char ** buff, char * path, char * new_path){ 
	char * a; 
	char * b; 
	int c= strlen (path); 
	int d= strlen (new_path); 
	int i; 
	int j; 
	char ** buf; 
	char * p; 
	for (i= 0; buff[i]; i += 2){ 
		// if (strcmp (buff[i], path)) continue; 
		p= buff[i]; 
		a= (char *)(1); 
		for (j= 0; p[j] && a; j++){ 
			if (!path[j]){ 
				a= 0; 
				j--; 
			} else if (p[j] - path[j]){ 
				a= 0; 
				j--; 
			} 
		} 
		if (path[j]) continue; 
		if (!(p[c] == '.' || p[c] == '=' || p[c] == 32 || p[c] == 9)) continue; 
		memmove (&p[d], &p[c], strlen (&p[c]) + 1); 
		memmove (p, new_path, d); 
	} 
} 

void DeleteProperties (char * buff, char * path){ 
	int i; 
	int j; 
	int k; 
	int c= strlen (path); 
	char * buffer= buff; 
	i= 0; 
	while (buffer[i]){ 
		k= 1; 
		for (j= 0; buffer[i+j] && buffer[i+j] != 61 && buffer[i+j] != 10 && k; j++){ 
			if (!path[j]){ 
				k= 0; 
				j--; 
			} else if (buffer[i+j] - path[j]){ 
				k= 0; 
				j--; 
			} 
		} 
		if (!path[j]){ 
			if (buffer[i+c] == '.' || buffer[i+c] == '=' || buffer[i+c] == 32 || buffer[i+c] == 9){ 
				for (; buffer[i+j] && buffer[i+j] != 10; j++); 
				if (buffer[i+j]) j++; 
				strcpy (&buffer[i], &buffer[i+j]); 
				continue; 
			} 
		} 
		for (j= i; buffer[j] && buffer[j] != 10; j++); 
		if (!buffer[j]) break; 
		i= j + 1; 
	} 
} 
void DeletePropertiesBin (char ** buff, char * path){ 
	char * a; 
	char * b; 
	int c= strlen (path); 
	int i; 
	int j; 
	char ** buf; 
	char * p; 
	for (i= 0; buff[i]; i += 2){ 
		// if (strcmp (buff[i], path)) continue; 
		p= buff[i]; 
		a= (char *)(1); 
		for (j= 0; p[j] && a; j++){ 
			if (!path[j]){ 
				a= 0; 
				j--; 
			} else if (p[j] - path[j]){ 
				a= 0; 
				j--; 
			} 
		} 
		if (path[j]) continue; 
		if (!(p[c] == '.' || p[c] == '=' || p[c] == 32 || p[c] == 9)) continue; 
		a= buff[i]; 
		b= buff[i+1]; 
		for (j= 2; buff[i+j]; j += 2); 
		memmove (&buff[i], &buff[i+2], j * 2 * sizeof (char *)); 
		GlobalFree (a); 
		if (b){ 
			buf= (char **)(b); 
			for (j= 0; buf[j]; j += 2){ 
				GlobalFree (buf[j]); 
				if (buf[j+1]) GlobalFree (buf[j+1]); 
			} 
			GlobalFree (b); 
		} 
		i -= 2; 
	} 
} 

void EnsurePath (char * buffer, char * path, char * value, double at, int del_key){ 
	char * a; 
	char * b; 
	char * zero= "0"; 
	char * stop= "STOP"; 
	char string1[4096]; 
	char string2[4096]; 
	char string3[512]; 
	char string4[4096]; 
	int c; 
	int i; 
	int j; 
	int k; 
	int l; 
	int delete_key= del_key; 
	sprintf (string3, "%f", at); 
	for (i= 0; string3[i] && string3[i] != '.'; i++); 
	for (j= i; string3[j] && j - 1 < i + 2; j++); 
	string3[j]= 0; 
	string1[0]= 0; 
	c= strlen (stop); 
	i= 0; 
	while (buffer[i]){ 
		k= 1; 
		for (j= 0; buffer[i+j] && buffer[i+j] != 10 && k; j++){ 
			if (!stop[j]){ 
				k= 0; 
				j--; 
			} else if (buffer[i+j] - stop[j]){ 
				k= 0; 
				j--; 
			} 
		} 
		if (!stop[j]){ 
			if (buffer[i+c] == 13 || buffer[i+c] == 10){ 
				buffer[i]= 0; 
				break; 
			} 
		} 
		for (j= i; buffer[j] && buffer[j] != 10; j++); 
		if (!buffer[j]) break; 
		i= j + 1; 
	} 
	i= 0; 
	while (buffer[i]){ 
		for (j= 0; buffer[i+j] && buffer[i+j] != 61; j++){ 
			if (!path[j]) goto cont; 
			if (buffer[i+j] - path[j]) goto cont; 
		} 
		if (!buffer[i+j]) goto cont; 
		if (path[j]) goto cont; 
			j++; 
			for (k= 0; buffer[i+j+k] && buffer[i+j+k] != 10; k++) string1[k]= buffer[i+j+k]; 
			string1[k]= 0; 
			if (string1[k-1] == 13) string1[k-1]= 0; 
			if (buffer[i+j+k]) k++; 
			strcpy (&buffer[i], &buffer[i+j+k]); 
			break; 
		; cont: ; 
		for (j= i; buffer[j] && buffer[j] != 10; j++); 
		if (!buffer[j]) break; 
		i= j + 1; 
	} 
		// Terminate on 13, if any. 
		// This part is taken care of by earlier code. 
		// for (i= 0; string1[i] && string1[i] != 13; i++); 
		// string1[i]= 0; 
	if (at){ 
		if (string1[0]){ 
			a= strchr (string1, ':'); 
			b= strchr (string1, ';'); 
			if (a && b){ 
				if (a > b){ 
					for (i= 0; string1[i] != ';'; i++) string4[i]= string1[i]; 
					string4[i]= 0; 
					i++; 
				} else { 
					string4[0]= 48; 
					string4[1]= 0; 
					i= 0; 
				} 
				l= i; 
				if (delete_key || value){ 
					while (string1[i]){ 
						k= 1; 
						for (j= 0; string1[i+j] && string1[i+j] != ':' && string1[i+j] != ';' && k; j++){ 
							if (!string3[j]){ 
								k= 0; 
								j--; 
								continue; 
							} 
							if ((int)(string1[i+j] - string3[j])) goto cont2; 
						} 
						if (string3[j]) goto cont2; 
						for (; string1[i+j] && string1[i+j] != ';'; j++); 
						if (string1[i+j]) j++; 
						strcpy (&string1[i], &string1[i+j]); 
						if (value){ 
							k= strlen (string1); 
							for (k--; k + 1 && (string1[k] == 32 || string1[k] == 9); k--); 
							if (string1[k] != ';') strcat (string1, ";"); 
							strcat (string1, string3); 
							strcat (string1, ":"); 
							strcat (string1, value); 
							strcat (string1, ";"); 
						} 
						sprintf (string2, "%s=%s;%s\r\n", path, string4, &string1[l]); 
						strcpy (string1, string2); 
						goto cont4; 
						; cont2: ; 
						for (j= i; string1[j] && string1[j] != ';'; j++); 
						if (!string1[j]) break; 
						i= j + 1; 
					} 
					if (value){ 
						k= strlen (string1); 
						for (k--; k + 1 && (string1[k] == 32 || string1[k] == 9); k--); 
						if (string1[k] != ';') strcat (string1, ";"); 
						strcat (string1, string3); 
						strcat (string1, ":"); 
						strcat (string1, value); 
						strcat (string1, ";"); 
					} 
					; cont4: ; 
				} 
			} else if (a){ 
				for (i= 0; string1[i] && string1[i] != ':'; i++){ 
					if (!string3[i]) break; 
					if ((int)(string1[i] - string3[i])){ 
						if (value){ 
							strcat (string1, ";"); 
							strcat (string1, string3); 
							strcat (string1, ":"); 
							strcat (string1, value); 
							strcat (string1, ";"); 
						} 
						goto cont3; 
					} 
				} 
				if (value){ 
					strcpy (string1, string3); 
					strcat (string1, ":"); 
					strcat (string1, value); 
					strcat (string1, ";"); 
				} else string1[0]= 0; 
				; cont3: ; 
			} else if (b){ 
				if (value){ 
					k= strlen (string1); 
					for (k--; k + 1 && (string1[k] == 32 || string1[k] == 9); k--); 
					if (string1[k] != ';') strcat (string1, ";"); 
					strcat (string1, string3); 
					strcat (string1, ":"); 
					strcat (string1, value); 
					strcat (string1, ";"); 
				} 
			} else { 
				// printf ("%s, %s, %s, %s\r\n", path, string1, string3, value); 
				if (value){ 
					sprintf (string2, "%s=%s;%s:%s;", path, string1, string3, value); 
					strcpy (string1, string2); 
				} 
			} 
		} else { 
			sprintf (string1, "%s=0;%f:%s\r\n", path, at, (value? value:zero)); 
		} 
	} else { 
		if (string1[0]){ 
			a= strchr (string1, ':'); 
			b= strchr (string1, ';'); 
			if (a && b){ 
				if (a > b){ 
					sprintf (string2, "%s=%s;%s\r\n", path, (value? value:zero), b + 1); 
					strcpy (string1, string2); 
				} else { 
					sprintf (string2, "%s=%s;%s\r\n", path, (value? value:zero), string1); 
					strcpy (string1, string2); 
				} 
			} else if (a){ 
				sprintf (string2, "%s=%s;%s\r\n", path, (value? value:zero), string1); 
				strcpy (string1, string2); 
			} else if (b){ 
				sprintf (string2, "%s=%s;%s\r\n", path, (value? value:zero), b + 1); 
				strcpy (string1, string2); 
			} else { 
				sprintf (string1, "%s=%s\r\n", path, (value? value:zero)); 
			} 
		} else { 
			sprintf (string1, "%s=%s\r\n", path, (value? value:zero)); 
		} 
	} 
	if (strstr (string1, path) != string1){ 
		strcpy (string2, string1); 
		sprintf (string1, "%s=%s", path, string2); 
	} 
	if (buffer[0]){ 
		for (i= strlen (buffer) - 1; i + 1 && (buffer[i] == 32 || buffer[i] == 9); i--); 
		if (buffer[i] != 10) strcat (&buffer[i], "\r\n"); 
	} 
	strcat (buffer, string1); 
	for (i += strlen (&buffer[i]) - 1; i + 1 && (buffer[i] == 32 || buffer[i] == 9); i--); 
	if (buffer[i] != 10) strcat (&buffer[i], "\r\n"); 
} 
char ** EnsurePathBin (char ** buffer, char * path, char * value, double at, int del_key){ 
	char * a; 
	char * b; 
	char * zero= "0"; 
	char * stop= "STOP"; 
	char string1[4096]; 
	char string2[4096]; 
	char string3[512]; 
	char string4[4096]; 
	double left_side; 
	double right_side; 
	int c; 
	int i; 
	int j; 
	int k; 
	int l; 
	int delete_key= del_key; 
	char ** buf; 
	char ** pBuffer= buffer; 
	double rAt= floor (at * 100) / 100; 
	double dbl_val= 0; 
	if (value) sscanf (value, " %lf", &dbl_val); 
	sprintf (string3, "%f", at); 
	for (i= 0; string3[i] && string3[i] != '.'; i++); 
	for (j= i; string3[j] && j - 1 < i + 2; j++); 
	string3[j]= 0; 
	string1[0]= 0; 
	c= strlen (stop); 
	i= 0; 
	while (buffer[i]){ 
		if (!buffer[i+1]) continue; 
		if (!strcmp (buffer[i], path)){ 
			buf= (char **)(buffer[i+1]); 
			j= i; 
			i= 0; 
			while (buf[i]){ 
				if (!buf[i+1]) continue; 
				left_side= (double)(floor ((*((double *)(buf[i]))) * 100)) / 100; 
				right_side= rAt; 
				// printf ("%f - %f\r\n", left_side, right_side); 
				if (!(left_side - right_side)){ 
					// printf ("We're here. \r\n"); 
					if (delete_key && !value){ 
						GlobalFree (buf[i]); 
						GlobalFree (buf[i+1]); 
						for (k= 0; buf[k+i+2]; k += 2); 
						memmove (&buf[i], &buf[i+2], (k + 2) * sizeof (char *)); 
					} else if (value){ 
						*((double *)(buf[i+1]))= dbl_val; 
						strncpy (buf[i+1] + sizeof (double), value, STRING_LIMIT); 
					} 
					return pBuffer; 
				} 
				i += 2; 
			} 
			if (value){ 
				buffer[j+1]= GlobalReAlloc (buf, (i + 4 + 4 /* extra 4 for margin */) * sizeof (char *), GMEM_ZEROINIT); 
				buf[i]= GlobalAlloc (GMEM_ZEROINIT, sizeof (double) + STRING_LIMIT); 
				*((double *)(buf[i]))= at; 
				strncpy (buf[i] + sizeof (double), string3, STRING_LIMIT); 
				buf[i+1]= GlobalAlloc (GMEM_ZEROINIT, sizeof (double) + STRING_LIMIT); 
				*((double *)(buf[i+1]))= dbl_val; 
				strncpy (buf[i+1] + sizeof (double), value, STRING_LIMIT); 
			} 
			return pBuffer; 
		} 
		i += 2; 
	} 
	if (value){ 
		pBuffer= GlobalReAlloc (buffer, (i + 4 + 4) * sizeof (char *), GMEM_ZEROINIT); 
		pBuffer[i]= GlobalAlloc (GMEM_ZEROINIT, STRING_LIMIT); 
		strncpy (pBuffer[i], path, STRING_LIMIT); 
		buf= (char **)(pBuffer[i+1]= GlobalAlloc (GMEM_ZEROINIT, 8 * sizeof (char *))); 
		i= 0; 
		buf[i]= GlobalAlloc (GMEM_ZEROINIT, sizeof (double) + STRING_LIMIT); 
		*((double *)(buf[i]))= at; 
		strncpy (buf[i] + sizeof (double), string3, STRING_LIMIT); 
		buf[i+1]= GlobalAlloc (GMEM_ZEROINIT, sizeof (double) + STRING_LIMIT); 
		*((double *)(buf[i+1]))= dbl_val; 
		strncpy (buf[i+1] + sizeof (double), value, STRING_LIMIT); 
	} 
	return pBuffer; 
	// while (buffer[i]){ 
		// k= 1; 
		// for (j= 0; buffer[i+j] && buffer[i+j] != 10 && k; j++){ 
			// if (!stop[j]){ 
				// k= 0; 
				// j--; 
			// } else if (buffer[i+j] - stop[j]){ 
				// k= 0; 
				// j--; 
			// } 
		// } 
		// if (!stop[j]){ 
			// if (buffer[i+c] == 13 || buffer[i+c] == 10){ 
				// buffer[i]= 0; 
				// break; 
			// } 
		// } 
		// for (j= i; buffer[j] && buffer[j] != 10; j++); 
		// if (!buffer[j]) break; 
		// i= j + 1; 
	// } 
	// i= 0; 
	// while (buffer[i]){ 
		// for (j= 0; buffer[i+j] && buffer[i+j] != 61; j++){ 
			// if (!path[j]) goto cont; 
			// if (buffer[i+j] - path[j]) goto cont; 
		// } 
		// if (!buffer[i+j]) goto cont; 
		// if (path[j]) goto cont; 
			// j++; 
			// for (k= 0; buffer[i+j+k] && buffer[i+j+k] != 10; k++) string1[k]= buffer[i+j+k]; 
			// string1[k]= 0; 
			// if (string1[k-1] == 13) string1[k-1]= 0; 
			// if (buffer[i+j+k]) k++; 
			// strcpy (&buffer[i], &buffer[i+j+k]); 
			// break; 
		// ; cont: ; 
		// for (j= i; buffer[j] && buffer[j] != 10; j++); 
		// if (!buffer[j]) break; 
		// i= j + 1; 
	// } 
		// // Terminate on 13, if any. 
		// // This part is taken care of by earlier code. 
		// // for (i= 0; string1[i] && string1[i] != 13; i++); 
		// // string1[i]= 0; 
	// if (at){ 
		// if (string1[0]){ 
			// a= strchr (string1, ':'); 
			// b= strchr (string1, ';'); 
			// if (a && b){ 
				// if (a > b){ 
					// for (i= 0; string1[i] != ';'; i++) string4[i]= string1[i]; 
					// string4[i]= 0; 
					// i++; 
				// } else { 
					// string4[0]= 48; 
					// string4[1]= 0; 
					// i= 0; 
				// } 
				// l= i; 
				// if (delete_key || value){ 
					// while (string1[i]){ 
						// k= 1; 
						// for (j= 0; string1[i+j] && string1[i+j] != ':' && string1[i+j] != ';' && k; j++){ 
							// if (!string3[j]){ 
								// k= 0; 
								// j--; 
								// continue; 
							// } 
							// if ((int)(string1[i+j] - string3[j])) goto cont2; 
						// } 
						// if (string3[j]) goto cont2; 
						// for (; string1[i+j] && string1[i+j] != ';'; j++); 
						// if (string1[i+j]) j++; 
						// strcpy (&string1[i], &string1[i+j]); 
						// if (value){ 
							// k= strlen (string1); 
							// for (k--; k + 1 && (string1[k] == 32 || string1[k] == 9); k--); 
							// if (string1[k] != ';') strcat (string1, ";"); 
							// strcat (string1, string3); 
							// strcat (string1, ":"); 
							// strcat (string1, value); 
							// strcat (string1, ";"); 
						// } 
						// sprintf (string2, "%s=%s;%s\r\n", path, string4, &string1[l]); 
						// strcpy (string1, string2); 
						// goto cont4; 
						// ; cont2: ; 
						// for (j= i; string1[j] && string1[j] != ';'; j++); 
						// if (!string1[j]) break; 
						// i= j + 1; 
					// } 
					// if (value){ 
						// k= strlen (string1); 
						// for (k--; k + 1 && (string1[k] == 32 || string1[k] == 9); k--); 
						// if (string1[k] != ';') strcat (string1, ";"); 
						// strcat (string1, string3); 
						// strcat (string1, ":"); 
						// strcat (string1, value); 
						// strcat (string1, ";"); 
					// } 
					// ; cont4: ; 
				// } 
			// } else if (a){ 
				// for (i= 0; string1[i] && string1[i] != ':'; i++){ 
					// if (!string3[i]) break; 
					// if ((int)(string1[i] - string3[i])){ 
						// if (value){ 
							// strcat (string1, ";"); 
							// strcat (string1, string3); 
							// strcat (string1, ":"); 
							// strcat (string1, value); 
							// strcat (string1, ";"); 
						// } 
						// goto cont3; 
					// } 
				// } 
				// if (value){ 
					// strcpy (string1, string3); 
					// strcat (string1, ":"); 
					// strcat (string1, value); 
					// strcat (string1, ";"); 
				// } else string1[0]= 0; 
				// ; cont3: ; 
			// } else if (b){ 
				// if (value){ 
					// k= strlen (string1); 
					// for (k--; k + 1 && (string1[k] == 32 || string1[k] == 9); k--); 
					// if (string1[k] != ';') strcat (string1, ";"); 
					// strcat (string1, string3); 
					// strcat (string1, ":"); 
					// strcat (string1, value); 
					// strcat (string1, ";"); 
				// } 
			// } else { 
				// if (value){ 
					// sprintf (string2, "%s=%s;%s:%s;", path, string1, string3, value); 
				// } 
			// } 
		// } else { 
			// sprintf (string1, "%s=0;%f:%s\r\n", path, at, (value? value:zero)); 
		// } 
	// } else { 
		// if (string1[0]){ 
			// a= strchr (string1, ':'); 
			// b= strchr (string1, ';'); 
			// if (a && b){ 
				// if (a > b){ 
					// sprintf (string2, "%s=%s;%s\r\n", path, (value? value:zero), b + 1); 
					// strcpy (string1, string2); 
				// } else { 
					// sprintf (string2, "%s=%s;%s\r\n", path, (value? value:zero), string1); 
					// strcpy (string1, string2); 
				// } 
			// } else if (a){ 
				// sprintf (string2, "%s=%s;%s\r\n", path, (value? value:zero), string1); 
				// strcpy (string1, string2); 
			// } else if (b){ 
				// sprintf (string2, "%s=%s;%s\r\n", path, (value? value:zero), b + 1); 
				// strcpy (string1, string2); 
			// } else { 
				// sprintf (string1, "%s=%s\r\n", path, (value? value:zero)); 
			// } 
		// } else { 
			// sprintf (string1, "%s=%s\r\n", path, (value? value:zero)); 
		// } 
	// } 
	// if (strstr (string1, path) != string1){ 
		// strcpy (string2, string1); 
		// sprintf (string1, "%s=%s", path, string2); 
	// } 
	// if (buffer[0]){ 
		// for (i= strlen (buffer) - 1; i + 1 && (buffer[i] == 32 || buffer[i] == 9); i--); 
		// if (buffer[i] != 10) strcat (&buffer[i], "\r\n"); 
	// } 
	// strcat (buffer, string1); 
	// for (i += strlen (&buffer[i]) - 1; i + 1 && (buffer[i] == 32 || buffer[i] == 9); i--); 
	// if (buffer[i] != 10) strcat (&buffer[i], "\r\n"); 
} 

char ** ImportPropertiesBin (char * buffer){ 
	char ** pBuffer; 
	char ** buf; 
	int c= 0; 
	int d; 
	int i; 
	int j; 
	int k; 
	char * pScan= buffer; 
	char * pKeyScan; 
	char line[STRING_LIMIT * 8]; 
	char path[STRING_LIMIT]; 
	char key[STRING_LIMIT]; 
	char value[STRING_LIMIT]; 
	double val; 
	double dbl_key; 
	for (i= 0; buffer[i]; i++){ 
		if (buffer[i] == 10) c++; 
	} 
	pBuffer= GlobalAlloc (GMEM_ZEROINIT, (c * 2 + 4) * sizeof (char *)); 
	j= 0; 
	pScan= scan_line (buffer, line, sizeof (line)); 
	do { 
		if (!strcmp (line, "STOP")) break; 
		// if (debug_mode) printf (":\t\tline: %s\r\n", line); 
		c= 1; 
		d= 0; 
		for (i= 0; line[i]; i++){ 
			if (line[i] == 59) c++; 
			if (!d) path[i]= line[i]; 
			if (line[i] == 61){ 
				d= i; 
				path[i]= 0; 
			} 
		} 
		// if (debug_mode) printf (":\t\t\tval: %s\r\n", &line[d+1]); 
		pBuffer[j]= GlobalAlloc (GMEM_ZEROINIT, STRING_LIMIT); 
		strncpy (pBuffer[j], path, STRING_LIMIT); 
		buf= (char **)(pBuffer[j+1]= GlobalAlloc (GMEM_ZEROINIT, (c * 2 + 16) * sizeof (char *))); 
		k= 0; 
		// printf (":\t\t\t"); 
		pKeyScan= scan_key (&line[d+1], key, sizeof (key), value, sizeof (value)); 
		do { 
			// printf ("%s:%s;", key, value); 
			dbl_key= 0; 
			val= 0; 
			sscanf (key, " %lf", &dbl_key); 
			sscanf (value, " %lf", &val); 
			buf[k]= GlobalAlloc (GMEM_ZEROINIT, sizeof (double) + STRING_LIMIT); 
			*((double *)(buf[k]))= dbl_key; 
			strncpy (buf[k] + sizeof (double), key, STRING_LIMIT); 
			buf[k+1]= GlobalAlloc (GMEM_ZEROINIT, sizeof (double) + STRING_LIMIT); 
			*((double *)(buf[k+1]))= val; 
			strncpy (buf[k+1] + sizeof (double), value, STRING_LIMIT); 
			// printf ("%s:%s\r\n", key, value); 
			k += 2; 
		} while (pKeyScan= scan_key (pKeyScan, key, sizeof (key), value, sizeof (value))); 
		// printf ("\r\n"); 
		buf[k]= 0; 
		buf[k+1]= 0; 
		j += 2; 
	} while (pScan= scan_line (pScan, line, sizeof (line))); 
	return pBuffer; 
} 
char * ExportPropertiesBin (char ** buffer){ 
	char * text; 
	char ** buf; 
	int i; 
	int j; 
	int c= 0; 
	char string1[512]; 
	char * p; 
	for (i= 0; buffer[i]; i += 2){ 
	// printf ("\r\n%s:\r\n\t", buffer[i]); 
		c += strlen (buffer[i]) + 1 + 2 + 32; 
		if (!(buf= (char **)(buffer[i+1]))) continue; 
		for (j= 0; buf[j]; j += 2){ 
			// printf ("%d, %d; ", buf[j], buf[j+1]); 
			c += strlen (buf[j] + sizeof (double)) + 2 + 32; 
			if (!buf[j+1]) continue; 
			c += strlen (buf[j+1] + sizeof (double)); 
		} 
	} 
	text= GlobalAlloc (GMEM_ZEROINIT, c + 512); 
	for (i= 0; buffer[i]; i += 2){ 
		// sprintf (string1, "%512s", buffer[i]); 
		// asm { 
			// mov eax, dword ptr [buffer] 
			// mov ebx, eax 
			// mov eax, dword ptr [i] 
			// mov ecx, sizeof (char *) 
			// mul ecx 
			// add ebx, eax 
			// mov eax, ebx 
			// mov dword ptr [p], eax 
		// } 
		p= buffer[i]; 
		// strcat (text, buffer[i]); 
		strcat (text, p); 
		// strcat (text, string1); 
		strcat (text, "="); 
		if (buf= (char **)(buffer[i+1])){ 
			for (j= 0; buf[j]; j += 2){ 
				// printf ("%s:%s;", buf[j] + sizeof (double), buf[j+1] + sizeof (double)); 
				if (!(*((double *)(buf[j])))){ 
					if (buf[j+1]){ 
						strcat (text, buf[j+1] + sizeof (double)); 
						strcat (text, ";"); 
					} 
				} 
			} 
		} 
		// printf ("\r\n"); 
		if (buf= (char **)(buffer[i+1])){ 
			for (j= 0; buf[j]; j += 2){ 
				string1[0]= 48; 
				string1[1]= 0; 
				if (buf[j]) sprintf (string1, "%f", *((double *)(buf[j]))); 
				strcat (text, string1); 
				strcat (text, ":"); 
				if (buf[j+1]) strcat (text, buf[j+1] + sizeof (double)); 
				strcat (text, ";"); 
			} 
		} 
		strcat (text, "\r\n"); 
	} 
	return text; 
} 
void DestroyPropertiesBin (char ** buffer){ 
	char ** buf; 
	int i; 
	int j; 
	for (i= 0; buffer[i]; i += 2){ 
		GlobalFree (buffer[i]); 
		if (!(buf= (char **)(buffer[i+1]))) continue; 
		for (j= 0; buf[j]; j += 2){ 
			GlobalFree (buf[j]); 
			GlobalFree (buf[j+1]); 
		} 
		GlobalFree (buf); 
	} 
	GlobalFree (buffer); 
} 

char * ParseAnimOvCommands (char * cmds){ 
	char line[4096]; 
	char * pMem; 
	char * pTmp; 
	int sMem= 8192; 
	char * pScan; 
	char cmd[512]; 
	char tail[4096]; 
	int i; 
	int an; 
	int an2; 
	char * p; 
	double at= 0; 
	double fps= 12; 
	double len= 1; 
	char two[1024]; 
	char three[1024]; 
	char four[1024]; 
	char path[1024]; 
	char npth[1024]; 
	// Do things by blocks of 4 pages; the page size is 512 bytes. 
	if (!cmds) return 0; 
	if (!strstr (cmds, "<end of message>")) return 0; 
	pMem= GlobalAlloc (GMEM_ZEROINIT, sMem); 
	pScan= scan_line (cmds, line, sizeof (line)); 
	do { 
		if (!strstr (line, "<stop>")) continue; 
		cmd[0]= 0; 
		tail[0]= 0; 
		an= sscanf (line, " %*d %s %[^<>\r\n]", cmd, tail); 
		if (!an) an= sscanf (line, " %s %[^<>\r\n]", cmd, tail); 
		if (!an) continue; 
		p= strchr (tail, '@'); 
		if (p){ 
			an2= sscanf (p + 1, " %lf", &at); 
			if (an2) if (fps) at /= fps; 
			if (p= strchr (p, '$')){ 
				sscanf (p + 1, " %lf", &at); 
			} 
		} 
		an2= sscanf (tail, " %s %s %s", two, three, four); 
		if (!strcmp (cmd, "set")){ 
			if (!strcmp (two, "background")){ 
				
			} else { 
				path[0]= 0; 
				if (!strcmp (two, "camera")){ 
					strcpy (path, "[CAM]"); 
					strcat (path, three); 
				} else if (!strcmp (two, "figure")){ 
					strcpy (path, three); 
				} 
				if (path[0]){ 
					// sprintf (main_debug, 
					// "data: debug:command=set;path=%s;param=%s;at=%f;\r\ndata:\ttest=%f\r\n\r\n", 
					// path, four, at, LookUpNumberEx (pMem, path, at)); 
					// strcat (main_debug, extra_string); 
					CheckValue (four); 
					// file_put_contents ("animov_vct_debug_0.txt", pMem, strlen (pMem)); 
					EnsurePath (pMem, path, four, at, 0); 
					// sprintf (main_debug, 
					// "data: debug:command=set;path=%s;param=%s;at=%f;\r\ndata:\ttest=%f\r\n\r\n", 
					// path, four, at, LookUpNumberEx (pMem, path, at)); 
					// file_put_contents ("animov_vct_debug_1.txt", pMem, strlen (pMem)); 
				} 
			} 
		} else if (!strcmp (cmd, "ren")){ 
			path[0]= 0; 
			if (!strcmp (two, "camera")){ 
				strcpy (path, "[CAM]"); 
			} 
			strcat (path, three); 
			if (FindParentObject (path, npth)){ 
				strcat (npth, "."); 
				strcat (npth, four); 
			} else strcpy (npth, four); 
			RenameProperties (pMem, path, npth); 
		} else if (!strcmp (cmd, "rna")){ 
			path[0]= 0; 
			npth[0]= 0; 
			if (!strcmp (two, "camera")){ 
				strcpy (path, "[CAM]"); 
				strcpy (npth, "[CAM]"); 
			} 
			strcat (path, three); 
			strcat (npth, four); 
			RenameProperties (pMem, path, npth); 
		} else if (!strcmp (cmd, "new")){ 
			
		} else if (!strcmp (cmd, "del")){ 
			if (!strcmp (two, "camera") || !strcmp (two, "figure")){ 
				path[0]= 0; 
				if (two[0] == 'c'){ 
					strcpy (path, "[CAM]"); 
				} 
				strcat (path, three); 
				DeleteProperties (pMem, path); 
			} else if (!strcmp (two, "key")){ 
				EnsurePath (pMem, three, 0, at, 1); 
			} 
		} else if (!strcmp (cmd, "fps")){ 
			sscanf (tail, "%lf", &fps); 
		} else if (!strcmp (cmd, "len")){ 
			sscanf (tail, "%lf", &len); 
		} else if (!strcmp (cmd, "cam")){ 
			EnsurePath (pMem, "CAM", two, at, 0); 
		} 
		if (strlen (pMem) > sMem - 4096){ 
			pTmp= GlobalAlloc (0, sMem + 4096); 
			strcpy (pTmp, pMem); 
			GlobalFree (pMem); 
			pMem= pTmp; 
			sMem += 4096; 
		} 
	} while (pScan= scan_line (pScan, line, sizeof (line))); 
	sprintf (line, "FPS=%f\r\nLEN=%f\r\n", fps, len); 
	i= strlen (line); 
	memmove (&pMem[i], pMem, strlen (pMem) + 1); 
	memmove (pMem, line, i); 
	return pMem; 
} 
void ExecAnimOvCommands (char * cmds, char * vctbuf, char ** vctbin, char ** pVctBuf, char *** pVctBin){ 
	char line[4096]; 
	char * pMem; 
	char * pTmp; 
	int sMem= strlen (vctbuf); //8192; 
	char * pScan; 
	char cmd[512]; 
	char tail[4096]; 
	int i; 
	int an; 
	int an2; 
	char * p; 
	double at= 0; 
	double fps= 12; 
	double len= 1; 
	char two[1024]; 
	char three[1024]; 
	char four[1024]; 
	char path[1024]; 
	char npth[1024]; 
	char ** pBin= vctbin; 
	// Do things by blocks of 4 pages; the page size is 512 bytes. 
	if (!cmds) return; 
	if (!strstr (cmds, "<end of message>")) return; 
	pMem= GlobalReAlloc (pMem, sMem + 8192, GMEM_ZEROINIT/*, sMem*/); 
	sMem += 8192; 
	pScan= scan_line (cmds, line, sizeof (line)); 
	do { 
		if (!strstr (line, "<stop>")) continue; 
		cmd[0]= 0; 
		tail[0]= 0; 
		an= sscanf (line, " %*d %s %[^<>\r\n]", cmd, tail); 
		if (!an) an= sscanf (line, " %s %[^<>\r\n]", cmd, tail); 
		if (!an) continue; 
		p= strchr (tail, '@'); 
		if (p){ 
			an2= sscanf (p + 1, " %lf", &at); 
			if (an2) if (fps) at /= fps; 
			if (p= strchr (p, '$')){ 
				sscanf (p + 1, " %lf", &at); 
			} 
		} 
		an2= sscanf (tail, " %s %s %s", two, three, four); 
		if (!strcmp (cmd, "set")){ 
			if (!strcmp (two, "background")){ 
				
			} else { 
				path[0]= 0; 
				if (!strcmp (two, "camera")){ 
					strcpy (path, "[CAM]"); 
					strcat (path, three); 
				} else if (!strcmp (two, "figure")){ 
					strcpy (path, three); 
				} 
				if (path[0]){ 
					CheckValue (four); 
					EnsurePath (pMem, path, four, at, 0); 
					pBin= EnsurePathBin (pBin, path, four, at, 0); 
				} 
			} 
		} else if (!strcmp (cmd, "ren")){ 
			path[0]= 0; 
			if (!strcmp (two, "camera")){ 
				strcpy (path, "[CAM]"); 
			} 
			strcat (path, three); 
			if (FindParentObject (path, npth)){ 
				strcat (npth, "."); 
				strcat (npth, four); 
			} else strcpy (npth, four); 
			RenameProperties (pMem, path, npth); 
			RenamePropertiesBin (pBin, path, npth); 
		} else if (!strcmp (cmd, "rna")){ 
			path[0]= 0; 
			npth[0]= 0; 
			if (!strcmp (two, "camera")){ 
				strcpy (path, "[CAM]"); 
				strcpy (npth, "[CAM]"); 
			} 
			strcat (path, three); 
			strcat (npth, four); 
			RenameProperties (pMem, path, npth); 
			RenamePropertiesBin (pBin, path, npth); 
		} else if (!strcmp (cmd, "new")){ 
			
		} else if (!strcmp (cmd, "del")){ 
			if (!strcmp (two, "camera") || !strcmp (two, "figure")){ 
				path[0]= 0; 
				if (two[0] == 'c'){ 
					strcpy (path, "[CAM]"); 
				} 
				strcat (path, three); 
				DeleteProperties (pMem, path); 
				DeletePropertiesBin (pBin, path); 
			} else if (!strcmp (two, "key")){ 
				EnsurePath (pMem, three, 0, at, 1); 
				pBin= EnsurePathBin (pBin, three, 0, at, 1); 
			} 
		} else if (!strcmp (cmd, "fps")){ 
			sscanf (tail, "%lf", &fps); 
		} else if (!strcmp (cmd, "len")){ 
			sscanf (tail, "%lf", &len); 
		} else if (!strcmp (cmd, "cam")){ 
			EnsurePath (pMem, "CAM", two, at, 0); 
			pBin= EnsurePathBin (pBin, "CAM", two, at, 0); 
		} 
		if (strlen (pMem) > sMem - 4096){ 
			pTmp= GlobalAlloc (0, sMem + 4096); 
			strcpy (pTmp, pMem); 
			GlobalFree (pMem); 
			pMem= pTmp; 
			sMem += 4096; 
		} 
	} while (pScan= scan_line (pScan, line, sizeof (line))); 
	sprintf (line, "FPS=%f\r\nLEN=%f\r\n", fps, len); 
	i= strlen (line); 
	memmove (&pMem[i], pMem, strlen (pMem) + 1); 
	memmove (pMem, line, i); 
	sprintf (line, "%f", fps); 
	pBin= EnsurePathBin (pBin, "FPS", line, 0, 0); 
	sprintf (line, "%f", len); 
	pBin= EnsurePathBin (pBin, "LEN", line, 0, 0); 
	*pVctBuf= pMem; 
	*pVctBin= pBin; 
} 

void * mpsFileOpenContext (char * filename){ 
	MPS_CONTEXT * ctx= MPS_ALLOC (GMEM_ZEROINIT, sizeof (MPS_CONTEXT)); 
	unsigned char * mpsbuf= (unsigned char *)(MPS_FILE_GET_CONTENTS (filename)); 
	unsigned char * p= mpsbuf; 
	unsigned char * pActualBaseMap; 
	long sActualBaseMap; 
	HANDLE hFile; 
	
	char string1[512]; 
	sprintf (string1, "~%s.1.%d.01.buf", filename, GetCurrentProcessId ()); 
	hFile= CreateFile (string1, GENERIC_WRITE | GENERIC_READ, 7, 0, 2, FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_RANDOM_ACCESS | FILE_FLAG_DELETE_ON_CLOSE, 0); 
	ctx->cbSize= sizeof (MPS_CONTEXT); 
	strcpy (ctx->type, "FILE"); 
	ctx->h= hFile; 
	// sActualBaseMap= 
} 

char hex2char (char high, char low){ 
	char a= high; 
	char b= low; 
	if (a >= 48 && a < 58) a -= 48; 
	else a= (a | 0x20) - 87; 
	if (b >= 48 && b < 58) b -= 48; 
	else b= (b | 0x20) - 87; 
	return ((a<<4) | b); 
} 
void urldecode (char * qstr, char * out8, wchar_t * out16, int max_len){ 
	int i= 0; 
	int j= 0; 
	char a; 
	wchar_t b; 
	char * p= out8; 
	wchar_t * q= out16; 
	while (qstr[i] && j + 1 < max_len){ 
		a= qstr[i]; 
		if (a == '%'){ 
			if (qstr[i+1] == 'u'){ 
				a= hex2char (qstr[i+2], qstr[i+3]); 
				b= a<<8; 
				a= hex2char (qstr[i+4], qstr[i+5]); 
				b= b | a; 
				if (p){ 
					*p= a; 
					p += sizeof (char); 
				} 
				if (q){ 
					*q= b; 
					q += sizeof (wchar_t); 
				} 
				i += 6; 
				j++; 
				continue; 
			} else { 
				a= hex2char (qstr[i+1], qstr[i+2]); 
				if (p){ 
					*p= a; 
					p += sizeof (char); 
				} 
				if (q){ 
					*q= (wchar_t)(a); 
					q += sizeof (wchar_t); 
				} 
				i += 3; 
				j++; 
				continue; 
			} 
		} else if (a == '+'){ 
			if (p){ 
				*p= 32; 
				p += sizeof (char); 
			} 
			if (q){ 
				*q= 32; 
				q += sizeof (wchar_t); 
			} 
			i++; 
			j++; 
			continue; 
		} else { 
			if (p){ 
				*p= a; 
				p += sizeof (char); 
			} 
			if (q){ 
				*q= (wchar_t)(a); 
				q += sizeof (wchar_t); 
			} 
			i++; 
			j++; 
			continue; 
		} 
	} 
	if (p){ 
		*p= 0; 
	} 
	if (q){ 
		*q= 0; 
	} 
} 

void CheckCmdSwitch (char * s){ 
	char string1[512]; 
	int i; 
	int j= 0; 
	// file_put_contents ("debug01a1.txt", "a", 1); 
	// for (i= 0; s[i]; i++); 
	for (i= 0; s[i] && (j= s[i]) != 32 && j != 9 && j != 13 && j != 10; i++); 
	if (j == 32 || j == 9 || j == 13 || j == 10){ 
		memmove (s + 1, s, strlen (s) + 1); 
		s[0]= 34; 
		i= 1; 
		while (s[i]){ 
			if (s[i] == 34){ 
				memmove (&s[i+1], &s[i], strlen (&s[i]) + 1); 
				s[i]= 92; 
				i += 2; 
				continue; 
			} else if (s[i] == 92){ 
				memmove (&s[i+1], &s[i], strlen (&s[i]) + 1); 
				i += 2; 
				continue; 
			} 
			i++; 
		} 
		s[i]= 34; 
		i++; 
		s[i]= 0; 
	} 
	i= 0; 
	while (s[i]){ 
		if (s[i] == 13 || s[i] == 10) s[i]= 32; 
		i++; 
	} 
} 

char * EnsureQueryHTTP (char * old_command_line){ 
	char method[512]; 
	char query[1024]; 
	char get_var[1024]; 
	char get_val[1024]; 
	char add_options[2048]; 
	char string1[512]; 
	int i; 
	int j; 
	char * p; 
	char * q; 
	char * result; 
	add_options[0]= 0; 
	method[0]= 0; 
	query[0]= 0; 
	if (!GetEnvironmentVariable ("REQUEST_METHOD", method, 512)) return old_command_line; 
	if (!GetEnvironmentVariable ("QUERY_STRING", query, 1024)) return old_command_line; 
	i= 0; 
	while (query[i]){ 
		strcpy (get_var, &query[i]); 
		p= strchr (get_var, '='); 
		q= strchr (get_var, '&'); 
		if (p && q){ 
			if (p < q){ 
				*p= 0; 
				strcpy (get_val, p + 1); 
				q= strchr (get_val, '&'); 
				*q= 0; 
			} else { 
				*p= 0; 
				get_val[0]= 0; 
			} 
		} else if (p){ 
			*p= 0; 
			strcpy (get_val, p + 1); 
		} else { 
			get_var[0]= 0; 
			get_val[0]= 0; 
		} 
		if (get_var[0]){ 
			urldecode (get_var, get_var, 0, 1024); 
			urldecode (get_val, get_val, 0, 1024); 
			CheckCmdSwitch (get_var); 
			CheckCmdSwitch (get_val); 
			sprintf (string1, " -%s %s", get_var, get_val); 
			strcat (add_options, string1); 
		} 
		for (j= 0; query[i+j] && query[i+j] != '&'; j++); 
		if (!query[i+j]) break; 
		i += 1 + j; 
	} 
	result= GlobalAlloc (GMEM_ZEROINIT, strlen (old_command_line) + strlen (add_options) + 16); 
	strcpy (result, old_command_line); 
	strcat (result, add_options); 
	return result; 
} 

void InspectAnimOvCommands (char * cmds){ 
	char * s1= "figures."; 
	int i= 0; 
	int j; 
	while (cmds[i]){ 
		for (j= 0; cmds[i+j] && s1[j] && cmds[i+j] == s1[j]; j++); 
		if (!cmds[i+j]){ 
			i++; 
			continue; 
		} 
		if (s1[j]){ 
			i++; 
			continue; 
		} 
		if (cmds[i-1] != 32 && cmds[i-1] != 9 && cmds[i-1] != 13 && cmds[i-1] != 10) continue; 
		memmove (&cmds[i], &cmds[i+j], strlen (&cmds[i+j]) + 1); 
	} 
} 

char * scan_line (char * buffer, char * output, int max_output_size){ 
	char a; 
	int i= 0; 
	if (!buffer) return 0; 
	if (!(*buffer)) return 0; 
	while (1){ 
		a= buffer[i]; 
		if (a == 13) break; 
		if (a == 10) break; 
		if (i + 1 < max_output_size) output[i]= a; 
		else if (i < max_output_size) output[i]= 0; 
		if (!a) break; 
		i++; 
	} 
	if (i < max_output_size) output[i]= 0; 
	while ((buffer[i] == 13) | (buffer[i] == 10)) i++; 
	return buffer + i; 
} 

void clear_file (char * filename){ 
	HANDLE hFile; 
	hFile= CreateFile (filename, GENERIC_WRITE, 5, 0, 2, FILE_ATTRIBUTE_NORMAL, 0); 
	SetFilePointer (hFile, 0, 0, 0); 
	SetEndOfFile (hFile); 
	CloseHandle (hFile); 
} 

char conv_table_2base64 [64]= { 
	65, 66, 67, 68, 
		69, 70, 71, 72, 
		73, 74, 75, 76, 
		77, 78, 79, 80, 
		81, 82, 83, 84, 
		85, 86, 87, 88, 
			89, 90, 
	97, 98, 99, 100, 
		101, 102, 103, 104, 
		105, 106, 107, 108, 
		109, 110, 111, 112, 
		113, 114, 115, 116, 
		117, 118, 119, 120, 
			121, 122, 
	48, 49, 50, 51, 
		52, 53, 54, 55, 
			56, 57, 
	43, 47 
}; 
char * base64_encode (unsigned char * buffer_data, int buffer_size){ 
	char * base64= GlobalAlloc (GMEM_ZEROINIT, buffer_size * 4 / 3 + 16); 
	unsigned char a; 
	unsigned char b; 
	unsigned char c; 
	char small_string[5]= {0}; 
	int padding= (3 - buffer_size % 3) % 3; 
	int i; 
	int j; 
	for (i= 0, j= 0; i < buffer_size; i += 3, j += 4){ 
		a= buffer_data[i]; 
		if (i + 1 < buffer_size) b= buffer_data[i+1]; 
		else b= 0; 
		if (i + 2 < buffer_size) c= buffer_data[i+2]; 
		else c= 0; 
		small_string[0]= conv_table_2base64[a>>2]; 
		small_string[1]= conv_table_2base64[((a & 3)<<4) | (b>>4)]; 
		small_string[2]= conv_table_2base64[((b & 15)<<2) | (c>>6)]; 
		small_string[3]= conv_table_2base64[c & 63]; 
		if (!(small_string[0] * small_string[1] * small_string[2] * small_string[3])){ 
			printf ("to %d; %d,%d,%d,%d\r\n", j, small_string[0], small_string[1], small_string[2], small_string[3]); 
			printf ("\tIndices: %d, %d, %d, %d\r\n", a>>2, ((a & 3)<<4) | (b>>4), ((b & 15)<<2) | (c>>6), c & 63); 
		} 
		// for (j= 0; j < padding; j++) small_string[3-j]= 61; 
		memmove (base64 + j, small_string, 4); 
		// printf ("to %d; %d,%d,%d,%d\r\n", j, small_string[0], small_string[1], small_string[2], small_string[3]); 
		// for (j= 0; j < padding; j++) strcat (base64, "="); 
		// memmove (base64 + strlen (base64), small_string, 4); 
	} 
	if (padding) j= strlen (base64); 
	if (padding == 1) base64[j-1]= '='; 
	else if (padding == 2){ 
		base64[j-2]= '='; 
		base64[j-1]= '='; 
	} 
	// for (j= 0; j < padding; j++) strcat (base64, "="); 
	return base64; 
} 

char * base64_encodeB (unsigned char * buffer_data, int buffer_size){ 
	char * base64= GlobalAlloc (GMEM_ZEROINIT, buffer_size * 4 / 3 + 16); 
	unsigned char a; 
	unsigned char b; 
	unsigned char c; 
	char small_string[5]= {0}; 
	char conv_table_2base64 [64]= { 
		65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 
		97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 
		48, 49, 50, 51, 52, 53, 54, 55, 56, 57 
	}; 
	int padding= 0; 
	int i; 
	int j; 
	for (i= 0; i < buffer_size; i += 3){ 
		a= buffer_data[i]; 
		if (i + 1 < buffer_size) b= buffer_data[i+1]; 
		else { 
			b= 0; 
			padding++; 
		} 
		if (i + 2 < buffer_size) c= buffer_data[i+2]; 
		else { 
			c= 0; 
			padding++; 
		} 
		small_string[0]= conv_table_2base64[a>>2]; 
		small_string[1]= conv_table_2base64[((a & 3)<<4) | (b>>4)]; 
		small_string[2]= conv_table_2base64[((b & 15)<<2) | (c>>6)]; 
		small_string[3]= conv_table_2base64[c & 63]; 
		for (j= 0; j < padding; j++) small_string[3-j]= 61; 
		strcat (base64, small_string); 
		// memmove (base64 + strlen (base64), small_string, 4); 
	} 
	return base64; 
} 