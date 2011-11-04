/*** Prompter Demo Application *******************************************************************/

#ifndef __MARKET_PACKAGEMANAGER_H__
#define __MARKET_PACKAGEMANAGER_H__
#include "market_platform.h"
#include "market_local_data.h"

class CQTPackManager: public MarketInterface
{
public:
	CQTPackManager(PGlobalParam param);
	~CQTPackManager();
	
	//这里应该该为线程比较好
	//需要一个线程不断的去扫描安装状态，或许有更好的办法，比如获取系统安装程序通知消息
	bool				CPM_ScanLocalPackInfo(JNIEnv *env);	
	void				CPM_ScanService();

	//安装
	int					CPM_InstallPackageThread(JNIEnv *env,char* lpAppID);
	int					CPM_InstallPackage(char* lpAppID);

	//卸载
	int					CPM_UnintallPackageThread(JNIEnv *env,char* lpAppID);
	int					CPM_UnintallPackage(char* lpAppID);

	//更新
	int					CPM_UpdatePackage(char* lpAppID);
	
	//是否可以更新  0不更新， 1更新
	int					CPM_IsUpdatePackage(char* lpAppID);

	//是否已经安装，0没有安装，1已经安装
	int					CPM_IsInstallPackage(char* lpAppID);	
	
	//设置本地信息表
	void				CPM_SetAppDataInfo(CQTLocalAppData* pAppData) {m_pcqtLocalAppData = pAppData;};
	
	char m_szAppID[256];
private:
	CQTLocalAppData*	m_pcqtLocalAppData;		
};

#endif