#include "stdafx.h"
#include "sdat.h"

// application info
#define APP_NAME   "NDS Sound Extractor"
#define APP_CMD    "ndssndext.exe"
#define APP_VER    "v0.3"
#define APP_AUTHOR "TENDON"

// directory settings
#define DIR_SSEQ	"sequence"
#define DIR_STRM	"stream"
#define DIR_SWAR	"wavesample"
#define DIR_SBNK	"bank"

// prototype
void appShowUsage(void);
bool appDispatchOptionChar(const char opt);
bool appDispatchOptionStr(const char* optStr);
bool appDispatchFilePath(const char *fpath);
bool ExtractSDAT(const SDAT *sdat, const char *dir);
