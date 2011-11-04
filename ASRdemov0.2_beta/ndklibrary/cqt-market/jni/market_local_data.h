/*** Prompter Demo Application *******************************************************************/

#ifndef __MARKET_LOCAL_DATA_H_
#define __MARKET_LOCAL_DATA_H_
#include "market_platform.h"
//这儿应该加入数据库管理，
//由于是demo程序，我的算法是：
//1、从网络下载的程序信息，最多支持100个，产品的时候需要改成分页模式存取数据。
//2、缓存本地安装的程序数目最多支持100个，产品的时候可以使用数据库管理。
//3、数据全部读入内存。

#define APP_USE_SIMPLE_DEMO_FILE		//使用文件管理本地数据
//#define APP_USE_DATABASE				//使用数据库

#define MAX_CLASS_COUNT	20				//最大分类
#define MAX_APP_COUNT	100				//最大程序个数
#define LOCAL_INSTALLED_CLASSID		"installlocal01"

//程序分类信息
typedef struct __tagAppClassData
{
	char szClassID[MARKET_STRING_LARGER];			//分类的ID，是唯一的
	char szClassName[MARKET_STRING_LARGER];			//分类名字
	char szClassImgPath[MARKET_STRING_LARGER];		//图片地址	
	int  nAppCount;									//分类程序的个数
}TAppClassData,*PAppClassData;

//程序信息列表
typedef struct __tagAPPBaseInfo
{
	char szClassID[MARKET_STRING_LARGER];			//分类的ID，是唯一的
	char szAppID[MARKET_STRING_LARGER];				//程序ID，是唯一的，注意：要求云端AMS中的编号是唯一的。甚至可以要求在开发者创建应用程序的时候就确定。
	char szPlatformID[MARKET_STRING_LARGER];		//平台的ID。以后自己规划多平台的时候需要考虑：什么平台，产品是否需要有界面显示等特性，都定义在该ID中。
	char szName[MARKET_STRING_MIN];					//名字
	char szLanguage[MARKET_STRING_MIN];				//语言
	char szCompany[MARKET_STRING_MIN];				//公司
	char szDescribe[MARKET_STRING_HUGE];			//描述
	char szVersion[MARKET_STRING_MIN];				//版本
	char szIcon[MARKET_STRING_HUGE];				//icon位置
	char szPublicDate[MARKET_STRING_MIN];			//发布日期
	char szPrice[MARKET_STRING_MIN];				//价格
	int  nStar;										//几个星
	int	 nDownCount;								//下载次数
}TAPPBaseInfo,*PAPPBaseInfo;

typedef struct __tagAppInstallInfo
{	
	char szAppID[MARKET_STRING_LARGER];	
	char szPackageName[MARKET_STRING_LARGER];						//安装的包名
	char szPackageVersion[MARKET_STRING_LARGER];					//安装时候的版本名字		
}TAppInstall,*PAppInstall;

typedef struct __tagCqtApp
{
	TAPPBaseInfo tAppBaseInfo;	
}TCqtApp,*PCqtApp;

//////////////////////////////////////////////////////////////////////////
//文件头
typedef struct __tagAppFileHead
{
	char szDescribe[MARKET_STRING_NORMAL];
	char szVersion[MARKET_STRING_MIN];
}TAppFileHead,*PAppFileHead;

#define FILE_PATH_APP_DATA	"/mnt/sdcard/cqtmarket.data"
#define APP_FILE_INFO		"CQT-MARKET Data"
#define APP_VERSION_INFO	"ver0.1"
class CQTLocalAppData: public MarketInterface
{
public:
	CQTLocalAppData(PGlobalParam param);
	~CQTLocalAppData();


	//初始化，从本地文件读取信息
	void			CA_Init();
	void			CA_Uninit();

	int				CA_GetClassCount(){return m_nClassCount;};
	PAppClassData	CA_GetClassItem(int idx) { return m_pClassData[idx];}; 
	PAppClassData	CA_GetClassItemByID(char* lpID,int& nIdx);
	int				CA_AddClassItem(PAppClassData pItem);
	bool			CA_ModiflyClassItem(int idx, PAppClassData pItem);
	bool			CA_DeleteClassItem(int idx);//删除的时候所有的序号要往前缩进

	//lpClassID=NULL的时候，查询的是全部应用程序
	int				CA_GetAppCount(const char* lpClassID);
	PCqtApp			CA_GetAppItem(const char* lpClassID, int idx);
	PCqtApp			CA_GetAppItemFromID(const char* lpID);
	bool			CA_ModiflyAppItem(const char* lpClassID,int idx,PCqtApp pItem);
	bool			CA_DeleteAppItem(const char* lpClassID, int idx);
	bool			CA_AddAppItem(PCqtApp pItem);

	//查找
	int				CA_FindStart(const char* lpClassID,const char* lpKey);
	int				CA_FindNext();
	int				CA_GetAppItemFromKey(const char* lpClassID,const char* lpAppID);

	//本地已经安装好的接口
	int				CA_GetLocalCount();
	PAppInstall		CA_GetLocalItem(int idx);
	PAppInstall		CA_GetLocalItemByPackageName(char* lpPackageName);
	PAppInstall		CA_GetLocalItemByAppID(char* lpID);
	int				CA_GetLocalItemIdxByPackageName(char* lpPackageName);
	int				CA_AddLocalItem(PAppInstall pItem);
	bool			CA_DeleteLocalItem(int idx);
	void			CA_SaveLocal();
	void			CA_ReadLocal();
	void			CA_LocalClear();


private:
	//从网页下载的数据
	TAppFileHead  m_tAppHead;
	PAppClassData m_pClassData[MAX_CLASS_COUNT];
	int			  m_nClassCount;
	PCqtApp		  m_ppApp[MAX_APP_COUNT];
	int			  m_nAppCount;
	//////////////////////////
	//已经安装好了的程序列表
	PAppInstall  m_pAppInstalled[MAX_APP_COUNT];
	int			 m_nAppInstalledCount;

	//是否初始化了
	bool		  m_isInitFlag;

private:
	void Read();
	void Write();
	bool isInit() {return m_isInitFlag;};
	bool IsMaxClass();
	bool IsCheckClassValid(int idx);	
};

#endif