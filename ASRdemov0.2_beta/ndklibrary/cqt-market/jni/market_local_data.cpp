
#include "market_local_data.h"
#include <stdio.h>

CQTLocalAppData::CQTLocalAppData(PGlobalParam param)
{
	MAP_SetParam(param);
	memset(&m_tAppHead,0,sizeof(TAppFileHead));
	memset(m_pClassData,0,sizeof(PAppClassData)*MAX_CLASS_COUNT);
	memset(m_ppApp,0,sizeof(PCqtApp)*MAX_APP_COUNT);
	memset(m_pAppInstalled,0,sizeof(PAppInstall)*MAX_APP_COUNT);
	m_isInitFlag = false;
	m_nAppCount = 0;
	m_nClassCount = 0;
	m_nAppInstalledCount = 0;
}

CQTLocalAppData::~CQTLocalAppData()
{
	CA_Uninit();
}

void CQTLocalAppData::CA_Init()
{
	Read();
	m_isInitFlag = true;
}

void CQTLocalAppData::CA_Uninit()
{
	int i = 0;
	
	for(i = 0; i < m_nClassCount; i++)
	{
		if(m_pClassData[i])
			delete m_pClassData[i];
	}
	m_nClassCount = 0;
	
	
	for(i = 0; i < m_nAppCount; i++)
	{
		if(m_ppApp[i])
			delete m_ppApp[i];
	}
	m_nAppCount = 0;
}

bool CQTLocalAppData::IsMaxClass()
{
	if(m_nClassCount >= MAX_CLASS_COUNT)
		return true;

	return false;
}

bool CQTLocalAppData::IsCheckClassValid(int idx)
{
	if(idx >= m_nClassCount || idx < 0)
		return false;

	if(!m_pClassData[idx])
	{
		return false;
	}

	return true;
}

PAppClassData CQTLocalAppData::CA_GetClassItemByID(char* lpID,int& nIdx)
{
	nIdx = -1;
	if(!lpID)
		return 0;

	for(int i = 0; i < m_nClassCount; i++)
	{
		PAppClassData pItem = m_pClassData[i];
		if(0 == strcmp(pItem->szClassID,lpID))
		{
			nIdx = i;
			return pItem;
		}
	}

	return 0;
}

int CQTLocalAppData::CA_AddClassItem(PAppClassData pItem)
{
	if(IsMaxClass())
	{
		return -1;
	}

	int nIDx = -1;
	PAppClassData p = CA_GetClassItemByID(pItem->szClassID,nIDx);
	if(p)
	{
		memcpy(p,pItem,sizeof(TAppClassData));
	}else
	{
		m_pClassData[m_nClassCount] = new TAppClassData;
		memcpy(m_pClassData[m_nClassCount],pItem,sizeof(TAppClassData));
		nIDx = m_nClassCount;
		m_nClassCount++;
	}

	//Write();
	MY_SAPI_DB_PRINTF("CA_AddClassItem nIDx=%d",nIDx);
	return nIDx;
}

bool CQTLocalAppData::CA_ModiflyClassItem(int idx, PAppClassData pItem)
{
	if(!IsCheckClassValid(idx))
		return false;

	PAppClassData p = m_pClassData[idx];
	memcpy(p,pItem,sizeof(TAppClassData));	
	return true;
}

bool CQTLocalAppData::CA_DeleteClassItem(int idx)
{
	if(!IsCheckClassValid(idx))
		return false;

	PAppClassData p = m_pClassData[idx];
	delete p;
	for(int i = idx; i < (m_nClassCount-1); i++)
	{
		m_pClassData[i] = m_pClassData[i+1];
	}
	m_nClassCount--;
	return true;
}

int	CQTLocalAppData::CA_GetAppCount(const char* lpClassID)
{
	int nCount = 0;
	if(!lpClassID)
		return m_nAppCount;

	for(int i = 0; i < m_nAppCount; i++)
	{
		PCqtApp p = m_ppApp[i];
		
		if(0 == strcmp(p->tAppBaseInfo.szClassID,lpClassID))
		{
			nCount++;
		}
	}

	return nCount;
}

