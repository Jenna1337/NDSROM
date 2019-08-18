#ifndef _FMT_NDS_H_
#define _FMT_NDS_H_

#include "stdafx.h"

struct NDS{
	UINT size;
	BYTE *image;
};

/*---------------------------------
* NDS�t�@�C������Q�[���^�C�g����
* �擾����
*	buf �i�[����o�b�t�@(13 bytes)
*---------------------------------*/
bool nds_GetTitle(char *buf, const char *path);

/*---------------------------------
* NDS�t�@�C������Q�[���R�[�h��
* �擾����
*	buf �i�[����o�b�t�@(5 bytes)
*---------------------------------*/
bool nds_GetCode(char *buf, const char *path);

#endif	//_FMT_NDS_H_
