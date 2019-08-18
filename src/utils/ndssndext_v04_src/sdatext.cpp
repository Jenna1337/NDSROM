#include "stdafx.h"
#include "main.h"
#include "sdat.h"
#include "fmt_nds.h"
#include "fmt_swar.h"
#include "decoder/nsstrm.h"
#include "decoder/nsswav.h"
#include "decoder/sseq2mid.h"

//====== global vars =======
/*
	ファイルのデコードを行うか否か
	true:	ファイルをデコードする
	false:	格納されたファイルのまま書き出す
	default = true
*/
bool bDecodeFile = true;
/*
	進行状況を表示するか否か
	true:	表示する
	false:	表示しない
*/
bool bShowProgress = false;
//==========================

/* show application usage */
void appShowUsage(void)
{
	const char *options[] = {
		"", "-x", "extract files only(no decoding)", 
		"", "-s", "show processing status", 
		"", "--help", "show this usage",
	};
	int optIndex;

	cout << "<<" << APP_NAME << " " << APP_VER << " by " << APP_AUTHOR << ">>" << endl;
	puts("Usage  : "APP_CMD" [options] <file...>");
	puts("Options:");
	for(optIndex = 0; optIndex < countof(options); optIndex += 3){
		printf("%-2s  %-12s  %s\n", options[optIndex], options[optIndex + 1], options[optIndex + 2]);
	}
}

/* dispatch option char */
bool appDispatchOptionChar(const char opt)
{
	switch(opt)
	{
	case 'x':
		//ファイルをデコードしない
		bDecodeFile = false;
		return true;
	case 's':
		//進行状況を表示
		bShowProgress = true;
		return true;
	}
	return false;
}

/* dispatch option string */
bool appDispatchOptionStr(const char* optStr)
{
	if(strcmp(optStr, "help") == 0){
		appShowUsage();
	}
	else{
		return false;
	}
	return true;
}

bool appDispatchFilePath(const char *fpath)
{
	char titleStr[13], codeStr[5];
	char output_dir[MAX_PATH + 1], output_dir_tmp[MAX_PATH + 1];
	SDAT **sdat_arr;
	int numSDAT;
	int i;

	if(!nds_GetTitle(titleStr, fpath)){
		cout << "Failed to open NDS." << endl;
		return false;
	}
	cout << "TITLE : \"" << titleStr << "\"" << endl;
	if(!nds_GetCode(codeStr, fpath)){
		cout << "Failed to open NDS." << endl;
		return false;
	}
	cout << "CODE  : \"" << codeStr << "\"" << endl;

	//出力フォルダ名生成
	strcpy(output_dir_tmp, fpath);
	PathRemoveFileSpec(output_dir_tmp);
	if(strlen(output_dir_tmp) == 0){
		sprintf(output_dir, "%s_%s", titleStr, codeStr);
	}
	else{
		sprintf(output_dir, "%s\\%s_%s", output_dir_tmp, titleStr, codeStr);
	}

	cout << "Analyzing...";
	sdat_arr = sdat_OpenFromNDS(&numSDAT, fpath);
	if(!sdat_arr){
		cout << "Failed to open SDAT." << endl << endl;
		return 1;
	}
	cout << "done." << endl;
	if(!PathFileExists(output_dir)){
		CreateDirectory(output_dir, NULL);
	}

	if(numSDAT == 1){
		ExtractSDAT(sdat_arr[0], output_dir);
	}
	else if(numSDAT > 1){
		strcpy(output_dir_tmp, output_dir);
		for(i = 0; i < numSDAT; i ++){
			printf("Block: %.2X\n", i);
			sprintf(output_dir, "%s\\%.2X", output_dir_tmp, i);
			if(!PathFileExists(output_dir)){
				CreateDirectory(output_dir, NULL);
			}
			ExtractSDAT(sdat_arr[i], output_dir);
		}
	}
	sdat_Close_arr(sdat_arr, numSDAT);

	return true;
}

