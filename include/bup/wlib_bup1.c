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

void file_put_contents (char * filename, char * text, int the_size){ 
	HANDLE hFile; 
	OFSTRUCT ofs; 
	unsigned long bWritten; 
	hFile= (HANDLE)(OpenFile (filename, &ofs, 0x1001)); 
	SetEndOfFile (hFile); 
	WriteFile (hFile, text, the_size, &bWritten, 0); 
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
		x= LookUpNumber2 (fig, obj); 
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
		x= LookUpNumber (fig, obj); 
		sprintf (obj, "%s.%s", vct, "y"); 
		y= LookUpNumber (fig, obj); 
		sprintf (obj, "%s.%s", vct, "z"); 
		z= LookUpNumber (fig, obj); 
		sprintf (obj, "%s.%s", vct, "aboutX"); 
		rX= LookUpNumber (fig, obj); 
		sprintf (obj, "%s.%s", vct, "aboutY"); 
		rY= LookUpNumber (fig, obj); 
		sprintf (obj, "%s.%s", vct, "aboutZ"); 
		rZ= LookUpNumber (fig, obj); 
		RotateAboutOrigin (pt, &rX); 
		pt[0] += x; 
		pt[1] += y; 
		pt[2] += z; 
	} while (FindParentObject (vct, start_vct)); 
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
	Rel2Abs ((double *)(&primeP), objVector, InputBuffer); 
	Rel2Abs ((double *)(&primeI), objVector, InputBuffer); 
	Rel2Abs ((double *)(&primeJ), objVector, InputBuffer); 
	Rel2Abs ((double *)(&primeK), objVector, InputBuffer); 
	Rel2AbsOpc (&opc, objVector, InputBuffer); 
	if (!opc) return; 
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
	sOutputMatrix (cp00101, mat3x6, 3, 6); 
	// file_put_contents ("\\rrefstat.txt", cp00101, strlen (cp00101)); 
	file_put_contents ("C:\\WBord\\wwwroot\\AnimOv\\rrefstat.txt", cp00101, strlen (cp00101)); 
	GlobalFree (cp00101); 
	RREF ((double *)(mat3x6), 3, 6); 
	cp00101= GlobalAlloc (GMEM_ZEROINIT, 512); 
	sOutputMatrix (cp00101, mat3x6, 3, 6); 
	// file_put_contents ("\\rrefstat.txt", cp00101, strlen (cp00101)); 
	file_put_contents ("C:\\WBord\\wwwroot\\AnimOv\\rrefstat2.txt", cp00101, strlen (cp00101)); 
	GlobalFree (cp00101); 
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
	ScalePoint (&corners[0], 0); 
	ScalePoint (&corners[1], 0); 
	ScalePoint (&corners[2], 0); 
	ScalePoint (&corners[3], 0); 
	ScalePoint (&corners[4], 0); 
	ScalePoint (&corners[5], 0); 
	ScalePoint (&corners[6], 0); 
	ScalePoint (&corners[7], 0); 
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
	SwapYZ (corners, 8); 
	OpticTangentTransform (corners, 8, main_w, main_h, cam_rh, cam_rv, cam_zoom); 
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
	for (i= 0; i < 8; i++){ 
		ctrX += corners[i].x; 
		ctrY += corners[i].y; 
		ctrZ += corners[i].z; 
		ctrCnt++; 
	} 
	if (ctrCnt){ 
		ctrX /= ctrCnt; 
		ctrY /= ctrCnt; 
		ctrZ /= ctrCnt; 
	} 
	for (i= 0; i < 8; i++){ 
		ith= &arr[i]; 
		ith->x= corners[i].x; 
		ith->y= corners[i].y; 
		ith->z= corners[i].z; 
		ith->angle= atan2 (ctrY - (ith->y), (ith->x) - ctrX); 
	} 
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
		// printf ("p:(%f,%f); last:(%f,%f); next:(%f,%f); %f ", p.x, p.y, ptLast->x, ptLast->y, ptNext->x, ptNext->y, c); 
		if (c > 0){ 
			if (i + 1 < arr_cnt) memmove (&arr[i+0], &arr[i+1], (arr_cnt - i - 1) * sizeof (PT_ANGLE)); 
			arr_cnt--; 
			i--; 
			// printf ("dropping... \r\n"); 
			continue; 
		} 
		// printf ("\r\n"); 
	} 
	minC= main_h; 
	maxC= 0; 
	for (i= 0; i < arr_cnt; i++){ 
		if ((int)(arr[i].y) < minC) minC= (int)(arr[i].y); 
		if ((int)(arr[i].y) > maxC) maxC= (int)(arr[i].y); 
	} 
	if (UseVerbose && UseDebug){ 
		printf ("Object Color:  RGB(%d,%d,%d)\r\n", (int)(objColor[0]), (int)(objColor[1]), (int)(objColor[2])); 
	} 
	// if (UseVerbose && UseDebug) OutputMatrix ((double *)(matRinv), 3, 3); 
	// if (UseVerbose && UseDebug) printf ("minC: %d\r\nmaxC: %d\r\n", minC, maxC); 
	for (i= minC; i < maxC; i++){ 
		//if (UseVerbose) if (!(i % 10)) printf ("\ti= %d \r\n", i); 
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
			if (d_cnt >= 32) break; 
		} 
		minD= main_w; 
		maxD= 0; 
		//if (UseVerbose && UseDebug) 
		//printf ("Iterating d array... \r\n"); 
		for (j= 0; j < d_cnt; j++){ 
			//if (UseVerbose && UseDebug) printf ("d[%d]=%d\r\n", j, d[j]); 
			if (d[j] < minD) minD= d[j]; 
			if (d[j] > maxD) maxD= d[j]; 
		} 
		// if (UseVerbose && UseDebug){ 
			// //printf ("\tminD: %d\r\n\tmaxD: %d\r\n", minD, maxD); 
		// } 
		y= i; 
		if (minD > maxD) continue; 
		for (j= minD; j < maxD; j++){ 
			x= j; 
			if (x < 0) continue; 
			if (y < 0) continue; 
			if (x >= main_w) continue; 
			if (y >= main_h) continue; 
			px= (DEPTH_PIXEL *)(BitmapData + main_w * 8 * y + 8 * x); 
			matE1[0]= matE1[1]= matE1[2]= 0; 
			matD1[0]= matD1[2]= 0; 
			matD1[1]= -1; 
			angles01.x= -(y - main_h / 2) * 2 * (cam_rv * zm) / main_h; 
			angles01.y= 0; 
			angles01.z= -(x - main_w / 2) * 2 * (cam_rh * zm) / main_w; 
			RotateAboutOrigin ((double *)(matD1), (double *)(&angles01)); 
			SubPoint ((POINT3D *)(matE1), (POINT3D *)(&primeP)); 
			MatMult (matRinv, 3, 3, matE1, 3, 1, matE); 
			MatMult (matRinv, 3, 3, matD1, 3, 1, matD); 
			// RotateAboutOrigin ((double *)(matD), (double *)(&angles01)); 
			// SubPoint ((POINT3D *)(matE), (POINT3D *)(&primeP)); 
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
					if (-t > maxY) continue; 
				if (px->depth == PX_UNDEFINED || ((px->depth) > (long)(t * DepthScalar))){ 
					px->r= objColor[0]; 
					px->g= objColor[1]; 
					px->b= objColor[2]; 
					px->depth= (long)(t * DepthScalar); 
				} 
			} else if (UseDebugBox){ 
				if (px->depth == PX_UNDEFINED){ 
					px->depth= 500; 
					px->g= 200; 
				} 
			} 
			// if (rand () < 64){ 
				// printf ("Current eyepoint and direction: \r\n"); 
				// OutputMatrix ((double *)(matE), 1, 3); 
				// OutputMatrix ((double *)(matD), 1, 3); 
				// // printf ("Current t: %f\r\n", t); 
				// // OutputMatrix ((double *)(&angles01), 3, 1); 
			// } 
		} 
	} 
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
	do { 
		if (count > 288) break; 
		count++; 
		x0= x1; 
		x1= x0 - (a * pow (x0, 3) + b * pow (x0, 2) + c * x0 + d) / (3 * a * pow (x0, 2) + 2 * b * x0 + c); 
	} while (abs ((x0 - x1) * 2000)); 
	return x1; 
} 

