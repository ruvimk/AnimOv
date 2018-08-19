#define MZ_MALLOC(x) GlobalAlloc (GMEM_ZEROINIT, x) 
#define MZ_FREE(x) GlobalFree (x) 
#define MZ_REALLOC(p, x) GlobalReAlloc (p, x, GMEM_ZEROINIT) 

#include "miniz.c" 