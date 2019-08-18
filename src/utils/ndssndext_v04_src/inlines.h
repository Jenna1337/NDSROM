#include "stdafx.h"

#define SAFE_FREE(x) if((x)){ free(x); (x) = NULL; }

#ifndef countof
  #define countof(a)	(sizeof(a) / sizeof(a[0]))
#endif

inline UINT mget_uint(BYTE *ptr){
	int a;
	memcpy(&a, ptr, sizeof(UINT));
	return a;
}