PCqtApp	CQTLocalAppData::CA_GetAppItem(const char* lpClassID, int idx)
{
	if(!lpClassID)
		return m_ppApp[idx];
	
	int nLocalIdx = 0;
	for(int i = 0; i < m_nAppCount; i++)
	{
		PCqtApp p = m_ppApp[i];		
		if(0 == strcmp(p->tAppBaseInfo.szClassID,lpClassID))
		{
			if(idx == nLocalIdx)
			{
				return p;
			}			
			nLocalIdx++;
		}
	}
	
	return 0;
}

PCqtApp CQTLocalAppData::CA_GetAppItemFromID(const char* lpID)
{
	if(!lpID)
		return 0;
	
	for(int i = 0; i < m_nAppCount; i++)
	{
		PCqtApp p = m_ppApp[i];		
		if(0 == strcmp(p->tAppBaseInfo.szAppID,lpID))
		{
			return p;
		}
	}
	
	return 0;
}

bool CQTLocalAppData::CA_ModiflyAppItem(const char* lpClassID,int idx,PCqtApp pItem)
{
	int nCount = 0;
	if(!lpClassID)
	{
		memcpy(m_ppApp[idx],pItem,sizeof(TCqtApp));
		return true;
	}
	
	for(int i = 0; i < m_nAppCount; i++)
	{
		PCqtApp p = m_ppApp[i];		
		if(0 == strcmp(p->tAppBaseInfo.szClassID,lpClassID))
		{
			if(idx == i)
			{
				memcpy(m_ppApp[idx],pItem,sizeof(TCqtApp));
				return true;
			}
			nCount++;
		}
	}
	
	return false;
}

bool CQTLocalAppData::CA_DeleteAppItem(const char* lpClassID, int idx)
{
#if 0
	int nCount = 0;
	if(!lpClassID)
	{
		memcpy(m_ppApp[idx],pItem,sizeof(TCqtApp));
		return true;
	}else
	{
		for(int i = 0; i < m_nAppCount; i++)
		{
			PCqtApp p = m_ppApp[i];		
			if(0 == strcmp(p->tAppBaseInfo.szClassID,lpClassID))
			{
				if(idx == i)
				{
					memcpy(m_ppApp[idx],pItem,sizeof(TCqtApp));
					return true;
				}
				nCount++;
			}
		}
	}	
#endif

	return false;
}

bool CQTLocalAppData::CA_AddAppItem(PCqtApp pItem)
{
	if(m_nAppCount >= MAX_APP_COUNT)
		return false;

	PCqtApp pI = CA_GetAppItemFromID(pItem->tAppBaseInfo.szAppID);
	if(pI)
	{
		memcpy(pI,pItem,sizeof(TCqtApp));
	}else
	{
		m_ppApp[m_nAppCount] = new TCqtApp;
		memcpy(m_ppApp[m_nAppCount],pItem,sizeof(TCqtApp));
		m_nAppCount++;
	}
	
	//Write();
	return true;
}

int	CQTLocalAppData::CA_FindStart(const char* lpClassID,const char* lpKey)
{
	return 0;
}

int	CQTLocalAppData::CA_FindNext()
{
	return 0;
}

int	CQTLocalAppData::CA_GetAppItemFromKey(const char* lpClassID,const char* lpAppID)
{
	return 0;
}

//////////////////////////////////////////////////////////////////////////
int	CQTLocalAppData::CA_GetLocalCount()
{
	return m_nAppInstalledCount;
}

PAppInstall	CQTLocalAppData::CA_GetLocalItem(int idx)
{
	return m_pAppInstalled[idx];
}

