
#include "SAPIPlatform.h"

CQtObject::CQtObject(CQtObject* parent)
#if defined(PLATFORM_SAPI_LINUX_QT) || defined(PLATFROM_SAPI_ANDROID_QT) || defined(PLATFROM_SAPI_WINCE_QT)
:QObject(parent)
#endif
{
}

CQtObject::~CQtObject()
{
}

CThirdEngine::CThirdEngine(CQtObject* parent)
:CQtObject(parent)
{

}

#if defined(PLATFROM_SAPI_WINCE_EVC)
static UINT Thread_Proc_Thread(LPVOID lparam)
{
	CQtThread* pThis = (CQtThread*)lparam;
	pThis->run();
	return 0;
}

void CQtThread::T_start()
{
	AfxBeginThread(Thread_Proc_Thread,this);
}

void CQtThread::T_Quit()
{
}
#endif


#if defined(PLATFROM_SAPI_ANDROID_NDK)
#include <pthread.h>
pthread_t SAPI_thread;
void* ThreadSAPINDK(void* param)
{
	CQtThread* pThis = (CQtThread*)param;
	pThis->run();
	return 0;
}

void CQtThread::T_start()
{
	int rr = pthread_create(&SAPI_thread, NULL, ThreadSAPINDK, this);
}

void CQtThread::T_Quit()
{
}
#endif