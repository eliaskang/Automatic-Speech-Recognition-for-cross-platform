/*** Prompter Demo Application *******************************************************************/

#ifndef __MARKET_SERVICE_H_
#define __MARKET_SERVICE_H_
#include "market_platform.h"
#include "market_service.h"
#include "market_local_data.h"
#include "market_readams.h"
#include "market_packagemanager.h"

//该版本考虑比较简单，假定服务端应用程序不超过100个的情况下设计的，
//如果是超过了.
//1）需要分段去取程序数据，比如按25条为一页从服务器去取，
//2）服务端程序信息不能全部缓存到本机上
class CQTMarketService: public MarketInterface
{
public:
	CQTMarketService(PGlobalParam param);
	~CQTMarketService();

	//开启线程，从网络下载APP信息
	bool				CMS_StartService();

	//得到本地数据库信息的表
	CQTLocalAppData*	CMS_GetAppDataInfo() {return m_pcqtLocalAppData;};
	

public:
	CQTLocalAppData*	m_pcqtLocalAppData;
	CQTMarket_ReadAMS*  m_pcqtReadAMS;
	CQTPackManager*		m_pPackManager;

};

#endif