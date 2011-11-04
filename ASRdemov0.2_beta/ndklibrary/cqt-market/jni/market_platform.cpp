
#include "market_platform.h"
#include <string.h>
#include <strings.h>
#define stricmp strcasecmp
#include <stdarg.h>
#include <stdio.h>
#include <android/log.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>


void MY_SAPI_DB_PRINTF(char* fmt,...)
{
	char szTempInfo[MARKET_STRING_LARGER*2];
	memset(szTempInfo,0,MARKET_STRING_LARGER*2);
	
	va_list args;
	int n;
	va_start(args, fmt);
	n = vsprintf(szTempInfo, fmt, args);
	va_end(args);
	strcat(szTempInfo,"\r\n");
	__android_log_print(ANDROID_LOG_INFO,"SAPI_Printf",szTempInfo);
}

MarketInterface::MarketInterface()
{
	memset(&m_tGlobalParam,0,sizeof(TGlobalParam));
}

MarketInterface::~MarketInterface()
{
}

void MarketInterface::MAP_SetParam(PGlobalParam param)
{
	memcpy(&m_tGlobalParam,param,sizeof(TGlobalParam));
}

PGlobalParam MarketInterface::MAP_GetParam()
{
	return &m_tGlobalParam;
}


//////////////////////////////////////////////////////////////////////////
