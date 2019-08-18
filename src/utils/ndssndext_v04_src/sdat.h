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
* NDS�t�@�C������SDAT���쐬����
* SDAT�͕����̉\��������
*	num(in): �Ԃ����SDAT�̐�
*   ----
*	return(out): SDAT**
*---------------------------------*/
SDAT** sdat_OpenFromNDS(int *num, LPCSTR fname);

void sdat_Close(SDAT *psdat);
void sdat_Close_arr(SDAT **psdat_arr, int num);

/*---------------------------------
* SDAT���̎w�肵�����O�̃`�����N�ւ�
* �|�C���^�𓾂�
*---------------------------------*/
BYTE* sdat_GetChunkAddr(SDAT *psdat, const char *chunkname);

/*---------------------------------
* SDAT���̎w�肵�����O�̃`�����N��
* ���̂𓾂�(��)
*---------------------------------*/
BYTE* sdat_GetChunkImage(SDAT *psdat, const char *chunkname);

/*---------------------------------
* SDAT����FAT���𓾂�
*---------------------------------*/
bool sdat_GetFATInfo(SDAT *psdat);

#endif	//_SDAT_H_
