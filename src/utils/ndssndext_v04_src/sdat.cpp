#include "stdafx.h"
#include "sdat.h"
#include <vector>

typedef vector<unsigned int> uint_varr;
typedef vector<SDAT*> psdat_varr;

SDAT* sdat_Open(LPCSTR fname)
{
	SDAT *psdat;
	FILE *fp;
	char readbuf[4];

	if((fp = fopen(fname, "rb")) == NULL) return NULL;
	fread(readbuf, 1, 4, fp);
	//ヘッダチェック
	if(strncmp(readbuf, "SDAT", 4) != 0){
		cout << "SDAT header error." << endl;
		return NULL;
	}
	psdat = (SDAT*)calloc(sizeof(SDAT), 1);
	//読み込み
	fseek(fp, 0, SEEK_END);
	psdat->size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	psdat->image = (BYTE*)malloc(psdat->size);
	fread(psdat->image, 1, psdat->size, fp);

	//FAT情報取得
	if(!sdat_GetFATInfo(psdat)){
		cout << "Failed to read FAT(SDAT)." << endl;
		return NULL;
	}

	return psdat;
}

SDAT** sdat_OpenFromNDS(int *num, LPCSTR fname)
{
	SDAT *psdat_tmp, **psdat_arr;
	FILE *fp;
	UINT nds_size;
	BYTE *nds_image;
	uint_varr offset_arr;
	psdat_varr psdat_arr_tmp;
	UINT i, j;
	BYTE SDAT_delim[]		= {'S','D','A','T',0xFF,0xFE,0x00,0x01};
	BYTE SDAT_fake1_delim[]	= {0x00,0xE0,0x51,0x00,0x00};
	BYTE SDAT_fake2_delim[]	= {'I','N','F','O'};

	//NDS読み込み
	if((fp = fopen(fname, "rb")) == NULL) return NULL;
	fseek(fp, 0, SEEK_END);
	nds_size = ftell(fp);
	if(nds_size < 0x400){
		fclose(fp);
		return NULL;
	}
	fseek(fp, 0, SEEK_SET);
	nds_image = (BYTE*)malloc(nds_size);
	if(!nds_image){
		cout << "Memory allocation error." << endl;
		fclose(fp);
		return NULL;
	}
	fread(nds_image, 1, nds_size, fp);
	fclose(fp);

	//SDAT探索
	bool bPass;
	for(i = 0; i < nds_size - 0x250; i ++){
		if(nds_image[i] == SDAT_delim[0]){
			bPass = true;
			for(j = 0; j < countof(SDAT_delim); j ++){
				if(nds_image[i + j] != SDAT_delim[j]){
					bPass = false;
					break;
				}
			}
			if(bPass){
				//有効なSDATであるかどうか判断する（確実な手法は不明）
				if( memcmp(&nds_image[i + 0x08], SDAT_fake1_delim, countof(SDAT_fake1_delim)) == 0 ||
					memcmp(&nds_image[i + 0x2C], SDAT_fake2_delim, countof(SDAT_fake2_delim)) == 0 ||
					mget_uint(&nds_image[i + 0x08]) > nds_size - i)
				{
					bPass = false;
					continue;
				}
				else{
					offset_arr.push_back(i);
					i += mget_uint(&nds_image[i + 0x08]);
					continue;
				}
			}
		}
	}
	if(offset_arr.size() == 0){
		SAFE_FREE(nds_image);
		return NULL;
	}

	//SDAT作成
	for(i = 0; i < offset_arr.size(); i ++){
		psdat_tmp = (SDAT*)calloc(sizeof(SDAT), 1);
		psdat_tmp->size = mget_uint(&nds_image[offset_arr[i] + 0x08]);
		psdat_tmp->image = (BYTE*)malloc(psdat_tmp->size);
		//printf("offset = %.8X  size = %.8X\n", offset_arr[i], psdat_tmp->size);
		memcpy(psdat_tmp->image, &nds_image[offset_arr[i]], psdat_tmp->size);

		//FAT情報取得
		if(!sdat_GetFATInfo(psdat_tmp)){
			cout << "Failed to read FAT(SDAT)." << endl;
			sdat_Close(psdat_tmp);
		}
		else{
			psdat_arr_tmp.push_back(psdat_tmp);
		}
	}

	if(psdat_arr_tmp.size() == 0){
		offset_arr.clear();
		psdat_arr_tmp.clear();
		SAFE_FREE(nds_image);
		return NULL;
	}

	//vector→配列
	psdat_arr = (SDAT**)calloc(sizeof(SDAT*) * psdat_arr_tmp.size(), 1);
	for(i = 0; i < psdat_arr_tmp.size(); i ++){
		psdat_arr[i] = psdat_arr_tmp[i];
	}
	*num = (int)psdat_arr_tmp.size();

	SAFE_FREE(nds_image);
	offset_arr.clear();
	psdat_arr_tmp.clear();

	return psdat_arr;
}