PAppInstall	CQTLocalAppData::CA_GetLocalItemByPackageName(char* lpPackageName)
{
	for(int i =0; i < m_nAppInstalledCount; i++)
	{
		PAppInstall pInstall = m_pAppInstalled[i];
		MY_SAPI_DB_PRINTF("CA_GetLocalItemByAppID-- pInstall->szPackageName= %s,lpPackageName=%s",pInstall->szPackageName,lpPackageName);
		if(0 == strcmp(pInstall->szPackageName,lpPackageName))
		{
			return pInstall;
		}
	}

	return 0;
}

PAppInstall	CQTLocalAppData::CA_GetLocalItemByAppID(char* lpID)
{
	for(int i =0; i < m_nAppInstalledCount; i++)
	{
		PAppInstall pInstall = m_pAppInstalled[i];
		MY_SAPI_DB_PRINTF("CA_GetLocalItemByAppID-- pInstall->szAppID= %s,lpID=%s",pInstall->szAppID,lpID);
		if(0 == strcmp(pInstall->szAppID,lpID))
		{
			return pInstall;
		}
	}
	
	return 0;
}

int	CQTLocalAppData::CA_GetLocalItemIdxByPackageName(char* lpPackageName)
{
	for(int i =0; i < m_nAppInstalledCount; i++)
	{
		PAppInstall pInstall = m_pAppInstalled[i];
		MY_SAPI_DB_PRINTF("CA_GetLocalItemByAppID-- pInstall->szPackageName= %s,lpPackageName=%s",pInstall->szPackageName,lpPackageName);
		if(0 == strcmp(pInstall->szPackageName,lpPackageName))
		{
			return i;
		}
	}

	return -1;
}

int	CQTLocalAppData::CA_AddLocalItem(PAppInstall pItem)
{
	int idx = CA_GetLocalItemIdxByPackageName(pItem->szPackageName);
	if(idx >= 0)
	{
		memcpy(m_pAppInstalled[idx],pItem,sizeof(TAppInstall));
	}else
	{
		if(m_nAppInstalledCount >= MAX_APP_COUNT)
			return -1;

		idx = m_nAppInstalledCount;
		m_pAppInstalled[idx] = new TAppInstall;
		memcpy(m_pAppInstalled[idx],pItem,sizeof(TAppInstall));
		m_nAppInstalledCount++;
	}

	//Write();
	return idx;
}

bool CQTLocalAppData::CA_DeleteLocalItem(int idx)
{
	PAppInstall pI = m_pAppInstalled[idx];
	delete pI;
	 m_pAppInstalled[idx] = 0;
	for(int i = idx; i < m_nAppInstalledCount-1; i++)
	{
		m_pAppInstalled[i] = m_pAppInstalled[i+1];
	}
	m_nAppInstalledCount--;

	//Write();
	return true;
}

void CQTLocalAppData::CA_SaveLocal()
{
	Write();
}

void CQTLocalAppData::CA_ReadLocal()
{
	Read();
}

void CQTLocalAppData::CA_LocalClear()
{
	for(int i = 0; i < m_nAppInstalledCount; i++)
	{
		delete m_pAppInstalled[i];
		m_pAppInstalled[i] = 0;
	}
	m_nAppInstalledCount = 0;
}

void CQTLocalAppData::Read()
{	
	int nCount = 0;
	int i = 0;	
	CA_LocalClear();
	m_nAppInstalledCount = 0;
	
	MY_SAPI_DB_PRINTF("Read--1");
	FILE* fp = fopen(FILE_PATH_APP_DATA,"rb");
	if(!fp)
		return;
	
	MY_SAPI_DB_PRINTF("Read--2");
	fread(&m_tAppHead,1,sizeof(TAppFileHead),fp);
	if(0 != strcmp(m_tAppHead.szDescribe,APP_FILE_INFO))
	{
		MY_SAPI_DB_PRINTF("Read--3");
		goto READERROR;
	}
	
	MY_SAPI_DB_PRINTF("Read--4");
	fread(&nCount,1,sizeof(int),fp);
	MY_SAPI_DB_PRINTF("Read--5");
	for(i = 0; i < nCount; i++)
	{
		m_pAppInstalled[i] = new TAppInstall;
		fread(m_pAppInstalled[i],1,sizeof(TAppInstall),fp);
		MY_SAPI_DB_PRINTF("Read--55 m_pAppInstalled[i]->szAppID = %s",m_pAppInstalled[i]->szAppID);
		m_nAppInstalledCount++;
	}
	MY_SAPI_DB_PRINTF("Read--6");
		
READERROR:
	fclose(fp);
}

