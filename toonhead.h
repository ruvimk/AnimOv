#define NO_INTERSECT      65536 
#define PX_UNDEFINED      -65537 
#define DP_SCALAR         1000 

#include "include/wlib.h" 

double F_NaN; 

long DepthScalar= DP_SCALAR; 

int line_width= 1; 

int main_w= 400; 
int main_h= 300; 

int maxY= 0; 

double cam_x= 0; 
double cam_y= 0; 
double cam_z= 0; 
double cam_aboutX= 0; 
double cam_aboutY= 0; 
double cam_aboutZ= 0; 
double cam_rh= 0; 
double cam_rv= 0; 
double cam_zoom= 0; 

char * RenderMode= 0; 
char * SourceFile= 0; 
char * SourceFormat= 0; 
char * DrawOutlines= 0; 
char * OutputBitmap= 0; 
char * ExportBmMode= 0; 
char * OutputMap= 0; 

char * verbose= 0; 

char * OutputW= 0; 
char * OutputH= 0; 

char * InputBuffer= 0; 

char * BitmapData= 0; 

void * BitmapBuffer= 0; 

int UseVerbose= 0; 
int UseDebug= 0; 
int UseDebugBox= 0; 

int BmpExpMode= 1; 

unsigned char objColor[3]; 
unsigned char objOutline[3]; 

double objLine; 

char objVector[1024]; 

double atX, atY, atZ; 
double aboutX, aboutY, aboutZ; 
double axisX, axisY, axisZ; 

char cmdString[1024]; 
char cmdTail[1024]; 

void ToonInit (){ 
	int line_width= 1; 
	
	// int main_w= 400; 
	// int main_h= 300; 
	
	maxY= 0; 
	
	cam_x= 0; 
	cam_y= 0; 
	cam_z= 0; 
	cam_aboutX= 0; 
	cam_aboutY= 0; 
	cam_aboutZ= 0; 
	cam_rh= 0; 
	cam_rv= 0; 
	cam_zoom= 0; 
	
	RenderMode= 0; 
	SourceFile= 0; 
	SourceFormat= 0; 
	DrawOutlines= 0; 
	OutputBitmap= 0; 
	ExportBmMode= 0; 
	OutputMap= 0; 
	
	verbose= 0; 
	
	// OutputW= 0; 
	// OutputH= 0; 
	
	// InputBuffer= 0; 
	
	// BitmapData= 0; 
	
	// BitmapBuffer= 0; 
	
	UseVerbose= 0; 
	UseDebug= 0; 
	UseDebugBox= 0; 
	
	BmpExpMode= 1; 
	
	*((long *)(objColor))= 0; 
	*((long *)(objOutline))= 0; 
	
	objLine= 1; 
	
	objVector[0]= 0; 
	
	atX= atY= atZ= 0; 
	aboutX= aboutY= aboutZ= 0; 
	axisX= axisY= axisZ= 0; 
} 

typedef struct { 
	void * pLast; 
	void * pNext; 
	double x; 
	double y; 
	double z; 
	double opc; 
	double reserved1; 
	char vct[256]; 
} POINT_LINK; 

POINT_LINK * pFirst= 0; 