void sdat_Close(SDAT *psdat)
{
	SAFE_FREE(psdat->image);
	SAFE_FREE(psdat->filesize);
	SAFE_FREE(psdat->fileoffset);
	SAFE_FREE(psdat);
}

void sdat_Close_arr(SDAT **psdat_arr, int num)
{
	for(int i = 0; i < num; i ++){
		SAFE_FREE(psdat_arr[i]->image);
		SAFE_FREE(psdat_arr[i]->filesize);
		SAFE_FREE(psdat_arr[i]->fileoffset);
		SAFE_FREE(psdat_arr[i]);
	}
	SAFE_FREE(psdat_arr);
}

/*---------------------------------
* SDAT内の指定した名前のチャンクへの
* ポインタを得る
*---------------------------------*/
BYTE* sdat_GetChunkAddr(SDAT *psdat, const char *chunkname)
{
	BYTE *current_pos;
	//int chunk_size;

	//最初のチャンク
	//current_pos = psdat->image + mget_uint(&psdat->image[0x10]);
	current_pos = psdat->image + 0x40;	//ここで固定だといいな
	while(true){
		if(strncmp((char*)current_pos, chunkname, 4) == 0){
			return current_pos;
			break;
		}
		//次のチャンク
		current_pos += mget_uint(current_pos + 0x04);
		if(current_pos >= psdat->image + psdat->size){
			return NULL;
		}
	}
	return NULL;
}

/*---------------------------------
* SDAT内の指定した名前のチャンクの
* 実体を得る(未)
*---------------------------------*/
BYTE* sdat_GetChunkImage(SDAT *psdat, const char *chunkname)
{
	return NULL;
}

/*---------------------------------
* SDAT内のFAT情報を得る
*---------------------------------*/
bool sdat_GetFATInfo(SDAT *psdat)
{
	BYTE *FAT_image, *current_pos;

	FAT_image = sdat_GetChunkAddr(psdat, "FAT ");
	if(!FAT_image){
		cout << "FAT chunk not found." << endl;
		return false;
	}
	psdat->files = mget_uint(FAT_image + 0x08);
	psdat->fileoffset = (BYTE**)malloc(sizeof(BYTE*) * psdat->files);
	psdat->filesize = (UINT*)malloc(sizeof(UINT) * psdat->files);

	current_pos = FAT_image + 0x0C;
	for(UINT i = 0; i < psdat->files; i ++){
		psdat->fileoffset[i] = psdat->image + mget_uint(current_pos);
		psdat->filesize[i] = mget_uint(current_pos + 0x04);
		current_pos += 0x10;
	}
	// debug
	//FILE *fp;
	//fp = fopen("FATtest.bin", "wb");
	//fwrite(FAT_image, 1, mget_uint(FAT_image + 0x04), fp);
	//fclose(fp);

	return true;
}
