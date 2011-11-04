
#include "market_service.h"

CQTMarketService::CQTMarketService(PGlobalParam param)
{
	m_pcqtLocalAppData = new CQTLocalAppData(param);
	m_pcqtReadAMS = new CQTMarket_ReadAMS(param);
	m_pPackManager = new CQTPackManager(param);
	MAP_SetParam(param);

	m_pcqtLocalAppData->CA_Init();
	m_pcqtReadAMS->AMS_SetLocalHandle(m_pcqtLocalAppData);
	m_pPackManager->CPM_SetAppDataInfo(m_pcqtLocalAppData);
}

CQTMarketService::~CQTMarketService()
{
	m_pcqtLocalAppData->CA_Uninit();
}

bool CQTMarketService::CMS_StartService()
{
	m_pcqtReadAMS->AMS_StartService();
	m_pPackManager->CPM_ScanService();
	return true;
}