int IsPointInPath (int x, int y, POINT_LINK * path, int len){ 
	
} 
int * GetPathIntersects (int y, int * count, POINT_LINK * path, int len){ 
	int * isects; 
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
		c= 3 * (p1->y - p0->y); 
		b= 3 * (p2->y - p1->y) - c; 
		a= p3->y - p0->y - c - b; 
		number= sqrt (4 * b * b - 12 * a * c) / (3 * a); 
		infl= -b / (3 * a); 
		ans1= NewtonsMethodCubicZerosIntegerDouble (a, b, c, p0->y - y, infl - number); 
		ans2= NewtonsMethodCubicZerosIntegerDouble (a, b, c, p0->y - y, infl); 
		ans3= NewtonsMethodCubicZerosIntegerDouble (a, b, c, p0->y - y, infl + number); 
		if (ans1 < 0 || ans1 > 1) ans1= F_NaN; 
		if (ans2 < 0 || ans2 > 1) ans2= F_NaN; 
		if (ans3 < 0 || ans3 > 1) ans3= F_NaN; 
		c= 3 * (p1->x - p0->x); 
		b= 3 * (p2->x - p1->x) - c; 
		a= p3->x - p0->x - c - b; 
		x0= (int)(a * pow (ans1, 3) + b * pow (ans1, 2) + c * ans1 + p0->x); 
		x1= (int)(a * pow (ans2, 3) + b * pow (ans2, 2) + c * ans2 + p0->x); 
		x2= (int)(a * pow (ans3, 3) + b * pow (ans3, 2) + c * ans3 + p0->x); 
		if (x0 == x2 && x1 != NaN){ 
			x0= x2= NaN; 
		} else if (x1 == NaN && x0 != NaN){ 
			x2= NaN; 
		} 
		if (x0 != NaN){ 
			isects[j]= x0; 
			j++; 
		} 
		if (x1 != NaN){ 
			isects[j]= x1; 
			j++; 
		} 
		if (x2 != NaN){ 
			isects[j]= x2; 
			j++; 
		} 
	} 
	for (; i < len; i++){ 
		p0= &path[i-1]; 
		p1= &path[i]; 
		t= ((double)(y) - p0->y) / (p1->y - p0->y); 
		if (t < 0) continue; 
		if (t > 1) continue; 
		x0= (p1->x - p0->x) * t + p0->x; 
		isects[j]= x0; 
		j++; 
	} 
	*count= j; 
	return isects; 
} 
int GetPathRangeX (int y, POINT_RANGE * range, int max_range_count, POINT_LINK * path, int len){ 
	char string1[8172]; 
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
			} else return 0; 
			return 1; 
		} 
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
	return i / 2; 
} 

