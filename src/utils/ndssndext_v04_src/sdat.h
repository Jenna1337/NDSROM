#ifndef _SDAT_H_
#define _SDAT_H_

#include "stdafx.h"

struct CHUNK_IMAGE{
	UINT size;
	BYTE *image;
};

struct SDAT{
	UINT size;
	BYTE *image;

	//SYMB

	//FAT
	UINT files;
	BYTE **fileoffset;
	UINT *filesize;
};

SDAT* sdat_Open(LPCSTR fname);

/*---------------------------------
* NDSファイルからSDATを作成する
* SDATは複数の可能性がある
*	num(in): 返されるSDATの数
*   ----
*	return(out): SDAT**
*---------------------------------*/
SDAT** sdat_OpenFromNDS(int *num, LPCSTR fname);

void sdat_Close(SDAT *psdat);
void sdat_Close_arr(SDAT **psdat_arr, int num);

/*---------------------------------
* SDAT内の指定した名前のチャンクへの
* ポインタを得る
*---------------------------------*/
BYTE* sdat_GetChunkAddr(SDAT *psdat, const char *chunkname);

/*---------------------------------
* SDAT内の指定した名前のチャンクの
* 実体を得る(未)
*---------------------------------*/
BYTE* sdat_GetChunkImage(SDAT *psdat, const char *chunkname);

/*---------------------------------
* SDAT内のFAT情報を得る
*---------------------------------*/
bool sdat_GetFATInfo(SDAT *psdat);

#endif	//_SDAT_H_