//这里有线程同步问题
void CQTLocalAppData::Write()
{
	int nCount = 0;
	int i = 0;
	
	MY_SAPI_DB_PRINTF("Write--1");
	FILE* fp = fopen(FILE_PATH_APP_DATA,"wb");
	if(!fp)
		return;
	
	MY_SAPI_DB_PRINTF("Write--2");
	strcpy(m_tAppHead.szDescribe,APP_FILE_INFO);
	strcpy(m_tAppHead.szVersion,APP_VERSION_INFO);
	fwrite(&m_tAppHead,1,sizeof(TAppFileHead),fp);
	
	MY_SAPI_DB_PRINTF("Write--3 m_nAppInstalledCount=%d",m_nAppInstalledCount);
	fwrite(&m_nAppInstalledCount,1,sizeof(int),fp);
	for(i = 0; i < m_nAppInstalledCount; i++)
	{
		MY_SAPI_DB_PRINTF("Write--m_pAppInstalled[i]->szAppID=%s,m_pAppInstalled[i]->szPackageName=%s,m_pAppInstalled[i]->szPackageVersion=%s",
			m_pAppInstalled[i]->szAppID,m_pAppInstalled[i]->szPackageName,m_pAppInstalled[i]->szPackageVersion);
		fwrite(m_pAppInstalled[i],1,sizeof(TAppInstall),fp);
	}
	fclose(fp);
	MY_SAPI_DB_PRINTF("Write--4");
}

#if 0
void CQTLocalAppData::Read()
{
	int nCount = 0;
	int i = 0;
	m_nAppCount = 0;
	m_nClassCount = 0;

	FILE* fp = fopen(FILE_PATH_APP_DATA,"rb");
	if(!fp)
		return;

	fread(&m_tAppHead,1,sizeof(TAppFileHead),fp);
	if(0 != strcmp(m_tAppHead.szDescribe,APP_FILE_INFO))
	{
		goto READERROR;
	}

	fread(&nCount,1,sizeof(int),fp);
	for(i = 0; i < nCount; i++)
	{
		m_pClassData[i] = new TAppClassData;
		fread(m_pClassData[i],1,sizeof(TAppClassData),fp);
		m_nClassCount++;
	}
	
	fread(&nCount,1,sizeof(int),fp);
	for(i = 0; i < nCount; i++)
	{
		m_ppApp[i] = new TCqtApp;
		fread(m_ppApp[i],1,sizeof(TCqtApp),fp);
		m_nAppCount++;
	}

READERROR:
	fclose(fp);
}

void CQTLocalAppData::Write()
{
	int nCount = 0;
	int i = 0;	
	
	FILE* fp = fopen(FILE_PATH_APP_DATA,"wb");
	if(!fp)
		return;
	
	strcpy(m_tAppHead.szDescribe,APP_FILE_INFO);
	strcpy(m_tAppHead.szVersion,APP_VERSION_INFO);
	fwrite(&m_tAppHead,1,sizeof(TAppFileHead),fp);	
	
	fwrite(&m_nClassCount,1,sizeof(int),fp);
	for(i = 0; i < m_nClassCount; i++)
	{
		fwrite(m_pClassData[i],1,sizeof(TAppClassData),fp);
	}
	
	fwrite(&m_nAppCount,1,sizeof(int),fp);
	for(i = 0; i < m_nAppCount; i++)
	{		
		fwrite(m_ppApp[i],1,sizeof(TCqtApp),fp);
	}

	fclose(fp);
}
#endif

