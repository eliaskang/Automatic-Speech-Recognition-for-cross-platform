package com.demos;

import android.content.Context;
import android.util.Log;
import android.webkit.WebView;

public class CQTSapi {
	public WebView mWebView;
	Context mContext;
	
	final void init_library()
	{
		try 
		{
			System.loadLibrary("utils");
            System.loadLibrary("media");
            System.loadLibrary("testsvox");
            System.loadLibrary("cqtsapi_ndk");
        } catch (UnsatisfiedLinkError e)
        {
            Log.d("cqtmarketapi", "cqtmarketapi jni library not found!");
        }
	}
	
	CQTSapi(){
     	init_library();
     }
     
	void SetContext(Context c)
	{
		mContext = c;
	}
	
	public void JS_OpenFirstURL(final String strURL)
    {
   	 mWebView.loadUrl(strURL);
    }
	
	public void SetWebview(WebView v)
    {
    	mWebView = v;
    	//Log.i("CQTSapi","--------------------------- SetWebview");
    	//JS_CQT_API_Init();
    	//Log.i("CQTSapi","--------------------------- SetWebview OVER");
    }	
	
	//输出调试信息
	public void callback_SAPI_DebugOutInfo(final String strInfo)
	{
		//Log.i("callback_SAPI_DebugOutInfo",strInfo);
		mWebView.loadUrl("javascript:callback_Debuginfo('" + strInfo + "')");
		//Log.i("callback_SAPI_DebugOutInfo",strInfo + "--Over");
	}
	
	//输出识别的结果,这里没有做多结果返回，将来产品版本的时候需要考虑用户选择
	public void callback_SAPI_ASRResult(final String strASR)
	{
		mWebView.loadUrl("javascript:callback_ASRResult('" + strASR + "')");
	}
	
	public native int JS_SAPI_Init(final String strResultPath, final String strConfigName);
	
	//下面4个函数必须在调用函数JS_SAPI_Hi_Start，JS_SAPI_Lo_ASRRule，JS_SAPI_Lo_TTSRule之前设置，
	//否则调用失败或者无效	
	//1 清空设别规则中的变量
	public native int JS_SAPI_ASRClear(final String strASRVar);	
	
	//2 设置设别规则中的变量	
	public native int JS_SAPI_SetASRRuleValue(final String strASRVar,final String strValue);
	
	//3 清空合成规则中的变量
	public native int JS_SAPI_TTSClear(final String strTTSVar);
	
	//4 设置合成规则中的变量
	public native int JS_SAPI_SetTTSRuleValue(final String strTTSVar,final String strValue);
	
	//停止
	public native int JS_SAPI_Stop();
	
	//高级API
	public native int JS_SAPI_Hi_Start();
		
	//开始一个ASR
	public native int JS_SAPI_Lo_ASRRule(final String strRule);
	
	//开始一个TTS
	public native int JS_SAPI_Lo_TTSRule(final String strRule);
	
	//开始一个VoiceRule
	public native int JS_SAPI_Lo_VoiceRule(final String strRule);
	
	//开始播放一个TTS文本
	public native int JS_SAPI_Lo_TTSText(final String strText);
	
	//开始播放一个Voice File
	public native int JS_SAPI_Lo_VoiceFile(final String strFile);
}
