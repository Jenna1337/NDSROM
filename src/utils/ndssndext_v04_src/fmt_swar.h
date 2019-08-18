#ifndef _FMT_SWAR_H_
#define _FMT_SWAR_H_

#include "stdafx.h"
#include "sdat.h"

struct SWAR{
	UINT size;
	BYTE *image;

	//FAT
	UINT files;
	BYTE **fileoffset;
	UINT *filesize;
};

struct SWAV{
	UINT size;
	BYTE *image;
};

SWAR* swar_OpenMem(BYTE *base, UINT size);
void swar_Close(SWAR *pswar);

/*---------------------------------
* SWARから取り出した個別データから
* SWAVを作成する
*---------------------------------*/
SWAV* swav_CreateFromDataMem(BYTE *base, UINT size);

void swav_Close(SWAV *pswav);

/*---------------------------------
* SWAR内のFAT情報を得る
*---------------------------------*/
bool swar_GetFATInfo(SWAR *pswar);

#endif	//_FMT_SWAR_H_