// int GetPathRangeX (int y, POINT_RANGE * range, int max_range_count, POINT_LINK * path, int len) 

void RenderCurve (){ 
	char string1[8172]; 
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
		Rel2Abs (&pt->x, objVector, InputBuffer); 
		Rel2AbsOpc (&opc, objVector, InputBuffer); 
		CameraPositionTransformPt ((POINT3D *)(&pt->x), &cam_x); 
		z= pt->z; 
		pt->z= pt->y; 
		pt->y= z; 
		if (UseDebug){ 
			sprintf (string2, "Absolute (w/ cam): (%f, %f, %f); \r\n", pts[i].x, pts[i].y, pts[i].z); 
			strcat (string1, string2); 
		} 
		OpticTangentTransform ((POINT3D *)(&pt->x), 1, main_w, main_h, cam_rh, cam_rv, cam_zoom); 
		if (UseDebug){ 
			sprintf (string2, "Absolute (after OTT): (%f, %f, %f); \r\n", pts[i].x, pts[i].y, pts[i].z); 
			strcat (string1, string2); 
		} 
		if (pt->y < minY) minY= pt->y; 
		if (pt->y > maxY) maxY= pt->y; 
		p= (POINT_LINK *)(p->pNext); 
	} 
	if (UseDebug) printf ("******************** opc=%f\r\n", opc); 
	if (UseDebug) printf ("minY, maxY = %d, %d\r\n", minY, maxY); 
	if (!opc) return; 
	if (minY <= maxY && ((minY >= 0 && minY < main_h) || (maxY >= 0 && maxY < main_h))){ 
		for (i= minY; i < maxY; i++){ 
			range_len= GetPathRangeX (i, range, 512, pts, len); 
			for (k= 0; k < range_len; k++){ 
				minX= range[k].startX; 
				maxX= range[k].stopX; 
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
	if (UseDebug) file_put_contents ("C:\\WBord\\wwwroot\\AnimOv\\debug1.txt", string1, strlen (string1)); 
	GlobalFree (pts); 
	GlobalFree (range); 
} 

// int * GetPathIntersects (int y, int * count, POINT_LINK * path, int len) 

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
	double opc= 1; 
	double sum; 
	double bottom; 
	double gdv; 
	double g; 
	for (i= 0; i < len; i++){ 
		pt= &pts[i]; 
		memmove (pt, p, sizeof (POINT_LINK)); 
		Rel2Abs (&pt->x, objVector, InputBuffer); 
		Rel2AbsOpc (&opc, objVector, InputBuffer); 
		CameraPositionTransformPt ((POINT3D *)(&pt->x), &cam_x); 
		z= pt->z; 
		pt->z= pt->y; 
		pt->y= z; 
		OpticTangentTransform ((POINT3D *)(&pt->x), 1, main_w, main_h, cam_rh, cam_rv, cam_zoom); 
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
				if (px->depth == PX_UNDEFINED || ((px->depth) > (long)(z * DepthScalar))){ 
					px->r= objOutline[0]; 
					px->g= objOutline[1]; 
					px->b= objOutline[2]; 
					px->depth= (long)(z * DepthScalar); 
				} 
			} 
			GlobalFree (xs); 
		} 
	} 
	GlobalFree (pts); 
} 

