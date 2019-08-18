#include "stdafx.h"
#include "fmt_swar.h"

SWAR* swar_OpenMem(BYTE *base, UINT size)
{
	SWAR *pswar;

	//ヘッダチェック
	if(strncmp((char*)base, "SWAR", 4) != 0){
		cout << "SWAR header error." << endl;
		return NULL;
	}
	pswar = (SWAR*)calloc(sizeof(SWAR), 1);

	pswar->size = size;
	pswar->image = (BYTE*)malloc(pswar->size);
	memcpy(pswar->image, base, pswar->size);

	//FAT情報取得
	if(!swar_GetFATInfo(pswar)){
		cout << "Failed to read FAT(SWAR)." << endl;
		return NULL;
	}

	return pswar;
}

void swar_Close(SWAR *pswar)
{
	SAFE_FREE(pswar->image);
	SAFE_FREE(pswar->filesize);
	SAFE_FREE(pswar->fileoffset);
	SAFE_FREE(pswar);
}

/*---------------------------------
* SWAR内のFAT情報を得る
*---------------------------------*/
bool swar_GetFATInfo(SWAR *pswar)
{
	BYTE *current_pos;

	pswar->files = mget_uint(pswar->image + 0x38);
	pswar->fileoffset = (BYTE**)malloc(sizeof(BYTE*) * pswar->files);
	pswar->filesize = (UINT*)malloc(sizeof(UINT) * pswar->files);

	current_pos = pswar->image + 0x3C;
	//先頭～ラスト - 1まで
	UINT i;
	if(pswar->files > 0)
	{
		for(i = 0; i < pswar->files - 1; i ++){
			pswar->fileoffset[i] = pswar->image + mget_uint(current_pos);
			pswar->filesize[i] = mget_uint(current_pos + 0x04) - mget_uint(current_pos);
			current_pos += 0x04;
		}
		//ラスト
		pswar->fileoffset[i] = pswar->image + mget_uint(current_pos);
		pswar->filesize[i] = pswar->size - mget_uint(current_pos);
	}

	// debug
	//FILE *fp;
	//fp = fopen("FATtest.bin", "wb");
	//fwrite(FAT_image, 1, mget_uint(FAT_image + 0x04), fp);
	//fclose(fp);

	return true;
}

/*---------------------------------
* SWARから取り出した個別データから
* SWAVを作成する
*---------------------------------*/
SWAV* swav_CreateFromDataMem(BYTE *base, UINT size)
{
	SWAV *pswav;

	pswav = (SWAV*)calloc(sizeof(SWAV), 1);

	pswav->size = size + 0x18;
	pswav->image = (BYTE*)malloc(pswav->size);

	strncpy((char*)pswav->image, "SWAV", 4);
	*(UINT*)(pswav->image + 0x04) = 0x0100FEFF;
	*(UINT*)(pswav->image + 0x08) = pswav->size;
	*(UINT*)(pswav->image + 0x0C) = 0x00010010;
	strncpy((char*)pswav->image + 0x10, "DATA", 4);
	*(UINT*)(pswav->image + 0x14) = size + 0x08;
	memcpy(pswav->image + 0x18, base, size);

	return pswav;
}

void swav_Close(SWAV *pswav)
{
	SAFE_FREE(pswav->image);
	SAFE_FREE(pswav);
}
