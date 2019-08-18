#include "stdafx.h"

bool nds_GetTitle(char *buf, const char *path)
{
	char invalid_chars[] = {'/','\\','<','>','*','?','\"','|',':',';',' '};
	FILE *fp;
	UINT size;

	fp = fopen(path, "rb");
	if(!fp){
		return false;
	}
	
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	if(size < 0x400){
		fclose(fp);
		return false;
	}
	memset(buf, 0, 13);
	fread(buf, 1, 12, fp);
	buf[12] = '\0';
	fclose(fp);

	//�����ɋ󔒂���������폜
	//for(int i = 11; i >= 0; i --){
	//	if(buf[i] == ' ') buf[i] = '\0';
	//	
	//}

	//�t�@�C�����Ɏg���Ȃ�������u��������
	for(int i = 0; i < (int)strlen(buf); i ++){
		for(int j = 0; j < countof(invalid_chars); j ++){
			if(buf[i] == invalid_chars[j]){
				buf[i] = '_';
				break;
			}
		}
	}

	return true;
}

bool nds_GetCode(char *buf, const char *path)
{
	FILE *fp;
	UINT size;

	fp = fopen(path, "rb");
	if(!fp){
		return false;
	}
	
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	if(size < 0x400){
		fclose(fp);
		return false;
	}
	fseek(fp, 0x0C, SEEK_SET);
	fread(buf, 1, 4, fp);
	buf[4] = '\0';
	fclose(fp);

	return true;
}