void * ExportDepthPixelsToBitmap (int mode, int w, int h, DEPTH_PIXEL * map){ 
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
				} else { 
					scalar= (px->depth - minDepth) / (maxDepth - minDepth); 
					clr->r= (unsigned char)((double)(px->r) * scalar); 
					clr->g= (unsigned char)((double)(px->g) * scalar); 
					clr->b= (unsigned char)((double)(px->b) * scalar); 
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

char * file_get_contents (char * filename){ 
	HANDLE hFile; 
	OFSTRUCT ofs; 
	int sFile; 
	char * pFile; 
	unsigned long bRead; 
	hFile= (HANDLE)(OpenFile (filename, &ofs, 0)); 
	sFile= GetFileSize (hFile, 0); 
	pFile= GlobalAlloc (GMEM_ZEROINIT, sFile + 16); 
	ReadFile (hFile, pFile, sFile, &bRead, 0); 
	CloseHandle (hFile); 
	return pFile; 
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

void EnsurePath (char * buffer, char * path, char * value, double at){ 
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
	int delete_key= 0; 
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
			} else if (buffer[i+j] - stop[j]){ 
				k= 0; 
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
				if (value){ 
					sprintf (string2, "%s=%s;%s:%s;", path, string1, string3, value); 
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
	for (i= strlen (buffer) - 1; i + 1 && (buffer[i] == 32 || buffer[i] == 9); i--); 
	if (buffer[i] != 10) strcat (&buffer[i], "\r\n"); 
	strcat (buffer, string1); 
	for (i += strlen (&buffer[i]) - 1; i + 1 && (buffer[i] == 32 || buffer[i] == 9); i--); 
	if (buffer[i] != 10) strcat (&buffer[i], "\r\n"); 
} 

char * ParseAnimOvCommands (char * cmds){ 
	char line[4096]; 
	char * pMem; 
	char * pTmp; 
	int sMem= 512; 
	char * pScan; 
	char cmd[512]; 
	char tail[4096]; 
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
	// Do things by blocks of 4 pages; the page size is 512 bytes. 
	if (!strstr (cmds, "<end of message>")) return 0; 
	pMem= GlobalAlloc (GMEM_ZEROINIT, 4096); 
	pScan= scan_line (cmds, line, sizeof (line)); 
	do { 
		if (!strstr (line, "<stop>")) continue; 
		cmd[0]= 0; 
		tail[0]= 0; 
		an= sscanf (line, "%*d %s %[^\r\n]", cmd, tail); 
		if (!an) an= sscanf (line, "%s %[^\r\n]", cmd, tail); 
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
					EnsurePath (pMem, path, four, at); 
				} 
			} 
		} else if (!strcmp (cmd, "ren")){ 
			
		} else if (!strcmp (cmd, "new")){ 
			
		} else if (!strcmp (cmd, "del")){ 
			
		} else if (!strcmp (cmd, "fps")){ 
			sscanf (tail, "%lf", &fps); 
		} else if (!strcmp (cmd, "len")){ 
			sscanf (tail, "%lf", &len); 
		} else if (!strcmp (cmd, "cam")){ 
			
		} 
	} while (pScan= scan_line (pScan, line, sizeof (line))); 
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