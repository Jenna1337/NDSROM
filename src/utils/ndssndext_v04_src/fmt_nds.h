#ifndef _FMT_NDS_H_
#define _FMT_NDS_H_

#include "stdafx.h"

struct NDS{
	UINT size;
	BYTE *image;
};

/*---------------------------------
* NDSファイルからゲームタイトルを
* 取得する
*	buf 格納するバッファ(13 bytes)
*---------------------------------*/
bool nds_GetTitle(char *buf, const char *path);

/*---------------------------------
* NDSファイルからゲームコードを
* 取得する
*	buf 格納するバッファ(5 bytes)
*---------------------------------*/
bool nds_GetCode(char *buf, const char *path);

#endif	//_FMT_NDS_H_
