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
* SWAR������o�����ʃf�[�^����
* SWAV���쐬����
*---------------------------------*/
SWAV* swav_CreateFromDataMem(BYTE *base, UINT size);

void swav_Close(SWAV *pswav);

/*---------------------------------
* SWAR����FAT���𓾂�
*---------------------------------*/
bool swar_GetFATInfo(SWAR *pswar);

#endif	//_FMT_SWAR_H_