bool ExtractSDAT(const SDAT *sdat, const char *dir)
{
	char file_output_dir[MAX_PATH + 1],
		file_output_dir_sseq[MAX_PATH + 1],
		file_output_dir_strm[MAX_PATH + 1],
		file_output_dir_swar[MAX_PATH + 1],
		file_output[MAX_PATH + 1];
	FILE *fp_out;
	SWAR *swar = NULL;
	SWAV *swav = NULL;
	NSStrm *nsstrm = NULL;
	NSSwav *nsswav = NULL;
	Sseq2mid *nssseq = NULL;
	int numSSEQ = 0, numSTRM = 0, numSWAR = 0, numSWAV = 0;

	//フォルダ作成
	strcpy(file_output_dir, dir);
	sprintf(file_output_dir_sseq, "%s\\%s", file_output_dir, DIR_SSEQ);
	sprintf(file_output_dir_strm, "%s\\%s", file_output_dir, DIR_STRM);
	sprintf(file_output_dir_swar, "%s\\%s", file_output_dir, DIR_SWAR);
	if(!PathFileExists(file_output_dir)){
		CreateDirectory(file_output_dir, NULL);
	}
	if(!PathFileExists(file_output_dir_sseq)){
		CreateDirectory(file_output_dir_sseq, NULL);
	}
	if(!PathFileExists(file_output_dir_strm)){
		CreateDirectory(file_output_dir_strm, NULL);
	}
	if(!PathFileExists(file_output_dir_swar)){
		CreateDirectory(file_output_dir_swar, NULL);
	}

	for(UINT i = 0; i < sdat->files; i ++){
		if(strncmp((char*)sdat->fileoffset[i], "SSEQ", 4) == 0){
			if(bDecodeFile){
				sprintf(file_output, "%s\\%.4X.mid", file_output_dir_sseq, i);
				if(bShowProgress){
					cout << file_output << endl;
				}
				else{
					cout << ".";
				}

				//SSEQ open
				nssseq = sseq2midCreate(sdat->fileoffset[i], sdat->filesize[i], false);
				if(!nssseq){
					cout << "SSEQ open error." << endl;
					goto SSEQ_DECODE_END;
				}
				sseq2midSetLoopCount(nssseq, 1);
				sseq2midNoReverb(nssseq, false);
				if(!sseq2midConvert(nssseq)){
					cout << "SSEQ convert error." << endl;
					goto SSEQ_DECODE_END;
				}

				if(!sseq2midWriteMidiFile(nssseq, file_output)){
					cout << "MIDI write error." << endl;
					goto SSEQ_DECODE_END;
				}
				else numSSEQ ++;
SSEQ_DECODE_END:
				if(nssseq){sseq2midDelete(nssseq); nssseq = NULL;}
			}
			else{
				sprintf(file_output, "%s\\%.4X.sseq", file_output_dir_sseq, i);
				if(bShowProgress){
					cout << file_output << endl;
				}
				else{
					cout << ".";
				}

				fp_out = fopen(file_output, "wb");
				if(!fp_out){
					cout << "file open error." << endl;
					break;
				}
				fwrite(sdat->fileoffset[i], 1, sdat->filesize[i], fp_out);
				fclose(fp_out);
				numSSEQ ++;
			}
		}
		else if(strncmp((char*)sdat->fileoffset[i], "STRM", 4) == 0){
			if(bDecodeFile){
				sprintf(file_output, "%s\\%.4X.wav", file_output_dir_strm, i);
				if(bShowProgress){
					cout << file_output << endl;
				}
				else{
					cout << ".";
				}

				//STRM open
				nsstrm = nsStrmCreate(sdat->fileoffset[i], sdat->filesize[i]);
				if(!nsstrm){
					cout << "STRM open error." << endl;
					goto STRM_DECODE_END;
				}
				if(!nsStrmWriteToWaveFile(nsstrm, file_output)){
					cout << "WAVE write error." << endl;
					nsStrmDelete(nsstrm);
					goto STRM_DECODE_END;
				}
				else numSTRM ++;
STRM_DECODE_END:
				if(nsstrm){nsStrmDelete(nsstrm); nsstrm = NULL;}
			}
			else{
				sprintf(file_output, "%s\\%.4X.strm", file_output_dir_strm, i);
				if(bShowProgress){
					cout << file_output << endl;
				}
				else{
					cout << ".";
				}

				fp_out = fopen(file_output, "wb");
				if(!fp_out){
					cout << "file open error." << endl;
					break;
				}
				fwrite(sdat->fileoffset[i], 1, sdat->filesize[i], fp_out);
				fclose(fp_out);
				numSTRM ++;
			}
		}
		else if(strncmp((char*)sdat->fileoffset[i], "SWAR", 4) == 0){
			if(bDecodeFile){
				////SWARファイルを展開するフォルダ生成
				//sprintf(file_output_dir_swar_fileext, "%s\\SWAV", file_output_dir_swar, i);
				//if(!PathFileExists(file_output_dir_swar_fileext)){
				//	CreateDirectory(file_output_dir_swar_fileext, NULL);
				//}

				//SWAR open
				swar = swar_OpenMem(sdat->fileoffset[i], sdat->filesize[i]);
				if(!swar){
					cout << "SWAR open error." << endl;
					break;
				}

				//展開
				for(UINT j = 0; j < swar->files; j ++){
					sprintf(file_output, "%s\\%.4X_%.4X.wav", file_output_dir_swar, i, j);
					if(bShowProgress){
						cout << file_output << endl;
					}
					else{
						cout << ".";
					}

					//SWAV open
					swav = swav_CreateFromDataMem(swar->fileoffset[j], swar->filesize[j]);
					if(!swav){
						cout << "SWAV create error." << endl;
						goto SWAVE_DECODE_END;
					}
					nsswav = nsSwavCreate(swav->image, swav->size);
					if(!nsswav){
						cout << "SWAV open error." << endl;
						goto SWAVE_DECODE_END;
					}

					if(!nsSwavWriteToWaveFile(nsswav, file_output)){
						cout << "WAVE write error." << endl;
						goto SWAVE_DECODE_END;
					}
					else numSWAV ++;
SWAVE_DECODE_END:
					if(swav){swav_Close(swav); swav = NULL;}
					if(nsswav){nsSwavDelete(nsswav); nsswav = NULL;}
				}
				swar_Close(swar);
			}
			else{
				//SWAR書き出し
				sprintf(file_output, "%s\\%.4X.swar", file_output_dir_swar, i);
				if(bShowProgress){
					cout << file_output << endl;
				}
				else{
					cout << ".";
				}

				fp_out = fopen(file_output, "wb");
				if(!fp_out){
					cout << "file open error." << endl;
					break;
				}
				fwrite(sdat->fileoffset[i], 1, sdat->filesize[i], fp_out);
				fclose(fp_out);
				numSWAR ++;
			}
		}
	}

	cout << endl;
	printf("<Result>\n");
	printf("  Sequense : %d\n", numSSEQ);
	printf("  Stream   : %d\n", numSTRM);
	if(bDecodeFile){
		printf("  sample   : %d\n", numSWAV);
	}
	else{
		printf("  WaveArc  : %d\n", numSWAR);
	}
	cout << endl;

	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int argi = 1;
	int argci;

	if(argc == 1){
		/* no arguments */
		appShowUsage();
	}
	else{
		/* options */
		while((argi < argc) && (argv[argi][0] == '-'))
		{
			if(argv[argi][1] == '-'){
				/* --string */
				if(!appDispatchOptionStr(&argv[argi][2])){
					cout << "Invalid option." << endl;
					return 1;
				}
			}
			else{
				/* -letters */
				argci = 1;
				while(argv[argi][argci] != '\0')
				{
					if(!appDispatchOptionChar(argv[argi][argci])){
						cout << "Invalid option." << endl;
						return 1;
					}
					argci ++;
				}
			}
			argi++;
		}
	}

	/* input files */
	for(; argi < argc; argi ++){
		appDispatchFilePath(argv[argi]);
	}
	return 0;
}
